#include "catalog/catalog.h"
#include "common/instance.h"
#include "gtest/gtest.h"
#include "utils/utils.h"

static string db_file_name = "catalog_test.db";

TEST(CatalogTest, CatalogMetaTest) {
  SimpleMemHeap heap;
  char *buf = reinterpret_cast<char *>(heap.Allocate(PAGE_SIZE));
  CatalogMeta *meta = CatalogMeta::NewInstance(&heap);
  // fill data
  const int table_nums = 16;
  const int index_nums = 24;
  for (auto i = 0; i < table_nums; i++) {
    meta->GetTableMetaPages()->emplace(i, RandomUtils::RandomInt(0, 1 << 16));
  }
  meta->GetTableMetaPages()->emplace(table_nums, INVALID_PAGE_ID);
  for (auto i = 0; i < index_nums; i++) {
    meta->GetIndexMetaPages()->emplace(i, RandomUtils::RandomInt(0, 1 << 16));
  }
  meta->GetIndexMetaPages()->emplace(index_nums, INVALID_PAGE_ID);
  // serialize
  meta->SerializeTo(buf);
  // deserialize
  CatalogMeta *other = CatalogMeta::DeserializeFrom(buf, &heap);
  ASSERT_NE(nullptr, other);
  ASSERT_EQ(table_nums + 1, other->GetTableMetaPages()->size());
  ASSERT_EQ(index_nums + 1, other->GetIndexMetaPages()->size());
  ASSERT_EQ(INVALID_PAGE_ID, other->GetTableMetaPages()->at(table_nums));
  ASSERT_EQ(INVALID_PAGE_ID, other->GetIndexMetaPages()->at(index_nums));
  for (auto i = 0; i < table_nums; i++) {
    EXPECT_EQ(meta->GetTableMetaPages()->at(i), other->GetTableMetaPages()->at(i));
  }
  for (auto i = 0; i < index_nums; i++) {
    EXPECT_EQ(meta->GetIndexMetaPages()->at(i), other->GetIndexMetaPages()->at(i));
  }
}

TEST(CatalogTest, CatalogTableTest) {
  SimpleMemHeap heap;
  /** Stage 2: Testing simple operation */
  auto db_01 = new DBStorageEngine(db_file_name, true);
  auto &catalog_01 = db_01->catalog_mgr_;
  TableInfo *table_info = nullptr;
  ASSERT_EQ(DB_TABLE_NOT_EXIST, catalog_01->GetTable("table-1", table_info));
  std::vector<Column *> columns = {
          ALLOC_COLUMN(heap)("id", TypeId::kTypeInt, 0, false, false),
          ALLOC_COLUMN(heap)("name", TypeId::kTypeChar, 64, 1, true, false),
          ALLOC_COLUMN(heap)("account", TypeId::kTypeFloat, 2, true, false)
  };
  auto schema = std::make_shared<Schema>(columns);
  Transaction txn;
  catalog_01->CreateTable("table-1", schema.get(), &txn, table_info);
  ASSERT_TRUE(table_info != nullptr);
  TableInfo *table_info_02 = nullptr;
  ASSERT_EQ(DB_SUCCESS, catalog_01->GetTable("table-1", table_info_02));
  ASSERT_EQ(table_info, table_info_02);
  auto *table_heap = table_info->GetTableHeap();
  ASSERT_TRUE(table_heap != nullptr);
  delete db_01;
  /** Stage 2: Testing catalog loading */
  auto db_02 = new DBStorageEngine(db_file_name, false);
  auto &catalog_02 = db_02->catalog_mgr_;
  TableInfo *table_info_03 = nullptr;
  ASSERT_EQ(DB_TABLE_NOT_EXIST, catalog_02->GetTable("table-2", table_info_03));
  ASSERT_EQ(DB_SUCCESS, catalog_02->GetTable("table-1", table_info_03));
  delete db_02;
}

TEST(CatalogTest, CatalogIndexTest) {
  SimpleMemHeap heap;
  /** Stage 1: Testing simple operation */
  auto db_01 = new DBStorageEngine(db_file_name, true);
  auto &catalog_01 = db_01->catalog_mgr_;
  TableInfo *table_info = nullptr;
  ASSERT_EQ(DB_TABLE_NOT_EXIST, catalog_01->GetTable("table-1", table_info));
  std::vector<Column *> columns = {
          ALLOC_COLUMN(heap)("id", TypeId::kTypeInt, 0, false, false),
          ALLOC_COLUMN(heap)("name", TypeId::kTypeChar, 64, 1, true, false),
          ALLOC_COLUMN(heap)("account", TypeId::kTypeFloat, 2, true, false)
  };
  auto schema = std::make_shared<Schema>(columns);
  Transaction txn;
  catalog_01->CreateTable("table-1", schema.get(), &txn, table_info);
  ASSERT_TRUE(table_info != nullptr);

  IndexInfo *index_info = nullptr;
  std::vector<std::string> bad_index_keys{"id", "age", "name"};
  std::vector<std::string> index_keys{"id", "name"};
  auto r1 = catalog_01->CreateIndex("table-0", "index-0", index_keys, &txn, index_info);
  ASSERT_EQ(DB_TABLE_NOT_EXIST, r1);
  auto r2 = catalog_01->CreateIndex("table-1", "index-1", bad_index_keys, &txn, index_info);
  ASSERT_EQ(DB_COLUMN_NAME_NOT_EXIST, r2);
  auto r3 = catalog_01->CreateIndex("table-1", "index-1", index_keys, &txn, index_info);
  ASSERT_EQ(DB_SUCCESS, r3);
  for (int i = 0; i < 10; i++) {
    std::vector<Field> fields{
            Field(TypeId::kTypeInt, i),
            Field(TypeId::kTypeChar, const_cast<char *>("minisql"), 7, true)
    };
    Row row(fields);
    RowId rid(1000, i);
    ASSERT_EQ(DB_SUCCESS, index_info->GetIndex()->InsertEntry(row, rid, nullptr));
  }
  // Scan Key
  std::vector<RowId> ret;
  for (int i = 0; i < 10; i++) {
    std::vector<Field> fields{
            Field(TypeId::kTypeInt, i),
            Field(TypeId::kTypeChar, const_cast<char *>("minisql"), 7, true)
    };
    Row row(fields);
    RowId rid(1000, i);
    ASSERT_EQ(DB_SUCCESS, index_info->GetIndex()->ScanKey(row, ret, &txn));
    ASSERT_EQ(rid.Get(), ret[i].Get());
  }
  delete db_01;
  /** Stage 2: Testing catalog loading */
  auto db_02 = new DBStorageEngine(db_file_name, false);
  auto &catalog_02 = db_02->catalog_mgr_;
  auto r4 = catalog_02->CreateIndex("table-1", "index-1", index_keys, &txn, index_info);
  ASSERT_EQ(DB_INDEX_ALREADY_EXIST, r4);
  IndexInfo *index_info_02 = nullptr;
  ASSERT_EQ(DB_SUCCESS, catalog_02->GetIndex("table-1", "index-1", index_info_02));
  std::vector<RowId> ret_02;
  for (int i = 0; i < 10; i++) {
    std::vector<Field> fields{
            Field(TypeId::kTypeInt, i),
            Field(TypeId::kTypeChar, const_cast<char *>("minisql"), 7, true)
    };
    Row row(fields);
    RowId rid(1000, i);
    ASSERT_EQ(DB_SUCCESS, index_info_02->GetIndex()->ScanKey(row, ret_02, &txn));
    ASSERT_EQ(rid.Get(), ret_02[i].Get());
  }
  delete db_02;
}