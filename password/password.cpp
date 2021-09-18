#include <cctype>
#include "password.h"

bool ValidatePassword(const std::string& password) {
    // first restriction
    for (char elem : password) {
        if (elem < 33 || 126 < elem) {
            return false;
        }
    }
    // second restriction
    if (password.length() < 8 || password.length() > 14) {
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
