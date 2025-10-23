#include <gtest/gtest.h>
#include "../src/factorial.h"
#include "../src/duplicate_remover.h"
#include "../src/linked_list.h"

TEST(IntegrationTest, CombinedWorkflow) {
    auto factorials = FactorialCalculator::calculate(5);
    ASSERT_EQ(factorials.size(), 5u);

    std::vector<int> duplicates = {1, 2, 2, 3, 3, 3};
    auto unique = DuplicateRemover<int>::removeDuplicates(duplicates);
    ASSERT_EQ(unique.size(), 3u);

    LinkedList<int> list;
    list.push_back(10);
    list.push_back(20);
    list.reverse();
    auto reversed = list.toVector();
    ASSERT_EQ(reversed[0], 20);
    ASSERT_EQ(reversed[1], 10);
}
