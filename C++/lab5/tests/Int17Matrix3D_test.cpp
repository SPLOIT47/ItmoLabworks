#include <gtest/gtest.h>
#include <lib/Int17Matrix3D.h>
#include <sstream>

TEST(Int17Matrix3DTest, Constructor) {
  Int17Matrix3D matrix(2, 2, 2);
  ASSERT_NO_THROW();
}

TEST(Int17Matrix3DTest, make_array) {
  Int17Matrix3D matrix = Int17Matrix3D::make_array(2, 2, 2);
  ASSERT_NO_THROW();
}

TEST(Int17Matrix3DTest, CopyConstructor) {
  Int17Matrix3D original(1, 2, 3);
  ASSERT_NO_THROW(Int17Matrix3D copy = original);
}

TEST(Int17Matrix3DTest, ParenthesesOperator_EqTest) {
  Int17Matrix3D matrix(3, 3, 3);
  matrix[1][1][1] = 5;
  ASSERT_EQ(matrix[1][1][1], 5);
}

TEST(Int17Matrix3DTest, ParenthesesOperator_NonEqTest) {
  Int17Matrix3D matrix(3, 3, 3);
  matrix[1][1][1] = 5;
  ASSERT_NE(matrix[1][1][1], 7);
}

TEST(Int17Matrix3DTest, ParenthesesOperator_BoundsCheck) {
  Int17Matrix3D matrix(2, 2, 2);
  ASSERT_THROW(matrix[3][1][1] = 5, std::out_of_range);
  ASSERT_THROW(matrix[1][3][1] = 5, std::out_of_range);
  ASSERT_THROW(matrix[1][1][3] = 5, std::out_of_range);
}

TEST(Int17Matrix3DTest, EqualOperator_TwoEq) {
  Int17Matrix3D arr1(1, 1, 1);
  Int17Matrix3D arr2(2, 2, 2);
  arr1[0][0][0] = 10;
  arr2[0][0][0] = 10;
  arr2[1][1][1] = 10;
  ASSERT_EQ(arr1[0][0][0], arr2[0][0][0]);
  ASSERT_EQ(arr1[0][0][0], arr2[1][1][1]);
}

TEST(Int17Matrix3DTest, EqualOperator_NonEq) {
  Int17Matrix3D arr1(1, 1, 1);
  Int17Matrix3D arr2(1, 1, 1);
  arr1[0][0][0] = 1;
  arr2[0][0][0] = 3;
  ASSERT_NE(arr1[0][0][0], arr2[0][0][0]);
}

TEST(Int17Matrix3DTest, AssignmentOperator_NoThrow) {
  Int17Matrix3D arr1(1, 2, 3);
  Int17Matrix3D arr2(2, 2, 2);
  ASSERT_NO_THROW(arr2 = arr1);
}

TEST(Int17Matrix3DTest, AssignmentOperator_Trow) {
  Int17Matrix3D arr1(1, 2, 3);
  Int17Matrix3D arr2(2, 2, 2);
  arr2 = arr1;
  ASSERT_THROW(arr2[1][1][1], std::out_of_range);
}

TEST(Int17Matrix3DTest, AssignmentOperator) {
  Int17Matrix3D arr1(1, 2, 3);
  Int17Matrix3D arr2(2, 2, 2);
  arr1[0][1][2] = 5;
  arr2[0][0][0] = 3;
  arr2 = arr1;
  ASSERT_EQ(arr2[0][1][2], 5);
  ASSERT_NE(arr2[0][0][0], 3);
}

TEST(Int17Matrix3DTest, Addition_EmptyArrays) {
  Int17Matrix3D arr1(1, 1, 1);
  Int17Matrix3D arr2(1, 1, 1);
  Int17Matrix3D arr3 = arr1 + arr2;
  ASSERT_EQ(arr3[0][0][0], 0);
}

TEST(Int17Matrix3DTest, Addition_SingleCells) {
  Int17Matrix3D arr1(1, 1, 1);
  Int17Matrix3D arr2(1, 1, 1);
  arr1[0][0][0] = 1;
  arr2[0][0][0] = 2;
  Int17Matrix3D arr3 = arr1 + arr2;
  ASSERT_EQ(arr3[0][0][0], 3);
}

TEST(Int17Matrix3DTest, Addition_FewCells) {
  Int17Matrix3D arr1(1, 1, 2);
  Int17Matrix3D arr2(1, 1, 2);
  arr1[0][0][1] = 1;
  arr1[0][0][0] = 5;
  arr2[0][0][0] = 2;
  arr2[0][0][1] = 0;
  Int17Matrix3D arr3 = arr1 + arr2;
  ASSERT_EQ(arr3[0][0][0], 7);
  ASSERT_EQ(arr3[0][0][1], 1);
}

