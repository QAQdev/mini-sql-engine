#include "record/column.h"

Column::Column(std::string column_name, TypeId type, uint32_t index, bool nullable, bool unique)
        : name_(std::move(column_name)), type_(type), table_ind_(index),
          nullable_(nullable), unique_(unique) {
  ASSERT(type != TypeId::kTypeChar, "Wrong constructor for CHAR type.");
  switch (type) {
    case TypeId::kTypeInt :
      len_ = sizeof(int32_t);
      break;
    case TypeId::kTypeFloat :
      len_ = sizeof(float_t);
      break;
    default:
      ASSERT(false, "Unsupported column type.");
  }
}

Column::Column(std::string column_name, TypeId type, uint32_t length, uint32_t index, bool nullable, bool unique)
        : name_(std::move(column_name)), type_(type), len_(length),
          table_ind_(index), nullable_(nullable), unique_(unique) {
  ASSERT(type == TypeId::kTypeChar, "Wrong constructor for non-VARCHAR type.");
}

Column::Column(const Column *other) : name_(other->name_), type_(other->type_), len_(other->len_),
                                      table_ind_(other->table_ind_), nullable_(other->nullable_),
                                      unique_(other->unique_) {}

uint32_t Column::SerializeTo(char *buf) const {
  uint32_t ofs = 0, len;

  MACH_WRITE_TO(uint32_t, buf, COLUMN_MAGIC_NUM);
  ofs += sizeof(uint32_t);

  len = name_.size() * sizeof(char);  // bytes of string
  memcpy(buf + ofs, &len, sizeof(uint32_t));
  ofs += sizeof(uint32_t);
  memcpy(buf + ofs, name_.c_str(), len);
  ofs += len;

  MACH_WRITE_TO(TypeId, buf + ofs, type_);
  ofs += sizeof(TypeId);
  MACH_WRITE_TO(uint32_t, buf + ofs, len_);
  ofs += sizeof(uint32_t);
  MACH_WRITE_TO(uint32_t, buf + ofs, table_ind_);
  ofs += sizeof(uint32_t);
  MACH_WRITE_TO(bool, buf + ofs, nullable_);
  ofs += sizeof(bool);
  MACH_WRITE_TO(bool, buf + ofs, unique_);
  ofs += sizeof(bool);

  return ofs;
}

uint32_t Column::GetSerializedSize() const {
  return 4 * sizeof(uint32_t) + name_.size() * sizeof(char) + sizeof(TypeId) + 2 * sizeof(bool);
}

uint32_t Column::DeserializeFrom(char *buf, Column *&column, MemHeap *heap) {
  auto magic_num = MACH_READ_FROM(uint32_t, buf);
  ASSERT(magic_num == COLUMN_MAGIC_NUM, "Column magic num error.");

  uint32_t ofs = sizeof(uint32_t);

  auto len = MACH_READ_UINT32(buf + ofs);
  ofs += sizeof(uint32_t);

  char tmp[len / sizeof(char) + 1];
  memset(tmp, '\0', sizeof(tmp));
  memcpy(tmp, buf + ofs, len);
  auto name_(tmp);
  ofs += len;

  auto type_ = MACH_READ_FROM(TypeId, buf + ofs);
  ofs += sizeof(TypeId);
  auto len_ = MACH_READ_FROM(TypeId, buf + ofs);
  ofs += sizeof(uint32_t);
  auto table_ind_ = MACH_READ_FROM(uint32_t, buf + ofs);
  ofs += sizeof(uint32_t);
  auto nullable_ = MACH_READ_FROM(bool, buf + ofs);
  ofs += sizeof(bool);
  auto unique_ = MACH_READ_FROM(bool, buf + ofs);
  ofs += sizeof(bool);

  if (type_ != kTypeChar)  // not char type
    column = ALLOC_P(heap, Column)(name_, type_, table_ind_, nullable_, unique_);
  else
    column = ALLOC_P(heap, Column)(name_, type_, len_, table_ind_, nullable_, unique_);

  return ofs;
}
