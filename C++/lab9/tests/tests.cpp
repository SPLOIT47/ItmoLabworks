#include "gtest/gtest.h"
#include "src/MyAdapters.hpp"
#include <vector>

int UnaryOpFunc(int i) {
  return i * i;
}

bool UnaryPredFunc(int i) {
  return i % 2 == 0;
}

TEST(MyTransform_TEST, IteratorFuncLambda) {
  std::vector<int> a = {1, 2, 3, 4 ,5};
  std::vector<int> res;
  for (auto it : a) {
    res.push_back(it * it);
  }
  std::vector<int> b;
  my_transform(a.begin(), a.end(), std::inserter(b, b.end()), [](int i){ return i * i; });
  ASSERT_EQ(res, b);
}

TEST(MyTransform_TEST, OperatorPipeSameContainerLambda) {
  std::vector<int> a = {1, 2, 3, 4 ,5};
  std::vector<int> res;
  for (auto it : a) {
    res.push_back(it * it);
  }
  std::vector<int> b = a | my_transform<std::vector, int>([](int i){ return i*i; });
  
  ASSERT_EQ(res, b);
}

TEST(MyTransform_TEST, OperatorPipeDifferentContainerLambda) {
  std::set<int> a = {1, 2, 3, 4 ,5};
  std::vector<int> res;
  for (auto it : a) {
    res.push_back(it * it);
  }
  std::vector<int> b = a | my_transform<std::vector, int>([](int i){ return i*i; });
  
  ASSERT_EQ(res, b);
}

TEST(MyTransform_TEST, IteratorFuncSTDFunction) {
  std::vector<int> a = {1, 2, 3, 4 ,5};
  std::vector<int> res;
  for (auto it : a) {
    res.push_back(it * it);
  }
  std::vector<int> b;
  
  std::function<int(int)> func = [](int i) { return UnaryOpFunc(i); };
  my_transform(a.begin(), a.end(), std::inserter(b, b.end()), func);
  ASSERT_EQ(res, b);
}

TEST(MyTransform_TEST, OperatorPipeSameTypeSTDFunction) {
  std::vector<int> a = {1, 2, 3, 4 ,5};
  std::vector<int> res;
  for (auto it : a) {
    res.push_back(it * it);
  }

  std::function<int(int)> func = [](int i) { return UnaryOpFunc(i); };
  std::vector<int> b = a | my_transform<std::vector, int>(func);
  
  ASSERT_EQ(res, b);
}

TEST(MyTransform_TEST, OperatorPipeDifferentSTDFunction) {
  std::set<int> a = {1, 2, 3, 4 ,5};
  std::vector<int> res;
  for (auto it : a) {
    res.push_back(it * it);
  }

  std::function<int(int)> func = [](int i) { return UnaryOpFunc(i); };
  std::vector<int> b = a | my_transform<std::vector, int>(func);
  
  ASSERT_EQ(res, b);
}

TEST(MyFilter_TEST, IteratorFuncLambda) {
  std::vector<int> a = {1, 2, 3, 4 ,5};
  std::vector<int> res;
  for (auto it : a) {
    if (it % 2 == 0) res.push_back(it);
  }
  std::vector<int> b;
  my_filter(a.begin(), a.end(), std::inserter(b, b.end()), [](int i){ return i % 2 == 0; });
  ASSERT_EQ(res, b);
}

TEST(MyFilter_TEST, OperatorPipeSameContainerLambda) {
  std::vector<int> a = {1, 2, 3, 4 ,5};
  std::vector<int> res;
  for (auto it : a) {
    if (it % 2 == 0) res.push_back(it);
  }
  std::vector<int> b = a | my_filter<std::vector, int>([](int i){ return i % 2 == 0; });
  
  ASSERT_EQ(res, b);
}

TEST(MyFilter_TEST, OperatorPipeDifferentContainerLambda) {
  std::set<int> a = {1, 2, 3, 4 ,5};
  std::vector<int> res;
  for (auto it : a) {
    if (it % 2 == 0) res.push_back(it);
  }
  std::vector<int> b = a | my_filter<std::vector, int>([](int i){ return i % 2 == 0; });
  
  ASSERT_EQ(res, b);
}

