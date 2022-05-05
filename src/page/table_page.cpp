#include "page/table_page.h"

void TablePage::Init(page_id_t page_id, page_id_t prev_id, LogManager *log_mgr, Transaction *txn) {
  memcpy(GetData(), &page_id, sizeof(page_id));
  SetPrevPageId(prev_id);
  SetNextPageId(INVALID_PAGE_ID);
  SetFreeSpacePointer(PAGE_SIZE);
  SetTupleCount(0);
}

bool TablePage::InsertTuple(Row &row, Schema *schema, Transaction *txn,
                            LockManager *lock_manager, LogManager *log_manager) {
  uint32_t serialized_size = row.GetSerializedSize(schema);
  ASSERT(serialized_size > 0, "Can not have empty row.");
  if (GetFreeSpaceRemaining() < serialized_size + SIZE_TUPLE) {
    return false;
  }
  // Try to find a free slot to reuse.
  uint32_t i;
  for (i = 0; i < GetTupleCount(); i++) {
    // If the slot is empty, i.e. its tuple has size 0,
    if (GetTupleSize(i) == 0) {
      // Then we break out of the loop at index i.
      break;
    }
  }
  if (i == GetTupleCount() && GetFreeSpaceRemaining() < serialized_size + SIZE_TUPLE) {
    return false;
  }
  // Otherwise we claim available free space..
  SetFreeSpacePointer(GetFreeSpacePointer() - serialized_size);
  uint32_t __attribute__((unused)) write_bytes = row.SerializeTo(GetData() + GetFreeSpacePointer(), schema);
  ASSERT(write_bytes = serialized_size, "Unexpected behavior in row serialize.");

  // Set the tuple.
  SetTupleOffsetAtSlot(i, GetFreeSpacePointer());
  SetTupleSize(i, serialized_size);
  // Set rid
  row.SetRowId(RowId(GetTablePageId(), i));
  if (i == GetTupleCount()) {
    SetTupleCount(GetTupleCount() + 1);
  }
  return true;
}

bool TablePage::MarkDelete(const RowId &rid, Transaction *txn, LockManager *lock_manager, LogManager *log_manager) {
  uint32_t slot_num = rid.GetSlotNum();
  // If the slot number is invalid, abort.
  if (slot_num >= GetTupleCount()) {
    return false;
  }
  uint32_t tuple_size = GetTupleSize(slot_num);
  // If the tuple is already deleted, abort.
  if (IsDeleted(tuple_size)) {
    return false;
  }
  // Mark the tuple as deleted.
  if (tuple_size > 0) {
    SetTupleSize(slot_num, SetDeletedFlag(tuple_size));
  }
  return true;
}

bool TablePage::UpdateTuple(const Row &new_row, Row *old_row, Schema *schema,
                            Transaction *txn, LockManager *lock_manager, LogManager *log_manager) {
  ASSERT(old_row != nullptr && old_row->GetRowId().Get() != INVALID_ROWID.Get(), "invalid old row.");
  uint32_t serialized_size = new_row.GetSerializedSize(schema);
  ASSERT(serialized_size > 0, "Can not have empty row.");
  uint32_t slot_num = old_row->GetRowId().GetSlotNum();
  // If the slot number is invalid, abort.
  if (slot_num >= GetTupleCount()) {
    return false;
  }
  uint32_t tuple_size = GetTupleSize(slot_num);
  // If the tuple is deleted, abort.
  if (IsDeleted(tuple_size)) {
    return false;
  }
  // If there is not enough space to update, we need to update via delete followed by an insert (not enough space).
  if (GetFreeSpaceRemaining() + tuple_size < serialized_size) {
    return false;
  }
  // Copy out the old value.
  uint32_t tuple_offset = GetTupleOffsetAtSlot(slot_num);
  uint32_t __attribute__((unused)) read_bytes = old_row->DeserializeFrom(GetData() + tuple_offset, schema);
  ASSERT(tuple_size == read_bytes, "Unexpected behavior in tuple deserialize.");
  uint32_t free_space_pointer = GetFreeSpacePointer();
  ASSERT(tuple_offset >= free_space_pointer, "Offset should appear after current free space position.");
  memmove(GetData() + free_space_pointer + tuple_size - serialized_size, GetData() + free_space_pointer,
          tuple_offset - free_space_pointer);
  SetFreeSpacePointer(free_space_pointer + tuple_size - serialized_size);
  new_row.SerializeTo(GetData() + tuple_offset + tuple_size - serialized_size, schema);
  SetTupleSize(slot_num, serialized_size);

  // Update all tuple offsets.
  for (uint32_t i = 0; i < GetTupleCount(); ++i) {
    uint32_t tuple_offset_i = GetTupleOffsetAtSlot(i);
    if (GetTupleSize(i) > 0 && tuple_offset_i < tuple_offset + tuple_size) {
      SetTupleOffsetAtSlot(i, tuple_offset_i + tuple_size - new_row.GetSerializedSize(schema));
    }
  }
  return true;
}

