#include "catalog/catalog.h"
#include "page/index_roots_page.h"

void CatalogMeta::SerializeTo(char *buf) const {
  // ASSERT(false, "Not Implemented yet");
  auto ofs = 0;
  MACH_WRITE_INT32(buf, CATALOG_METADATA_MAGIC_NUM);
  ofs += sizeof(uint32_t);

  MACH_WRITE_TO(unsigned long, buf + ofs, table_meta_pages_.size());
  ofs += sizeof(unsigned long);
  for (auto &it : table_meta_pages_) {
    MACH_WRITE_TO(table_id_t, buf + ofs, it.first);
    ofs += sizeof(table_id_t);
    MACH_WRITE_TO(page_id_t, buf + ofs, it.second);
    ofs += sizeof(page_id_t);
  }

  MACH_WRITE_TO(unsigned long, buf + ofs, index_meta_pages_.size());
  ofs += sizeof(unsigned long);
  for (auto &it : index_meta_pages_) {
    MACH_WRITE_TO(index_id_t , buf + ofs, it.first);
    ofs += sizeof(index_id_t );
    MACH_WRITE_TO(page_id_t, buf + ofs, it.second);
    ofs += sizeof(page_id_t);
  }

  return;
}

CatalogMeta *CatalogMeta::DeserializeFrom(char *buf, MemHeap *heap) {
  // ASSERT(false, "Not Implemented yet");
  auto magic_num = MACH_READ_UINT32(buf);
  ASSERT(magic_num == CATALOG_METADATA_MAGIC_NUM, "Catalog meta magic num error.");

  auto ofs = sizeof(uint32_t);

  auto table_meta_size = MACH_READ_FROM(unsigned long, buf + ofs);
  ofs += sizeof(unsigned long);
  map<table_id_t, page_id_t> table_meta_pages;
  for (unsigned long i = 0; i < table_meta_size; i++) {
    auto table_id = MACH_READ_FROM(table_id_t, buf + ofs);
    ofs += sizeof(table_id_t);
    auto page_id = MACH_READ_FROM(page_id_t, buf + ofs);
    ofs += sizeof(page_id_t);
    table_meta_pages[table_id] = page_id;
  }

  auto index_meta_size = MACH_READ_FROM(unsigned long, buf + ofs);
  ofs += sizeof(unsigned long);
  map<table_id_t, page_id_t> index_meta_pages;
  for (unsigned long i = 0; i < index_meta_size; i++) {
    auto index_id = MACH_READ_FROM(index_id_t, buf + ofs);
    ofs += sizeof(table_id_t);
    auto page_id = MACH_READ_FROM(page_id_t, buf + ofs);
    ofs += sizeof(page_id_t);
    index_meta_pages[index_id] = page_id;
  }

  auto meta = CatalogMeta::NewInstance(heap);
  meta->table_meta_pages_ = table_meta_pages;
  meta->index_meta_pages_ = index_meta_pages;

  return meta;
}

uint32_t CatalogMeta::GetSerializedSize() const {
  // ASSERT(false, "Not Implemented yet");
  return sizeof(uint32_t) + 2 * sizeof(unsigned long) +
         (sizeof(table_id_t) + sizeof(page_id_t)) * table_meta_pages_.size() +
         (sizeof(index_id_t) + sizeof(page_id_t)) * index_meta_pages_.size();
}

CatalogMeta::CatalogMeta() {}


CatalogManager::CatalogManager(BufferPoolManager *buffer_pool_manager, LockManager *lock_manager,
                               LogManager *log_manager, bool init)
    : buffer_pool_manager_(buffer_pool_manager), lock_manager_(lock_manager),
      log_manager_(log_manager), heap_(new SimpleMemHeap()) {
  std::atomic_init(&next_table_id_,0);
  std::atomic_init(&next_index_id_,0);
  if (init) {
    catalog_meta_ = CatalogMeta::NewInstance(heap_);
  } else {
    auto meta_page = buffer_pool_manager_->FetchPage(CATALOG_META_PAGE_ID);
    catalog_meta_ = CatalogMeta::DeserializeFrom(meta_page->GetData(), heap_);

    for (auto it : catalog_meta_->table_meta_pages_) {
      auto table_meta_page = buffer_pool_manager_->FetchPage(it.second);
      TableMetadata *table_meta;
      TableMetadata::DeserializeFrom(table_meta_page->GetData(), table_meta, heap_);

      auto table_heap = TableHeap::Create(buffer_pool_manager_, table_meta->GetFirstPageId(), table_meta->GetSchema(),
                                          log_manager_, lock_manager_, heap_);
      table_names_[table_meta->GetTableName()] = table_meta->GetTableId();
      TableInfo *table_info = TableInfo::Create(heap_);
      table_info->Init(table_meta, table_heap);
      tables_[table_meta->GetTableId()] = table_info;
    }

    for (auto it : catalog_meta_->index_meta_pages_) {
      auto index_meta_page = buffer_pool_manager_->FetchPage(it.second);
      IndexMetadata *index_meta;
      IndexMetadata::DeserializeFrom(index_meta_page->GetData(), index_meta, heap_);
      index_names_[tables_[index_meta->GetTableId()]->GetTableName()][index_meta->GetIndexName()] =
          index_meta->GetIndexId();

      auto index_info = IndexInfo::Create(heap_);
      index_info->Init(index_meta, tables_[index_meta->GetTableId()], buffer_pool_manager_);

      indexes_[index_meta->GetIndexId()] = index_info;
    }
  }
}

