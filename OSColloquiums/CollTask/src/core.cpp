#include "core.h"
#include <stdexcept>

namespace validation {

    void validateFactorialInput(size_t n) {
        if (n > config::MAX_SAFE_FACTORIAL) {
            throw std::overflow_error("Factorial calculation would overflow uint64_t");
        }
    }

    void validateRecursionDepth(size_t depth) {
        if (depth > config::RECURSION_DEPTH_LIMIT) {
            throw std::runtime_error("Recursion depth limit exceeded");
        }
    }

}
