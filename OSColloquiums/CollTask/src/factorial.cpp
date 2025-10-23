#include "factorial.h"
#include "core.h"
#include <stdexcept>
#include <limits>

std::vector<uint64_t> FactorialCalculator::calculate(size_t n) {
    if (n == 0) {
        return std::vector<uint64_t>();
    }

    validation::validateFactorialInput(n);
    std::vector<uint64_t> result;
    result.reserve(n);
    uint64_t factorial = FIRST_FACTORIAL;
    for (size_t i = 1; i <= n; ++i) {
        if (i > 1 && factorial > std::numeric_limits<uint64_t>::max() / i) {
            throw std::overflow_error("Factorial overflow");
        }
        factorial *= i;
        result.push_back(factorial);
    }
    return result;
}
