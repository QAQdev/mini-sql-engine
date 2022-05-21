#include "index/basic_comparator.h"
#include "index/generic_key.h"
#include "page/b_plus_tree_internal_page.h"

// huangzheng 22.5.10  22:41
// insertion (line 98)
// 22.5.18 15:00
// All

/****************************************************************************
 * HELPER METHODS AND UTILITIES
 *****************************************************************************/
/*
 * Init method after creating a new internal page
 * Including set page type, set current size, set page id, set parent id and set
 * max page size
 */
INDEX_TEMPLATE_ARGUMENTS 
void B_PLUS_TREE_INTERNAL_PAGE_TYPE::Init(page_id_t page_id, page_id_t parent_id, int max_size) 
{
  this->SetPageType(IndexPageType::INTERNAL_PAGE);
  this->SetSize(0);
  this->SetPageId(page_id);
  this->SetParentPageId(parent_id);
  this->SetMaxSize(max_size);
}
/*
 * Helper method to get/set the key associated with input "index"(a.k.a
 * array offset)
 */
INDEX_TEMPLATE_ARGUMENTS
KeyType B_PLUS_TREE_INTERNAL_PAGE_TYPE::KeyAt(int index) const 
{
  // replace with your own code
  // KeyType key{};
  // return key;
  return this->array_[index].first;
}

INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_INTERNAL_PAGE_TYPE::SetKeyAt(int index, const KeyType &key) 
{
  this->array_[index].first = key;
}

/*
 * Helper method to find and return array index(or offset), so that its value
 * equals to input "value"
 */
INDEX_TEMPLATE_ARGUMENTS
int B_PLUS_TREE_INTERNAL_PAGE_TYPE::ValueIndex(const ValueType &value) const 
{
  for(int i=1; i<= this->GetSize(); i++)
  {
    if(value == this->array_[i].second)
    {
      return i;
    }
  }
  return 0;
}

/*
 * Helper method to get the value associated with input "index"(a.k.a array
 * offset)
 */
INDEX_TEMPLATE_ARGUMENTS
ValueType B_PLUS_TREE_INTERNAL_PAGE_TYPE::ValueAt(int index) const 
{
  // replace with your own code
  // ValueType val{};
  // return val;
  return this->array_[index].second;
}

/*****************************************************************************
 * LOOKUP
 *****************************************************************************/
/*
 * Find and return the child pointer(page_id) which points to the child page
 * that contains input "key"
 * Start the search from the second key(the first key should always be invalid)
 */
INDEX_TEMPLATE_ARGUMENTS
ValueType B_PLUS_TREE_INTERNAL_PAGE_TYPE::Lookup
(const KeyType &key, const KeyComparator &comparator) const 
{
  // replace with your own code
  // ValueType val{};
  // return val;
  for(int i=1; i<this->GetSize(); i++)
  {
    if(comparator(key, this->array_[i].first) < 0)
    {
      return this->array_[i-1].second;
    }
  }
  return this->array_[this->GetSize()-1].second;
}

/*****************************************************************************
 * INSERTION
 *****************************************************************************/
/*
 * Populate new root page with old_value + new_key & new_value
 * When the insertion cause overflow from leaf page all the way upto the root
 * page, you should create a new root page and populate its elements.
 * NOTE: This method is only called within InsertIntoParent()(b_plus_tree.cpp)
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_INTERNAL_PAGE_TYPE::PopulateNewRoot
(const ValueType &old_value, const KeyType &new_key,
const ValueType &new_value) 
{
  array_[0].second = old_value;
  array_[1].first = new_key;
  array_[1].second = new_value;
  this->SetSize(2);
}

/*
 * Insert new_key & new_value pair right after the pair with its value ==
 * old_value
 * @return:  new size after insertion
 */
INDEX_TEMPLATE_ARGUMENTS
int B_PLUS_TREE_INTERNAL_PAGE_TYPE::InsertNodeAfter
(const ValueType &old_value, const KeyType &new_key,
const ValueType &new_value) 
{
  int old_value_index = this->ValueIndex(old_value);
  std::move_backward(array_+old_value_index+1, array_+GetSize(), array_+GetSize()+1);
  array_[old_value_index+1].first = new_key;
  array_[old_value_index+1].second = new_value;
  IncreaseSize(1);
  return this->GetSize();
}

/*****************************************************************************
 * SPLIT
 *****************************************************************************/
/*
 * Remove half of key & value pairs from this page to "recipient" page
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_INTERNAL_PAGE_TYPE::MoveHalfTo
(BPlusTreeInternalPage *recipient,
BufferPoolManager *buffer_pool_manager) 
{
  int start_index = this->GetMinSize();
  this->SetSize(start_index);
  recipient->CopyNFrom(array_+start_index, 
    GetMaxSize()-start_index, buffer_pool_manager);
}

/* Copy entries into me, starting from {items} and copy {size} entries.
 * Since it is an internal page, for all entries (pages) moved, their parents page now changes to me.
 * So I need to 'adopt' them by changing their parent page id, which needs to be persisted with BufferPoolManger
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_INTERNAL_PAGE_TYPE::CopyNFrom
(MappingType *items, int size, BufferPoolManager *buffer_pool_manager) 
{
  std::copy(items, items + size, array_ + GetSize());
  for (int i = 0; i < size; i++) 
  {
    Page* fth_page = buffer_pool_manager->FetchPage(ValueAt(i + GetSize()));
    BPlusTreePage* tmp_node = reinterpret_cast<BPlusTreePage *>(fth_page->GetData());
    tmp_node->SetParentPageId(this->GetPageId());
    buffer_pool_manager->UnpinPage(fth_page->GetPageId(), true);
  }
  IncreaseSize(size);
}

/*****************************************************************************
 * REMOVE
 *****************************************************************************/
