#include <sstream>
#include "gtest.h"
#include "tmatrix.h"
//#include "tmatrix_for_cmp.h"

TEST(TDynamicVector, can_create_vector_with_positive_length)
{
  ASSERT_NO_THROW(TDynamicVector<int> v(5));
}

TEST(TDynamicVector, cant_create_too_large_vector)
{
  ASSERT_ANY_THROW(TDynamicVector<int> v(MAX_VECTOR_SIZE + 1));
}

TEST(TDynamicVector, throws_when_create_vector_with_negative_length)
{
  ASSERT_ANY_THROW(TDynamicVector<int> v(-5));
}

TEST(TDynamicVector, can_create_copied_vector)
{
  TDynamicVector<int> v(10);
  ASSERT_NO_THROW(TDynamicVector<int> v1(v));
}

TEST(TDynamicVector, can_move_vector_using_move_constructor)
{
    TDynamicVector<int> v1(5);
    v1[0] = 123;

    ASSERT_NO_THROW({
        TDynamicVector<int> v2(std::move(v1));
        EXPECT_EQ(0, v1.size());
        EXPECT_EQ(5, v2.size());
        EXPECT_EQ(123, v2[0]);
    });
}

TEST(TDynamicVector, can_move_vector_using_move_operator)
{
    TDynamicVector<double> v1(5);
    v1[0] = 42.24;
    TDynamicVector<double> v2(2);

    ASSERT_NO_THROW({
        v2 = std::move(v1);
        EXPECT_EQ(0, v1.size());
        EXPECT_EQ(5, v2.size());
        EXPECT_EQ(42.24, v2[0]);
    });
}

TEST(TDynamicVector, copied_vector_is_equal_to_source_one)
{
    TDynamicVector<int> v1(5);
    for (int i = 0; i < 5; i++) v1[i] = i;

    TDynamicVector<int> v2(v1);
    EXPECT_EQ(v1, v2);
}

TEST(TDynamicVector, copied_vector_has_its_own_memory)
{
    TDynamicVector<int> v1(5);
    v1[0] = 1;
    TDynamicVector<int> v2(v1);
    v2[0] = 2;

    EXPECT_NE(v1[0], v2[0]);
}

TEST(TDynamicVector, can_get_size)
{
  TDynamicVector<int> v(4);
  EXPECT_EQ(4, v.size());
}

TEST(TDynamicVector, can_set_and_get_element)
{
  TDynamicVector<int> v(4);
  v[0] = 4;
  EXPECT_EQ(4, v[0]);
}

TEST(TDynamicVector, throws_when_set_element_with_negative_index)
{
    TDynamicVector<int> v(4);
    ASSERT_ANY_THROW(v.at(-1));
}

TEST(TDynamicVector, throws_when_set_element_with_too_large_index)
{
    TDynamicVector<int> v(5);
    ASSERT_ANY_THROW(v.at(5));
}

TEST(TDynamicVector, can_assign_vector_to_itself)
{
    TDynamicVector<int> v(5);
    v[0] = 435768;
    TDynamicVector<int> tmp(v);
    v = v;
    EXPECT_EQ(tmp, v);
}

TEST(TDynamicVector, can_assign_vectors_of_equal_size)
{
    TDynamicVector<int> v1(5), v2(5);
    v1[3] = 33;
    v2 = v1;
    EXPECT_EQ(33, v2[3]);
}

TEST(TDynamicVector, assign_operator_change_vector_size)
{
    TDynamicVector<int> v1(12), v2(2);
    v2 = v1;
    EXPECT_EQ(12, v2.size());
}

TEST(TDynamicVector, can_assign_vectors_of_different_size)
{
    TDynamicVector<int> v1(12), v2(2);
    v1[6] = 6;
    v1[1] = 1;
    EXPECT_EQ(1, v2[1]);
    EXPECT_EQ(6, v2[6]);
    EXPECT_EQ(12, v2.size());
}

TEST(TDynamicVector, compare_equal_vectors_return_true)
{
    TDynamicVector<int> v1(3), v2(3);
    for (size_t i = 0; i < 3; i++) {
        v1[i] = 1;
        v2[i] = 1;
    }
    EXPECT_TRUE(v1 == v2);
}

