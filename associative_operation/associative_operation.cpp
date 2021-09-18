#include "associative_operation.h"

bool IsAssociative(const std::vector<std::vector<size_t>>& table) {
    size_t set_size = table.size();
    for (size_t elem1 = 0; elem1 < set_size; ++elem1) {
        for (size_t elem2 = 0; elem2 < set_size; ++elem2) {
            for (size_t elem3 = 0; elem3 < set_size; ++elem3) {
                if (table[table[elem1][elem2]][elem3] != table[elem1][table[elem2][elem3]]) {
                    return false;
                }
            }
        }
    }
    return true;
}
