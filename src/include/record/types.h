#ifndef MINISQL_TYPES_H
#define MINISQL_TYPES_H

#include <cstdint>
#include <cmath>
#include <exception>
#include "record/type_id.h"
#include "common/config.h"
#include "utils/mem_heap.h"

class Field;

enum CmpBool {
  kFalse = 0,
  kTrue,
  kNull
};

inline CmpBool GetCmpBool(bool boolean) { return boolean ? CmpBool::kTrue : CmpBool::kFalse; }

class Type {
public:
  explicit Type(TypeId type_id) : type_id_(type_id) {}

  virtual ~Type() = default;

  static uint32_t GetTypeSize(TypeId type_id) {
    switch (type_id) {
      case kTypeInt:
        return sizeof(int);
      case kTypeFloat:
        return sizeof(float);
      case kTypeChar:
        return 0;
      default:
        break;
    }
    throw "Unknown field type.";
  }

  inline static Type *GetInstance(TypeId type_id) {
    return type_singletons_[type_id];
  }

  inline TypeId GetTypeId() {
    return type_id_;
  }

  // Serialize this field into the given storage space.
  virtual uint32_t SerializeTo(const Field &field, char *buf) const;

  // Deserialize a field of the given type from the given storage space.
  virtual uint32_t DeserializeFrom(char *storage, Field **field, bool is_null, MemHeap *heap) const;

  // Get serialize size of a field
  virtual uint32_t GetSerializedSize(const Field &field, bool is_null) const;

  // Access the raw variable length data
  virtual const char *GetData(const Field &val) const;

  // Get the length of the variable length data
  virtual uint32_t GetLength(const Field &val) const;

  virtual CmpBool CompareEquals(const Field &left, const Field &right) const;

  virtual CmpBool CompareNotEquals(const Field &left, const Field &right) const;

  virtual CmpBool CompareLessThan(const Field &left, const Field &right) const;

  virtual CmpBool CompareLessThanEquals(const Field &left, const Field &right) const;

  virtual CmpBool CompareGreaterThan(const Field &left, const Field &right) const;

  virtual CmpBool CompareGreaterThanEquals(const Field &left, const Field &right) const;

protected:
  TypeId type_id_{TypeId::kTypeInvalid};
  static Type *type_singletons_[TypeId::KMaxTypeId + 1];
};

class TypeInt : public Type {
public:
  explicit TypeInt() : Type(TypeId::kTypeInt) {}

  virtual uint32_t SerializeTo(const Field &field, char *buf) const override;

  virtual uint32_t DeserializeFrom(char *storage, Field **field, bool is_null, MemHeap *heap) const override;

  virtual uint32_t GetSerializedSize(const Field &field, bool is_null) const override;

  virtual CmpBool CompareEquals(const Field &left, const Field &right) const override;

  virtual CmpBool CompareNotEquals(const Field &left, const Field &right) const override;

  virtual CmpBool CompareLessThan(const Field &left, const Field &right) const override;

  virtual CmpBool CompareLessThanEquals(const Field &left, const Field &right) const override;

  virtual CmpBool CompareGreaterThan(const Field &left, const Field &right) const override;

  virtual CmpBool CompareGreaterThanEquals(const Field &left, const Field &right) const override;
};

class TypeChar : public Type {
public:
  explicit TypeChar() : Type(TypeId::kTypeChar) {}

  virtual uint32_t SerializeTo(const Field &field, char *buf) const override;

  virtual uint32_t DeserializeFrom(char *storage, Field **field, bool is_null, MemHeap *heap) const override;

  virtual uint32_t GetSerializedSize(const Field &field, bool is_null) const override;

  virtual const char *GetData(const Field &val) const override;

  virtual uint32_t GetLength(const Field &val) const override;

  virtual CmpBool CompareEquals(const Field &left, const Field &right) const override;

  virtual CmpBool CompareNotEquals(const Field &left, const Field &right) const override;

  virtual CmpBool CompareLessThan(const Field &left, const Field &right) const override;

  virtual CmpBool CompareLessThanEquals(const Field &left, const Field &right) const override;

  virtual CmpBool CompareGreaterThan(const Field &left, const Field &right) const override;

  virtual CmpBool CompareGreaterThanEquals(const Field &left, const Field &right) const override;
};

class TypeFloat : public Type {
public:
  explicit TypeFloat() : Type(TypeId::kTypeFloat) {}

  virtual uint32_t SerializeTo(const Field &field, char *buf) const override;

  virtual uint32_t DeserializeFrom(char *storage, Field **field, bool is_null, MemHeap *heap) const override;

  virtual uint32_t GetSerializedSize(const Field &field, bool is_null) const override;

  virtual CmpBool CompareEquals(const Field &left, const Field &right) const override;

  virtual CmpBool CompareNotEquals(const Field &left, const Field &right) const override;

  virtual CmpBool CompareLessThan(const Field &left, const Field &right) const override;

  virtual CmpBool CompareLessThanEquals(const Field &left, const Field &right) const override;

  virtual CmpBool CompareGreaterThan(const Field &left, const Field &right) const override;

  virtual CmpBool CompareGreaterThanEquals(const Field &left, const Field &right) const override;
};


#endif //MINISQL_TYPES_H
