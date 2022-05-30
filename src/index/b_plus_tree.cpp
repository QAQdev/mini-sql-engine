#include <string>
#include "glog/logging.h"
#include "index/b_plus_tree.h"
#include "index/b_plus_tree_index.h"
#include "index/basic_comparator.h"
#include "index/generic_key.h"
#include "page/index_roots_page.h"
#include <algorithm>
#include <stack>

INDEX_TEMPLATE_ARGUMENTS
BPLUSTREE_TYPE::BPlusTree(index_id_t index_id, 
BufferPoolManager *buffer_pool_manager, const KeyComparator &comparator,
int leaf_max_size, int internal_max_size):
  index_id_(index_id),
  buffer_pool_manager_(buffer_pool_manager),
  comparator_(comparator),
  leaf_max_size_(leaf_max_size),
  internal_max_size_(internal_max_size) {
  auto root_page = reinterpret_cast<IndexRootsPage *>(buffer_pool_manager->FetchPage(INDEX_ROOTS_PAGE_ID));

  if (!root_page->GetRootId(index_id, &this->root_page_id_)) {
    this->root_page_id_ = INVALID_PAGE_ID;
  }
  buffer_pool_manager->UnpinPage(INDEX_ROOTS_PAGE_ID, true);
  buffer_pool_manager_->UnpinPage(root_page_id_, true);
}

// new added @hz
INDEX_TEMPLATE_ARGUMENTS
void BPLUSTREE_TYPE::ClrDeletePages()
{
  for(int i=0; i<int(delete_pages.size()); i++)
  {
    buffer_pool_manager_->DeletePage(delete_pages[i]);
  }
  delete_pages.clear();
  return ;
}

INDEX_TEMPLATE_ARGUMENTS
void BPLUSTREE_TYPE::Destroy() {
  // if(IsEmpty()){
  //   return;
  // }
  // std::stack<int> s;
  // InternalPage* page=reinterpret_cast<InternalPage*>(buffer_pool_manager_->FetchPage(root_page_id_));
  // int index=0;//get the root page
  // if(page->GetPageType()==IndexPageType::LEAF_PAGE){
  //   buffer_pool_manager_->UnpinPage(root_page_id_,false);
  //   buffer_pool_manager_->DeletePage(root_page_id_);
  //   return;//if the root is leaf,delete it directly and return back
  // }

  // while(true){
  //   if(index==page->GetSize()){
  //     if(ReturnFatherState(index,page,s)){
  //       break;//all the children of the internal page has been deleted
  //     }//if this page is root,end
  //   }else{
  //     page_id_t son=reinterpret_cast<page_id_t>(page->ValueAt(index));
  //     InternalPage* temp=reinterpret_cast<InternalPage*>(buffer_pool_manager_->FetchPage(son));
  //     if(temp->GetPageType()==IndexPageType::LEAF_PAGE){
  //       buffer_pool_manager_->UnpinPage(son,false);
  //       int size=page->GetSize();
  //       for(int i=0;i<size;i++){
  //         buffer_pool_manager_->DeletePage(page->ValueAt(i));
  //       }//if its son is leaf,delete all of them
  //       if(ReturnFatherState(index,page,s)){
  //         break;//delete itself and return to its father
  //       }
  //     }else{
  //       s.push(index);
  //       index=0;
  //       buffer_pool_manager_->UnpinPage(page->GetPageId(),false);
  //       page=temp;//continue pro-travel
  //     }
  //   }
  // }

  // root_page_id_=INVALID_PAGE_ID;
  // UpdateRootPageId(0);//delete the information in the index page
}

/*
 * Helper function to decide whether current b+tree is empty
 */
INDEX_TEMPLATE_ARGUMENTS
bool BPLUSTREE_TYPE::IsEmpty() const 
{
  if(root_page_id_ == INVALID_PAGE_ID)
  {
    return true;
  }
  return false;
}

/*****************************************************************************
 * SEARCH
 *****************************************************************************/

/*
 * Return the only value that associated with input key
 * This method is used for point query
 * @return : true means key exists
 */

// unsure

INDEX_TEMPLATE_ARGUMENTS
bool BPLUSTREE_TYPE::GetValue(const KeyType &key, 
std::vector<ValueType> &result, Transaction *transaction) 
{
  if(this->IsEmpty())
  {
    return false;
  }
  Page* leaf_page = this->FindLeafPage(key);
  LeafPage *leaf_node = reinterpret_cast<LeafPage *>(leaf_page->GetData());

  ValueType tmp_value;
  if(!leaf_node->Lookup(key, tmp_value, comparator_))
  {
    leaf_page->RUnlatch();
    buffer_pool_manager_->UnpinPage(leaf_page->GetPageId(), false);
    return false;
  }
  else 
  {
    leaf_page->RUnlatch();
    buffer_pool_manager_->UnpinPage(leaf_page->GetPageId(), false);
    result.push_back(tmp_value);
    return true;
  }
}

