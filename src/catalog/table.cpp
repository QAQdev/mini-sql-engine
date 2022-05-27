#include "catalog/table.h"

uint32_t TableMetadata::SerializeTo(char *buf) const {
  auto ofs = 0u;

  MACH_WRITE_TO(uint32_t, buf, TABLE_METADATA_MAGIC_NUM);
  ofs += sizeof(uint32_t);

  MACH_WRITE_UINT32(buf + ofs, table_id_);
  ofs += sizeof(table_id_t);

  // bytes of string
  uint32_t len = table_name_.size() * sizeof(char);
  memcpy(buf + ofs, &len, sizeof(uint32_t));
  ofs += sizeof(uint32_t);
  memcpy(buf + ofs, table_name_.c_str(), len);
  ofs += len;

  MACH_WRITE_TO(page_id_t, buf + ofs, root_page_id_);
  ofs += sizeof(page_id_t);

  ofs += schema_->SerializeTo(buf + ofs);

  return ofs;
}

uint32_t TableMetadata::GetSerializedSize() const {
  return 2 * sizeof(uint32_t) + sizeof(table_id_t) + table_name_.size() * sizeof(char) + sizeof(page_id_t) +
         schema_->GetSerializedSize();
}

/**
 * @param heap Memory heap passed by TableInfo
 */
uint32_t TableMetadata::DeserializeFrom(char *buf, TableMetadata *&table_meta, MemHeap *heap) {
  auto magic_num = MACH_READ_INT32(buf);
  ASSERT(magic_num == TABLE_METADATA_MAGIC_NUM, "Table meta magic num error.");

  auto ofs = sizeof(uint32_t);

  auto table_id = MACH_READ_FROM(table_id_t, buf + ofs);
  ofs += sizeof(table_id_t);

  auto len = MACH_READ_UINT32(buf + ofs);
  ofs += sizeof(uint32_t);

  char tmp[len / sizeof(char) + 1];
  memset(tmp, '\0', sizeof(tmp));
  memcpy(tmp, buf + ofs, len);
  auto table_name = tmp;
  ofs += len;

  auto root_page_id = MACH_READ_FROM(page_id_t, buf + ofs);
  ofs += sizeof(page_id_t);

  Schema *schema;
  ofs += Schema::DeserializeFrom(buf + ofs, schema, heap);

  table_meta = Create(table_id, table_name, root_page_id, schema, heap);

  return ofs;
}

/**
 * Only called by create table
 *
 * @param heap Memory heap passed by TableInfo
 */
TableMetadata *TableMetadata::Create(table_id_t table_id, std::string table_name,
                                     page_id_t root_page_id, TableSchema *schema, MemHeap *heap) {
  // allocate space for table metadata
  void *buf = heap->Allocate(sizeof(TableMetadata));
  return new(buf)TableMetadata(table_id, table_name, root_page_id, schema);
}

TableMetadata::TableMetadata(table_id_t table_id, std::string table_name, page_id_t root_page_id, TableSchema *schema)
        : table_id_(table_id), table_name_(table_name), root_page_id_(root_page_id), schema_(schema) {}
