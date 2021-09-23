#include "scorer.h"
#include <iostream>

struct TaskStatus {
    size_t merge_request_balance = 0;
    time_t last_submit = 0;
    bool is_submit_successful = false;
};

ScoreTable GetScoredStudents(const Events& events, time_t score_time) {
    std::map<StudentName, std::map<TaskName, TaskStatus>> task_solving_status;
    for (const Event& occasion : events) {
        auto& status = task_solving_status[occasion.student_name][occasion.task_name];
        if (occasion.time <= score_time) {
            if ((occasion.event_type == EventType::CheckFailed || occasion.event_type == EventType::CheckSuccess) &&
                status.last_submit <= occasion.time) {
                status.last_submit = occasion.time;
                if (occasion.event_type == EventType::CheckSuccess) {
                    status.is_submit_successful = true;
                } else {
                    status.is_submit_successful = false;
                }
            } else if (occasion.event_type == EventType::MergeRequestOpen) {
                ++status.merge_request_balance;
            } else if (occasion.event_type == EventType::MergeRequestClosed) {
                --status.merge_request_balance;
            }
        }
    }

    ScoreTable score_table;
    for (const auto& [student_name, tasks] : task_solving_status) {
        for (const auto& [task_name, status] : tasks) {
            if (status.is_submit_successful && status.merge_request_balance == 0) {
                score_table[student_name].insert(task_name);
            }
        }
    }
    return score_table;
}
