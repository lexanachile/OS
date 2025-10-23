#include <gtest/gtest.h>
#include "../src/factorial.h"
#include <vector>

namespace {
constexpr size_t ZERO_ITEMS = 0;
constexpr size_t ONE_ITEM = 1;
constexpr size_t FIVE_ITEMS = 5;
constexpr size_t TEN_ITEMS = 10;
constexpr size_t TWENTY_ITEMS = 20;
constexpr uint64_t FACTORIAL_1 = 1;
constexpr uint64_t FACTORIAL_2 = 2;
constexpr uint64_t FACTORIAL_3 = 6;
constexpr uint64_t FACTORIAL_4 = 24;
constexpr uint64_t FACTORIAL_5 = 120;
constexpr uint64_t FACTORIAL_10 = 3628800;
constexpr uint64_t FACTORIAL_20 = 2432902008176640000ULL;
}

class FactorialCalculatorTest : public ::testing::Test {
protected:
void SetUp() override {}
void TearDown() override {}
};

TEST_F(FactorialCalculatorTest, EmptyInput) {
auto result = FactorialCalculator::calculate(ZERO_ITEMS);
EXPECT_TRUE(result.empty());
}

TEST_F(FactorialCalculatorTest, SingleFactorial) {
auto result = FactorialCalculator::calculate(ONE_ITEM);
ASSERT_EQ(result.size(), ONE_ITEM);
EXPECT_EQ(result[0], FACTORIAL_1);
}

TEST_F(FactorialCalculatorTest, FirstFiveFactorials) {
auto result = FactorialCalculator::calculate(FIVE_ITEMS);
ASSERT_EQ(result.size(), FIVE_ITEMS);
EXPECT_EQ(result[0], FACTORIAL_1);
EXPECT_EQ(result[1], FACTORIAL_2);
EXPECT_EQ(result[2], FACTORIAL_3);
EXPECT_EQ(result[3], FACTORIAL_4);
EXPECT_EQ(result[4], FACTORIAL_5);
}

TEST_F(FactorialCalculatorTest, FirstTenFactorials) {
auto result = FactorialCalculator::calculate(TEN_ITEMS);
ASSERT_EQ(result.size(), TEN_ITEMS);
EXPECT_EQ(result[0], FACTORIAL_1);
EXPECT_EQ(result[9], FACTORIAL_10);
}

TEST_F(FactorialCalculatorTest, MaximumSafeFactorial) {
auto result = FactorialCalculator::calculate(TWENTY_ITEMS);
ASSERT_EQ(result.size(), TWENTY_ITEMS);
EXPECT_EQ(result[19], FACTORIAL_20);
}

TEST_F(FactorialCalculatorTest, OverflowDetection) {
EXPECT_THROW({
FactorialCalculator::calculate(25);
}, std::overflow_error);
}

TEST_F(FactorialCalculatorTest, BoundaryOverflow) {
EXPECT_THROW({
FactorialCalculator::calculate(21);
}, std::overflow_error);
}

struct FactorialTestData {
size_t input;
size_t expectedSize;
uint64_t firstValue;
uint64_t lastValue;
};

class FactorialParameterizedTest : public ::testing::TestWithParam<FactorialTestData> {};

TEST_P(FactorialParameterizedTest, ParameterizedCalculation) {
const auto& params = GetParam();
auto result = FactorialCalculator::calculate(params.input);
ASSERT_EQ(result.size(), params.expectedSize);
if (!result.empty()) {
EXPECT_EQ(result[0], params.firstValue);
EXPECT_EQ(result[result.size() - 1], params.lastValue);
}
}

INSTANTIATE_TEST_SUITE_P(
FactorialTests,
FactorialParameterizedTest,
::testing::Values(
FactorialTestData{1, 1, 1, 1},
FactorialTestData{2, 2, 1, 2},
FactorialTestData{3, 3, 1, 6},
FactorialTestData{5, 5, 1, 120},
FactorialTestData{10, 10, 1, 3628800}
)
);
