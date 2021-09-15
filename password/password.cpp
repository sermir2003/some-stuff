#include <cctype>
#include "password.h"

bool ValidatePassword(const std::string& password) {
    // first restriction
    for (char elem : password)
        if (elem < 33 || 126 < elem)
            return false;
    // second restriction
    if (password.length() < 8 || password.length() > 14)
        return false;
    // third restriction
    bool upperExist = false;
    bool lowerExist = false;
    bool digitExist = false;
    bool otherExist = false;
    for (char elem : password)
        if (isupper(elem))
            upperExist = true;
        else if (islower(elem))
            lowerExist = true;
        else if (isdigit(elem))
            digitExist = true;
        else
            otherExist = true;
    return int(upperExist) + int(lowerExist) +
    int(digitExist) + int(otherExist) >= 3;
}
