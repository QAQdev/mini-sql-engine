#include "common/instance.h"
#include "gtest/gtest.h"
#include "index/b_plus_tree.h"
#include "index/basic_comparator.h"
#include "utils/tree_file_mgr.h"
#include "utils/utils.h"

static const std::string db_name = "bp_tree_insert_test.db";

TEST(BPlusTreeTests, SampleTest) {
  // Init engine
  DBStorageEngine engine(db_name);
  BasicComparator<int> comparator;
  BPlusTree<int, int, BasicComparator<int>> tree(0, engine.bpm_, comparator, 4, 4);
  TreeFileManagers mgr("tree_");
  // Prepare data
  const int n = 1000;
  vector<int> keys;
  vector<int> values;
  vector<int> delete_seq;
  map<int, int> kv_map;
  // srand(1);
  // for (int i = 0; i < n; i++) 
  // {
  //   int val = rand()%20000;
  //   keys.push_back(val);
  //   values.push_back(val);
  //   delete_seq.push_back(val);
  // }
  for (int i = 0; i < n; i++) 
  {
    keys.push_back(i);
    values.push_back(i);
    delete_seq.push_back(i);
  }
  // keys.push_back(5);
  // values.push_back(5);
  // keys.push_back(9);
  // values.push_back(9);
  // keys.push_back(1);
  // values.push_back(1);
  // Shuffle data
  ShuffleArray(keys);
  ShuffleArray(values);
  ShuffleArray(delete_seq);
  // Map key value
  for (int i = 0; i < n; i++) 
  {
    kv_map[keys[i]] = values[i];
  }
  // Insert data
  for(int i = 0; i < n; i++)
  {
    tree.Insert(keys[i], values[i]);
    // tree.PrintTree(mgr[i]);
  }
  // for(int i = 5000; i < n; i++)
  // {
  //   tree.Insert(keys[i], values[i]);
  //   // tree.PrintTree(mgr[i]);
  // }
  // tree.PrintTree(mgr[0]);
  ASSERT_TRUE(tree.Check());
  // Print tree
  // tree.PrintTree(mgr[0]);
  // Search keys
  vector<int> ans;
  for (int i = 0; i < n; i++) 
  {
    tree.GetValue(i, ans);
    ASSERT_EQ(kv_map[i], ans[i]);
  }
  // ASSERT_TRUE(tree.Check());
  // tree.PrintTree(mgr[1]);
  // Delete half keys
  for (int i = 0; i < n/2; i++) 
  {
    tree.Remove(delete_seq[i]);
    // tree.PrintTree(mgr[i]);
  }
  // tree.PrintTree(mgr[2]);
  // for (int i = 11; i < 5*n/6; i++) 
  // {
  //   tree.Remove(delete_seq[i]);
  //   // tree.PrintTree(mgr[i]);
  // }
  // tree.PrintTree(mgr[10]);
  // // tree.Remove(0);
  // // tree.Remove(1);
  // // ASSERT_FALSE(tree.GetValue(0, ans)); 
  // // tree.PrintTree(mgr[17]);
  // Check valid
  ans.clear();
  // // ASSERT_FALSE(tree.GetValue(1, ans));
  for (int i = 0; i < n / 2; i++) 
  {
    ASSERT_FALSE(tree.GetValue(delete_seq[i], ans));
  }
  // ans.clear();
  // tree.PrintTree(mgr[20]);
  for (int i = n / 2; i < n; i++) 
  {
    ASSERT_TRUE(tree.GetValue(delete_seq[i], ans));
    ASSERT_EQ(kv_map[delete_seq[i]], ans[ans.size() - 1]);
  }


}