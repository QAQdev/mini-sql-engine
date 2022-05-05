#ifndef MINISQL_DISK_FILE_META_PAGE_H
#define MINISQL_DISK_FILE_META_PAGE_H

#include <cstdint>

#include "page/bitmap_page.h"

static constexpr page_id_t MAX_VALID_PAGE_ID = (PAGE_SIZE - 8) / 4 * BitmapPage<PAGE_SIZE>::GetMaxSupportedSize();

class DiskFileMetaPage {
public:
  uint32_t GetExtentNums() {
    return num_extents_;
  }

  uint32_t GetAllocatedPages() {
    return num_allocated_pages_;
  }

  uint32_t GetExtentUsedPage(uint32_t extent_id) {
    if (extent_id >= num_extents_) {
      return 0;
    }
    return extent_used_page_[extent_id];
  }

public:
  uint32_t num_allocated_pages_{0};
  uint32_t num_extents_{0};   // each extent consists with a bit map and BIT_MAP_SIZE pages
  uint32_t extent_used_page_[0];
};

#endif //MINISQL_DISK_FILE_META_PAGE_H
