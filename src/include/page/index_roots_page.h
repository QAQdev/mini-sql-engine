#ifndef MINISQL_INDEX_ROOTS_PAGE_H
#define MINISQL_INDEX_ROOTS_PAGE_H

#include <string>

#include "common/config.h"

/**
 * Database use the one as index roots page page to store all
 * index's root page id
 *
 * Format (size in byte):
 *  -----------------------------------------------------------------
 * | RecordCount (4) | Index_1 id (4) | Index_1 root_id (4) | ... |
 *  -----------------------------------------------------------------
 */
class IndexRootsPage {
public:
  void Init() {
    count_ = 0;
  }

  bool Insert(const index_id_t index_id, const page_id_t root_id);

  bool Delete(const index_id_t index_id);

  bool Update(const index_id_t index_id, const page_id_t root_id);

  // return root_id if success
  bool GetRootId(const index_id_t index_id, page_id_t *root_id);

  int GetIndexCount() { return count_; }

private:
  static constexpr int MAX_INDEX_COUNT = (PAGE_SIZE - 4) / 8;

  int FindIndex(const index_id_t index_id);

private:
  int count_;
  std::pair<index_id_t, page_id_t> roots_[0];
};

#endif //MINISQL_INDEX_ROOTS_PAGE_H