/*****************************************************************************
 * INSERTION
 *****************************************************************************/
/*
 * Insert constant key & value pair into b+ tree
 * if current tree is empty, start new tree, update root page id and insert
 * entry, otherwise insert into leaf page.
 * @return: since we only support unique key, if user try to insert duplicate
 * keys return false, otherwise return true.
 */
INDEX_TEMPLATE_ARGUMENTS
bool BPLUSTREE_TYPE::Insert(const KeyType &key, 
const ValueType &value, Transaction *transaction) 
{
  if (IsEmpty()) 
  {
    StartNewTree(key, value);
    return true;
  }

  return InsertIntoLeaf(key, value, transaction);
  // return false;
}
/*
 * Insert constant key & value pair into an empty tree
 * User needs to first ask for new page from buffer pool manager(NOTICE: throw
 * an "out of memory" exception if returned value is nullptr), then update b+
 * tree's root page id and insert entry directly into leaf page.
 */

/*                        *\
  hz/ no exception throws
          unsure
\*                        */
INDEX_TEMPLATE_ARGUMENTS
void BPLUSTREE_TYPE::StartNewTree(const KeyType &key, 
const ValueType &value) 
{
  Page* root_page = buffer_pool_manager_->NewPage(root_page_id_);

  LeafPage *leaf_page = reinterpret_cast<LeafPage *>(root_page->GetData());
  leaf_page->Init(root_page_id_, INVALID_PAGE_ID, leaf_max_size_);
  leaf_page->Insert(key, value, comparator_);

  buffer_pool_manager_->UnpinPage(leaf_page->GetPageId(), true);

  UpdateRootPageId(1);
}

/*
 * Insert constant key & value pair into leaf page
 * User needs to first find the right leaf page as insertion target, then look
 * through leaf page to see whether insert key exist or not. If exist, return
 * immediately, otherwise insert entry. Remember to deal with split if necessary.
 * @return: since we only support unique key, if user try to insert duplicate
 * keys return false, otherwise return true.
 */


// hz not filled for
// transaction

INDEX_TEMPLATE_ARGUMENTS
bool BPLUSTREE_TYPE::InsertIntoLeaf(const KeyType &key, 
const ValueType &value, Transaction *transaction) 
{
  Page* find_leaf_page = this->FindLeafPage(key);
  LeafPage* tmp_leaf_page = reinterpret_cast<LeafPage*>(find_leaf_page->GetData());
  int org_size = tmp_leaf_page->GetSize();
  int new_size = tmp_leaf_page->Insert(key, value, comparator_);

  if (new_size == org_size) 
  {
    find_leaf_page->WUnlatch();
    buffer_pool_manager_->UnpinPage(find_leaf_page->GetPageId(), false);
    return false;
  }
  else if (new_size < leaf_max_size_) 
  {
    find_leaf_page->WUnlatch();
    buffer_pool_manager_->UnpinPage(find_leaf_page->GetPageId(), true);
    return true;
  }
  else 
  {
    auto sibling_leaf_node = Split(tmp_leaf_page);
    sibling_leaf_node->SetNextPageId(tmp_leaf_page->GetNextPageId());
    tmp_leaf_page->SetNextPageId(sibling_leaf_node->GetPageId());
    KeyType risen_key = sibling_leaf_node->KeyAt(0);
    InsertIntoParent(tmp_leaf_page, risen_key, sibling_leaf_node, transaction);
    find_leaf_page->WUnlatch();
    buffer_pool_manager_->UnpinPage(find_leaf_page->GetPageId(), true);
    buffer_pool_manager_->UnpinPage(sibling_leaf_node->GetPageId(), true);
    return true;
  }
  return false;
}

/*
 * Split input page and return newly created page.
 * Using template N to represent either internal page or leaf page.
 * User needs to first ask for new page from buffer pool manager(NOTICE: throw
 * an "out of memory" exception if returned value is nullptr), then move half
 * of key & value pairs from input page to newly created page
 */

// hz
// no exception
// 
// if (page == nullptr) 
//   throw Exception(ExceptionType::OUT_OF_MEMORY, "Cannot allocate new page");


