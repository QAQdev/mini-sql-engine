#ifndef MINISQL_CATALOG_H
#define MINISQL_CATALOG_H

#include <string>
#include <map>
#include <unordered_map>

#include "buffer/buffer_pool_manager.h"
#include "catalog/indexes.h"
#include "catalog/table.h"
#include "common/config.h"
#include "common/dberr.h"
#include "transaction/lock_manager.h"
#include "transaction/log_manager.h"
#include "transaction/transaction.h"

class CatalogMeta {
  friend class CatalogManager;

public:
  void SerializeTo(char *buf) const;

  static CatalogMeta *DeserializeFrom(char *buf, MemHeap *heap);

  uint32_t GetSerializedSize() const;

  inline table_id_t GetNextTableId() const {
    return table_meta_pages_.size() == 0 ? 0 : table_meta_pages_.rbegin()->first;
  }

  inline index_id_t GetNextIndexId() const {
    return index_meta_pages_.size() == 0 ? 0 : index_meta_pages_.rbegin()->first;
  }

  static CatalogMeta *NewInstance(MemHeap *heap) {
    void *buf = heap->Allocate(sizeof(CatalogMeta));
    return new(buf) CatalogMeta();
  }

  /**
   * Used only for testing
   */
  inline std::map<table_id_t, page_id_t> *GetTableMetaPages() {
    return &table_meta_pages_;
  }

  /**
   * Used only for testing
   */
  inline std::map<index_id_t, page_id_t> *GetIndexMetaPages() {
    return &index_meta_pages_;
  }

private:
  explicit CatalogMeta();

private:
  static constexpr uint32_t CATALOG_METADATA_MAGIC_NUM = 89849;
  std::map<table_id_t, page_id_t> table_meta_pages_;
  std::map<index_id_t, page_id_t> index_meta_pages_;
};

/**
 * Catalog manager
 *
 */
class CatalogManager {
public:
  explicit CatalogManager(BufferPoolManager *buffer_pool_manager, LockManager *lock_manager,
                          LogManager *log_manager, bool init);

  ~CatalogManager();

  dberr_t CreateTable(const std::string &table_name, TableSchema *schema, Transaction *txn, TableInfo *&table_info);

  dberr_t GetTable(const std::string &table_name, TableInfo *&table_info);

  dberr_t GetTables(std::vector<TableInfo *> &tables) const;

  dberr_t CreateIndex(const std::string &table_name, const std::string &index_name,
                      const std::vector<std::string> &index_keys, Transaction *txn,
                      IndexInfo *&index_info);

  dberr_t GetIndex(const std::string &table_name, const std::string &index_name, IndexInfo *&index_info) const;

  dberr_t GetTableIndexes(const std::string &table_name, std::vector<IndexInfo *> &indexes) const;

  dberr_t DropTable(const std::string &table_name);

  dberr_t DropIndex(const std::string &table_name, const std::string &index_name);

private:
  dberr_t FlushCatalogMetaPage() const;

  dberr_t LoadTable(const table_id_t table_id, const page_id_t page_id);

  dberr_t LoadIndex(const index_id_t index_id, const page_id_t page_id);

  dberr_t GetTable(const table_id_t table_id, TableInfo *&table_info);

private:
  [[maybe_unused]] BufferPoolManager *buffer_pool_manager_;
  [[maybe_unused]] LockManager *lock_manager_;
  [[maybe_unused]] LogManager *log_manager_;
  [[maybe_unused]] CatalogMeta *catalog_meta_;
  [[maybe_unused]] std::atomic<table_id_t> next_table_id_;
  [[maybe_unused]] std::atomic<index_id_t> next_index_id_;
  // map for tables
  std::unordered_map<std::string, table_id_t> table_names_;
  std::unordered_map<table_id_t, TableInfo *> tables_;
  // map for indexes: table_name->index_name->indexes
  [[maybe_unused]] std::unordered_map<std::string, std::unordered_map<std::string, index_id_t>> index_names_;
  [[maybe_unused]] std::unordered_map<index_id_t, IndexInfo *> indexes_;
  // memory heap
  MemHeap *heap_;
};

#endif //MINISQL_CATALOG_H
