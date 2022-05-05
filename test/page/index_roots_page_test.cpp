#include <unordered_set>

#include "gtest/gtest.h"
#include "page/index_roots_page.h"

TEST(PageTests, IndexRootsPageTest) {
  char *buf = new char[PAGE_SIZE];
  memset(buf, 0, PAGE_SIZE);
  auto *page = reinterpret_cast<IndexRootsPage *>(buf);
  page->Init();
  for (int i = 0; i < 25; i++) {
    ASSERT_FALSE(page->Delete(i));
    ASSERT_TRUE(page->Insert(i, i * 100));
  }
  for (int i = 0; i < 25; i++) {
    ASSERT_FALSE(page->Insert(i, 0));
    page_id_t id;
    ASSERT_TRUE(page->GetRootId(i, &id));
    ASSERT_EQ(i * 100, id);
  }
  for (int i = 0; i < 25; i++) {
    ASSERT_TRUE(page->Update(i, i + 100));
    page_id_t id;
    ASSERT_TRUE(page->GetRootId(i, &id));
    ASSERT_EQ(i + 100, id);
  }
  std::unordered_set<index_id_t> set{0, 4, 5, 13, 12, 7, 22, 24};
  for (auto v : set) {
    ASSERT_TRUE(page->Delete(v));
  }
  for (int i = 0; i < 25; i++) {
    page_id_t id;
    if (set.find(i) != set.end()) {
      ASSERT_FALSE(page->GetRootId(i, &id));
    } else {
      ASSERT_TRUE(page->GetRootId(i, &id));
      ASSERT_EQ(i + 100, id);
    }
  }
  delete[] buf;
}
