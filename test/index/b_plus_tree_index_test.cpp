#include <string>

#include "common/instance.h"
#include "gtest/gtest.h"
#include "index/b_plus_tree_index.h"
#include "index/generic_key.h"

static const std::string db_name = "bp_tree_index_test.db";

TEST(BPlusTreeTests, BPlusTreeIndexGenericKeyTest) {
  using INDEX_KEY_TYPE = GenericKey<32>;
  using INDEX_COMPARATOR_TYPE = GenericComparator<32>;
  DBStorageEngine engine(db_name);
  SimpleMemHeap heap;
  std::vector<Column *> columns = {
          ALLOC_COLUMN(heap)("id", TypeId::kTypeInt, 0, false, false),
          ALLOC_COLUMN(heap)("name", TypeId::kTypeChar, 64, 1, true, false),
          ALLOC_COLUMN(heap)("account", TypeId::kTypeFloat, 2, true, false)
  };
  std::vector<uint32_t> index_key_map{0, 1};
  const TableSchema table_schema(columns);
  auto *key_schema = Schema::ShallowCopySchema(&table_schema, index_key_map, &heap);
  std::vector<Field> fields{
          Field(TypeId::kTypeInt, 27),
          Field(TypeId::kTypeChar, const_cast<char *>("minisql"), 7, true)
  };
  Row key(fields);
  INDEX_KEY_TYPE k1;
  k1.SerializeFromKey(key, key_schema);
  INDEX_KEY_TYPE k2;
  Row copy_key(fields);
  k2.SerializeFromKey(copy_key, key_schema);
  INDEX_COMPARATOR_TYPE comparator(key_schema);
  ASSERT_EQ(0, comparator(k1, k2));
}

TEST(BPlusTreeTests, BPlusTreeIndexSimpleTest) {
  using INDEX_KEY_TYPE = GenericKey<32>;
  using INDEX_COMPARATOR_TYPE = GenericComparator<32>;
  using BP_TREE_INDEX = BPlusTreeIndex<INDEX_KEY_TYPE, RowId, INDEX_COMPARATOR_TYPE>;
  DBStorageEngine engine(db_name);
  SimpleMemHeap heap;
  std::vector<Column *> columns = {
          ALLOC_COLUMN(heap)("id", TypeId::kTypeInt, 0, false, false),
          ALLOC_COLUMN(heap)("name", TypeId::kTypeChar, 64, 1, true, false),
          ALLOC_COLUMN(heap)("account", TypeId::kTypeFloat, 2, true, false)
  };
  std::vector<uint32_t> index_key_map{0, 1};
  const TableSchema table_schema(columns);
  auto *index_schema = Schema::ShallowCopySchema(&table_schema, index_key_map, &heap);
  auto *index = ALLOC(heap, BP_TREE_INDEX)(0, index_schema, engine.bpm_);
  for (int i = 0; i < 10; i++) {
    std::vector<Field> fields{
            Field(TypeId::kTypeInt, i),
            Field(TypeId::kTypeChar, const_cast<char *>("minisql"), 7, true)
    };
    Row row(fields);
    RowId rid(1000, i);
    ASSERT_EQ(DB_SUCCESS, index->InsertEntry(row, rid, nullptr));
  }
  // Test Scan
  std::vector<RowId> ret;
  for (int i = 0; i < 10; i++) {
    std::vector<Field> fields{
            Field(TypeId::kTypeInt, i),
            Field(TypeId::kTypeChar, const_cast<char *>("minisql"), 7, true)
    };
    Row row(fields);
    RowId rid(1000, i);
    ASSERT_EQ(DB_SUCCESS, index->ScanKey(row, ret, nullptr));
    ASSERT_EQ(rid.Get(), ret[i].Get());
  }
  // Iterator Scan
  IndexIterator<INDEX_KEY_TYPE, RowId, INDEX_COMPARATOR_TYPE> iter = index->GetBeginIterator();
  uint32_t i = 0;
  for (; iter != index->GetEndIterator(); ++iter) {
    ASSERT_EQ(1000, (*iter).second.GetPageId());
    ASSERT_EQ(i, (*iter).second.GetSlotNum());
    i++;
  }
}