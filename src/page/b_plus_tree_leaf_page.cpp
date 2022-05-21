#include <algorithm>
#include "index/basic_comparator.h"
#include "index/generic_key.h"
#include "page/b_plus_tree_leaf_page.h"

// huangzheng 22.5.18  13.25
// All

/*****************************************************************************
 * HELPER METHODS AND UTILITIES
 *****************************************************************************/

/**
 * Init method after creating a new leaf page
 * Including set page type, set current size to zero, set page id/parent id, set
 * next page id and set max size
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_LEAF_PAGE_TYPE::Init(page_id_t page_id, 
page_id_t parent_id, int max_size) 
{
  this->SetPageId(page_id);
  this->SetParentPageId(parent_id);
  this->SetMaxSize(max_size);
  this->next_page_id_ = INVALID_PAGE_ID;
  this->SetSize(0);
  this->SetPageType(IndexPageType::LEAF_PAGE);
}

/**
 * Helper methods to set/get next page id
 */
INDEX_TEMPLATE_ARGUMENTS
page_id_t B_PLUS_TREE_LEAF_PAGE_TYPE::GetNextPageId() const 
{
  return this->next_page_id_;
}

INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_LEAF_PAGE_TYPE::SetNextPageId(page_id_t next_page_id) 
{
  this->next_page_id_ = next_page_id;
}

/**
 * Helper method to find the first index i so that 
 * array_[i].first >= key
 * NOTE: This method is only used when generating index iterator
 */
INDEX_TEMPLATE_ARGUMENTS
int B_PLUS_TREE_LEAF_PAGE_TYPE::KeyIndex(const KeyType &key, 
const KeyComparator &comparator) const 
{
  int i;
  // if(comparator(key, array_[0].first) < 0)
  // {
  //   return this->GetSize();
  // }
  for(i=0; i<this->GetSize(); i++)
  {
    if(comparator(key, array_[i].first) <= 0)
      return i;
  }
  return i;
}

/*
 * Helper method to find and return the key associated with input "index"(a.k.a
 * array offset)
 */
INDEX_TEMPLATE_ARGUMENTS
KeyType B_PLUS_TREE_LEAF_PAGE_TYPE::KeyAt(int index) const 
{
  // replace with your own code
  // KeyType key{};
  // return key;
  return this->array_[index].first;
}

/*
 * Helper method to find and return the key & value pair associated with input
 * "index"(a.k.a array offset)
 */
INDEX_TEMPLATE_ARGUMENTS
const MappingType &B_PLUS_TREE_LEAF_PAGE_TYPE::GetItem(int index) 
{
  // replace with your own code
  return array_[index];
}

/*****************************************************************************
 * INSERTION
 *****************************************************************************/
/*
 * Insert key & value pair into leaf page ordered by key
 * @return page size after insertion
 */
INDEX_TEMPLATE_ARGUMENTS
int B_PLUS_TREE_LEAF_PAGE_TYPE::Insert(const KeyType &key, 
const ValueType &value, const KeyComparator &comparator) 
{
  int key_index = this->KeyIndex(key, comparator);
  if(key_index == this->GetSize())
  {
    array_[key_index].first = key;
    array_[key_index].second = value;
    this->IncreaseSize(1);
    return this->GetSize();
  }
  if(comparator(array_[key_index].first, key) == 0)
  {
    return this->GetSize();
  }

  std::move_backward(array_+key_index,
  array_ + this->GetSize(), array_ + this->GetSize() + 1);

  array_[key_index].first = key;
  array_[key_index].second = value;

  this->IncreaseSize(1);
  return this->GetSize();
  // return 0;
}

/*****************************************************************************
 * SPLIT
 *****************************************************************************/
/*
 * Remove half of key & value pairs from this page to "recipient" page
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_LEAF_PAGE_TYPE::MoveHalfTo(BPlusTreeLeafPage *recipient) 
{
  int first_half_index = this->GetMinSize();
  int second_half_size = this->GetMaxSize() - first_half_index;
  recipient->CopyNFrom(array_+first_half_index, second_half_size);
  this->IncreaseSize(-second_half_size);
}

/*
 * Copy starting from items, and copy {size} number of elements into me.
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_LEAF_PAGE_TYPE::CopyNFrom(MappingType *items, int size) 
{
  std::copy(items, items+size, array_+this->GetSize());
  this->IncreaseSize(size);
}

/*****************************************************************************
 * LOOKUP
 *****************************************************************************/