INDEX_TEMPLATE_ARGUMENTS
template<typename N>
N *BPLUSTREE_TYPE::Split(N *node) 
{
  page_id_t new_page_id;
  Page* new_page = buffer_pool_manager_->NewPage(new_page_id);
  N* new_node = reinterpret_cast<N*>(new_page->GetData());

  if (node->IsLeafPage()) 
  {
    new_node->SetPageType(IndexPageType::LEAF_PAGE);
    LeafPage *leaf = reinterpret_cast<LeafPage *>(node);
    LeafPage *new_leaf = reinterpret_cast<LeafPage *>(new_node);
    new_leaf->Init(new_page->GetPageId(), node->GetParentPageId(), 
          leaf_max_size_);
    leaf->MoveHalfTo(new_leaf);
  } 
  else 
  {
    new_node->SetPageType(IndexPageType::INTERNAL_PAGE);
    InternalPage *internal = reinterpret_cast<InternalPage *>(node);
    InternalPage *new_internal = reinterpret_cast<InternalPage *>(new_node);
    new_internal->Init(new_page->GetPageId(), node->GetParentPageId(), 
          internal_max_size_);
    internal->MoveHalfTo(new_internal, buffer_pool_manager_);
  }
  // buffer_pool_manager_->UnpinPage(new_page_id, true);
  return new_node;
  // return nullptr;
}

/*
 * Insert key & value pair into internal page after split
 * @param   old_node      input page from split() method
 * @param   key
 * @param   new_node      returned page from split() method
 * User needs to first find the parent page of old_node, parent node must be
 * adjusted to take info of new_node into account. Remember to deal with split
 * recursively if necessary.
 */

// no expection
// if (new_page == nullptr) 
// {
//   throw Exception(ExceptionType::OUT_OF_MEMORY, "Cannot allocate new page");
// }

INDEX_TEMPLATE_ARGUMENTS
void BPLUSTREE_TYPE::InsertIntoParent(BPlusTreePage *old_node, 
const KeyType &key, BPlusTreePage *new_node,
Transaction *transaction) 
{
  if (old_node->IsRootPage()) 
  {
    Page* new_page = buffer_pool_manager_->NewPage(root_page_id_);
    InternalPage *new_root = reinterpret_cast<InternalPage *>(new_page->GetData());
    new_root->Init(root_page_id_, INVALID_PAGE_ID, internal_max_size_);
    new_root->PopulateNewRoot(old_node->GetPageId(), key, new_node->GetPageId());
    old_node->SetParentPageId(new_root->GetPageId());
    new_node->SetParentPageId(new_root->GetPageId());
    buffer_pool_manager_->UnpinPage(new_page->GetPageId(), true);
    UpdateRootPageId(0);
    return;
  }

  Page* parent_page = buffer_pool_manager_->FetchPage(old_node->GetParentPageId());
  InternalPage *parent_node = reinterpret_cast<InternalPage *>(parent_page->GetData());
  int new_size = parent_node->InsertNodeAfter(old_node->GetPageId(), 
                                        key, new_node->GetPageId());

  if (new_size < internal_max_size_) 
  {
    buffer_pool_manager_->UnpinPage(parent_page->GetPageId(), true);
    return;
  }
  auto parent_new_sibling_node = Split(parent_node);
  KeyType new_key = parent_new_sibling_node->KeyAt(0);
  InsertIntoParent(parent_node, new_key, parent_new_sibling_node, transaction);

  buffer_pool_manager_->UnpinPage(parent_page->GetPageId(), true);
  buffer_pool_manager_->UnpinPage(parent_new_sibling_node->GetPageId(), true);
}


/*****************************************************************************
 * REMOVE
 *****************************************************************************/
/*
 * Delete key & value pair associated with input key
 * If current tree is empty, return immediately.
 * If not, User needs to first find the right leaf page as deletion target, then
 * delete entry from leaf page. Remember to deal with redistribute or merge if
 * necessary.
 */
INDEX_TEMPLATE_ARGUMENTS
void BPLUSTREE_TYPE::Remove(const KeyType &key, Transaction *transaction) 
{
  if (IsEmpty()) 
  {
    return;
  }

  Page* leaf_page = FindLeafPage(key);
  LeafPage *node = reinterpret_cast<LeafPage *>(leaf_page->GetData());
  int org_size = node->GetSize();

  if (org_size == node->RemoveAndDeleteRecord(key, comparator_)) 
  {
    leaf_page->WUnlatch();
    buffer_pool_manager_->UnpinPage(leaf_page->GetPageId(), false);
    return;
  }
  else 
  {
    leaf_page->WUnlatch();
    bool if_deletion_occur = CoalesceOrRedistribute(node, transaction);
    if(if_deletion_occur)
    {
      delete_pages.push_back(node->GetPageId());
    }
    buffer_pool_manager_->UnpinPage(leaf_page->GetPageId(), true);
    this->ClrDeletePages();
  }
  return ;
}

/*
 * User needs to first find the sibling of input page. If sibling's size + input
 * page's size > page's max size, then redistribute. Otherwise, merge.
 * Using template N to represent either internal page or leaf page.
 * @return: true means target leaf page should be deleted, false means no
 * deletion happens
 */
