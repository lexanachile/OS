#include <iostream>
#include <vector>
#include <iomanip>
#include "factorial.h"
#include "duplicate_remover.h"
#include "linked_list.h"

namespace {
constexpr size_t DEFAULT_FACTORIAL_COUNT = 10;
constexpr int LIST_ELEMENT_START = 1;
constexpr int LIST_ELEMENT_END = 5;
constexpr int CONSOLE_WIDTH = 50;
}

int main() {
try {
std::cout << std::string(CONSOLE_WIDTH, '=') << '\n';
std::cout << "Task 1: First N Factorials\n";
std::cout << std::string(CONSOLE_WIDTH, '=') << '\n';
size_t n = 0;
std::cout << "Enter N (1-20): ";
std::cin >> n;
if (std::cin.fail()) {
std::cin.clear();
std::cin.ignore(10000, '\n');
n = DEFAULT_FACTORIAL_COUNT;
std::cout << "Invalid input. Using default N = " << n << '\n';
}

auto factorials = FactorialCalculator::calculate(n);
std::cout << "\nResult:\n";
for (size_t i = 0; i < factorials.size(); ++i) {
std::cout << std::setw(2) << (i + 1) << "! = "
<< factorials[i] << '\n';
}

std::cout << "\n" << std::string(CONSOLE_WIDTH, '=') << '\n';
std::cout << "Task 2: Remove Duplicates\n";
std::cout << std::string(CONSOLE_WIDTH, '=') << '\n';
std::vector<int> numbers;
int count = 0;
std::cout << "Enter number of elements: ";
std::cin >> count;
if (std::cin.fail() || count <= 0) {
std::cin.clear();
std::cin.ignore(10000, '\n');
numbers = {1, 2, 3, 2, 4, 1, 5, 3, 6};
std::cout << "Invalid input. Using default array.\n";
} else {
std::cout << "Enter " << count << " numbers:\n";
for (int i = 0; i < count; ++i) {
int value = 0;
std::cin >> value;
if (std::cin.fail()) {
std::cin.clear();
std::cin.ignore(10000, '\n');
value = i;
}
numbers.push_back(value);
}
}

std::cout << "\nOriginal array: ";
for (const auto& num : numbers) {
std::cout << num << " ";
}
std::cout << '\n';

auto unique = DuplicateRemover<int>::removeDuplicates(numbers);
std::cout << "Without duplicates: ";
for (const auto& num : unique) {
std::cout << num << " ";
}
std::cout << '\n';

std::cout << "\n" << std::string(CONSOLE_WIDTH, '=') << '\n';
std::cout << "Task 3: Reverse Linked List\n";
std::cout << std::string(CONSOLE_WIDTH, '=') << '\n';
LinkedList<int> list;
int listSize = 0;
std::cout << "Enter list size: ";
std::cin >> listSize;
if (std::cin.fail() || listSize <= 0) {
std::cin.clear();
std::cin.ignore(10000, '\n');
listSize = LIST_ELEMENT_END;
std::cout << "Invalid input. Using default size = " << listSize << '\n';
}

std::cout << "Enter " << listSize << " elements:\n";
for (int i = 0; i < listSize; ++i) {
int value = 0;
std::cin >> value;
if (std::cin.fail()) {
std::cin.clear();
std::cin.ignore(10000, '\n');
value = i + LIST_ELEMENT_START;
}
list.push_back(value);
}

std::cout << "\nOriginal list: ";
auto original = list.toVector();
for (const auto& val : original) {
std::cout << val << " ";
}
std::cout << '\n';

list.reverse();
std::cout << "Reversed list: ";
auto reversed = list.toVector();
for (const auto& val : reversed) {
std::cout << val << " ";
}
std::cout << '\n';

std::cout << "\n" << std::string(CONSOLE_WIDTH, '=') << '\n';
std::cout << "All tasks completed successfully!\n";
std::cout << std::string(CONSOLE_WIDTH, '=') << '\n';
return 0;

} catch (const std::overflow_error& e) {
std::cerr << "Overflow error: " << e.what() << '\n';
return 1;
} catch (const std::runtime_error& e) {
std::cerr << "Runtime error: " << e.what() << '\n';
return 2;
} catch (const std::exception& e) {
std::cerr << "Fatal error: " << e.what() << '\n';
return 3;
} catch (...) {
std::cerr << "Unknown fatal error\n";
return 4;
}
}
