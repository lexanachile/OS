#ifndef DUPLICATE_REMOVER_H
#define DUPLICATE_REMOVER_H

#include <vector>
#include <unordered_set>

template<typename T>
class DuplicateRemover {
public:
    static std::vector<T> removeDuplicates(const std::vector<T>& input) {
        if (input.empty()) {
            return std::vector<T>();
        }

        std::vector<T> result;
        result.reserve(input.size());
        std::unordered_set<T> seen;
        seen.reserve(input.size());
        for (const auto& element : input) {
            auto insertResult = seen.insert(element);
            if (insertResult.second) {
                result.push_back(element);
            }
        }
        result.shrink_to_fit();
        return result;
    }
};

#endif
