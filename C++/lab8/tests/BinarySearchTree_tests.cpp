#include "lib/BinarySearchTree.hpp"
#include "gtest/gtest.h"
#include <vector>

class BinarySearchTreeTest : public ::testing::Test {
public:
  BinarySearchTree<int> int_tree_;
  BinarySearchTree<int> int_tree2_;
};

TEST_F(BinarySearchTreeTest, SingleInsertConstReferenceTest) {
  int_tree_.InitializeBSTobject();
  int a = 2;
  auto res = int_tree_.insert(a);
  ASSERT_EQ(*res.first, 2);
  ASSERT_TRUE(res.second);
  auto sec_res = int_tree_.insert(a);
  ASSERT_EQ(*sec_res.first, 2);
  ASSERT_FALSE(sec_res.second);
}

TEST_F(BinarySearchTreeTest, SingleInsertRvalueTest) {
  int_tree_.InitializeBSTobject();
  auto res = int_tree_.insert(2);
  ASSERT_EQ(*res.first, 2);
  ASSERT_FALSE(!res.second);
  auto sec_res = int_tree_.insert(2);
  ASSERT_EQ(*sec_res.first, 2);
  ASSERT_FALSE(sec_res.second);
}

TEST_F(BinarySearchTreeTest, InsertArrayIteratorsTest) {
  int_tree_.InitializeBSTobject();
  std::vector<int> test_arr = {1, 2, 3, 4, 5, 6, 7, 8};
  int_tree_.insert(test_arr.begin(), test_arr.end());
  std::vector<int> res;
  for (auto it = int_tree_.begin<tag::inorder_tag>(); it != int_tree_.end<tag::inorder_tag>(); ++it) {
    res.push_back(*it);
  }
  ASSERT_EQ(test_arr, res);
}

TEST_F(BinarySearchTreeTest, InsertInitializerListTest) {
  int_tree_.InitializeBSTobject();
  std::vector<int> ilist = {1, 2, 3, 4, 5, 6, 7, 8};
  int_tree_.insert({1, 2, 3, 4, 5, 6, 7, 8});
  std::vector<int> res;
  for (auto it = int_tree_.begin<tag::inorder_tag>(); it != int_tree_.end<tag::inorder_tag>(); ++it) {
    res.push_back(*it);
  }
  ASSERT_EQ(ilist, res);
}

TEST_F(BinarySearchTreeTest, InsertEmptyInitializerListTest) {
  int_tree_.InitializeBSTobject();
  int_tree_.insert({});
  ASSERT_TRUE(int_tree_.empty());
}

TEST_F(BinarySearchTreeTest, ConstInorderIteratorTest) {
  int_tree_.InitializeBSTobject();
  std::vector<int> ilist = {3, 12, 25, 26, 27, 30, 35, 36, 38, 40, 50, 75, 79, 80};
  
  int_tree_.insert({50, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3, 75, 80, 79});
  std::vector<int> res;
  
  for (auto it = int_tree_.cbegin<tag::inorder_tag>(); it != int_tree_.cend<tag::inorder_tag>(); ++it) {
    res.push_back(*it);
    ASSERT_TRUE(std::is_const<typename std::remove_reference<decltype(*it)>::type>::value);
  }
  ASSERT_EQ(ilist, res);
}

TEST_F(BinarySearchTreeTest, ConstPreorderIteratorTest) {
  int_tree_.InitializeBSTobject();
  std::vector<int> ilist = {50, 25, 12, 3, 30, 27, 26, 35, 40, 36, 38, 75, 80, 79};
  
  int_tree_.insert({50, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3, 75, 80, 79});
  std::vector<int> res;
  
  for (auto it = int_tree_.cbegin<tag::preorder_tag>(); it != int_tree_.cend<tag::preorder_tag>(); ++it) {
    res.push_back(*it);
    ASSERT_TRUE(std::is_const<typename std::remove_reference<decltype(*it)>::type>::value);
  }
  ASSERT_EQ(ilist, res);
}


TEST_F(BinarySearchTreeTest, ConstPostorderIteratorTest) {
  int_tree_.InitializeBSTobject();
  std::vector<int> ilist = {3, 12, 26, 27, 38, 36, 40, 35, 30, 25, 79, 80, 75, 50};
  
  int_tree_.insert({50, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3, 75, 80, 79});
  std::vector<int> res;
  
  for (auto it = int_tree_.cbegin<tag::postorder_tag>(); it != int_tree_.cend<tag::postorder_tag>(); ++it) {
    res.push_back(*it);
    ASSERT_TRUE(std::is_const<typename std::remove_reference<decltype(*it)>::type>::value);
  }
  ASSERT_EQ(ilist, res);
}

