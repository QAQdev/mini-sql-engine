#include "page/bitmap_page.h"
#include <cmath>

template<size_t PageSize>
bool BitmapPage<PageSize>::AllocatePage(uint32_t &page_offset) {
  if (allocate_pages.test(page_offset) || page_offset >= 8 * MAX_CHARS) {
    return false;
  } else {
    allocate_pages.set(page_offset, 1);
    return true;
  }
}

template<size_t PageSize>
bool BitmapPage<PageSize>::DeAllocatePage(uint32_t page_offset) {
  if (allocate_pages.test(page_offset)) {
    allocate_pages.set(page_offset, 0);
    next_free_page_ = page_offset;
    page_allocated_--;
    return true;
  } else {
    return false;
  }
}

template<size_t PageSize>
bool BitmapPage<PageSize>::IsPageFree(uint32_t page_offset) const {
  return !allocate_pages.test(page_offset);
}

template<size_t PageSize>
bool BitmapPage<PageSize>::IsPageFreeLow(uint32_t byte_index, uint8_t bit_index) const {
  return false;
}
template <size_t PageSize>
bool BitmapPage<PageSize>::IsBitmapFull() {
  return page_allocated_ == 8 * MAX_CHARS;
}

template
class BitmapPage<64>;

template
class BitmapPage<128>;

template
class BitmapPage<256>;

template
class BitmapPage<512>;

template
class BitmapPage<1024>;

template
class BitmapPage<2048>;

template
class BitmapPage<4096>;