INDEX_TEMPLATE_ARGUMENTS
template<typename N>
bool BPLUSTREE_TYPE::CoalesceOrRedistribute(N *node, 
Transaction *transaction) 
{
  if (node->IsRootPage()) 
  {
    bool if_delete_root = AdjustRoot(node);
    return if_delete_root;
  }
  else if (node->GetSize() >= node->GetMinSize()) 
  {
    return false;
  }
  else 
  {}

  Page* fth_parent_page = buffer_pool_manager_->FetchPage(node->GetParentPageId());
  InternalPage* tmp_parent_page 
      = reinterpret_cast<InternalPage *>(fth_parent_page->GetData());
  int index = tmp_parent_page->ValueIndex(node->GetPageId());
  int r_index;
  if(index == 0)
    r_index = 1;
  else 
    r_index = index -1;

  Page* sibling_page = buffer_pool_manager_->FetchPage(
    tmp_parent_page->ValueAt(r_index));
  // sibling_page->WLatch();
  N* sibling_node = reinterpret_cast<N*>(sibling_page->GetData());

  if (node->GetSize() + sibling_node->GetSize() >= node->GetMaxSize()) 
  {
    Redistribute(sibling_node, node, index);
    buffer_pool_manager_->UnpinPage(tmp_parent_page->GetPageId(), true);
    sibling_page->WUnlatch();
    buffer_pool_manager_->UnpinPage(sibling_page->GetPageId(), true);
  // Page* nine_page = buffer_pool_manager_->FetchPage(9);
  // if(nine_page != nullptr)
  // {
  //   LeafPage* nine_leaf_page = reinterpret_cast<LeafPage*>(nine_page);
  //   LOG(INFO) << "CoalesceOrRedistribute" 
  //   << nine_leaf_page->GetSize() << "###" << std::endl;
  // }
    return false;
  }
  else 
  {
    bool if_should_delete = Coalesce(&sibling_node, &node, &tmp_parent_page, index);
    if(if_should_delete)
    {
      delete_pages.push_back(tmp_parent_page->GetPageId());
    }
    buffer_pool_manager_->UnpinPage(tmp_parent_page->GetPageId(), true);
    sibling_page->WUnlatch();
    buffer_pool_manager_->UnpinPage(sibling_page->GetPageId(), true);
  // Page* nine_page = buffer_pool_manager_->FetchPage(9);
  // if(nine_page != nullptr)
  // {
  //   LeafPage* nine_leaf_page = reinterpret_cast<LeafPage*>(nine_page);
  //   LOG(INFO) << "CoalesceOrRedistribute" 
  //   << nine_leaf_page->GetSize() << "###" << std::endl;
  // }
    return true;
  }
  return false;
}

/*
 * Move all the key & value pairs from one page to its sibling page, and notify
 * buffer pool manager to delete this page. Parent page must be adjusted to
 * take info of deletion into account. Remember to deal with coalesce or
 * redistribute recursively if necessary.
 * Using template N to represent either internal page or leaf page.
 * @param   neighbor_node      sibling page of input "node"
 * @param   node               input from method coalesceOrRedistribute()
 * @param   parent             parent page of input "node"
 * @return  true means parent node should be deleted, false means no deletion happened
 */
INDEX_TEMPLATE_ARGUMENTS
template<typename N>
bool BPLUSTREE_TYPE::Coalesce(N **neighbor_node, N **node,
BPlusTreeInternalPage<KeyType, page_id_t, KeyComparator> **parent, int index,
Transaction *transaction) 
{
  int r_index = index;
  if (index == 0) 
  {
    r_index = 1;
    std::swap(node, neighbor_node);
  }

  KeyType middle_key = (*parent)->KeyAt(r_index);

  if ((*node)->IsLeafPage()) 
  {
    LeafPage* leaf_node = reinterpret_cast<LeafPage *>((*node));
    LeafPage* last_leaf_node = reinterpret_cast<LeafPage *>((*neighbor_node));
    leaf_node->MoveAllTo(last_leaf_node);
    // LOG(INFO) << "SIZE:" << last_leaf_node->GetSize() << std::endl;
    // LOG(INFO) << "ID" << last_leaf_node->GetPageId() << std::endl;
    buffer_pool_manager_->UnpinPage(last_leaf_node->GetPageId(), true);
  } 
  else 
  {
    InternalPage *internal_node = reinterpret_cast<InternalPage *>((*node));
    InternalPage *last_internal_node = reinterpret_cast<InternalPage *>((*neighbor_node));
    internal_node->MoveAllTo(last_internal_node, middle_key, buffer_pool_manager_);
  }
  (*parent)->Remove(r_index);

  // Page* nine_page = buffer_pool_manager_->FetchPage(9);
  // if(nine_page != nullptr)
  // {
  //   LeafPage* nine_leaf_page = reinterpret_cast<LeafPage*>(nine_page);
  //   LOG(INFO) << "Coales" 
  //   << nine_leaf_page->GetSize() << "###" << std::endl;
  // }

  bool if_got_deletion = CoalesceOrRedistribute(*parent);
  // nine_page = buffer_pool_manager_->FetchPage(9);
  // if(nine_page != nullptr)
  // {
  //   LeafPage* nine_leaf_page = reinterpret_cast<LeafPage*>(nine_page);
  //   LOG(INFO) << "Coales2#" 
  //   << nine_leaf_page->GetSize() << "###" << std::endl;
  // }
  
  return if_got_deletion;
}


