#include <cctype>
#include "password.h"

const int MIN_RANGE_ALLOWED_SYMBOLS = 33;
const int MAX_RANGE_ALLOWED_SYMBOLS = 126;
const int MIN_PASSWORD_LENGTH = 8;
const int MAX_PASSWORD_LENGTH = 14;

bool ValidatePassword(const std::string& password) {
    // first restriction
    for (char elem : password) {
        if (elem < MIN_RANGE_ALLOWED_SYMBOLS || MAX_RANGE_ALLOWED_SYMBOLS < elem) {
            return false;
        }
    }
    // second restriction
    if (password.length() < MIN_PASSWORD_LENGTH || password.length() > MAX_PASSWORD_LENGTH) {
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
