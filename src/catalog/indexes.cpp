#include "catalog/indexes.h"

IndexMetadata *IndexMetadata::Create(const index_id_t index_id, const string &index_name,
                                     const table_id_t table_id, const vector<uint32_t> &key_map,
                                     MemHeap *heap) {
  void *buf = heap->Allocate(sizeof(IndexMetadata));
  return new(buf)IndexMetadata(index_id, index_name, table_id, key_map);
}

uint32_t IndexMetadata::SerializeTo(char *buf) const {
  auto ofs = 0;

  MACH_WRITE_INT32(buf, INDEX_METADATA_MAGIC_NUM);
  ofs += sizeof(uint32_t);

  MACH_WRITE_TO(index_id_t, buf + ofs, index_id_);
  ofs += sizeof(index_id_t);

  // bytes of string
  uint32_t len = index_name_.size() * sizeof(char);
  memcpy(buf + ofs, &len, sizeof(uint32_t));
  ofs += sizeof(uint32_t);
  memcpy(buf + ofs, index_name_.c_str(), len);
  ofs += len;

  MACH_WRITE_TO(table_id_t, buf + ofs, table_id_);
  ofs += sizeof(table_id_t);

  MACH_WRITE_UINT32(buf + ofs, key_map_.size());
  ofs += sizeof(uint32_t);

  for (auto &it : key_map_) {
    MACH_WRITE_UINT32(buf + ofs, it);
    ofs += sizeof(uint32_t);
  }

  return ofs;
}

uint32_t IndexMetadata::GetSerializedSize() const {
  return sizeof(uint32_t) + sizeof(index_id_t) + sizeof(uint32_t) + index_name_.size() * sizeof(char) +
         sizeof(table_id_t) + sizeof(uint32_t) + key_map_.size() * sizeof(uint32_t);
}

uint32_t IndexMetadata::DeserializeFrom(char *buf, IndexMetadata *&index_meta, MemHeap *heap) {
  auto magic_num = MACH_READ_INT32(buf);
  ASSERT(magic_num == INDEX_METADATA_MAGIC_NUM, "Index meta magic num error.");

  auto ofs = sizeof(uint32_t);

  auto index_id = MACH_READ_FROM(index_id_t, buf + ofs);
  ofs+= sizeof(index_id_t);

  auto len = MACH_READ_INT32(buf + ofs);
  ofs += sizeof(uint32_t);

  char tmp[len / sizeof(char) + 1];
  memset(tmp, '\0', sizeof(tmp));
  memcpy(tmp, buf + ofs, len);
  auto index_name = tmp;
  ofs += len;

  auto table_id = MACH_READ_FROM(table_id_t, buf + ofs);
  ofs += sizeof(table_id_t);

  vector<uint32_t> key_map;

  auto key_map_size = MACH_READ_UINT32(buf + ofs);
  ofs += sizeof(uint32_t);

  for (unsigned i = 0; i < key_map_size; i++) {
    auto val = MACH_READ_UINT32(buf + ofs);
    key_map.push_back(val);
    ofs += sizeof(uint32_t);
  }

  index_meta = Create(index_id, index_name, table_id, key_map, heap);

  return ofs;
}
