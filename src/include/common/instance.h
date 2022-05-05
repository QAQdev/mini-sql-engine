#ifndef MINISQL_INSTANCE_H
#define MINISQL_INSTANCE_H

#include <memory>
#include <string>

#include "buffer/buffer_pool_manager.h"
#include "catalog/catalog.h"
#include "common/config.h"
#include "common/dberr.h"
#include "storage/disk_manager.h"

class DBStorageEngine {
public:
  explicit DBStorageEngine(std::string db_name, bool init = true,
                           uint32_t buffer_pool_size = DEFAULT_BUFFER_POOL_SIZE)
          : db_file_name_(std::move(db_name)), init_(init) {
    // Init database file if needed
    if (init_) {
      remove(db_file_name_.c_str());
    }
    // Initialize components
    disk_mgr_ = new DiskManager(db_file_name_);
    bpm_ = new BufferPoolManager(buffer_pool_size, disk_mgr_);
    catalog_mgr_ = new CatalogManager(bpm_, nullptr, nullptr, init);
    // Allocate static page for db storage engine
    if (init) {
      page_id_t id;
      ASSERT(bpm_->IsPageFree(CATALOG_META_PAGE_ID), "Catalog meta page not free.");
      ASSERT(bpm_->IsPageFree(INDEX_ROOTS_PAGE_ID), "Header page not free.");
      ASSERT(bpm_->NewPage(id) != nullptr && id == CATALOG_META_PAGE_ID, "Failed to allocate catalog meta page.");
      ASSERT(bpm_->NewPage(id) != nullptr && id == INDEX_ROOTS_PAGE_ID, "Failed to allocate header page.");
      bpm_->UnpinPage(CATALOG_META_PAGE_ID, false);
      bpm_->UnpinPage(INDEX_ROOTS_PAGE_ID, false);
    } else {
      ASSERT(!bpm_->IsPageFree(CATALOG_META_PAGE_ID), "Invalid catalog meta page.");
      ASSERT(!bpm_->IsPageFree(INDEX_ROOTS_PAGE_ID), "Invalid header page.");
    }
  }

  ~DBStorageEngine() {
    delete catalog_mgr_;
    delete bpm_;
    delete disk_mgr_;
  }

public:
  DiskManager *disk_mgr_;
  BufferPoolManager *bpm_;
  CatalogManager *catalog_mgr_;
  std::string db_file_name_;
  bool init_;
};

#endif //MINISQL_INSTANCE_H