TEST(TDynamicVector, compare_vector_with_itself_return_true)
{
    TDynamicVector<int> v(5);
    EXPECT_TRUE(v == v);
}

TEST(TDynamicVector, vectors_with_different_size_are_not_equal)
{
    TDynamicVector<int> v1(5), v2(3);
    EXPECT_TRUE(v1 != v2);
}

TEST(TDynamicVector, can_add_scalar_to_vector)
{
    TDynamicVector<int> v(3);
    v[0] = 1; v[1] = 2; v[2] = 3;
    TDynamicVector<int> res = v + 10;

    EXPECT_EQ(11, res[0]);
    EXPECT_EQ(12, res[1]);
    EXPECT_EQ(13, res[2]);
}

TEST(TDynamicVector, can_subtract_scalar_from_vector)
{
    TDynamicVector<int> v(3);
    v[0] = 1; v[1] = 2; v[2] = 3;
    TDynamicVector<int> res = v - 1;

    EXPECT_EQ(0, res[0]);
    EXPECT_EQ(1, res[1]);
    EXPECT_EQ(2, res[2]);
}

TEST(TDynamicVector, can_multiply_scalar_by_vector)
{
    TDynamicVector<int> v(3);
    v[0] = 1; v[1] = 2; v[2] = 3;
    TDynamicVector<int> res = v * 3;

    EXPECT_EQ(3, res[0]);
    EXPECT_EQ(6, res[1]);
    EXPECT_EQ(9, res[2]);
}

TEST(TDynamicVector, can_add_vectors_with_equal_size)
{
    TDynamicVector<int> v1(3), v2(3);
    v1[0] = 1; v1[1] = 2; v1[2] = 3;
    v2[0] = 4; v2[1] = 5; v2[2] = 6;

    TDynamicVector<int> res = v1 + v2;
    EXPECT_EQ(5, res[0]);
    EXPECT_EQ(7, res[1]);
    EXPECT_EQ(9, res[2]);
}

TEST(TDynamicVector, cant_add_vectors_with_not_equal_size)
{
    TDynamicVector<int> v1(5), v2(3);
    ASSERT_ANY_THROW(v1 + v2);
}

TEST(TDynamicVector, can_subtract_vectors_with_equal_size)
{
    TDynamicVector<int> v1(3), v2(3);
    v1[0] = 10; v1[1] = 20; v1[2] = 30;
    v2[0] = 1;  v2[1] = 2;  v2[2] = 3;

    TDynamicVector<int> res = v1 - v2;
    EXPECT_EQ(9, res[0]);
    EXPECT_EQ(18, res[1]);
    EXPECT_EQ(27, res[2]);
}

TEST(TDynamicVector, cant_subtract_vectors_with_not_equal_size)
{
    TDynamicVector<int> v1(5), v2(3);
    ASSERT_ANY_THROW(v1 - v2);
}

TEST(TDynamicVector, can_multiply_vectors_with_equal_size)
{
    TDynamicVector<int> v1(3), v2(3);
    v1[0] = 1; v1[1] = 2; v1[2] = 3;
    v2[0] = 4; v2[1] = 5; v2[2] = 6;

    int res = v1 * v2;
    EXPECT_EQ(32, res);
}

TEST(TDynamicVector, cant_multiply_vectors_with_not_equal_size)
{
    TDynamicVector<int> v1(5), v2(3);
    ASSERT_ANY_THROW(v1 * v2);
}

TEST(TDynamicVector, can_read_vector)
{
  TDynamicVector<int> v(5);
  TDynamicVector<int> expected(5);
  for (int i = 0; i < 5; i++) expected[i] = i;
  
  std::stringstream sstr;
  sstr << "1 2 3 4 5\n";
  sstr >> v;
  
  EXPECT_EQ(expected, v);
}

TEST(TDynamicVector, can_write_vector)
{
  TDynamicVector<int> v;
  for (int i = 0; i < 5; i++) v[i] = i;
  std::string expected = "1 2 3 4 5 ";
  
  std::stringstream sstr;
  sstr << v;
  
  EXPECT_EQ(expected, sstr.str());
}
