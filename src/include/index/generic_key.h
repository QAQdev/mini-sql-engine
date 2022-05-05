#ifndef MINISQL_GENERIC_KEY_H
#define MINISQL_GENERIC_KEY_H

#include <cstring>

#include "record/row.h"
#include "record/field.h"

template<size_t KeySize>
class GenericKey {
public:
  inline void SerializeFromKey(const Row &key, Schema *schema) {
    // initialize to 0
    uint32_t size = key.GetSerializedSize(schema);
    ASSERT(key.GetFieldCount() == schema->GetColumnCount(), "field nums not match.");
    ASSERT(size <= KeySize, "Index key size exceed max key size.");
    memset(data, 0, KeySize);
    key.SerializeTo(data, schema);
  }

  inline void DeserializeToKey(Row &key, Schema *schema) const {
    uint32_t ofs = key.DeserializeFrom(const_cast<char *>(data), schema);
    ASSERT(ofs <= KeySize, "Index key size exceed max key size.");
    return;
  }

  // compare
  inline bool operator==(const GenericKey &other) {
    return memcmp(data, other.data, KeySize) == 0;
  }

  // NOTE: for test purpose only
  // interpret the first 8 bytes as int64_t from data vector
  inline int64_t ToString() const {
    return *reinterpret_cast<int64_t *>(const_cast<char *>(data));
  }

  // NOTE: for test purpose only
  // interpret the first 8 bytes as int64_t from data vector
  friend std::ostream &operator<<(std::ostream &os, const GenericKey &key) {
    os << key.ToString();
    return os;
  }

  // actual location of data, extends past the end.
  char data[KeySize];
};

/**
 * Function object returns true if lhs < rhs, used for trees
 */
template<size_t KeySize>
class GenericComparator {
public:
  inline int operator()(const GenericKey<KeySize> &lhs,
                        const GenericKey<KeySize> &rhs) const {
    int column_count = key_schema_->GetColumnCount();
    Row lhs_key(INVALID_ROWID);
    Row rhs_key(INVALID_ROWID);
    lhs.DeserializeToKey(lhs_key, key_schema_);
    rhs.DeserializeToKey(rhs_key, key_schema_);

    for (int i = 0; i < column_count; i++) {
      Field *lhs_value = lhs_key.GetField(i);
      Field *rhs_value = rhs_key.GetField(i);

      if (lhs_value->CompareLessThan(*rhs_value) == CmpBool::kTrue)
        return -1;

      if (lhs_value->CompareGreaterThan(*rhs_value) == CmpBool::kTrue)
        return 1;
    }
    // equals
    return 0;
  }

  GenericComparator(const GenericComparator &other) {
    this->key_schema_ = other.key_schema_;
  }

  // constructor
  GenericComparator(Schema *key_schema) : key_schema_(key_schema) {}

private:
  Schema *key_schema_;
};

#endif  // MINISQL_GENERIC_KEY_H
