#ifndef MINISQL_ROW_H
#define MINISQL_ROW_H

#include <bitset>
#include <map>

#include <memory>
#include <vector>
#include "common/macros.h"
#include "common/rowid.h"
#include "record/field.h"
#include "record/schema.h"
#include "utils/mem_heap.h"

/**
 *  Row format:
 * -------------------------------------------
 * | Header | Field-1 | ... | Field-N |
 * -------------------------------------------
 *  Header format:
 * --------------------------------------------
 * | Field Nums | Null bitmap |
 * -------------------------------------------
 *  
 *
 */
class Row {
 public:
  /**
   * Row used for insert
   * Field integrity should check by upper level
   */
  explicit Row(std::vector<Field> &fields) : heap_(new SimpleMemHeap) {
    // deep copy
    for (auto &field : fields) {
      void *buf = heap_->Allocate(sizeof(Field));
      fields_.push_back(new (buf) Field(field));
      if (field.IsNull()) null_nums++;
    }

    fields_nums = fields.size();
  }

  /**
   * Row used for deserialize
   */
  Row() = delete;

  /**
   * Row used for deserialize and update
   */
  Row(RowId rid) : rid_(rid), heap_(new SimpleMemHeap) {}

  /**
   * Row copy function
   */
  Row(const Row &other) : heap_(new SimpleMemHeap) {
    if (!fields_.empty()) {
      for (auto &field : fields_) {
        heap_->Free(field);
      }
      fields_.clear();
    }
    rid_ = other.rid_;
    for (auto &field : other.fields_) {
      void *buf = heap_->Allocate(sizeof(Field));
      fields_.push_back(new (buf) Field(*field));
      if (field->IsNull()) null_nums++;
    }

    fields_nums = other.fields_nums;
  }

  virtual ~Row() { delete heap_; }

  /**
   * Note: Make sure that bytes write to buf is equal to GetSerializedSize()
   */
  uint32_t SerializeTo(char *buf, Schema *schema) const;

  uint32_t DeserializeFrom(char *buf, Schema *schema);

  /*************************************************************************************\
   * For empty row, return 0                                                           *
   * For non-empty row with null fields, eg: |null|null|null|, return header size only *
   * @return                                                                           *
  \*************************************************************************************/
  uint32_t GetSerializedSize(Schema *schema) const;

  inline const RowId GetRowId() const { return rid_; }

  inline void SetRowId(RowId rid) { rid_ = rid; }

  inline std::vector<Field *> &GetFields() { return fields_; }

  inline Field *GetField(uint32_t idx) const {
    ASSERT(idx < fields_.size(), "Failed to access field");
    return fields_[idx];
  }

  inline size_t GetFieldCount() const { return fields_.size(); }

 private:
  Row &operator=(const Row &other) = delete;

 private:
  RowId rid_{};
  std::vector<Field *> fields_; /** Make sure that all fields are created by mem heap */
  MemHeap *heap_{nullptr};

  uint32_t fields_nums{0};
  uint32_t null_nums{0};
//  std::bitset<PAGE_SIZE>null_bitmap;
};

#endif //MINISQL_TUPLE_H