/*
 * Redistribute key & value pairs from one page to its sibling page. If index ==
 * 0, move sibling page's first key & value pair into end of input "node",
 * otherwise move sibling page's last key & value pair into head of input
 * "node".
 * Using template N to represent either internal page or leaf page.
 * @param   neighbor_node      sibling page of input "node"
 * @param   node               input from method coalesceOrRedistribute()
 */
INDEX_TEMPLATE_ARGUMENTS
template<typename N>
void BPLUSTREE_TYPE::Redistribute(N *neighbor_node, N *node, int index) 
{
  if (node->IsLeafPage()) 
  {
    LeafPage *leaf_node = reinterpret_cast<LeafPage *>(node);
    LeafPage *neighbor_leaf_node = reinterpret_cast<LeafPage *>(neighbor_node);
    Page* tmp_parent = buffer_pool_manager_->FetchPage(leaf_node->GetParentPageId());
    InternalPage* parent = reinterpret_cast<InternalPage*>(tmp_parent);

    if (index == 0) 
    {
      neighbor_leaf_node->MoveFirstToEndOf(leaf_node);
      parent->SetKeyAt(1, neighbor_leaf_node->KeyAt(0));
    } 
    else 
    {
      neighbor_leaf_node->MoveLastToFrontOf(leaf_node);
      parent->SetKeyAt(index, leaf_node->KeyAt(0));
    }
    buffer_pool_manager_->UnpinPage(parent->GetPageId(), true);
  } 
  else 
  {
    InternalPage *internal_node = reinterpret_cast<InternalPage *>(node);
    InternalPage *neighbor_internal_node = reinterpret_cast<InternalPage *>(neighbor_node);
    Page* tmp_parent = buffer_pool_manager_->FetchPage(internal_node->GetParentPageId());
    InternalPage* parent = reinterpret_cast<InternalPage*>(tmp_parent);
    if (index == 0) 
    {
      neighbor_internal_node->MoveFirstToEndOf(internal_node, 
        parent->KeyAt(1), buffer_pool_manager_);
      parent->SetKeyAt(1, neighbor_internal_node->KeyAt(0));
    } 
    else 
    {
      neighbor_internal_node->MoveLastToFrontOf(internal_node, 
        parent->KeyAt(index), buffer_pool_manager_);
      parent->SetKeyAt(index, internal_node->KeyAt(0));
    }
    buffer_pool_manager_->UnpinPage(parent->GetPageId(), true);
  }

  return ;
}
/*
 * Update root page if necessary
 * NOTE: size of root page can be less than min size and this method is only
 * called within coalesceOrRedistribute() method
 * case 1: when you delete the last element in root page, but root page still
 * has one last child
 * case 2: when you delete the last element in whole b+ tree
 * @return : true means root page should be deleted, false means no deletion
 * happened
 */

// unsure

INDEX_TEMPLATE_ARGUMENTS
bool BPLUSTREE_TYPE::AdjustRoot(BPlusTreePage *old_root_node)
{
  // Page* nine_page = buffer_pool_manager_->FetchPage(9);
  // if(nine_page != nullptr)
  // {
  //   LeafPage* nine_leaf_page = reinterpret_cast<LeafPage*>(nine_page);
  //   LOG(INFO) << "adj-1#" 
  //   << nine_leaf_page->GetSize() << "###" << std::endl;
  // }
  if (!old_root_node->IsLeafPage() && old_root_node->GetSize() == 1) 
  {
    InternalPage *root_node = 
    reinterpret_cast<InternalPage*>(old_root_node);
    Page* fth_child_page = buffer_pool_manager_->FetchPage(root_node->ValueAt(0));

  // Page* nine_page = buffer_pool_manager_->FetchPage(9);
  // if(nine_page != nullptr)
  // {
  //   LeafPage* nine_leaf_page = reinterpret_cast<LeafPage*>(nine_page);
  //   LOG(INFO) << "adj0#" 
  //   << nine_leaf_page->GetSize() << "###" << std::endl;
  // }
    BPlusTreePage *tmp_child_node 
    = reinterpret_cast<BPlusTreePage*>(fth_child_page->GetData());

  // Page* nine_page = buffer_pool_manager_->FetchPage(9);
  // if(nine_page != nullptr)
  // {
  //   LeafPage* nine_leaf_page = reinterpret_cast<LeafPage*>(nine_page);
  //   LOG(INFO) << "Adj2#" 
  //   << nine_leaf_page->GetSize() << "###" << std::endl;
  // }

    tmp_child_node->SetParentPageId(INVALID_PAGE_ID);
  // Page* nine_page = buffer_pool_manager_->FetchPage(9);
  // if(nine_page != nullptr)
  // {
  //   LeafPage* nine_leaf_page = reinterpret_cast<LeafPage*>(nine_page);
  //   LOG(INFO) << "Adj3#" 
  //   << nine_leaf_page->GetSize() << "###" << std::endl;
  // }
  
    this->root_page_id_ = tmp_child_node->GetPageId();
    UpdateRootPageId(0);
  // Page* nine_page = buffer_pool_manager_->FetchPage(9);
  // if(nine_page != nullptr)
  // {
  //   LeafPage* nine_leaf_page = reinterpret_cast<LeafPage*>(nine_page);
  //   LOG(INFO) << "Adj3#" 
  //   << nine_leaf_page->GetSize() << "###" << std::endl;
  // }

    buffer_pool_manager_->UnpinPage(fth_child_page->GetPageId(), true);
    return true;
  }
  return old_root_node->IsLeafPage() && old_root_node->GetSize() == 0;
  // return false;
}