CatalogManager::~CatalogManager() {
  // Flush all pages to disk
  ASSERT(FlushCatalogMetaPage() == DB_SUCCESS, "CatalogManager flush meta page failed.");
  for (auto &it : catalog_meta_->table_meta_pages_) {
    buffer_pool_manager_->FlushPage(it.second);
  }
  for (auto &it : catalog_meta_->index_meta_pages_) {
    buffer_pool_manager_->FlushPage(it.second);
  }

  delete heap_;
}

dberr_t CatalogManager::CreateTable(const string &table_name, TableSchema *schema,
                                    Transaction *txn, TableInfo *&table_info) {
  if (table_names_.find(table_name) != table_names_.end()) return DB_TABLE_ALREADY_EXIST;
  /**
   * Get table id
   */
  auto table_id = catalog_meta_->GetNextTableId();
  table_names_[table_name] = table_id;  // update table_names_
  /**
   * New a page for table meta data
   */
  page_id_t page_id;
  auto table_meta_page = buffer_pool_manager_->NewPage(page_id);
  catalog_meta_->table_meta_pages_[table_id] = page_id;

  /**
   * Initialize a new table heap
   */
  TableHeap *table_heap = TableHeap::Create(buffer_pool_manager_, schema, txn, log_manager_, lock_manager_, heap_);

  /**
   * Construct table meta data
   */
  /**
   * VERY IMPORTANT CHANGE!!!
   */
  auto table_meta = TableMetadata::Create(table_id, table_name, table_heap->GetFirstPageId(), schema, heap_);
  table_meta->SerializeTo(table_meta_page->GetData());
  buffer_pool_manager_->UnpinPage(page_id, true);

  table_info = TableInfo::Create(heap_);
  table_info->Init(table_meta, table_heap);

  tables_[table_id] = table_info;  // update tables_

  return DB_SUCCESS;
}

dberr_t CatalogManager::GetTable(const string &table_name, TableInfo *&table_info) {
  if (table_names_.find(table_name) == table_names_.end()) return DB_TABLE_NOT_EXIST;

  /**
   * Found, get table info from tables_
   */
  auto table_id = table_names_[table_name];
  table_info = tables_[table_id];

  return DB_SUCCESS;
}

dberr_t CatalogManager::GetTables(vector<TableInfo *> &tables) const {
  if (tables_.empty()) return DB_FAILED;

  // Get all tables
  for (auto &it : tables_) {
    tables.push_back(it.second);
  }

  return DB_SUCCESS;
}

dberr_t CatalogManager::CreateIndex(const std::string &table_name, const string &index_name,
                                    const std::vector<std::string> &index_keys, Transaction *txn,
                                    IndexInfo *&index_info) {
  if (table_names_.find(table_name) == table_names_.end()) return DB_TABLE_NOT_EXIST;
  if ((index_names_[table_name].find(index_name) != index_names_[table_name].end())) return DB_INDEX_ALREADY_EXIST;

  // Construct key map
  auto schema = tables_[table_names_[table_name]]->GetSchema();
  auto table_info = tables_[table_names_[table_name]];
  std::vector<uint32_t> key_map;
  for (auto &it : index_keys) {
    uint32_t col_idx;
    if (schema->GetColumnIndex(it, col_idx) == DB_COLUMN_NAME_NOT_EXIST) return DB_COLUMN_NAME_NOT_EXIST;
    key_map.push_back(col_idx);
  }

  auto index_id = catalog_meta_->GetNextIndexId();
  index_names_[table_name][index_name] = index_id;

  // New a page for index meta page
  page_id_t page_id;
  auto index_meta_page =  buffer_pool_manager_->NewPage(page_id);
  catalog_meta_->index_meta_pages_[index_id] = page_id;
  buffer_pool_manager_->UnpinPage(page_id, true);

  // Construct index meta data
  auto index_meta = IndexMetadata::Create(index_id, index_name, table_names_[table_name], key_map, heap_);
  index_meta->SerializeTo(index_meta_page->GetData());

  // Create index info
  index_info = IndexInfo::Create(heap_);

  index_info->Init(index_meta, table_info, buffer_pool_manager_);

  indexes_[index_id] = index_info;

  return DB_SUCCESS;
}

