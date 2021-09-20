#include <cctype>
#include "password.h"

const int minRangeAllowedSymbols = 33;
const int maxRangeAllowedSymbols = 126;
const int minPasswordLength = 8;
const int maxPasswordLength = 8;

bool ValidatePassword(const std::string& password) {
    // first restriction
    for (char elem : password) {
        if (elem < minRangeAllowedSymbols || maxRangeAllowedSymbols < elem) {
            return false;
        }
    }
    // second restriction
    if (password.length() < minPasswordLength || password.length() > maxPasswordLength) {
        return false;
    }
    // third restriction
    bool upper_exist = false;
    bool lower_exist = false;
    bool digit_exist = false;
    bool other_exist = false;
    for (char elem : password) {
        if (isupper(elem)) {
            upper_exist = true;
        } else if (islower(elem)) {
            lower_exist = true;
        } else if (isdigit(elem)) {
            digit_exist = true;
        } else {
            other_exist = true;
        }
    }
    return static_cast<int>(upper_exist) + static_cast<int>(lower_exist) +
               static_cast<int>(digit_exist) + static_cast<int>(other_exist) >= 3;
}
