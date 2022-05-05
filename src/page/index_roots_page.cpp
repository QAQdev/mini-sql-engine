#include "page/index_roots_page.h"

bool IndexRootsPage::Insert(const index_id_t index_id, const page_id_t root_id) {
  auto index = FindIndex(index_id);
  // check for duplicate index id
  if (index != -1) {
    return false;
  }
  roots_[count_].first = index_id;
  roots_[count_].second = root_id;
  count_++;
  return true;
}

bool IndexRootsPage::Delete(const index_id_t index_id) {
  auto index = FindIndex(index_id);
  if (index == -1) {
    return false;
  }
  for (auto i = index; i < count_ - 1; i++) {
    roots_[i] = roots_[i + 1];
  }
  count_--;
  return true;
}

bool IndexRootsPage::Update(const index_id_t index_id, const page_id_t root_id) {
  auto index = FindIndex(index_id);
  if (index == -1) {
    return false;
  }
  roots_[index].second = root_id;
  return true;
}

bool IndexRootsPage::GetRootId(const index_id_t index_id, page_id_t *root_id) {
  auto index = FindIndex(index_id);
  if (index == -1) {
    return false;
  }
  *root_id = roots_[index].second;
  return true;
}

int IndexRootsPage::FindIndex(const index_id_t index_id) {
  for (auto i = 0; i < count_; i++) {
    if (roots_[i].first == index_id) {
      return i;
    }
  }
  return -1;
}