/*****************************************************************************
 * INDEX ITERATOR
 *****************************************************************************/
/*
 * Input parameter is void, find the left most leaf page first, then construct
 * index iterator
 * @return : index iterator
 */
INDEX_TEMPLATE_ARGUMENTS
INDEXITERATOR_TYPE BPLUSTREE_TYPE::Begin() 
{
  Page* left_most_leaf_page = FindLeafPage(KeyType(), true);
  return INDEXITERATOR_TYPE(buffer_pool_manager_, left_most_leaf_page, 0);
  // return INDEXITERATOR_TYPE();
}

/*
 * Input parameter is low key, find the leaf page that contains the input key
 * first, then construct index iterator
 * @return : index iterator
 */
INDEX_TEMPLATE_ARGUMENTS
INDEXITERATOR_TYPE BPLUSTREE_TYPE::Begin(const KeyType &key) 
{
  Page* found_leaf_page = FindLeafPage(key);
  LeafPage* leaf_node = reinterpret_cast<LeafPage*>(found_leaf_page->GetData());
  int node_index = leaf_node->KeyIndex(key, comparator_);
  return INDEXITERATOR_TYPE(buffer_pool_manager_, found_leaf_page, node_index);
  // return INDEXITERATOR_TYPE();
}

/*
 * Input parameter is void, construct an index iterator representing the end
 * of the key/value pair in the leaf node
 * @return : index iterator
 */
INDEX_TEMPLATE_ARGUMENTS
INDEXITERATOR_TYPE BPLUSTREE_TYPE::End() 
{
  Page* right_most_leaf_page = FindLeafPage(KeyType(), 'R');
  LeafPage* right_most_leaf_node 
  = reinterpret_cast<LeafPage*>(right_most_leaf_page->GetData());
  int node_index = right_most_leaf_node->GetSize();
  return INDEXITERATOR_TYPE(buffer_pool_manager_, right_most_leaf_page, node_index);
  // return INDEXITERATOR_TYPE();
}

/*****************************************************************************
 * UTILITIES AND DEBUG
 *****************************************************************************/
/*
 * Find leaf page containing particular key, if leftMost flag == true, find
 * the left most leaf page
 * Note: the leaf page is pinned, you need to unpin it after use.
 */
INDEX_TEMPLATE_ARGUMENTS
Page *BPLUSTREE_TYPE::FindLeafPage(const KeyType &key, bool leftMost) 
{
  Page* fth_page = buffer_pool_manager_->FetchPage(root_page_id_);
  BPlusTreePage* tmp_node = reinterpret_cast<BPlusTreePage *>(fth_page->GetData());
  fth_page->RLatch();

  while (!tmp_node->IsLeafPage()) 
  {
    InternalPage* internal_node = reinterpret_cast<InternalPage *>(tmp_node);
    page_id_t child_page_id;
    if (leftMost) 
    {
      child_page_id = internal_node->ValueAt(0);
    }
    else 
    {
      child_page_id = internal_node->Lookup(key, comparator_);
    }

    Page* child_page = buffer_pool_manager_->FetchPage(child_page_id);
    BPlusTreePage* child_BP_node = reinterpret_cast<BPlusTreePage *>(child_page->GetData());

    child_page->RLatch();
    fth_page->RUnlatch();
    buffer_pool_manager_->UnpinPage(fth_page->GetPageId(), false);

    fth_page = child_page;
    tmp_node = child_BP_node;
  }

  return fth_page;
  // return nullptr;
}

