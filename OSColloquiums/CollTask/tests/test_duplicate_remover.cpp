#include <gtest/gtest.h>
#include "../src/duplicate_remover.h"
#include <vector>

namespace {
constexpr size_t ONE_ITEM = 1;
constexpr size_t FIVE_ITEMS = 5;
constexpr size_t VERY_LARGE_SIZE = 10000;
}

class DuplicateRemoverTest : public ::testing::Test {};

TEST_F(DuplicateRemoverTest, EmptyContainer) {
std::vector<int> input;
auto result = DuplicateRemover<int>::removeDuplicates(input);
EXPECT_TRUE(result.empty());
}

TEST_F(DuplicateRemoverTest, NoDuplicates) {
std::vector<int> input = {1, 2, 3, 4, 5};
auto result = DuplicateRemover<int>::removeDuplicates(input);
EXPECT_EQ(result, input);
}

TEST_F(DuplicateRemoverTest, AllDuplicates) {
std::vector<int> input = {5, 5, 5, 5};
auto result = DuplicateRemover<int>::removeDuplicates(input);
ASSERT_EQ(result.size(), ONE_ITEM);
EXPECT_EQ(result[0], 5);
}

TEST_F(DuplicateRemoverTest, MixedDuplicates) {
std::vector<int> input = {1, 2, 3, 2, 4, 1, 5, 3};
auto result = DuplicateRemover<int>::removeDuplicates(input);
ASSERT_EQ(result.size(), FIVE_ITEMS);
EXPECT_EQ(result[0], 1);
EXPECT_EQ(result[1], 2);
EXPECT_EQ(result[2], 3);
EXPECT_EQ(result[3], 4);
EXPECT_EQ(result[4], 5);
}

TEST_F(DuplicateRemoverTest, OrderPreservation) {
std::vector<int> input = {5, 1, 3, 1, 2, 3};
auto result = DuplicateRemover<int>::removeDuplicates(input);
ASSERT_EQ(result.size(), 4u);
EXPECT_EQ(result[0], 5);
EXPECT_EQ(result[1], 1);
EXPECT_EQ(result[2], 3);
EXPECT_EQ(result[3], 2);
}

TEST_F(DuplicateRemoverTest, SingleElement) {
std::vector<int> input = {42};
auto result = DuplicateRemover<int>::removeDuplicates(input);
ASSERT_EQ(result.size(), ONE_ITEM);
EXPECT_EQ(result[0], 42);
}

TEST_F(DuplicateRemoverTest, StringType) {
std::vector<std::string> input = {"apple", "banana", "apple", "cherry"};
auto result = DuplicateRemover<std::string>::removeDuplicates(input);
ASSERT_EQ(result.size(), 3u);
EXPECT_EQ(result[0], "apple");
EXPECT_EQ(result[1], "banana");
EXPECT_EQ(result[2], "cherry");
}

TEST_F(DuplicateRemoverTest, NegativeNumbers) {
std::vector<int> input = {-1, -2, -1, 0, -2, 1};
auto result = DuplicateRemover<int>::removeDuplicates(input);
ASSERT_EQ(result.size(), 4u);
EXPECT_EQ(result[0], -1);
EXPECT_EQ(result[1], -2);
EXPECT_EQ(result[2], 0);
EXPECT_EQ(result[3], 1);
}

TEST_F(DuplicateRemoverTest, LargeDataSet) {
std::vector<int> input(VERY_LARGE_SIZE);
for (size_t i = 0; i < input.size(); ++i) {
input[i] = static_cast<int>(i % 100);
}
auto result = DuplicateRemover<int>::removeDuplicates(input);
EXPECT_EQ(result.size(), 100u);
}

struct DuplicateTestData {
std::vector<int> input;
std::vector<int> expected;
};

class DuplicateParameterizedTest : public ::testing::TestWithParam<DuplicateTestData> {};

TEST_P(DuplicateParameterizedTest, ParameterizedRemoval) {
const auto& params = GetParam();
auto result = DuplicateRemover<int>::removeDuplicates(params.input);
EXPECT_EQ(result, params.expected);
}

INSTANTIATE_TEST_SUITE_P(
DuplicateTests,
DuplicateParameterizedTest,
::testing::Values(
DuplicateTestData{{}, {}},
DuplicateTestData{{1}, {1}},
DuplicateTestData{{1, 1}, {1}},
DuplicateTestData{{1, 2, 1}, {1, 2}},
DuplicateTestData{{1, 2, 3, 2, 1}, {1, 2, 3}}
)
);