/*
 * For the given key, check to see whether it exists in the leaf page. If it
 * does, then store its corresponding value in input "value" and return true.
 * If the key does not exist, then return false
 */
INDEX_TEMPLATE_ARGUMENTS
bool B_PLUS_TREE_LEAF_PAGE_TYPE::Lookup(const KeyType &key, 
ValueType &value, const KeyComparator &comparator) const 
{
  // return false;
  int key_index = this->KeyIndex(key, comparator);
  if(key_index == this->GetSize())
  {
    return false;
  }
  else if(comparator(this->KeyAt(key_index), key) != 0)
  {
    return false;
  }
  else 
  {
    value = array_[key_index].second; 
    return true;
  }
  return false;
}

/*****************************************************************************
 * REMOVE
 *****************************************************************************/
/*
 * First look through leaf page to see whether delete key exist or not. If
 * exist, perform deletion, otherwise return immediately.
 * NOTE: store key&value pair continuously after deletion
 * @return  page size after deletion
 */
INDEX_TEMPLATE_ARGUMENTS
int B_PLUS_TREE_LEAF_PAGE_TYPE::RemoveAndDeleteRecord(const KeyType &key, 
const KeyComparator &comparator) 
{
  int key_index = this->KeyIndex(key, comparator);
  if(key_index == this->GetSize())
  {
    return this->GetSize();
  }
  else if(comparator(key, this->KeyAt(key_index)))
  {
    return this->GetSize();
  }
  else 
  {
    std::move(array_+key_index+1, array_+GetSize(), array_+key_index);
    this->IncreaseSize(-1);
    return this->GetSize();
  }

  return this->GetSize();
}

/*****************************************************************************
 * MERGE
 *****************************************************************************/
/*
 * Remove all of key & value pairs from this page to "recipient" page. 
 * Don't forget
 * to update the next_page id in the sibling page
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_LEAF_PAGE_TYPE::MoveAllTo(BPlusTreeLeafPage *recipient) 
{
  recipient->CopyNFrom(array_, this->GetSize());
  recipient->SetNextPageId(this->GetNextPageId());
  this->IncreaseSize(-1*this->GetSize());
  return ;
}

/*****************************************************************************
 * REDISTRIBUTE
 *****************************************************************************/
/*
 * Remove the first key & value pair from this page to "recipient" page.
 *
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_LEAF_PAGE_TYPE::MoveFirstToEndOf(BPlusTreeLeafPage *recipient) 
{
  MappingType first_item = GetItem(0);
  std::move(array_ + 1, array_ + GetSize(), array_);
  IncreaseSize(-1);

  recipient->CopyLastFrom(first_item);
}

/*
 * Copy the item into the end of my item list. (Append item to my array)
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_LEAF_PAGE_TYPE::CopyLastFrom(const MappingType &item) 
{
  array_[this->GetSize()] = item;
  this->IncreaseSize(1);
}

/*
 * Remove the last key & value pair from this page to "recipient" page.
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_LEAF_PAGE_TYPE::MoveLastToFrontOf(BPlusTreeLeafPage *recipient) 
{
  MappingType last_item = array_[GetSize()-1];
  this->IncreaseSize(-1);
  recipient->CopyFirstFrom(last_item);
}

/*
 * Insert item at the front of my items. Move items accordingly.
 *
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_LEAF_PAGE_TYPE::CopyFirstFrom(const MappingType &item) 
{
  std::move_backward(array_, array_+GetSize(), array_+GetSize()+1);
  array_[0] = item;
  IncreaseSize(1);
}

template
class BPlusTreeLeafPage<int, int, BasicComparator<int>>;

template
class BPlusTreeLeafPage<GenericKey<4>, RowId, GenericComparator<4>>;

template
class BPlusTreeLeafPage<GenericKey<8>, RowId, GenericComparator<8>>;

template
class BPlusTreeLeafPage<GenericKey<16>, RowId, GenericComparator<16>>;

template
class BPlusTreeLeafPage<GenericKey<32>, RowId, GenericComparator<32>>;

template
class BPlusTreeLeafPage<GenericKey<64>, RowId, GenericComparator<64>>;