#include "catalog/catalog.h"

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
  auto magic_num = MACH_READ_INT32(buf);
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
  // ASSERT(false, "Not Implemented yet");
}

CatalogManager::~CatalogManager() {
  delete heap_;
}

dberr_t CatalogManager::CreateTable(const string &table_name, TableSchema *schema,
                                    Transaction *txn, TableInfo *&table_info) {
  // ASSERT(false, "Not Implemented yet");
  return DB_FAILED;
}

dberr_t CatalogManager::GetTable(const string &table_name, TableInfo *&table_info) {
  // ASSERT(false, "Not Implemented yet");
  return DB_FAILED;
}

dberr_t CatalogManager::GetTables(vector<TableInfo *> &tables) const {
  // ASSERT(false, "Not Implemented yet");
  return DB_FAILED;
}

dberr_t CatalogManager::CreateIndex(const std::string &table_name, const string &index_name,
                                    const std::vector<std::string> &index_keys, Transaction *txn,
                                    IndexInfo *&index_info) {
  // ASSERT(false, "Not Implemented yet");
  return DB_FAILED;
}

dberr_t CatalogManager::GetIndex(const std::string &table_name, const std::string &index_name,
                                 IndexInfo *&index_info) const {
  // ASSERT(false, "Not Implemented yet");
  return DB_FAILED;
}

dberr_t CatalogManager::GetTableIndexes(const std::string &table_name, std::vector<IndexInfo *> &indexes) const {
  // ASSERT(false, "Not Implemented yet");
  return DB_FAILED;
}

dberr_t CatalogManager::DropTable(const string &table_name) {
  // ASSERT(false, "Not Implemented yet");
  return DB_FAILED;
}

dberr_t CatalogManager::DropIndex(const string &table_name, const string &index_name) {
  // ASSERT(false, "Not Implemented yet");
  return DB_FAILED;
}


dberr_t CatalogManager::FlushCatalogMetaPage() const {
  // ASSERT(false, "Not Implemented yet");
  return DB_FAILED;
}

dberr_t CatalogManager::LoadTable(const table_id_t table_id, const page_id_t page_id) {
  // ASSERT(false, "Not Implemented yet");
  return DB_FAILED;
}

dberr_t CatalogManager::LoadIndex(const index_id_t index_id, const page_id_t page_id) {
  // ASSERT(false, "Not Implemented yet");
  return DB_FAILED;
}

dberr_t CatalogManager::GetTable(const table_id_t table_id, TableInfo *&table_info) {
  // ASSERT(false, "Not Implemented yet");
  return DB_FAILED;
}