TEST_F(BinarySearchTreeTest, InorderIteratorTest) {
  int_tree_.InitializeBSTobject();
  std::vector<int> ilist = {3, 12, 25, 26, 27, 30, 35, 36, 38, 40, 50, 75, 79, 80};
  
  int_tree_.insert({50, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3, 75, 80, 79});
  std::vector<int> res;
  
  for (auto it = int_tree_.begin<tag::inorder_tag>(); it != int_tree_.end<tag::inorder_tag>(); ++it) {
    res.push_back(*it);
  }
  ASSERT_EQ(ilist, res);
}

TEST_F(BinarySearchTreeTest, PreorderIteratorTest) {
  int_tree_.InitializeBSTobject();
  std::vector<int> ilist = {50, 25, 12, 3, 30, 27, 26, 35, 40, 36, 38, 75, 80, 79};
  
  int_tree_.insert({50, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3, 75, 80, 79});
  std::vector<int> res;
  
  for (auto it = int_tree_.begin<tag::preorder_tag>(); it != int_tree_.end<tag::preorder_tag>(); ++it) {
    res.push_back(*it);
  }
  ASSERT_EQ(ilist, res);
}

TEST_F(BinarySearchTreeTest, PostorderIteratorTest) {
  int_tree_.InitializeBSTobject();
  std::vector<int> ilist = {3, 12, 26, 27, 38, 36, 40, 35, 30, 25, 79, 80, 75, 50};
  
  int_tree_.insert({50, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3, 75, 80, 79});
  std::vector<int> res;
  
  for (auto it = int_tree_.begin<tag::postorder_tag>(); it != int_tree_.end<tag::postorder_tag>(); ++it) {
    res.push_back(*it);
  }
  ASSERT_EQ(ilist, res);
}

TEST_F(BinarySearchTreeTest, ClearTest) {
  int_tree_.InitializeBSTobject();
  int_tree_.insert({50, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3, 75, 80, 79});
  ASSERT_EQ(int_tree_.size(), 14);
  int_tree_.clear();
  ASSERT_EQ(int_tree_.size(), 0);
}

TEST_F(BinarySearchTreeTest, EmplaceTest) {
  int_tree_.InitializeBSTobject();
  std::vector<int> ilist = {3, 12, 25, 26, 27, 30, 35, 36, 38, 40, 50, 75, 79, 80};
  
  int_tree_.emplace<tag::inorder_tag>(50, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3, 75, 80, 79);
  std::vector<int> res;
  
  for (auto it = int_tree_.begin<tag::inorder_tag>(); it != int_tree_.end<tag::inorder_tag>(); ++it) {
    res.push_back(*it);
  }
  ASSERT_EQ(ilist, res);
}

TEST_F(BinarySearchTreeTest, FindTest) {
  int_tree_.InitializeBSTobject();
  int_tree_.emplace<tag::inorder_tag>(50, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3, 75, 80, 79);
  auto it1 = int_tree_.find<tag::inorder_tag>(50);
  auto it2 = int_tree_.find<tag::inorder_tag>(38);
  auto it3 = int_tree_.find<tag::inorder_tag>(3);
  auto it4 = int_tree_.find<tag::inorder_tag>(80);
  auto it5 = int_tree_.find<tag::inorder_tag>(100);
  ASSERT_EQ(*it1, 50);
  ASSERT_EQ(*it2, 38);
  ASSERT_EQ(*it3, 3);
  ASSERT_EQ(*it4, 80);
  ASSERT_THROW(*it5, std::runtime_error);
}

TEST_F(BinarySearchTreeTest, ContainsTest) {
  int_tree_.InitializeBSTobject();
  int_tree_.emplace<tag::inorder_tag>(50, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3, 75, 80, 79);
  bool b1 = int_tree_.contains(50);
  bool b2 = int_tree_.contains(38);
  bool b3 = int_tree_.contains(3);
  bool b4 = int_tree_.contains(80);
  bool b5 = int_tree_.contains(100);

  ASSERT_EQ(b1, true);
  ASSERT_EQ(b2, true);
  ASSERT_EQ(b3, true);
  ASSERT_EQ(b4, true);
  ASSERT_EQ(b5, false);
}