TEST(MyFilter_TEST, IteratorFuncSTDFunction) {
  std::vector<int> a = {1, 2, 3, 4 ,5};
  std::vector<int> res;
  for (auto it : a) {
    if (it % 2 == 0) res.push_back(it);
  }
  std::vector<int> b;
  
  std::function<bool(int)> func = [](int i) { return UnaryPredFunc(i); };
  my_filter(a.begin(), a.end(), std::inserter(b, b.end()), func);
  ASSERT_EQ(res, b);
}

TEST(MyFilter_TEST, OperatorPipeSameTypeSTDFunction) {
  std::vector<int> a = {1, 2, 3, 4 ,5};
  std::vector<int> res;
  for (auto it : a) {
    if (it % 2 == 0) res.push_back(it);
  }

  std::function<bool(int)> func = [](int i) { return UnaryPredFunc(i); };
  std::vector<int> b = a | my_filter<std::vector, int>(func);
  
  ASSERT_EQ(res, b);
}

TEST(MyFilter_TEST, OperatorPipeDifferentSTDFunction) {
  std::set<int> a = {1, 2, 3, 4 ,5};
  std::vector<int> res;
  for (auto it : a) {
    if (it % 2 == 0) res.push_back(it);
  }

  std::function<bool(int)> func = [](int i) { return UnaryPredFunc(i); };
  std::vector<int> b = a | my_filter<std::vector, int>(func);
  
  ASSERT_EQ(res, b);
}

TEST(MyTake_TEST, IteratorFunc) {
  std::vector<int> a = {1, 2, 3, 4 ,5};
  std::vector<int> res;
  int N = 3;
  int N1 = N;
  for (auto it : a) {
    --N;
    if (N >= 0) res.push_back(it);
  }
  std::vector<int> b;
  my_take(a.begin(), a.end(), std::inserter(b, b.end()), N1);
  ASSERT_EQ(res, b);
}

TEST(MyTake_TEST, OperatorPipeSameContainer) {
  std::vector<int> a = {1, 2, 3, 4 ,5};
  std::vector<int> res;
  int N = 3;
  int N1 = N;
  for (auto it : a) {
    --N;
    if (N >= 0) res.push_back(it);
  }
  std::vector<int> b = a | my_take<std::vector, int>(N1);
  
  ASSERT_EQ(res, b);
}

TEST(MyTake_TEST, OperatorPipeDifferentContainer) {
  std::set<int> a = {1, 2, 3, 4 ,5};
  std::vector<int> res;
  int N = 3;
  int N1 = N;
  for (auto it : a) {
    --N;
    if (N >= 0) res.push_back(it);
  }
  std::vector<int> b = a | my_take<std::vector, int>(N1);
  
  ASSERT_EQ(res, b);
}

TEST(MyDrop_TEST, IteratorFunc) {
  std::vector<int> a = {1, 2, 3, 4 ,5};
  std::vector<int> res;
  int N = 3;
  int N1 = N;
  for (auto it : a) {
    --N;
    if (N < 0) res.push_back(it);
  }
  std::vector<int> b;
  my_drop(a.begin(), a.end(), std::inserter(b, b.end()), N1);
  ASSERT_EQ(res, b);
}

TEST(MyDrop_TEST, OperatorPipeSameContainer) {
  std::vector<int> a = {1, 2, 3, 4 ,5};
  std::vector<int> res;
  int N = 3;
  int N1 = N;
  for (auto it : a) {
    --N;
    if (N < 0) res.push_back(it);
  }
  std::vector<int> b = a | my_drop<std::vector, int>(N1);
  
  ASSERT_EQ(res, b);
}

TEST(MyDrop_TEST, OperatorPipeDifferentContainer) {
  std::set<int> a = {1, 2, 3, 4 ,5};
  std::vector<int> res;
  int N = 3;
  int N1 = N;
  for (auto it : a) {
    --N;
    if (N < 0) res.push_back(it);
  }
  std::vector<int> b = a | my_drop<std::vector, int>(N1);
  
  ASSERT_EQ(res, b);
}

TEST(MyReverse_TEST, IteratorFunc) {
  std::vector<int> a = {1, 2, 3, 4 ,5};
  std::vector<int> res;
  auto it = a.end();
  --it;
  for (; it != a.begin(); --it) {
    res.push_back(*it);
  }
  res.push_back(*it);
  std::vector<int> b;
  my_reverse(a.begin(), a.end(), std::inserter(b, b.end()));
  ASSERT_EQ(res, b);
}

