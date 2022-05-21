#ifndef DISK_MGR_H
#define DISK_MGR_H

#include <atomic>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <unordered_map>
#include "common/config.h"
#include "common/macros.h"
#include "page/bitmap_page.h"
#include "page/disk_file_meta_page.h"

/**
 * DiskManager takes care of the allocation and de allocation of pages within a database. It performs the reading and
 * writing of pages to and from disk, providing a logical file layer within the context of a database management system.
 *
 * Disk page storage format: (Free Page BitMap Size = PAGE_SIZE * 8, we note it as N)
 * | Meta Page | Free Page BitMap 1 | Page 1 | Page 2 | ....
 *      | Page N | Free Page BitMap 2 | Page N+1 | ... | Page 2N | ... |
 */
class DiskManager {
 public:
  explicit DiskManager(const std::string &db_file);

  ~DiskManager() {
    if (!closed) {
      Close();
    }
  }

  /**
   * Read page from specific page_id
   * Note: page_id = 0 is reserved for disk meta page
   */
  void ReadPage(page_id_t logical_page_id, char *page_data);

  /**
   * Write data to specific page
   * Note: page_id = 0 is reserved for disk meta page
   */
  void WritePage(page_id_t logical_page_id, const char *page_data);

  /**
   * Get next free page from disk
   * @return logical page id of allocated page
   */
  page_id_t AllocatePage();

  /**
   * Free this page and reset bit map
   */
  void DeAllocatePage(page_id_t logical_page_id);

  /**
   * Return whether specific logical_page_id is free
   */
  bool IsPageFree(page_id_t logical_page_id);

  /**
   * Shut down the disk manager and close all the file resources.
   */
  void Close();

  /**
   * Get Meta Page
   * Note: Used only for debug
   */
  char *GetMetaData() { return meta_data_; }

  static constexpr size_t BITMAP_SIZE = BitmapPage<PAGE_SIZE>::GetMaxSupportedSize();

 private:
  /**
   * Helper function to get disk file size
   */
  int GetFileSize(const std::string &file_name);

  /**
   * Read physical page from disk
   */
  void ReadPhysicalPage(page_id_t physical_page_id, char *page_data);

  /**
   * Write data to physical page in disk
   */
  void WritePhysicalPage(page_id_t physical_page_id, const char *page_data);

  /**
   * Map logical page id to physical page id
   */
  page_id_t MapPageId(page_id_t logical_page_id);

 private:
  // stream to write db file
  std::fstream db_io_;
  std::string file_name_;
  // with multiple buffer pool instances, need to protect file access
  std::recursive_mutex db_io_latch_;
  bool closed{false};
  char meta_data_[PAGE_SIZE];
};

#endif