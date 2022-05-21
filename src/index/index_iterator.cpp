#include "index/basic_comparator.h"
#include "index/generic_key.h"
#include "index/index_iterator.h"

INDEX_TEMPLATE_ARGUMENTS INDEXITERATOR_TYPE::IndexIterator(BufferPoolManager *buffer_pool_manager, 
        Page *page, int index) :
        buffer_pool_manager_(buffer_pool_manager),
        page_(page),
        index_(index)
{
  leaf_page_ = reinterpret_cast<LeafPage *>(page->GetData());
}

INDEX_TEMPLATE_ARGUMENTS INDEXITERATOR_TYPE::~IndexIterator() 
{
  page_->RUnlatch();
  buffer_pool_manager_->UnpinPage(page_->GetPageId(), false);
}

INDEX_TEMPLATE_ARGUMENTS const MappingType &INDEXITERATOR_TYPE::operator*() 
{
  // ASSERT(false, "Not implemented yet.");
  return leaf_page_->GetItem(index_);
}

INDEX_TEMPLATE_ARGUMENTS INDEXITERATOR_TYPE &INDEXITERATOR_TYPE::operator++() 
{
  if (index_ == leaf_page_->GetSize() - 1 
      && leaf_page_->GetNextPageId() != INVALID_PAGE_ID) 
  {
    Page* next_page = buffer_pool_manager_->FetchPage(leaf_page_->GetNextPageId());
    next_page->RLatch();
    page_->RUnlatch();
    buffer_pool_manager_->UnpinPage(page_->GetPageId(), false);
    page_ = next_page;
    leaf_page_ = reinterpret_cast<LeafPage *>(page_->GetData());
    index_ = 0;
  } 
  else 
  {
    index_++;
  }

  return *this;
}

INDEX_TEMPLATE_ARGUMENTS
bool INDEXITERATOR_TYPE::operator==(const IndexIterator &itr) const 
{
  if(itr.index_ == this->index_ 
    && itr.leaf_page_->GetPageId() == this->leaf_page_->GetPageId())
  {
    return true;
  }
  return false;
}

INDEX_TEMPLATE_ARGUMENTS
bool INDEXITERATOR_TYPE::operator!=(const IndexIterator &itr) const 
{
  if(itr.index_ == this->index_ 
    && itr.leaf_page_->GetPageId() == this->leaf_page_->GetPageId())
  {
    return false;
  }
  return true;
  // return false;
}

template
class IndexIterator<int, int, BasicComparator<int>>;

template
class IndexIterator<GenericKey<4>, RowId, GenericComparator<4>>;

template
class IndexIterator<GenericKey<8>, RowId, GenericComparator<8>>;

template
class IndexIterator<GenericKey<16>, RowId, GenericComparator<16>>;

template
class IndexIterator<GenericKey<32>, RowId, GenericComparator<32>>;

template
class IndexIterator<GenericKey<64>, RowId, GenericComparator<64>>;