TEST_F(BinarySearchTreeTest, EraseTopValueTest) {
  int_tree_.InitializeBSTobject();
  int_tree_.emplace<tag::inorder_tag>(50, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3, 75, 80, 79);
  int e1 = int_tree_.erase(50);
  ASSERT_EQ(e1, 1);
  std::vector<int> res;
  std::vector<int> ilist = {3, 12, 25, 26, 27, 30, 35, 36, 38, 40, 75, 79, 80};
  for (auto it = int_tree_.begin<tag::inorder_tag>(); it != int_tree_.end<tag::inorder_tag>(); ++it) {
    res.push_back(*it);
  }
  ASSERT_EQ(res, ilist);
}

TEST_F(BinarySearchTreeTest, EraseRightmostValueTest) {
  int_tree_.InitializeBSTobject();
  int_tree_.emplace<tag::inorder_tag>(50, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3, 75, 80, 79);
  int e1 = int_tree_.erase(80);
  ASSERT_EQ(e1, 1);
  std::vector<int> res;
  std::vector<int> ilist = {3, 12, 25, 26, 27, 30, 35, 36, 38, 40, 50, 75, 79};
  for (auto it = int_tree_.begin<tag::inorder_tag>(); it != int_tree_.end<tag::inorder_tag>(); ++it) {
    res.push_back(*it);
  }
  ASSERT_EQ(res, ilist);
}

TEST_F(BinarySearchTreeTest, EraseLeftmostValueTest) {
  int_tree_.InitializeBSTobject();
  int_tree_.emplace<tag::inorder_tag>(50, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3, 75, 80, 79);
  int e1 = int_tree_.erase(3);
  ASSERT_EQ(e1, 1);
  std::vector<int> res;
  std::vector<int> ilist = {12, 25, 26, 27, 30, 35, 36, 38, 40, 50, 75, 79, 80};
  for (auto it = int_tree_.begin<tag::inorder_tag>(); it != int_tree_.end<tag::inorder_tag>(); ++it) {
    res.push_back(*it);
  }
  ASSERT_EQ(res, ilist);
}

TEST_F(BinarySearchTreeTest, EraseConstIteratorTopTest) {
  int_tree_.InitializeBSTobject();
  int_tree_.emplace<tag::inorder_tag>(50, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3, 75, 80, 79);
  auto it1 = int_tree_.erase(int_tree_.cbegin<tag::preorder_tag>());
  ASSERT_EQ(*it1, 25);
  std::vector<int> res;
  std::vector<int> ilist = {3, 12, 25, 26, 27, 30, 35, 36, 38, 40, 75, 79, 80};
  for (auto it = int_tree_.begin<tag::inorder_tag>(); it != int_tree_.end<tag::inorder_tag>(); ++it) {
    res.push_back(*it);
  }
  ASSERT_EQ(res, ilist);
}

TEST_F(BinarySearchTreeTest, EraseConstIteratorRightmostTest) {
  int_tree_.InitializeBSTobject();
  int_tree_.emplace<tag::inorder_tag>(50, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3, 75, 80, 79);
  auto it1 = int_tree_.cbegin<tag::inorder_tag>();
  for ( ; *it1 != 80; ++it1) {}
  auto it2 = int_tree_.erase(it1);
  ASSERT_THROW(*it2, std::runtime_error);
  std::vector<int> res;
  std::vector<int> ilist = {3, 12, 25, 26, 27, 30, 35, 36, 38, 40, 50, 75, 79};
  for (auto it = int_tree_.begin<tag::inorder_tag>(); it != int_tree_.end<tag::inorder_tag>(); ++it) {
    res.push_back(*it);
  }
  ASSERT_EQ(res, ilist);
}

TEST_F(BinarySearchTreeTest, EraseConstIteratorLeftmostTest) {
  int_tree_.InitializeBSTobject();
  int_tree_.emplace<tag::inorder_tag>(50, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3, 75, 80, 79);
  auto it1 = int_tree_.cbegin<tag::inorder_tag>();
  auto it2 = int_tree_.erase(it1);
  ASSERT_EQ(*it2, 12);
  std::vector<int> res;
  std::vector<int> ilist = {12, 25, 26, 27, 30, 35, 36, 38, 40, 50, 75, 79, 80};
  for (auto it = int_tree_.begin<tag::inorder_tag>(); it != int_tree_.end<tag::inorder_tag>(); ++it) {
    res.push_back(*it);
  }
  ASSERT_EQ(res, ilist);
}

