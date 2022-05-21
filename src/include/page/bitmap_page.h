#ifndef MINISQL_BITMAP_PAGE_H
#define MINISQL_BITMAP_PAGE_H

#include <bitset>

#include "common/macros.h"
#include "common/config.h"

template<size_t PageSize>
class BitmapPage {
 public:
  /**
   * @return The number of pages that the bitmap page can record, i.e. the capacity of an extent.
   */
  static constexpr size_t GetMaxSupportedSize() { return 8 * MAX_CHARS; }

  /**
   * @param page_offset Index in extent of the page allocated.
   * @return true if successfully allocate a page.
   */
  bool AllocatePage(uint32_t &page_offset);

  /**
   * @return true if successfully de-allocate a page.
   */
  bool DeAllocatePage(uint32_t page_offset);

  /**
   * @return whether a page in the extent is free
   */
  bool IsPageFree(uint32_t page_offset) const;

  /**
   * @return whether a bitmap is full
   */
  bool IsBitmapFull();

 private:
  /**
   * @brief check a bit(byte_index, bit_index) in bytes is free(value 0).
   *
   * @param byte_index value of page_offset / 8
   * @param bit_index value of page_offset % 8
   * @return true if a bit is 0, false if 1.
   */
  bool IsPageFreeLow(uint32_t byte_index, uint8_t bit_index) const;

  /** Note: need to update if modify page structure. */
  static constexpr size_t MAX_CHARS = PageSize - 2 * sizeof(uint32_t);

 private:
  /** The space occupied by all members of the class should be equal to the PageSize */
  [[maybe_unused]] uint32_t page_allocated_{0};

 public:
  [[maybe_unused]] uint32_t next_free_page_{0};

 private:
  std::bitset<8 * MAX_CHARS> allocate_pages;
  //[[maybe_unused]] unsigned char bytes[MAX_CHARS]{};
};

#endif //MINISQL_BITMAP_PAGE_H
