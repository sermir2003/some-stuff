#include <algorithm>
#include "sort_students.h"

int ComparingDates(const Date& date1, const Date& date2) {
    // Returns -1 if date1 < date2,
    // 0 if date1 == date2,
    // 1 if date1 > date2.
    if (date1.year < date2.year) {
        return -1;
    } else if (date1.year > date2.year) {
        return 1;
    }
    if (date1.month < date2.month) {
        return -1;
    } else if (date1.month > date2.month) {
        return 1;
    }
    if (date1.day < date2.day) {
        return -1;
    } else if (date1.day > date2.day) {
        return 1;
    }
    return 0;
}

int ComparingNames(const Student& st1, const Student& st2) {
    // Returns -1 if lexicographically name of student A is earlier than name of student B,
    // 0 if names are the same,
    // 1 if if lexicographically name of student A is later than name of student B.
    if (st1.last_name < st2.last_name) {
        return -1;
    } else if (st1.last_name > st2.last_name) {
        return 1;
    }
    if (st1.name < st2.name) {
        return -1;
    } else if (st1.name > st2.name) {
        return 1;
    }
    return 0;
}

bool ComparatorByDate(const Student& st1, const Student& st2) {
    int comp_date_res = ComparingDates(st1.birth_date, st2.birth_date);
    if (comp_date_res == -1) {
        return true;
    } else if (comp_date_res == 1) {
        return false;
    } else {
        int comp_name_res = ComparingNames(st1, st2);
        return comp_name_res != 1;
    }
}

bool ComparatorByName(const Student& st1, const Student& st2) {
    int comp_name_res = ComparingNames(st1, st2);
    if (comp_name_res == -1) {
        return true;
    } else if (comp_name_res == 1) {
        return false;
    } else {
        int comp_date_res = ComparingDates(st1.birth_date, st2.birth_date);
        return comp_date_res != 1;
    }
}

void SortStudents(std::vector<Student>& students, SortKind sort_kind) {
    if (sort_kind == SortKind::Date) {
        std::sort(students.begin(), students.end(), ComparatorByDate);
    } else if (sort_kind == SortKind::Name) {
        std::sort(students.begin(), students.end(), ComparatorByName);
    }
}
