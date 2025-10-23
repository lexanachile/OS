#ifndef CORE_H
#define CORE_H

#include <cstddef>

namespace config {
    constexpr size_t MAX_SAFE_FACTORIAL = 20;
    constexpr size_t INITIAL_CAPACITY = 16;
    constexpr size_t RECURSION_DEPTH_LIMIT = 10000;
}

namespace validation {
    void validateFactorialInput(size_t n);
    void validateRecursionDepth(size_t depth);
}

#endif
