#ifndef MINISQL_B_PLUS_TREE_PAGE_H
#define MINISQL_B_PLUS_TREE_PAGE_H

#include <cassert>
#include <climits>
#include <cstdlib>
#include <string>
#include "buffer/buffer_pool_manager.h"

#define MappingType std::pair<KeyType, ValueType>

#define INDEX_TEMPLATE_ARGUMENTS template <typename KeyType, typename ValueType, typename KeyComparator>

// define page type enum
enum class IndexPageType {
  INVALID_INDEX_PAGE = 0, LEAF_PAGE, INTERNAL_PAGE
};

/**
 * Both internal and leaf page are inherited from this page.
 *
 * It actually serves as a header part for each B+ tree page and
 * contains information shared by both leaf page and internal page.
 *
 * Header format (size in byte, 24 bytes in total):
 * ----------------------------------------------------------------------------
 * | PageType (4) | LSN (4) | CurrentSize (4) | MaxSize (4) |
 * ----------------------------------------------------------------------------
 * | ParentPageId (4) | PageId(4) |
 * ----------------------------------------------------------------------------
 */
class BPlusTreePage {
public:
  bool IsLeafPage() const;

  bool IsRootPage() const;

  void SetPageType(IndexPageType page_type);

  int GetSize() const;

  void SetSize(int size);

  void IncreaseSize(int amount);

  int GetMaxSize() const;

  void SetMaxSize(int max_size);

  int GetMinSize() const;

  page_id_t GetParentPageId() const;

  void SetParentPageId(page_id_t parent_page_id);

  page_id_t GetPageId() const;

  void SetPageId(page_id_t page_id);

  void SetLSN(lsn_t lsn = INVALID_LSN);

private:
  // member variable, attributes that both internal and leaf page share
  [[maybe_unused]] IndexPageType page_type_;
  [[maybe_unused]] lsn_t lsn_;
  [[maybe_unused]] int size_;
  [[maybe_unused]] int max_size_;
  [[maybe_unused]] page_id_t parent_page_id_;
  [[maybe_unused]] page_id_t page_id_;
};

#endif  // MINISQL_B_PLUS_TREE_PAGE_H
