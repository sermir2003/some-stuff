#include "admission.h"
#include <map>
#include <tuple>

AdmissionTable FillUniversities(const std::vector<University>& universities, const std::vector<Applicant>& applicants) {
    std::vector<const Applicant*> ordered_applicants(applicants.size(), nullptr);
    for (size_t i = 0; i < applicants.size(); ++i) {
        ordered_applicants[i] = &applicants[i];
    }
    std::sort(ordered_applicants.begin(), ordered_applicants.end(), [] (const Applicant* const  ap1,
                                                                       const Applicant* const ap2) {
        return std::make_tuple(-ap1->points, ap1->student.birth_date.year, ap1->student.birth_date.month,
                               ap1->student.birth_date.day, ap1->student.name) <
               std::make_tuple(-ap2->points, ap2->student.birth_date.year, ap2->student.birth_date.month,
                               ap2->student.birth_date.day, ap2->student.name);
    });

    std::map<std::string, size_t> cnt_place_for_university;
    for (const University& college : universities) {
        cnt_place_for_university[college.name] = college.max_students;
    }

    AdmissionTable admission_table;
    for (const Applicant* const ptr_to_app : ordered_applicants) {
        for (const std::string& college : ptr_to_app->wish_list) {
            if (cnt_place_for_university[college] > 0) {
                --cnt_place_for_university[college];
                admission_table[college].push_back(&ptr_to_app->student);
                break;
            }
        }
    }

    for (auto it = admission_table.begin(); it != admission_table.end(); ++it) {
        std::sort(it->second.begin(), it->second.end(), [] (const Student* const st1, const Student* const st2) {
            return std::make_tuple(st1->name, st1->birth_date.year, st1->birth_date.month, st1->birth_date.day) <
                   std::make_tuple(st2->name, st2->birth_date.year, st2->birth_date.month, st2->birth_date.day);
        });
    }
    return admission_table;
}