/*
 * Remove the key & value pair in internal page according to input index(a.k.a
 * array offset)
 * NOTE: store key&value pair continuously after deletion
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_INTERNAL_PAGE_TYPE::Remove(int index) 
{
  std::move(array_+index+1, array_+GetSize(), array_+index);
  this->IncreaseSize(-1);
}

/*
 * Remove the only key & value pair in internal page and return the value
 * NOTE: only call this method within AdjustRoot()(in b_plus_tree.cpp)
 */
INDEX_TEMPLATE_ARGUMENTS
ValueType B_PLUS_TREE_INTERNAL_PAGE_TYPE::RemoveAndReturnOnlyChild() 
{
  // replace with your own code
  ValueType val = this->ValueAt(0);
  this->SetSize(0);
  return val;
}

/*****************************************************************************
 * MERGE
 *****************************************************************************/
/*
 * Remove all of key & value pairs from this page to "recipient" page.
 * The middle_key is the separation key you should get from the parent. You need
 * to make sure the middle key is added to the recipient to maintain the invariant.
 * You also need to use BufferPoolManager to persist changes to the parent page id for those
 * pages that are moved to the recipient
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_INTERNAL_PAGE_TYPE::MoveAllTo
(BPlusTreeInternalPage *recipient, const KeyType &middle_key,
BufferPoolManager *buffer_pool_manager) 
{
  this->SetKeyAt(0, middle_key);
  recipient->CopyNFrom(array_, GetSize(), buffer_pool_manager);
  this->SetSize(0);
}

/*****************************************************************************
 * REDISTRIBUTE
 *****************************************************************************/
/*
 * Remove the first key & value pair from this page to tail of "recipient" page.
 *
 * The middle_key is the separation key you should get from the parent. You need
 * to make sure the middle key is added to the recipient to maintain the invariant.
 * You also need to use BufferPoolManager to persist changes to the parent page id for those
 * pages that are moved to the recipient
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_INTERNAL_PAGE_TYPE::MoveFirstToEndOf
(BPlusTreeInternalPage *recipient, const KeyType &middle_key,
BufferPoolManager *buffer_pool_manager) 
{
  SetKeyAt(0, middle_key);
  MappingType first_item = *array_;
  recipient->CopyLastFrom(first_item, buffer_pool_manager);
  this->Remove(0);
}

/* Append an entry at the end.
 * Since it is an internal page, the moved entry(page)'s parent needs to be updated.
 * So I need to 'adopt' it by changing its parent page id, which needs to be persisted with BufferPoolManger
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_INTERNAL_PAGE_TYPE::CopyLastFrom
(const MappingType &pair, BufferPoolManager *buffer_pool_manager) 
{
  array_[GetSize()] = pair;
  this->IncreaseSize(1);
  Page* fth_page = buffer_pool_manager->FetchPage(pair.second);
  BPlusTreePage* tmp_node = reinterpret_cast<BPlusTreePage*>(fth_page->GetData());
  tmp_node->SetParentPageId(this->GetPageId());
  buffer_pool_manager->UnpinPage(fth_page->GetPageId(), true);
}

/*
 * Remove the last key & value pair from this page to head of "recipient" page.
 * You need to handle the original dummy key properly, e.g. updating recipient’s array to position the middle_key at the
 * right place.
 * You also need to use BufferPoolManager to persist changes to the parent page id for those pages that are
 * moved to the recipient
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_INTERNAL_PAGE_TYPE::MoveLastToFrontOf
(BPlusTreeInternalPage *recipient, const KeyType &middle_key,
BufferPoolManager *buffer_pool_manager) 
{
  MappingType array_last_item = array_[GetSize()-1];
  recipient->SetKeyAt(0, middle_key);
  recipient->CopyFirstFrom(array_last_item, buffer_pool_manager);
  IncreaseSize(-1);
}

/* Append an entry at the beginning.
 * Since it is an internal page, the moved entry(page)'s parent needs to be updated.
 * So I need to 'adopt' it by changing its parent page id, which needs to be persisted with BufferPoolManger
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_INTERNAL_PAGE_TYPE::CopyFirstFrom
(const MappingType &pair, BufferPoolManager *buffer_pool_manager) 
{
  std::move_backward(array_, array_ + GetSize(), array_ + GetSize() + 1);
  array_[0] = pair;
  IncreaseSize(1);
  Page* fth_page = buffer_pool_manager->FetchPage(pair.second);
  BPlusTreePage* tmp_node = reinterpret_cast<BPlusTreePage *>(fth_page->GetData());
  tmp_node->SetParentPageId(GetPageId());
  buffer_pool_manager->UnpinPage(fth_page->GetPageId(), true);
}

template
class BPlusTreeInternalPage<int, int, BasicComparator<int>>;

template
class BPlusTreeInternalPage<GenericKey<4>, page_id_t, GenericComparator<4>>;

template
class BPlusTreeInternalPage<GenericKey<8>, page_id_t, GenericComparator<8>>;

template
class BPlusTreeInternalPage<GenericKey<16>, page_id_t, GenericComparator<16>>;

template
class BPlusTreeInternalPage<GenericKey<32>, page_id_t, GenericComparator<32>>;

template
class BPlusTreeInternalPage<GenericKey<64>, page_id_t, GenericComparator<64>>;