// new added @hz 
// -1 for leftmost, 0 for simple search, 1 for rightmost
INDEX_TEMPLATE_ARGUMENTS
Page *BPLUSTREE_TYPE::FindLeafPage(const KeyType &key, char target_location)
{
  Page* fth_page = buffer_pool_manager_->FetchPage(root_page_id_);
  BPlusTreePage* tmp_node = reinterpret_cast<BPlusTreePage *>(fth_page->GetData());
  fth_page->RLatch();

  while (!tmp_node->IsLeafPage()) 
  {
    InternalPage* internal_node = reinterpret_cast<InternalPage *>(tmp_node);
    page_id_t child_page_id;
    if (target_location == 'L') 
    {
      child_page_id = internal_node->ValueAt(0);
    }
    else if(target_location == 'R')
    {
      child_page_id = internal_node->ValueAt(internal_node->GetSize()-1);
    }
    else 
    {
      child_page_id = internal_node->Lookup(key, comparator_);
    }

    Page* child_page = buffer_pool_manager_->FetchPage(child_page_id);
    BPlusTreePage* child_BP_node = reinterpret_cast<BPlusTreePage *>(child_page->GetData());

    child_page->RLatch();
    fth_page->RUnlatch();
    buffer_pool_manager_->UnpinPage(fth_page->GetPageId(), false);

    fth_page = child_page;
    tmp_node = child_BP_node;
  }

  return fth_page;
} 

/*
 * Update/Insert root page id in header page
 * (where page_id = 0, header_page is
 * defined under include/page/header_page.h)
 * Call this method everytime root page id is changed.
 * @parameter: insert_record      default value is false. 
 * When set to true,
 * insert a record <index_name, root_page_id> into 
 * header page instead of
 * updating it.
 */
INDEX_TEMPLATE_ARGUMENTS
void BPLUSTREE_TYPE::UpdateRootPageId(int insert_record) {
  // HeaderPage *header_page =
  // static_cast<HeaderPage *>
  // (buffer_pool_manager_->FetchPage(HEADER_PAGE_ID));
  // if (insert_record != 0) {
  //   // create a new record<index_name + root_page_id> in header_page
  //   header_page->InsertRecord(index_name_, root_page_id_);
  // } else {
  //   // update root_page_id in header_page
  //   header_page->UpdateRecord(index_name_, root_page_id_);
  // }
  // buffer_pool_manager_->UnpinPage(HEADER_PAGE_ID, true);
  IndexRootsPage *tmp_index_root_page =
      reinterpret_cast<IndexRootsPage *>(buffer_pool_manager_->FetchPage(INDEX_ROOTS_PAGE_ID));

  if (insert_record != 0) {
    // tmp_index_root_page->Insert()
    tmp_index_root_page->Insert(index_id_, root_page_id_);
  } else {
    tmp_index_root_page->Update(index_id_, root_page_id_);
  }
  buffer_pool_manager_->UnpinPage(INDEX_ROOTS_PAGE_ID, true);

  return;
}

/**
 * This method is used for debug only, You don't need to modify
 */
