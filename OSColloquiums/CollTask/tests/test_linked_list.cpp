#include <gtest/gtest.h>
#include "../src/linked_list.h"
#include <vector>

namespace {
constexpr size_t ZERO_ITEMS = 0;
constexpr size_t ONE_ITEM = 1;
constexpr size_t TWO_ITEMS = 2;
constexpr size_t FIVE_ITEMS = 5;
constexpr size_t LARGE_SIZE = 1000;
}

class LinkedListTest : public ::testing::Test {};

TEST_F(LinkedListTest, EmptyList) {
LinkedList<int> list;
EXPECT_TRUE(list.empty());
EXPECT_EQ(list.size(), ZERO_ITEMS);
}

TEST_F(LinkedListTest, EmptyListReverse) {
LinkedList<int> list;
list.reverse();
EXPECT_TRUE(list.empty());
}

TEST_F(LinkedListTest, SingleElement) {
LinkedList<int> list;
list.push_back(42);
EXPECT_FALSE(list.empty());
EXPECT_EQ(list.size(), ONE_ITEM);
list.reverse();
auto result = list.toVector();
ASSERT_EQ(result.size(), ONE_ITEM);
EXPECT_EQ(result[0], 42);
}

TEST_F(LinkedListTest, TwoElements) {
LinkedList<int> list;
list.push_back(1);
list.push_back(2);
EXPECT_EQ(list.size(), TWO_ITEMS);
list.reverse();
auto result = list.toVector();
ASSERT_EQ(result.size(), TWO_ITEMS);
EXPECT_EQ(result[0], 2);
EXPECT_EQ(result[1], 1);
}

TEST_F(LinkedListTest, MultipleElements) {
LinkedList<int> list;
for (size_t i = 1; i <= FIVE_ITEMS; ++i) {
list.push_back(static_cast<int>(i));
}
EXPECT_EQ(list.size(), FIVE_ITEMS);
list.reverse();
auto result = list.toVector();
ASSERT_EQ(result.size(), FIVE_ITEMS);
for (size_t i = 0; i < result.size(); ++i) {
EXPECT_EQ(result[i], static_cast<int>(FIVE_ITEMS - i));
}
}

TEST_F(LinkedListTest, DoubleReverse) {
LinkedList<int> list;
for (size_t i = 1; i <= 3; ++i) {
list.push_back(static_cast<int>(i));
}
list.reverse();
list.reverse();
auto result = list.toVector();
ASSERT_EQ(result.size(), 3u);
EXPECT_EQ(result[0], 1);
EXPECT_EQ(result[1], 2);
EXPECT_EQ(result[2], 3);
}

TEST_F(LinkedListTest, LargeList) {
LinkedList<int> list;
for (size_t i = 0; i < LARGE_SIZE; ++i) {
list.push_back(static_cast<int>(i));
}
EXPECT_EQ(list.size(), LARGE_SIZE);
list.reverse();
auto result = list.toVector();
ASSERT_EQ(result.size(), LARGE_SIZE);
for (size_t i = 0; i < LARGE_SIZE; ++i) {
EXPECT_EQ(result[i], static_cast<int>(LARGE_SIZE - 1 - i));
}
}

TEST_F(LinkedListTest, StringType) {
LinkedList<std::string> list;
list.push_back("first");
list.push_back("second");
list.push_back("third");
list.reverse();
auto result = list.toVector();
ASSERT_EQ(result.size(), 3u);
EXPECT_EQ(result[0], "third");
EXPECT_EQ(result[1], "second");
EXPECT_EQ(result[2], "first");
}

TEST_F(LinkedListTest, MoveConstructor) {
LinkedList<int> list1;
list1.push_back(1);
list1.push_back(2);
LinkedList<int> list2(std::move(list1));
EXPECT_TRUE(list1.empty());
EXPECT_EQ(list1.size(), ZERO_ITEMS);
auto result = list2.toVector();
ASSERT_EQ(result.size(), TWO_ITEMS);
EXPECT_EQ(result[0], 1);
EXPECT_EQ(result[1], 2);
}

TEST_F(LinkedListTest, MoveAssignment) {
LinkedList<int> list1;
list1.push_back(1);
list1.push_back(2);
LinkedList<int> list2;
list2.push_back(99);
list2 = std::move(list1);
EXPECT_TRUE(list1.empty());
auto result = list2.toVector();
ASSERT_EQ(result.size(), TWO_ITEMS);
EXPECT_EQ(result[0], 1);
EXPECT_EQ(result[1], 2);
}

struct LinkedListTestData {
std::vector<int> input;
std::vector<int> expectedAfterReverse;
};

class LinkedListParameterizedTest : public ::testing::TestWithParam<LinkedListTestData> {};

TEST_P(LinkedListParameterizedTest, ParameterizedReverse) {
const auto& params = GetParam();
LinkedList<int> list;
for (const auto& value : params.input) {
list.push_back(value);
}
list.reverse();
auto result = list.toVector();
EXPECT_EQ(result, params.expectedAfterReverse);
}

INSTANTIATE_TEST_SUITE_P(
LinkedListTests,
LinkedListParameterizedTest,
::testing::Values(
LinkedListTestData{{1}, {1}},
LinkedListTestData{{1, 2}, {2, 1}},
LinkedListTestData{{1, 2, 3}, {3, 2, 1}},
LinkedListTestData{{1, 2, 3, 4, 5}, {5, 4, 3, 2, 1}}
)
);
