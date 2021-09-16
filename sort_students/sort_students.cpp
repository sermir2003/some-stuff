#include <algorithm>
#include "sort_students.h"

int comparingDates(const Date& dateA, const Date& dateB) {
    // Returns -1 if dateA < dateB,
    // 0 if dateA == dateB,
    // 1 if dateA > dateB.
    if (dateA.year < dateB.year)
        return -1;
    else if (dateA.year > dateB.year)
        return 1;
    if (dateA.month < dateB.month)
        return -1;
    else if (dateA.month > dateB.month)
        return 1;
    if (dateA.day < dateB.day)
        return -1;
    else if (dateA.day > dateB.day)
        return 1;
    return 0;
}

int comparingNames(const Student& stA, const Student& stB) {
    // Returns -1 if lexicographically name of student A is earlier than name of student B,
    // 0 if names are the same,
    // 1 if if lexicographically name of student A is later than name of student B.
    if (stA.last_name < stB.last_name)
        return -1;
    else if (stA.last_name > stB.last_name)
        return 1;
    if (stA.name < stB.name)
        return -1;
    else if (stA.name > stB.name)
        return 1;
    return 0;
}

bool comparatorByDate(const Student& stA, const Student& stB) {
    int compDateRes = comparingDates(stA.birth_date, stB.birth_date);
    if (compDateRes == -1)
        return true;
    else if (compDateRes == 1)
        return false;
    else {
        int compNameRes = comparingNames(stA, stB);
        return compNameRes != 1;
    }
}

bool comparatorByName(const Student& stA, const Student& stB) {
    int compNameRes = comparingNames(stA, stB);
    if (compNameRes == -1)
        return true;
    else if (compNameRes == 1)
        return false;
    else {
        int compDateRes = comparingDates(stA.birth_date, stB.birth_date);
        return compDateRes != 1;
    }
}

void SortStudents(std::vector<Student>& students, SortKind sortKind) {
    if (sortKind == SortKind::Date)
        std::sort(students.begin(), students.end(), comparatorByDate);
    else if (sortKind == SortKind::Name)
        std::sort(students.begin(), students.end(), comparatorByName);
}
