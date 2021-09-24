#include <deque>
#include "tests_checking.h"


std::vector<std::string> StudentsOrder(const std::vector<StudentAction>& student_actions,
                                       const std::vector<size_t>& queries) {
    std::deque<const std::string*> pile;
    for (const StudentAction& action : student_actions) {
        if (action.side == Side::Top) {
            pile.push_front(&action.name);
        } else {
            pile.push_back(&action.name);
        }
    }

    std::vector<std::string> result(queries.size());
    for (size_t i = 0; i < queries.size(); ++i) {
        result[i] = *pile[queries[i] - 1];
    }
    return result;
}
