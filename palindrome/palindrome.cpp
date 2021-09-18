#include <cctype>
#include "palindrome.h"

bool IsPalindrome(const std::string& str) {
    size_t right = str.length() - 1;
    for (size_t left = 0; left < str.size(); ++left) {
        if (!isspace(str[left])) {
            while (isspace(str[right])) {
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
