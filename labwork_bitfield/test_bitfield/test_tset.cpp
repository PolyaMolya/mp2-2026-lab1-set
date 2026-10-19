#include "pch.h"

#include "../labwork_bitfield/tset.h"


TEST(TSet, can_get_max_power_set)
{
	const int size = 5;
	TSet set(size);

	EXPECT_EQ(size, set.GetMaxPower());
}

TEST(TSet, can_insert_non_existing_element)
{
	const int size = 5, k = 3;
	TSet set(size);
	set.InsElem(k);

	EXPECT_NE(set.IsMember(k), 0);
}

TEST(TSet, can_insert_existing_element)
{
	const int size = 5;
	const int k = 3;
	TSet set(size);
	set.InsElem(k);
	set.InsElem(k);

	EXPECT_NE(set.IsMember(k), 0);
}

TEST(TSet, can_delete_non_existing_element)
{
	const int size = 5, k = 3;
	TSet set(size);
	set.DelElem(k);

	EXPECT_EQ(set.IsMember(k), 0);
}

TEST(TSet, can_delete_existing_element)
{
	const int size = 5, k = 3;
	TSet set(size);

	set.InsElem(k);
	EXPECT_GT(set.IsMember(k), 0);

	set.DelElem(k);
	EXPECT_EQ(set.IsMember(k), 0);
}

TEST(TSet, compare_two_sets_of_non_equal_sizes)
{
	const int size1 = 4, size2 = 6;
	TSet set1(size1), set2(size2);

	EXPECT_EQ(1, set1 != set2);
}

TEST(TSet, compare_two_equal_sets)
{
	const int size = 4;
	TSet set1(size), set2(size);
	set1.InsElem(1);
	set1.InsElem(3);
	set2.InsElem(1);
	set2.InsElem(3);

	EXPECT_EQ(set1, set2);
}

TEST(TSet, compare_two_non_equal_sets)
{
	const int size = 4;
	TSet set1(size), set2(size);
	set1.InsElem(1);
	set1.InsElem(3);
	set2.InsElem(1);
	set2.InsElem(2);

	EXPECT_EQ(1, set1 != set2);
}

TEST(TSet, can_assign_set_of_equal_size)
{
	const int size = 4;
	TSet set1(size), set2(size);
	set1.InsElem(1);
	set1.InsElem(3);
	set2 = set1;

	EXPECT_EQ(set1, set2);
}

TEST(TSet, can_assign_set_of_greater_size)
{
	const int size1 = 4, size2 = 6;
	TSet set1(size1), set2(size2);
	set1.InsElem(1);
	set1.InsElem(3);
	set2 = set1;

	EXPECT_EQ(set1, set2);
}

TEST(TSet, can_assign_set_of_less_size)
{
	const int size1 = 6, size2 = 4;
	TSet set1(size1), set2(size2);
	set1.InsElem(1);
	set1.InsElem(3);
	set1.InsElem(5);
	set2 = set1;

	EXPECT_EQ(set1, set2);
}

TEST(TSet, can_insert_non_existing_element_using_plus_operator)
{
	const int size = 4;
	const int k = 3;
	TSet set(size), updatedSet(size);
	set.InsElem(0);
	set.InsElem(2);
	updatedSet = set + k;

	EXPECT_NE(0, updatedSet.IsMember(k));
}

TEST(TSet, throws_when_insert_non_existing_element_out_of_range_using_plus_operator)
{
	const int size = 4;
	const int k = 6;
	TSet set(size), updatedSet(size);
	set.InsElem(0);
	set.InsElem(2);

	ASSERT_ANY_THROW(updatedSet = set + k);
}

TEST(TSet, can_insert_existing_element_using_plus_operator)
{
	const int size = 4;
	const int k = 3;
	TSet set(size), updatedSet(size);
	set.InsElem(0);
	set.InsElem(k);
	updatedSet = set + k;

	EXPECT_NE(0, set.IsMember(k));
}

TEST(TSet, check_size_of_the_combination_of_two_sets_of_equal_size)
{
	const int size = 5;
	TSet set1(size), set2(size), set3(size);
	set1.InsElem(1);
	set1.InsElem(2);
	set1.InsElem(4);
	set2.InsElem(0);
	set2.InsElem(1);
	set2.InsElem(2);
	set3 = set1 + set2;

	EXPECT_EQ(size, set3.GetMaxPower());
}

TEST(TSet, can_combine_two_sets_of_equal_size)
{
	const int size = 5;
	TSet set1(size), set2(size), set3(size), expSet(size);
	set1.InsElem(1);
	set1.InsElem(2);
	set1.InsElem(4);
	set2.InsElem(0);
	set2.InsElem(1);
	set2.InsElem(2);
	set3 = set1 + set2;
	expSet.InsElem(0);
	expSet.InsElem(1);
	expSet.InsElem(2);
	expSet.InsElem(4);

	EXPECT_EQ(expSet, set3);
}

