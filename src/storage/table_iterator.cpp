#include "common/macros.h"
#include "storage/table_iterator.h"
#include "storage/table_heap.h"

TableIterator::TableIterator(TableHeap *table_heap, RowId rid,Transaction*txn)
    :table_heap(table_heap),
      row(new Row(rid)),
      txn(txn) {
  if (rid.GetPageId() != INVALID_PAGE_ID) {
    this->table_heap->GetTuple(row, txn);
  }
}

TableIterator::TableIterator(const TableIterator &other)
    :table_heap(other.table_heap),
     row(other.row),
      txn(other.txn) {

}

TableIterator::~TableIterator() { delete row; }

bool TableIterator::operator==(const TableIterator &itr) const {
  return row->GetRowId().Get() == itr.row->GetRowId().Get();
}

bool TableIterator::operator!=(const TableIterator &itr) const {
  return !(*this == itr);
}

const Row &TableIterator::operator*() {
  //  ASSERT(false, "Not implemented yet.");
  ASSERT(*this != table_heap->End(), "TableHeap iterator out of range, invalid dereference.");
  return *row;
}

Row *TableIterator::operator->() {
  ASSERT(*this != table_heap->End(), "TableHeap iterator out of range, invalid dereference.");
  return row;
}

TableIterator &TableIterator::operator++() {
  (*this)++;
  return *this;
}

TableIterator TableIterator::operator++(int) {  // postfix
  TableIterator that(*this);

  auto cur_page = (TablePage *)table_heap->buffer_pool_manager_->FetchPage(row->GetRowId().GetPageId());

  TablePage *next_page;
  RowId next_row_id = INVALID_ROWID;

  /**
   * end of the cur page, we get no valid row after current row,
   * we need to look for the next page and move to it
   */

  if (!cur_page->GetNextTupleRid(row->GetRowId(), &next_row_id)) {
    while (cur_page->GetPageId() != INVALID_PAGE_ID) {
      next_page = (TablePage *)table_heap->buffer_pool_manager_->FetchPage(cur_page->GetNextPageId());

      cur_page = next_page;
      // successfully get the next row
      if (cur_page->GetFirstTupleRid(&next_row_id)) {
        break;
      }
    }
  }

  row->SetRowId(next_row_id);  // move to next row

  if (*this != table_heap->End()) {
    table_heap->GetTuple(row, txn);
  }

  table_heap->buffer_pool_manager_->UnpinPage(row->GetRowId().GetPageId(), false);
  return that;
}