TEST_F(BinarySearchTreeTest, EraseConstIteratorIternalTest) {
  int_tree_.InitializeBSTobject();
  int_tree_.emplace<tag::inorder_tag>(50, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3, 75, 80, 79);
  auto it1 = int_tree_.cbegin<tag::inorder_tag>();
  for ( ; *it1 != 38; ++it1) {}
  auto it2 = int_tree_.erase(it1);
  ASSERT_EQ(*it2, 40);
  std::vector<int> res;
  std::vector<int> ilist = {3, 12, 25, 26, 27, 30, 35, 36, 40, 50, 75, 79, 80};
  for (auto it = int_tree_.begin<tag::inorder_tag>(); it != int_tree_.end<tag::inorder_tag>(); ++it) {
    res.push_back(*it);
  }
  ASSERT_EQ(res, ilist);
}

TEST_F(BinarySearchTreeTest, ExtactRightmostValueTest) {
  int_tree_.InitializeBSTobject();
  int_tree_.emplace<tag::inorder_tag>(50, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3, 75, 80, 79);
  auto node = int_tree_.extract(80);
  ASSERT_EQ(node.value_, 80);
  std::vector<int> res;
  std::vector<int> ilist = {3, 12, 25, 26, 27, 30, 35, 36, 38, 40, 50, 75, 79};
  for (auto it = int_tree_.begin<tag::inorder_tag>(); it != int_tree_.end<tag::inorder_tag>(); ++it) {
    res.push_back(*it);
  }
  ASSERT_EQ(res, ilist);
}

TEST_F(BinarySearchTreeTest, ExtactLeftmostValueTest) {
  int_tree_.InitializeBSTobject();
  int_tree_.emplace<tag::inorder_tag>(50, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3, 75, 80, 79);
  auto node = int_tree_.extract(3);
  ASSERT_EQ(node.value_, 3);
  std::vector<int> res;
  std::vector<int> ilist = {12, 25, 26, 27, 30, 35, 36, 38, 40, 50, 75, 79, 80};
  for (auto it = int_tree_.begin<tag::inorder_tag>(); it != int_tree_.end<tag::inorder_tag>(); ++it) {
    res.push_back(*it);
  }
  ASSERT_EQ(res, ilist);
}

TEST_F(BinarySearchTreeTest, ExtactTopValueTest) {
  int_tree_.InitializeBSTobject();
  int_tree_.emplace<tag::inorder_tag>(50, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3, 75, 80, 79);
  auto node = int_tree_.extract(50);
  ASSERT_EQ(node.value_, 50);
  std::vector<int> res;
  std::vector<int> ilist = {3, 12, 25, 26, 27, 30, 35, 36, 38, 40, 75, 79, 80};
  for (auto it = int_tree_.begin<tag::inorder_tag>(); it != int_tree_.end<tag::inorder_tag>(); ++it) {
    res.push_back(*it);
  }
  ASSERT_EQ(res, ilist);
}

TEST_F(BinarySearchTreeTest, ExtactIternalValueTest) {
  int_tree_.InitializeBSTobject();
  int_tree_.emplace<tag::inorder_tag>(50, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3, 75, 80, 79);
  auto node = int_tree_.extract(38);
  ASSERT_EQ(node.value_, 38);
  std::vector<int> res;
  std::vector<int> ilist = {3, 12, 25, 26, 27, 30, 35, 36, 40, 50, 75, 79, 80};
  for (auto it = int_tree_.begin<tag::inorder_tag>(); it != int_tree_.end<tag::inorder_tag>(); ++it) {
    res.push_back(*it);
  }
  ASSERT_EQ(res, ilist);
}

TEST_F(BinarySearchTreeTest, ExtractTopIteratorTest) {
  int_tree_.InitializeBSTobject();
  int_tree_.emplace<tag::inorder_tag>(50, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3, 75, 80, 79);
  auto node = int_tree_.extract(int_tree_.cbegin<tag::preorder_tag>());
  ASSERT_EQ(node.value_, 50);
  std::vector<int> res;
  std::vector<int> ilist = {3, 12, 25, 26, 27, 30, 35, 36, 38, 40, 75, 79, 80};
  for (auto it = int_tree_.begin<tag::inorder_tag>(); it != int_tree_.end<tag::inorder_tag>(); ++it) {
    res.push_back(*it);
  }
  ASSERT_EQ(res, ilist);
}

