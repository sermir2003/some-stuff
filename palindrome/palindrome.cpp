#include <cctype>
#include "palindrome.h"

bool IsPalindrome(const std::string& str) {
    size_t right = str.length() - 1;
    for (size_t left = 0; left < right; ++left) {
        if (str[left] != ' ') {
            while (str[right] == ' ') {
                --right;
            }
            if (str[left] != str[right]) {
                return false;
            }
            --right;
        }
    }
    return true;
}