TEST(MyReverse_TEST, OperatorPipeSameContainer) {
  std::vector<int> a = {1, 2, 3, 4 ,5};
  std::vector<int> res;
  auto it = a.end();
  --it;
  for (; it != a.begin(); --it) {
    res.push_back(*it);
  }
  res.push_back(*it);
  std::vector<int> b = a | my_reverse<std::vector, int>();
  
  ASSERT_EQ(res, b);
}

TEST(MyReverse_TEST, OperatorPipeDifferentContainer) {
  std::set<int> a = {1, 2, 3, 4 ,5};
  std::vector<int> res;
  auto it = a.end();
  --it;
  for (; it != a.begin(); --it) {
    res.push_back(*it);
  }
  res.push_back(*it);

  std::vector<int> b = a | my_reverse<std::vector, int>();
  
  ASSERT_EQ(res, b);
}

TEST(MyKeys_TEST, IteratorFuncKeysEqValues) {
  std::set<int> a = {1, 2, 3, 4 ,5};
  std::set<int> res = a;
  std::set<int> b;
  my_keys(a.begin(), a.end(), std::inserter(b, b.end()));
  ASSERT_EQ(res, b);
}

TEST(MyKeys_TEST, OperatorPipeSameContainerKeysEqValues) {
  std::set<int> a = {1, 2, 3, 4 ,5};
  std::set<int> res = a;
  std::set<int> b = a | my_keys<std::set, int>();
  
  ASSERT_EQ(res, b);
}

TEST(MyKeys_TEST, OperatorPipeDifferentContainerKeysEqValues) {
  std::set<int> a = {1, 2, 3, 4 ,5};
  std::vector<int> res;
  for (auto it : a) {
    res.push_back(it);
  }
  std::vector<int> b = a | my_keys<std::vector, int>();
  
  ASSERT_EQ(res, b);
}

TEST(MyKeys_TEST, IteratorFuncKeysNotEqValues) {
  std::map<int, int> a = {{1, 2}, {3, 4}, {5, 6}};
  std::vector<int> res;
  for (auto it : a) {
    res.push_back(it.first);
  }
  std::vector<int> b;
  my_keys(a.begin(), a.end(), std::inserter(b, b.end()));
  ASSERT_EQ(res, b);
}

TEST(MyKeys_TEST, OperatorPipeKeysNotEqValues) {
  std::map<int, int> a = {{1, 2}, {3, 4}, {5, 6}};
  std::vector<int> res;
  for (auto it : a) {
    res.push_back(it.first);
  }
  std::vector<int> b = a | my_keys<std::vector, int>();
  
  ASSERT_EQ(res, b);
}

TEST(MyValues_TEST, IteratorFuncKeysEqValues) {
  std::set<int> a = {1, 2, 3, 4 ,5};
  std::set<int> res = a;
  std::set<int> b;
  my_values(a.begin(), a.end(), std::inserter(b, b.end()));
  ASSERT_EQ(res, b);
}

TEST(MyValues_TEST, OperatorPipeSameContainerKeysEqValues) {
  std::set<int> a = {1, 2, 3, 4 ,5};
  std::set<int> res = a;
  std::set<int> b = a | my_values<std::set, int>();
  
  ASSERT_EQ(res, b);
}

TEST(MyValues_TEST, OperatorPipeDifferentContainerKeysEqValues) {
  std::set<int> a = {1, 2, 3, 4 ,5};
  std::vector<int> res;
  for (auto it : a) {
    res.push_back(it);
  }
  std::vector<int> b = a | my_values<std::vector, int>();
  
  ASSERT_EQ(res, b);
}

TEST(MyValues_TEST, IteratorFuncKeysNotEqValues) {
  std::map<int, int> a = {{1, 2}, {3, 4}, {5, 6}};
  std::vector<int> res;
  for (auto it : a) {
    res.push_back(it.second);
  }
  std::vector<int> b;
  my_values(a.begin(), a.end(), std::inserter(b, b.end()));
  ASSERT_EQ(res, b);
}

TEST(MyValues_TEST, OperatorPipeKeysNotEqValues) {
  std::map<int, int> a = {{1, 2}, {3, 4}, {5, 6}};
  std::vector<int> res;
  for (auto it : a) {
    res.push_back(it.second);
  }
  std::vector<int> b = a | my_values<std::vector, int>();
  
  ASSERT_EQ(res, b);
}