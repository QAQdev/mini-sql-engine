#include "record/row.h"

uint32_t Row::SerializeTo(char *buf, Schema *schema) const {
  uint32_t ofs = 0;

  MACH_WRITE_TO(uint32_t, buf, fields_nums);
  ofs += sizeof(uint32_t);
  MACH_WRITE_TO(uint32_t, buf + ofs, null_nums);
  ofs += sizeof(uint32_t);

  for (uint32_t i = 0; i < fields_.size(); i++) {
    if (fields_[i]->IsNull()) {
      MACH_WRITE_INT32(buf + ofs, i);
      ofs += sizeof(uint32_t);
    }
  }

  for (auto &itr : fields_) {
    if (!itr->IsNull()) ofs += itr->SerializeTo(buf + ofs);
  }

  return ofs;
}

uint32_t Row::DeserializeFrom(char *buf, Schema *schema) {
  uint32_t ofs = 0, i = 0;

  fields_nums = MACH_READ_UINT32(buf);
  ofs += sizeof(uint32_t);
  null_nums = MACH_READ_UINT32(buf + ofs);
  ofs += sizeof(uint32_t);

  std::vector<uint32_t> null_bitmap(fields_nums, 0);
  for (i = 0; i < null_nums; i++) {
    null_bitmap[MACH_READ_UINT32(buf + ofs)] = 1;
    ofs += sizeof(uint32_t);
  }

  for (i = 0; i < fields_nums; i++) {
    fields_.push_back(ALLOC_P(heap_, Field)(schema->GetColumn(i)->GetType()));
    if (!null_bitmap[i]) {
      ofs += Field::DeserializeFrom(buf + ofs, schema->GetColumn(i)->GetType(), &fields_[i], false, heap_);
    }
  }
  return ofs;
}

uint32_t Row::GetSerializedSize(Schema *schema) const {
  if (fields_.empty())
    return 0;
  else if (fields_nums == null_nums)
    return sizeof(uint32_t) * (2 + null_nums);
  else {
    uint32_t ofs = 0;
    for (auto &itr : fields_) {
      if (!itr->IsNull()) ofs += itr->GetSerializedSize();
    }

    return ofs + sizeof(uint32_t) * (2 + null_nums);
  }
}
