#include "record/row.h"

uint32_t Row::SerializeTo(char *buf, Schema *schema) const {
  uint32_t ofs = 0;

  MACH_WRITE_TO(uint32_t, buf, fields_nums);
  ofs += sizeof(uint32_t);

  for (auto &itr : fields_) {
    if (!itr->IsNull()) {
      ofs += itr->SerializeTo(buf + ofs);
    }
  }
  return ofs;
}

uint32_t Row::DeserializeFrom(char *buf, Schema *schema) {
  if (fields_.empty()) return 0;

  uint32_t ofs = 0, null_nums_ = 0;

  fields_nums = MACH_READ_UINT32(buf);
  ofs += sizeof(uint32_t);

  for (uint32_t i = 0; i < fields_nums; i++) {
    if (!fields_[i]->IsNull()) {
      ofs += fields_[i]->DeserializeFrom(buf + ofs, schema->GetColumn(i)->GetType(), &fields_[i], false, heap_);
    } else {
      null_nums_++;
    }
  }

  return ofs;
}

uint32_t Row::GetSerializedSize(Schema *schema) const {
  uint32_t size = 0;

  for (auto &itr : fields_) {
    if (!itr->IsNull()) {
      size += itr->GetSerializedSize();
    }
  }

  return size + sizeof(uint32_t);
}
