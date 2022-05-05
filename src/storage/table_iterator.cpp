#include "common/macros.h"
#include "storage/table_iterator.h"
#include "storage/table_heap.h"

TableIterator::TableIterator() {

}

TableIterator::TableIterator(const TableIterator &other) {

}

TableIterator::~TableIterator() {

}

bool TableIterator::operator==(const TableIterator &itr) const {
  return false;
}

bool TableIterator::operator!=(const TableIterator &itr) const {
  return false;
}

const Row &TableIterator::operator*() {
  ASSERT(false, "Not implemented yet.");
}

Row *TableIterator::operator->() {
  return nullptr;
}

TableIterator &TableIterator::operator++() {
  return *this;
}

TableIterator TableIterator::operator++(int) {
  return TableIterator();
}
