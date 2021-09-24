#include "word2vec.h"

#include <vector>

int64_t CalcScalarProduct(const std::vector<int>& vec1, const std::vector<int>& vec2) {
    int64_t result = 0;
    for (size_t i = 0; i < vec1.size(); ++i) {
        result += static_cast<int64_t>(vec1[i]) * static_cast<int64_t>(vec2[i]);
    }
    return result;
}

std::vector<std::string> FindClosestWords(const std::vector<std::string>& words,
                                          const std::vector<std::vector<int>>& vectors) {
    std::vector<int64_t> scalar_product_with_first(words.size());
    for (size_t i = 1; i < words.size(); ++i) {
        scalar_product_with_first[i] = CalcScalarProduct(vectors[0], vectors[i]);
    }
    int64_t best_scalar_product = INT64_MIN;
    for (size_t i = 1; i < words.size(); ++i) {
        best_scalar_product = std::max(best_scalar_product, scalar_product_with_first[i]);
    }
    std::vector<std::string> result;
    for (size_t i = 1; i < words.size(); ++i) {
        if (scalar_product_with_first[i] == best_scalar_product) {
            result.push_back(words[i]);
        }
    }
    return result;
}
