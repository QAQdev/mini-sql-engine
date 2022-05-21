#include "buffer/lru_replacer.h"

LRUReplacer::LRUReplacer(size_t num_pages)
    :capacity(num_pages) {

}

LRUReplacer::~LRUReplacer() = default;

bool LRUReplacer::Victim(frame_id_t *frame_id) {
  //  return false;
  if (visit_lst.empty()) return false;

  auto least = visit_lst.front();  // get header
  visit_lst.pop_front();

  auto least_itr = mapping.find(least);
  if (least_itr != mapping.end()) {  // found
    mapping.erase(least_itr);        // out of list
    *frame_id = least;
    return true;
  }

  return false;
}

void LRUReplacer::Pin(frame_id_t frame_id) {
  auto least_itr = mapping.find(frame_id);
  if (least_itr != mapping.end()) {
    visit_lst.erase(least_itr->second); //remove from lru list
    mapping.erase(least_itr); //remove from mapping
  }
}

void LRUReplacer::Unpin(frame_id_t frame_id) {
  // do the opposite of Pin()
  if (mapping.size() >= capacity) return;

  auto itr = mapping.find(frame_id);
  if (itr != mapping.end()) return;

  visit_lst.push_back(frame_id);
  mapping.insert(pair<frame_id_t, list<frame_id_t>::iterator>(frame_id, prev(visit_lst.end(), 1)));
}

size_t LRUReplacer::Size() { return mapping.size(); }