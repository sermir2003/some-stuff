#include "scorer.h"

void Scorer::OnCheckFailed(const StudentName &student_name, const TaskName &task_name) {
    auto it_student = data_.find(student_name);
    if (it_student != data_.end()) {
        auto it_task = it_student->second.find(task_name);
        if (it_task != it_student->second.end()) {
            it_task->second.is_submit_successful = false;
            if (!it_task->second.is_there_merge_request) {
                it_student->second.erase(it_task);
            }
        }
        if (it_student->second.empty()) {
            data_.erase(it_student);
        }
    }
}

void Scorer::OnCheckSuccess(const StudentName &student_name, const TaskName &task_name) {
    data_[student_name][task_name].is_submit_successful = true;
}

void Scorer::OnMergeRequestOpen(const StudentName &student_name, const TaskName &task_name) {
    data_[student_name][task_name].is_there_merge_request = true;
}

void Scorer::OnMergeRequestClosed(const StudentName &student_name, const TaskName &task_name) {
    auto it_student = data_.find(student_name);
    if (it_student != data_.end()) {
        auto it_task = it_student->second.find(task_name);
        if (it_task != it_student->second.end()) {
            it_task->second.is_there_merge_request = false;
            if (!it_task->second.is_submit_successful) {
                it_student->second.erase(it_task);
            }
        }
        if (it_student->second.empty()) {
            data_.erase(it_student);
        }
    }
}

void Scorer::Reset() {
    data_.clear();
}

ScoreTable Scorer::GetScoreTable() const {
    ScoreTable score_table;
    for (const auto& [student_name, tasks] : data_) {
        for (const auto& [task_name, status] : tasks) {
            if (status.is_submit_successful && !status.is_there_merge_request) {
                score_table[student_name].insert(task_name);
            }
        }
    }
    return score_table;
}