INDEX_TEMPLATE_ARGUMENTS
void BPLUSTREE_TYPE::ToGraph(BPlusTreePage *page, BufferPoolManager *bpm, std::ofstream &out) const {
  std::string leaf_prefix("LEAF_");
  std::string internal_prefix("INT_");
  if (page->IsLeafPage()) {
    auto *leaf = reinterpret_cast<LeafPage *>(page);
    // Print node name
    out << leaf_prefix << leaf->GetPageId();
    // Print node properties
    out << "[shape=plain color=green ";
    // Print data of the node
    out << "label=<<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING=\"4\">\n";
    // Print data
    out << "<TR><TD COLSPAN=\"" << leaf->GetSize() << "\">P=" << leaf->GetPageId()
        << ",Parent=" << leaf->GetParentPageId() << "</TD></TR>\n";
    out << "<TR><TD COLSPAN=\"" << leaf->GetSize() << "\">"
        << "max_size=" << leaf->GetMaxSize() << ",min_size=" << leaf->GetMinSize() << ",size=" << leaf->GetSize()
        << "</TD></TR>\n";
    out << "<TR>";
    for (int i = 0; i < leaf->GetSize(); i++) {
      out << "<TD>" << leaf->KeyAt(i) << "</TD>\n";
    }
    out << "</TR>";
    // Print table end
    out << "</TABLE>>];\n";
    // Print Leaf node link if there is a next page
    if (leaf->GetNextPageId() != INVALID_PAGE_ID) {
      out << leaf_prefix << leaf->GetPageId() << " -> " << leaf_prefix << leaf->GetNextPageId() << ";\n";
      out << "{rank=same " << leaf_prefix << leaf->GetPageId() << " " << leaf_prefix << leaf->GetNextPageId()
          << "};\n";
    }

    // Print parent links if there is a parent
    if (leaf->GetParentPageId() != INVALID_PAGE_ID) {
      out << internal_prefix << leaf->GetParentPageId() << ":p" << leaf->GetPageId() << " -> " << leaf_prefix
          << leaf->GetPageId() << ";\n";
    }
  } else {
    auto *inner = reinterpret_cast<InternalPage *>(page);
    // Print node name
    out << internal_prefix << inner->GetPageId();
    // Print node properties
    out << "[shape=plain color=pink ";  // why not?
    // Print data of the node
    out << "label=<<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING=\"4\">\n";
    // Print data
    out << "<TR><TD COLSPAN=\"" << inner->GetSize() << "\">P=" << inner->GetPageId()
        << ",Parent=" << inner->GetParentPageId() << "</TD></TR>\n";
    out << "<TR><TD COLSPAN=\"" << inner->GetSize() << "\">"
        << "max_size=" << inner->GetMaxSize() << ",min_size=" << inner->GetMinSize() << ",size=" << inner->GetSize()
        << "</TD></TR>\n";
    out << "<TR>";
    for (int i = 0; i < inner->GetSize(); i++) {
      out << "<TD PORT=\"p" << inner->ValueAt(i) << "\">";
      if (i > 0) {
        out << inner->KeyAt(i);
      } else {
        out << " ";
      }
      out << "</TD>\n";
    }
    out << "</TR>";
    // Print table end
    out << "</TABLE>>];\n";
    // Print Parent link
    if (inner->GetParentPageId() != INVALID_PAGE_ID) {
      out << internal_prefix << inner->GetParentPageId() << ":p" << inner->GetPageId() << " -> "
          << internal_prefix
          << inner->GetPageId() << ";\n";
    }
    // Print leaves
    for (int i = 0; i < inner->GetSize(); i++) {
      auto child_page = reinterpret_cast<BPlusTreePage *>(bpm->FetchPage(inner->ValueAt(i))->GetData());
      ToGraph(child_page, bpm, out);
      if (i > 0) {
        auto sibling_page = reinterpret_cast<BPlusTreePage *>(bpm->FetchPage(inner->ValueAt(i - 1))->GetData());
        if (!sibling_page->IsLeafPage() && !child_page->IsLeafPage()) {
          out << "{rank=same " << internal_prefix << sibling_page->GetPageId() << " " << internal_prefix
              << child_page->GetPageId() << "};\n";
        }
        bpm->UnpinPage(sibling_page->GetPageId(), false);
      }
    }
  }
  bpm->UnpinPage(page->GetPageId(), false);
}

/**
 * This function is for debug only, you don't need to modify
 */
INDEX_TEMPLATE_ARGUMENTS
void BPLUSTREE_TYPE::ToString(BPlusTreePage *page, BufferPoolManager *bpm) const {
  if (page->IsLeafPage()) {
    auto *leaf = reinterpret_cast<LeafPage *>(page);
    std::cout << "Leaf Page: " << leaf->GetPageId() << " parent: " << leaf->GetParentPageId()
              << " next: " << leaf->GetNextPageId() << std::endl;
    for (int i = 0; i < leaf->GetSize(); i++) {
      std::cout << leaf->KeyAt(i) << ",";
    }
    std::cout << std::endl;
    std::cout << std::endl;
  } else {
    auto *internal = reinterpret_cast<InternalPage *>(page);
    std::cout << "Internal Page: " << internal->GetPageId() << " parent: " << internal->GetParentPageId()
              << std::endl;
    for (int i = 0; i < internal->GetSize(); i++) {
      std::cout << internal->KeyAt(i) << ": " << internal->ValueAt(i) << ",";
    }
    std::cout << std::endl;
    std::cout << std::endl;
    for (int i = 0; i < internal->GetSize(); i++) {
      ToString(reinterpret_cast<BPlusTreePage *>(bpm->FetchPage(internal->ValueAt(i))->GetData()), bpm);
      bpm->UnpinPage(internal->ValueAt(i), false);
    }
  }
}

INDEX_TEMPLATE_ARGUMENTS
bool BPLUSTREE_TYPE::Check() 
{
  bool all_unpinned = buffer_pool_manager_->CheckAllUnpinned();
  if (!all_unpinned) {
    LOG(ERROR) << "problem in page unpin" << endl;
  }
  return all_unpinned;
}

template
class BPlusTree<int, int, BasicComparator<int>>;

template
class BPlusTree<GenericKey<4>, RowId, GenericComparator<4>>;

template
class BPlusTree<GenericKey<8>, RowId, GenericComparator<8>>;

template
class BPlusTree<GenericKey<16>, RowId, GenericComparator<16>>;

template
class BPlusTree<GenericKey<32>, RowId, GenericComparator<32>>;

template
class BPlusTree<GenericKey<64>, RowId, GenericComparator<64>>;