void TablePage::ApplyDelete(const RowId &rid, Transaction *txn, LogManager *log_manager) {
  uint32_t slot_num = rid.GetSlotNum();
  ASSERT(slot_num < GetTupleCount(), "Cannot have more slots than tuples.");

  uint32_t tuple_offset = GetTupleOffsetAtSlot(slot_num);
  uint32_t tuple_size = GetTupleSize(slot_num);
  // Check if this is a delete operation, i.e. commit a delete.
  if (IsDeleted(tuple_size)) {
    tuple_size = UnsetDeletedFlag(tuple_size);
  }

  uint32_t free_space_pointer = GetFreeSpacePointer();
  ASSERT(tuple_offset >= free_space_pointer, "Free space appears before tuples.");

  memmove(GetData() + free_space_pointer + tuple_size, GetData() + free_space_pointer,
          tuple_offset - free_space_pointer);
  SetFreeSpacePointer(free_space_pointer + tuple_size);
  SetTupleSize(slot_num, 0);
  SetTupleOffsetAtSlot(slot_num, 0);

  // Update all tuple offsets.
  for (uint32_t i = 0; i < GetTupleCount(); ++i) {
    uint32_t tuple_offset_i = GetTupleOffsetAtSlot(i);
    if (GetTupleSize(i) != 0 && tuple_offset_i < tuple_offset) {
      SetTupleOffsetAtSlot(i, tuple_offset_i + tuple_size);
    }
  }
}

void TablePage::RollbackDelete(const RowId &rid, Transaction *txn, LogManager *log_manager) {
  uint32_t slot_num = rid.GetSlotNum();
  ASSERT(slot_num < GetTupleCount(), "We can't have more slots than tuples.");
  uint32_t tuple_size = GetTupleSize(slot_num);

  // Unset the deleted flag.
  if (IsDeleted(tuple_size)) {
    SetTupleSize(slot_num, UnsetDeletedFlag(tuple_size));
  }
}

bool TablePage::GetTuple(Row *row, Schema *schema, Transaction *txn, LockManager *lock_manager) {
  ASSERT(row != nullptr && row->GetRowId().Get() != INVALID_ROWID.Get(), "Invalid row.");
  // Get the current slot number.
  uint32_t slot_num = row->GetRowId().GetSlotNum();
  // If somehow we have more slots than tuples, abort the transaction.
  if (slot_num >= GetTupleCount()) {
    return false;
  }
  // Otherwise get the current tuple size too.
  uint32_t tuple_size = GetTupleSize(slot_num);
  // If the tuple is deleted, abort the transaction.
  if (IsDeleted(tuple_size)) {
    return false;
  }
  // At this point, we have at least a shared lock on the RID. Copy the tuple data into our result.
  uint32_t tuple_offset = GetTupleOffsetAtSlot(slot_num);
  uint32_t __attribute__((unused)) read_bytes = row->DeserializeFrom(GetData() + tuple_offset, schema);
  ASSERT(tuple_size == read_bytes, "Unexpected behavior in tuple deserialize.");
  return true;
}

bool TablePage::GetFirstTupleRid(RowId *first_rid) {
  // Find and return the first valid tuple.
  for (uint32_t i = 0; i < GetTupleCount(); i++) {
    if (!IsDeleted(GetTupleSize(i))) {
      first_rid->Set(GetTablePageId(), i);
      return true;
    }
  }
  first_rid->Set(INVALID_PAGE_ID, 0);
  return false;
}

bool TablePage::GetNextTupleRid(const RowId &cur_rid, RowId *next_rid) {
  ASSERT(cur_rid.GetPageId() == GetTablePageId(), "Wrong table!");
  // Find and return the first valid tuple after our current slot number.
  for (auto i = cur_rid.GetSlotNum() + 1; i < GetTupleCount(); i++) {
    if (!IsDeleted(GetTupleSize(i))) {
      next_rid->Set(GetTablePageId(), i);
      return true;
    }
  }
  // Otherwise return false as there are no more tuples.
  next_rid->Set(INVALID_PAGE_ID, 0);
  return false;
}
