#ifndef FACTORIAL_H
#define FACTORIAL_H

#include <vector>
#include <cstdint>
#include <cstddef>

class FactorialCalculator {
public:
    static std::vector<uint64_t> calculate(size_t n);
private:
    static constexpr uint64_t FIRST_FACTORIAL = 1;
};

#endif