dberr_t CatalogManager::GetIndex(const std::string &table_name, const std::string &index_name,
                                 IndexInfo *&index_info) const {
  if (table_names_.find(table_name) == table_names_.end()) return DB_TABLE_NOT_EXIST;
  if (index_names_.at(table_name).find(index_name) == index_names_.at(table_name).end()) return DB_INDEX_NOT_FOUND;

  auto index_id = index_names_.at(table_name).at(index_name);
  index_info = indexes_.at(index_id);

  return DB_SUCCESS;
}

dberr_t CatalogManager::GetTableIndexes(const std::string &table_name, std::vector<IndexInfo *> &indexes) const {
  if (table_names_.find(table_name) == table_names_.end()) return DB_TABLE_NOT_EXIST;

  for (auto &it : indexes_) {
    if (it.second->GetTableInfo()->GetTableName() == table_name) {
      indexes.emplace_back(it.second);
    }
  }

  return DB_SUCCESS;
}

dberr_t CatalogManager::DropTable(const string &table_name) {
  if (table_names_.find(table_name) == table_names_.end()) return DB_TABLE_NOT_EXIST;

  auto table_id = table_names_[table_name];
  table_names_.erase(table_name);

  page_id_t page_id = catalog_meta_->table_meta_pages_[table_id];
  catalog_meta_->table_meta_pages_.erase(table_id);

  buffer_pool_manager_->DeletePage(page_id);

  tables_.erase(table_id);

  return DB_SUCCESS;
}

dberr_t CatalogManager::DropIndex(const string &table_name, const string &index_name) {
  if (table_names_.find(table_name) == table_names_.end()) return DB_TABLE_NOT_EXIST;
  if (index_names_.at(table_name).find(index_name) == index_names_.at(table_name).end()) return DB_INDEX_NOT_FOUND;

  auto index_id = index_names_[table_name][index_name];
  index_names_.erase(table_name);

  page_id_t page_id = catalog_meta_->index_meta_pages_[index_id];
  catalog_meta_->index_meta_pages_.erase(index_id);

  buffer_pool_manager_->DeletePage(page_id);
  indexes_.erase(index_id);

  return DB_SUCCESS;
}


dberr_t CatalogManager::FlushCatalogMetaPage() const {
  auto meta_page = buffer_pool_manager_->FetchPage(CATALOG_META_PAGE_ID);
  catalog_meta_->SerializeTo(meta_page->GetData());

  if (!buffer_pool_manager_->FlushPage(CATALOG_META_PAGE_ID)) {
    return DB_FAILED;
  }

  return DB_SUCCESS;
}

dberr_t CatalogManager::LoadTable(const table_id_t table_id, const page_id_t page_id) {
  if (tables_.find(table_id) != tables_.end()) return DB_TABLE_ALREADY_EXIST;

  catalog_meta_->table_meta_pages_[table_id] = page_id;
  auto table_meta_page = buffer_pool_manager_->FetchPage(page_id);

  // Deserialize and get data from the page
  TableMetadata *table_meta;
  TableMetadata::DeserializeFrom(table_meta_page->GetData(), table_meta, heap_);
  table_names_[table_meta->GetTableName()] = table_id;

  /**
   * Load existing table heap and construct table info
   */
  TableHeap *table_heap =
      TableHeap::Create(buffer_pool_manager_, page_id, table_meta->GetSchema(), log_manager_, lock_manager_, heap_);
  TableInfo *table_info = TableInfo::Create(heap_);
  table_info->Init(table_meta, table_heap);
  tables_[table_id] = table_info;

  catalog_meta_->table_meta_pages_[table_id] = page_id;

  return DB_SUCCESS;
}

dberr_t CatalogManager::LoadIndex(const index_id_t index_id, const page_id_t page_id) {
  if (indexes_.find(index_id) != indexes_.end()) return DB_INDEX_ALREADY_EXIST;

  catalog_meta_->index_meta_pages_[index_id] = page_id;
  auto index_mata_page = buffer_pool_manager_->FetchPage(page_id);

  // Deserialize and get data from the page
  IndexMetadata *index_meta;
  IndexMetadata::DeserializeFrom(index_mata_page->GetData(), index_meta, heap_);
  auto table_name = tables_[index_meta->GetTableId()]->GetTableName();
  index_names_[table_name][index_meta->GetIndexName()] = index_id;

  // Construct index info
  auto index_info = IndexInfo::Create(heap_);
  index_info->Init(index_meta, tables_[index_meta->GetTableId()], buffer_pool_manager_);
  indexes_[index_id] = index_info;

  catalog_meta_->index_meta_pages_[index_id] = page_id;

  return DB_SUCCESS;
}

dberr_t CatalogManager::GetTable(const table_id_t table_id, TableInfo *&table_info) {
  if (catalog_meta_->table_meta_pages_.find(table_id) == catalog_meta_->table_meta_pages_.end())
    return DB_TABLE_NOT_EXIST;

  table_info = tables_[table_id];
  return DB_SUCCESS;
}