TEST_F(BinarySearchTreeTest, ExtractRightmostIteratorTest) {
  int_tree_.InitializeBSTobject();
  int_tree_.emplace<tag::inorder_tag>(50, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3, 75, 80, 79);
  auto it  = int_tree_.cbegin<tag::inorder_tag>();
  for ( ; *it != 80; ++it) {}
  auto node = int_tree_.extract(it);
  ASSERT_EQ(node.value_, 80);
  std::vector<int> res;
  std::vector<int> ilist = {3, 12, 25, 26, 27, 30, 35, 36, 38, 40, 50, 75, 79};
  for (auto it = int_tree_.begin<tag::inorder_tag>(); it != int_tree_.end<tag::inorder_tag>(); ++it) {
    res.push_back(*it);
  }
  ASSERT_EQ(res, ilist);
}

TEST_F(BinarySearchTreeTest, ExtractLeftmostIteratorTest) {
  int_tree_.InitializeBSTobject();
  int_tree_.emplace<tag::inorder_tag>(50, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3, 75, 80, 79);
  auto node = int_tree_.extract(int_tree_.cbegin<tag::inorder_tag>());
  ASSERT_EQ(node.value_, 3);
  std::vector<int> res;
  std::vector<int> ilist = {12, 25, 26, 27, 30, 35, 36, 38, 40, 50, 75, 79, 80};
  for (auto it = int_tree_.begin<tag::inorder_tag>(); it != int_tree_.end<tag::inorder_tag>(); ++it) {
    res.push_back(*it);
  }
  ASSERT_EQ(res, ilist);
}

TEST_F(BinarySearchTreeTest, ExtractIternalIteratorTest) {
  int_tree_.InitializeBSTobject();
  int_tree_.emplace<tag::inorder_tag>(50, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3, 75, 80, 79);
  auto it  = int_tree_.cbegin<tag::inorder_tag>();
  for ( ; *it != 38; ++it) {}
  auto node = int_tree_.extract(it);
  ASSERT_EQ(node.value_, 38);
  std::vector<int> res;
  std::vector<int> ilist = {3, 12, 25, 26, 27, 30, 35, 36, 40, 50, 75, 79, 80};
  for (auto it = int_tree_.begin<tag::inorder_tag>(); it != int_tree_.end<tag::inorder_tag>(); ++it) {
    res.push_back(*it);
  }
  ASSERT_EQ(res, ilist);
}

TEST_F(BinarySearchTreeTest, MergeTest) {
  int_tree_.InitializeBSTobject();
  int_tree2_.InitializeBSTobject();
  int_tree_.emplace<tag::inorder_tag>(50, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3, 75, 80, 79);
  int_tree2_.emplace<tag::inorder_tag>(5, 42, 10, 172, 2, 8, 62);
  int_tree_.merge(int_tree2_);
  ASSERT_EQ(int_tree2_.size(), 0);
  ASSERT_EQ(int_tree_.size(), 21);
  std::vector<int> expected_res = {50, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3, 75, 80, 79, 5, 42, 10, 172, 2, 8, 62};
  std::vector<int> res;
  std::sort(expected_res.begin(), expected_res.end());
  for (auto it = int_tree_.begin<tag::inorder_tag>(); it != int_tree_.end<tag::inorder_tag>(); ++it) {
    res.push_back(*it);
  } 
  ASSERT_EQ(expected_res, res);
}

TEST_F(BinarySearchTreeTest, UpperBoundTest) {
  int_tree_.InitializeBSTobject();
  int_tree_.emplace<tag::inorder_tag>(50, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3, 75, 80, 79);
  auto it1 = int_tree_.upper_bound<tag::inorder_tag>(50);
  auto it2 = int_tree_.upper_bound<tag::inorder_tag>(80);
  auto it3 = int_tree_.upper_bound<tag::inorder_tag>(3);
  auto it4 = int_tree_.upper_bound<tag::inorder_tag>(38);
  ASSERT_EQ(*it1, 75);
  ASSERT_THROW(*it2, std::runtime_error);
  ASSERT_EQ(*it3, 12);
  ASSERT_EQ(*it4, 40);
}

TEST_F(BinarySearchTreeTest, LowerBoundTest) {
  int_tree_.InitializeBSTobject();
  int_tree_.emplace<tag::inorder_tag>(50, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3, 75, 80, 79);
  auto it1 = int_tree_.lower_bound<tag::inorder_tag>(50);
  auto it2 = int_tree_.lower_bound<tag::inorder_tag>(80);
  auto it3 = int_tree_.lower_bound<tag::inorder_tag>(3);
  auto it4 = int_tree_.lower_bound<tag::inorder_tag>(38);
  ASSERT_EQ(*it1, 40);
  ASSERT_EQ(*it2, 79);
  ASSERT_THROW(*it3, std::runtime_error);
  ASSERT_EQ(*it4, 36);
}

