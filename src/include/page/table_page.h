#ifndef MINISQL_TUPLE_H
#define MINISQL_TUPLE_H
/**
 * Basic Slotted page format:
 *  ---------------------------------------------------------
 *  | HEADER | ... FREE SPACE ... | ... INSERTED TUPLES ... |
 *  ---------------------------------------------------------
 *                                ^
 *                                free space pointer
 *
 *  Header format (size in bytes):
 *  ----------------------------------------------------------------------------
 *  | PageId (4)| LSN (4)| PrevPageId (4)| NextPageId (4)| FreeSpacePointer(4) |
 *  ----------------------------------------------------------------------------
 *  ----------------------------------------------------------------
 *  | TupleCount (4) | Tuple_1 offset (4) | Tuple_1 size (4) | ... |
 *  ----------------------------------------------------------------
 **/

#include <cstring>
#include "common/macros.h"
#include "common/rowid.h"
#include "page/page.h"
#include "record/row.h"
#include "transaction/lock_manager.h"
#include "transaction/log_manager.h"
#include "transaction/transaction.h"

class TablePage : public Page {
public:
  void Init(page_id_t page_id, page_id_t prev_id, LogManager *log_mgr, Transaction *txn);

  page_id_t GetTablePageId() { return *reinterpret_cast<page_id_t *>(GetData()); }

  page_id_t GetPrevPageId() { return *reinterpret_cast<page_id_t *>(GetData() + OFFSET_PREV_PAGE_ID); }

  page_id_t GetNextPageId() { return *reinterpret_cast<page_id_t *>(GetData() + OFFSET_NEXT_PAGE_ID); }

  void SetPrevPageId(page_id_t prev_page_id) {
    memcpy(GetData() + OFFSET_PREV_PAGE_ID, &prev_page_id, sizeof(page_id_t));
  }

  void SetNextPageId(page_id_t next_page_id) {
    memcpy(GetData() + OFFSET_NEXT_PAGE_ID, &next_page_id, sizeof(page_id_t));
  }

  bool InsertTuple(Row &row, Schema *schema, Transaction *txn, LockManager *lock_manager, LogManager *log_manager);

  bool MarkDelete(const RowId &rid, Transaction *txn, LockManager *lock_manager, LogManager *log_manager);

  bool UpdateTuple(const Row &new_row, Row *old_row, Schema *schema,
                   Transaction *txn, LockManager *lock_manager, LogManager *log_manager);

  void ApplyDelete(const RowId &rid, Transaction *txn, LogManager *log_manager);

  void RollbackDelete(const RowId &rid, Transaction *txn, LogManager *log_manager);

  bool GetTuple(Row *row, Schema *schema, Transaction *txn, LockManager *lock_manager);

  bool GetFirstTupleRid(RowId *first_rid);

  bool GetNextTupleRid(const RowId &cur_rid, RowId *next_rid);

private:
  uint32_t GetFreeSpacePointer() { return *reinterpret_cast<uint32_t *>(GetData() + OFFSET_FREE_SPACE); }

  void SetFreeSpacePointer(uint32_t free_space_pointer) {
    memcpy(GetData() + OFFSET_FREE_SPACE, &free_space_pointer, sizeof(uint32_t));
  }

  uint32_t GetTupleCount() { return *reinterpret_cast<uint32_t *>(GetData() + OFFSET_TUPLE_COUNT); }

  void SetTupleCount(uint32_t tuple_count) { memcpy(GetData() + OFFSET_TUPLE_COUNT, &tuple_count, sizeof(uint32_t)); }

  uint32_t GetFreeSpaceRemaining() {
    return GetFreeSpacePointer() - SIZE_TABLE_PAGE_HEADER - SIZE_TUPLE * GetTupleCount();
  }

  uint32_t GetTupleOffsetAtSlot(uint32_t slot_num) {
    return *reinterpret_cast<uint32_t *>(GetData() + OFFSET_TUPLE_OFFSET + SIZE_TUPLE * slot_num);
  }

  void SetTupleOffsetAtSlot(uint32_t slot_num, uint32_t offset) {
    memcpy(GetData() + OFFSET_TUPLE_OFFSET + SIZE_TUPLE * slot_num, &offset, sizeof(uint32_t));
  }

  uint32_t GetTupleSize(uint32_t slot_num) {
    return *reinterpret_cast<uint32_t *>(GetData() + OFFSET_TUPLE_SIZE + SIZE_TUPLE * slot_num);
  }

  void SetTupleSize(uint32_t slot_num, uint32_t size) {
    memcpy(GetData() + OFFSET_TUPLE_SIZE + SIZE_TUPLE * slot_num, &size, sizeof(uint32_t));
  }

  static bool IsDeleted(uint32_t tuple_size) { return static_cast<bool>(tuple_size & DELETE_MASK) || tuple_size == 0; }

  static uint32_t SetDeletedFlag(uint32_t tuple_size) { return static_cast<uint32_t>(tuple_size | DELETE_MASK); }

  static uint32_t UnsetDeletedFlag(uint32_t tuple_size) { return static_cast<uint32_t>(tuple_size & (~DELETE_MASK)); }

private:
  static_assert(sizeof(page_id_t) == 4);
  static constexpr uint64_t DELETE_MASK = (1U << (8 * sizeof(uint32_t) - 1));
  static constexpr size_t SIZE_TABLE_PAGE_HEADER = 24;
  static constexpr size_t SIZE_TUPLE = 8;
  static constexpr size_t OFFSET_PREV_PAGE_ID = 8;
  static constexpr size_t OFFSET_NEXT_PAGE_ID = 12;
  static constexpr size_t OFFSET_FREE_SPACE = 16;
  static constexpr size_t OFFSET_TUPLE_COUNT = 20;
  static constexpr size_t OFFSET_TUPLE_OFFSET = 24;
  static constexpr size_t OFFSET_TUPLE_SIZE = 28;

public:
  static constexpr size_t SIZE_MAX_ROW = PAGE_SIZE - SIZE_TABLE_PAGE_HEADER - SIZE_TUPLE;
};

#endif