//
// Created by kenshin on 5/2/22.
//

#ifndef MINISQL_CLOCK_REPLACER_H
#define MINISQL_CLOCK_REPLACER_H

#include <mutex>
#include <vector>
#include <algorithm>

#include "buffer/replacer.h"
#include "common/config.h"

using namespace std;

class ClockReplacer: public Replacer {
 private:
  /**
   * @brief state of the frame
   * if pin -> EMPTY
   * if unpin -> ACCESSED
   * after ACCESSED -> UNUSED
   */
  enum class State { EMPTY, ACCESSED, UNUSED };

 public:
  /**
   * Create a new ClockReplacer.
   * @param num_pages the maximum number of pages the ClockReplacer will be required to store
   */
  explicit ClockReplacer(size_t num_pages);

  /**
   * Destroys the ClockReplacer.
   */
  ~ClockReplacer() override;

  bool Victim(frame_id_t *frame_id) override;

  void Pin(frame_id_t frame_id) override;

  void Unpin(frame_id_t frame_id) override;

  size_t Size() override;

 private:
  static bool IsEmpty(ClockReplacer::State &);

 private:
  std::vector<State> second_chance;
  frame_id_t pointer{0};
  size_t capacity;
};

#endif  // MINISQL_CLOCK_REPLACER_H