TEST(TSet, check_size_changes_of_the_combination_of_two_sets_of_non_equal_size)
{
	const int size1 = 5, size2 = 7;
	TSet set1(size1), set2(size2), set3(size1);
	set1.InsElem(1);
	set1.InsElem(2);
	set1.InsElem(4);
	set2.InsElem(0);
	set2.InsElem(1);
	set2.InsElem(2);
	set3 = set1 + set2;

	EXPECT_EQ(size2, set3.GetMaxPower());
}

TEST(TSet, can_combine_two_sets_of_non_equal_size)
{
	const int size1 = 5, size2 = 7;
	TSet set1(size1), set2(size2), set3(size1), expSet(size2);
	set1.InsElem(1);
	set1.InsElem(2);
	set1.InsElem(4);
	set2.InsElem(0);
	set2.InsElem(1);
	set2.InsElem(2);
	set2.InsElem(6);
	set3 = set1 + set2;
	expSet.InsElem(0);
	expSet.InsElem(1);
	expSet.InsElem(2);
	expSet.InsElem(4);
	expSet.InsElem(6);

	EXPECT_EQ(expSet, set3);
}

TEST(TSet, can_intersect_two_sets_of_equal_size)
{
	const int size = 5;
	TSet set1(size), set2(size), set3(size), expSet(size);
	set1.InsElem(1);
	set1.InsElem(2);
	set1.InsElem(4);
	set2.InsElem(0);
	set2.InsElem(1);
	set2.InsElem(2);
	set3 = set1 * set2;
	expSet.InsElem(1);
	expSet.InsElem(2);

	EXPECT_EQ(expSet, set3);
}

TEST(TSet, can_intersect_two_sets_of_non_equal_size)
{
	const int size1 = 5, size2 = 7;
	TSet set1(size1), set2(size2), set3(size1), expSet(size2);
	set1.InsElem(1);
	set1.InsElem(2);
	set1.InsElem(4);
	set2.InsElem(0);
	set2.InsElem(1);
	set2.InsElem(2);
	set2.InsElem(4);
	set2.InsElem(6);
	set3 = set1 * set2;
	expSet.InsElem(1);
	expSet.InsElem(2);
	expSet.InsElem(4);

	EXPECT_EQ(expSet, set3);
}

TEST(TSet, check_negation_operator)
{
	const int size = 4;
	TSet set(size), set1(size), expSet(size);
	set.InsElem(1);
	set.InsElem(3);
	set1 = ~set;
	expSet.InsElem(0);
	expSet.InsElem(2);

	EXPECT_EQ(expSet, set1);
}
// Собственные тесты
TEST(TSet, can_combine_three_sets_in_one_expression)
{
	const int size = 8;
	TSet set1(size), set2(size), set3(size), expSet(size);
	set1.InsElem(0);
	set2.InsElem(3);
	set3.InsElem(7);

	expSet.InsElem(0);
	expSet.InsElem(3);
	expSet.InsElem(7);

	EXPECT_EQ(expSet, set1 + set2 + set3);
}

TEST(TSet, can_intersect_three_sets_in_one_expression)
{
	const int size = 8;
	TSet set1(size), set2(size), set3(size), expSet(size);
	set1.InsElem(1);
	set1.InsElem(2);
	set1.InsElem(5);
	set2.InsElem(2);
	set2.InsElem(5);
	set2.InsElem(6);
	set3.InsElem(5);
	set3.InsElem(0);

	expSet.InsElem(5);

	EXPECT_EQ(expSet, set1 * set2 * set3);
}

TEST(TSet, can_delete_element_using_minus_operator)
{
	const int size = 5;
	TSet set(size), updatedSet(size);
	set.InsElem(1);
	set.InsElem(3);
	updatedSet = set - 3;

	EXPECT_EQ(0, updatedSet.IsMember(3));
	EXPECT_NE(0, updatedSet.IsMember(1));
}

TEST(TSet, throws_when_delete_element_out_of_range_using_minus_operator)
{
	const int size = 4;
	TSet set(size);

	ASSERT_ANY_THROW(set - 6);
}

TEST(TSet, can_chain_plus_element_operators)
{
	const int size = 6;
	TSet set(size), expSet(size);
	expSet.InsElem(1);
	expSet.InsElem(2);
	expSet.InsElem(4);

	EXPECT_EQ(expSet, set + 1 + 2 + 4);
}

TEST(TSet, double_negation_returns_original_set)
{
	const int size = 10;
	TSet set(size);
	set.InsElem(0);
	set.InsElem(4);
	set.InsElem(9);

	EXPECT_EQ(set, ~~set);
}

TEST(TSet, set_and_its_complement_union_is_full_universe)
{
	const int size = 10;
	TSet set(size), full(size);
	set.InsElem(2);
	set.InsElem(7);
	for (int i = 0; i < size; i++)
		full.InsElem(i);

	EXPECT_EQ(full, set + ~set);
}

TEST(TSet, can_convert_set_to_bitfield_and_back)
{
	const int size = 12;
	TSet set(size);
	set.InsElem(3);
	set.InsElem(11);

	TBitField bf = set;
	TSet restored(bf);

	EXPECT_EQ(set, restored);
}