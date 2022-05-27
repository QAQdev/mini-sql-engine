#include "buffer/buffer_pool_manager.h"
#include "glog/logging.h"
#include "page/bitmap_page.h"

BufferPoolManager::BufferPoolManager(size_t pool_size, DiskManager *disk_manager)
        : pool_size_(pool_size), disk_manager_(disk_manager) {
  pages_ = new Page[pool_size_];
  replacer_ = new LRUReplacer(pool_size_);
  //  replacer_ = new ClockReplacer(pool_size_);
  for (size_t i = 0; i < pool_size_; i++) {
    free_list_.emplace_back(i);
  }
}

BufferPoolManager::~BufferPoolManager() {
  for (auto page : page_table_) {
    FlushPage(page.first);
  }
  delete[] pages_;
  delete replacer_;
}

Page *BufferPoolManager::FetchPage(page_id_t page_id) {
  // 1.     Search the page table for the requested page (P).
  // 1.1    If P exists, pin it and return it immediately.
  // 1.2    If P does not exist, find a replacement page (R) from either the free list or the replacer.
  //        Note that pages are always found from the free list first.
  // 2.     If R is dirty, write it back to the disk.
  // 3.     Delete R from the page table and insert P.
  // 4.     Update P's metadata, read in the page content from disk, and then return a pointer to P.

  auto item_itr = page_table_.find(page_id);
  frame_id_t frame_id;
  Page *page_frame;

  if (item_itr != page_table_.end()) {  // found
    frame_id = item_itr->second;
    page_frame = &pages_[frame_id];
    page_frame->pin_count_++;  // page gets pinned
    replacer_->Pin(frame_id);
    return page_frame;
  }

  // fetch from freelist first
  if (!free_list_.empty()) {
    frame_id = free_list_.front();
    free_list_.pop_front();
  } else {
    if (!replacer_->Victim(&frame_id)) return nullptr;
  }

  page_frame = &pages_[frame_id];
  if (page_frame->IsDirty()) {
    disk_manager_->WritePage(page_frame->page_id_, page_frame->data_);
    page_frame->is_dirty_ = false;
  }

  page_table_.erase(page_frame->page_id_);                             // erase dirty
  page_table_.insert(pair<page_id_t, frame_id_t>(page_id, frame_id));  // insert new

  page_frame->page_id_ = page_id;
  page_frame->pin_count_++;  // page gets pinned
  replacer_->Pin(frame_id);

  disk_manager_->ReadPage(page_frame->page_id_, page_frame->data_);  // load data

  return page_frame;
}

Page *BufferPoolManager::NewPage(page_id_t &page_id) {
  // 0.   Make sure you call AllocatePage!
  // 1.   If all the pages in the buffer pool are pinned, return nullptr.
  // 2.   Pick a victim page P from either the free list or the replacer. Always pick from the free list first.
  // 3.   Update P's metadata, zero out memory and add P to the page table.
  // 4.   Set the page ID output parameter. Return a pointer to P.

  size_t i = 0;
  for (i = 0; i < pool_size_; i++) {
    if (pages_[i].GetPinCount() <= 0) break;
  }

  if (i == pool_size_) {
    return nullptr;
  }

  frame_id_t frame_id;
  Page *page_frame;

  // fetch from freelist first
  if (!free_list_.empty()) {
    frame_id = free_list_.front();
    free_list_.pop_front();
  } else {
    if (!replacer_->Victim(&frame_id)) {
      return nullptr;
    }
  }

  page_frame = &pages_[frame_id];
  if (page_frame->IsDirty()) {
    disk_manager_->WritePage(page_frame->page_id_, page_frame->data_);
    page_frame->is_dirty_ = false;
  }

  auto new_page_id = AllocatePage();

  page_table_.erase(page_frame->page_id_);                                 // erase old
  page_table_.insert(pair<page_id_t, frame_id_t>(new_page_id, frame_id));  // insert new

  page_frame->page_id_ = new_page_id;
  page_frame->ResetMemory();  // memset 0
  page_frame->pin_count_++;   // page gets pinned
  replacer_->Pin(frame_id);

  page_id = new_page_id;
  return page_frame;
}

bool BufferPoolManager::DeletePage(page_id_t page_id) {
  // 0.   Make sure you call DeallocatePage!
  // 1.   Search the page table for the requested page (P).
  // 1.   If P does not exist, return true.
  // 2.   If P exists, but has a non-zero pin-count, return false. Someone is using the page.
  // 3.   Otherwise, P can be deleted. Remove P from the page table, reset its metadata and return it to the free list.

  auto item_itr = page_table_.find(page_id);
  frame_id_t frame_id;
  Page *page_frame;
  if (item_itr == page_table_.end()) return true;

  // exist
  frame_id = item_itr->second;
  page_frame = &pages_[frame_id];

  // pin count non zero, return false
  if (page_frame->pin_count_ > 0) return false;

  if (page_frame->IsDirty()) {
    disk_manager_->WritePage(page_frame->page_id_, page_frame->data_);
    page_frame->is_dirty_ = false;
  }

  // zero
  page_table_.erase(page_frame->page_id_);
  DeallocatePage(page_frame->page_id_);

  page_frame->ResetMemory();
  page_frame->page_id_ = INVALID_PAGE_ID;
  page_frame->pin_count_ = 0;
  page_frame->is_dirty_ = false;

  free_list_.emplace_back(page_frame->page_id_);

  return true;
}

bool BufferPoolManager::UnpinPage(page_id_t page_id, bool is_dirty) {
  auto item_itr = page_table_.find(page_id);
  frame_id_t frame_id;
  Page *page_frame;

  if (item_itr == page_table_.end()) return true;  // not found

  frame_id = item_itr->second;
  page_frame = &pages_[frame_id];

  page_frame->is_dirty_ = is_dirty;

  if (page_frame->pin_count_ == 0) return false;
  page_frame->pin_count_--;

  if (page_frame->pin_count_ == 0) replacer_->Unpin(frame_id);

  return true;
}

bool BufferPoolManager::FlushPage(page_id_t page_id) {
  auto item_itr = page_table_.find(page_id);
  frame_id_t frame_id;
  Page *page_frame;

  if (item_itr == page_table_.end()) return false;  // not found

  frame_id = item_itr->second;
  page_frame = &pages_[frame_id];

  disk_manager_->WritePage(page_frame->page_id_, page_frame->data_);
  page_frame->is_dirty_ = false;  // set false after flush to disk

  return true;
}

page_id_t BufferPoolManager::AllocatePage() {
  int next_page_id = disk_manager_->AllocatePage();
  return next_page_id;
}

void BufferPoolManager::DeallocatePage(page_id_t page_id) {
  disk_manager_->DeAllocatePage(page_id);
}

bool BufferPoolManager::IsPageFree(page_id_t page_id) {
  return disk_manager_->IsPageFree(page_id);
}

// Only used for debug
bool BufferPoolManager::CheckAllUnpinned() {
  bool res = true;
  for (size_t i = 0; i < pool_size_; i++) {
    if (pages_[i].pin_count_ != 0) {
      res = false;
      LOG(ERROR) << "page " << pages_[i].page_id_ << " pin count:" << pages_[i].pin_count_ << endl;
    }
  }
  return res;
}