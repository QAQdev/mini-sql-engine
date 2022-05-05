//
// Created by kenshin on 5/2/22.
//

#include "buffer/clock_replacer.h"
ClockReplacer::ClockReplacer(size_t num_pages)
    :capacity(num_pages) {

}

ClockReplacer::~ClockReplacer() {

}

bool ClockReplacer::Victim(frame_id_t *frame_id) { return false; }

void ClockReplacer::Pin(frame_id_t frame_id) {}

void ClockReplacer::Unpin(frame_id_t frame_id) {}

size_t ClockReplacer::Size() { return 0; }