TEST(Int17Matrix3DTest, Addition_DifferentSize) {
  Int17Matrix3D arr1(1, 1, 1);
  Int17Matrix3D arr2(1, 1, 2);
  arr1[0][0][0] = 1;
  arr2[0][0][0] = 2;
  ASSERT_THROW(Int17Matrix3D arr3 = arr1 + arr2, std::out_of_range);
}

TEST(Int17Matrix3DTest, Substraction_EmptyArrays) {
  Int17Matrix3D arr1(1, 1, 1);
  Int17Matrix3D arr2(1, 1, 1);
  Int17Matrix3D arr3 = arr1 - arr2;
  ASSERT_EQ(arr3[0][0][0], 0);
}

TEST(Int17Matrix3DTest, Substraction_SingleCells) {
  Int17Matrix3D arr1(1, 1, 1);
  Int17Matrix3D arr2(1, 1, 1);
  arr1[0][0][0] = 7;
  arr2[0][0][0] = 2;
  Int17Matrix3D arr3 = arr1 - arr2;
  ASSERT_EQ(arr3[0][0][0], 5);
}

TEST(Int17Matrix3DTest, Substraction_FewCells) {
  Int17Matrix3D arr1(1, 1, 2);
  Int17Matrix3D arr2(1, 1, 2);
  arr1[0][0][1] = 3;
  arr1[0][0][0] = 5;
  arr2[0][0][0] = 2;
  Int17Matrix3D arr3 = arr1 - arr2;
  ASSERT_EQ(arr3[0][0][0], 3);
  ASSERT_EQ(arr3[0][0][1], 3);
}

TEST(Int17Matrix3DTest, Multiplication_ByZero) {
  Int17Matrix3D arr1(2, 2, 2);
  arr1[0][0][0] = 1;
  arr1[0][0][1] = 2;
  arr1[0][1][0] = 3;
  arr1[0][1][1] = 4;
  arr1[1][0][0] = 5;
  arr1[1][0][1] = 6;
  arr1[1][1][0] = 7;
  arr1[1][1][1] = 8;
  int mult = 0;
  Int17Matrix3D arr3 = arr1 * mult;
  ASSERT_EQ(arr3[0][0][0], 0);
  ASSERT_EQ(arr3[0][0][1], 0);
  ASSERT_EQ(arr3[0][1][0], 0);
  ASSERT_EQ(arr3[0][1][1], 0);
  ASSERT_EQ(arr3[1][0][0], 0);
  ASSERT_EQ(arr3[1][0][1], 0);
  ASSERT_EQ(arr3[1][1][0], 0);
  ASSERT_EQ(arr3[1][1][1], 0);
}

TEST(Int17Matrix3DTest, Multiplication) {
  Int17Matrix3D arr1(2, 2, 2);
  Int17Matrix3D arr2 (2, 2, 2);
  arr1[0][0][0] = 1;
  arr1[0][0][1] = 2;
  arr1[0][1][0] = 3;
  arr1[0][1][1] = 4;
  arr1[1][0][0] = 5;
  arr1[1][0][1] = 6;
  arr1[1][1][0] = 7;
  arr1[1][1][1] = 8;
  int mult = 10;
  arr2 = arr1 * mult;
  ASSERT_EQ(arr2[0][0][0], 10);
  ASSERT_EQ(arr2[0][0][1], 20);
  ASSERT_EQ(arr2[0][1][0], 30);
  ASSERT_EQ(arr2[0][1][1], 40);
  ASSERT_EQ(arr2[1][0][0], 50);
  ASSERT_EQ(arr2[1][0][1], 60);
  ASSERT_EQ(arr2[1][1][0], 70);
  ASSERT_EQ(arr2[1][1][1], 80);
}

TEST(Int17Matrix3DTest, OutputStream) {
  Int17Matrix3D arr(1, 1, 1);
  arr[0][0][0] = 5;
  std::ostringstream stream;
  stream << arr[0][0][0];
  std::string str = "5";
  ASSERT_EQ(stream.str(), str);
}

TEST(Int17Matrix3DTest, InputStream) {
  Int17Matrix3D arr(1, 1, 1);
  Int17Matrix3D arr2(1, 1, 1);
  arr[0][0][0] = 5;
  std::ostringstream ostream;
  ostream << arr[0][0][0];
  std::istringstream istream(ostream.str());
  istream >> arr2;
  ASSERT_TRUE(arr == arr2);
}