TEST_F(BinarySearchTreeTest, InorderReverseIteratorTest) {
  int_tree_.InitializeBSTobject();
  int_tree_.emplace<tag::inorder_tag>(50, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3, 75, 80, 79);
  std::vector<int> expected_res = {50, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3, 75, 80, 79};
  std::sort(expected_res.begin(), expected_res.end());
  std::reverse(expected_res.begin(), expected_res.end());
  std::vector<int> res;
  for (auto it = int_tree_.rbegin<tag::inorder_tag>(); it != int_tree_.rend<tag::inorder_tag>(); ++it) {
    res.push_back(*it);
  }
  ASSERT_EQ(expected_res, res);
}

TEST_F(BinarySearchTreeTest, InorderConstReverseIteratorTest) {
  int_tree_.InitializeBSTobject();
  int_tree_.emplace<tag::inorder_tag>(50, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3, 75, 80, 79);
  std::vector<int> expected_res = {50, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3, 75, 80, 79};
  std::sort(expected_res.begin(), expected_res.end());
  std::reverse(expected_res.begin(), expected_res.end());
  std::vector<int> res;
  for (auto it = int_tree_.crbegin<tag::inorder_tag>(); it != int_tree_.crend<tag::inorder_tag>(); ++it) {
    res.push_back(*it);
  }
  ASSERT_EQ(expected_res, res);
}

TEST_F(BinarySearchTreeTest, PreorderReverseIteratorTest) {
  int_tree_.InitializeBSTobject();
  int_tree_.emplace<tag::inorder_tag>(50, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3, 75, 80, 79);
  std::vector<int> expected_res = {79, 80, 75, 38, 36, 40, 35, 26, 27, 30, 3, 12, 25, 50};
  std::vector<int> res;
  for (auto it = int_tree_.rbegin<tag::preorder_tag>(); it != int_tree_.rend<tag::preorder_tag>(); ++it) {
    res.push_back(*it);
  }
  ASSERT_EQ(expected_res, res);
}

TEST_F(BinarySearchTreeTest, PreorderConstReverseIteratorTest) {
  int_tree_.InitializeBSTobject();
  int_tree_.emplace<tag::inorder_tag>(50, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3, 75, 80, 79);
  std::vector<int> expected_res = {79, 80, 75, 38, 36, 40, 35, 26, 27, 30, 3, 12, 25, 50};
  std::vector<int> res;
  for (auto it = int_tree_.crbegin<tag::preorder_tag>(); it != int_tree_.crend<tag::preorder_tag>(); ++it) {
    res.push_back(*it);
  }
  ASSERT_EQ(expected_res, res);
}

TEST_F(BinarySearchTreeTest, PostorderReverseIteratorTest) {
  int_tree_.InitializeBSTobject();
  int_tree_.emplace<tag::inorder_tag>(50, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3, 75, 80, 79);
  std::vector<int> expected_res = {50, 75, 80, 79, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3};
  std::vector<int> res;
  for (auto it = int_tree_.rbegin<tag::postorder_tag>(); it != int_tree_.rend<tag::postorder_tag>(); ++it) {
    res.push_back(*it);
  }
  ASSERT_EQ(expected_res, res);
}

TEST_F(BinarySearchTreeTest, ConstPostorderReverseIteratorTest) {
  int_tree_.InitializeBSTobject();
  int_tree_.emplace<tag::inorder_tag>(50, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3, 75, 80, 79);
  std::vector<int> expected_res = {50, 75, 80, 79, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3};
  std::vector<int> res;
  for (auto it = int_tree_.crbegin<tag::postorder_tag>(); it != int_tree_.crend<tag::postorder_tag>(); ++it) {
    res.push_back(*it);
  }
  ASSERT_EQ(expected_res, res);
}

TEST_F(BinarySearchTreeTest, CompareTreesTest) {
  int_tree_.InitializeBSTobject();
  int_tree_.emplace<tag::inorder_tag>(50, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3, 75, 80, 79);
  int_tree2_.InitializeBSTobject();
  int_tree2_.emplace<tag::inorder_tag>(50, 25, 30, 35, 40, 36, 38, 27, 26, 12, 3, 75, 80, 79);
  ASSERT_TRUE(int_tree_ == int_tree2_);
  int_tree2_.clear();
  ASSERT_TRUE(int_tree_ != int_tree2_);
}