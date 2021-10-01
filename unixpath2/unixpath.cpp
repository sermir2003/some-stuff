#include "unixpath.h"

PathTokens UnixPath::ParsePath(std::string_view path) {
    std::vector<size_t> slash_pos;
    for (size_t i = 0; i < path.size(); ++i) {
        if (path[i] == '/') {
            slash_pos.push_back(i);
        }
    }
    slash_pos.push_back(path.size());
    PathTokens parsed_path;
    size_t begin = 0;
    for (size_t end : slash_pos) {
        if (begin < end) {
            parsed_path.push_back(std::string_view(path.data() + begin, end - begin));
        }
        begin = end + 1;
    }
    return parsed_path;
}

void UnixPath::NormalizePath(PathTokens& current_working_dir, const PathTokens& path) {
    for (std::string_view token : path) {
        if (token == "..") {
            if (!current_working_dir.empty()) {
                current_working_dir.pop_back();
            }
        } else if (token != ".") {
            current_working_dir.push_back(token);
        }
    }
}

UnixPath::UnixPath(std::string_view initial_dir) {
    cache_.emplace_back(initial_dir.begin(), initial_dir.end());
    PathTokens parsed_initial_dir = UnixPath::ParsePath(cache_.back());
    NormalizePath(init_path_, parsed_initial_dir);
    cur_path_ = init_path_;
}

void UnixPath::ChangeDirectory(std::string_view path) {
    cache_.emplace_back(path.begin(), path.end());
    PathTokens change_path = UnixPath::ParsePath(cache_.back());
    if (path.empty() || (!path.empty() && path[0] == '/')) {
        cur_path_.clear();
    }
    NormalizePath(cur_path_, change_path);
}

std::string UnixPath::GetAbsolutePath() const {
    if (cur_path_.empty()) {
        return "/";
    }
    std::string result;
    for (std::string_view token : cur_path_) {
        result.push_back('/');
        result += token;
    }
    return result;
}

std::string UnixPath::GetRelativePath() const {
    size_t common_part = 0;
    for (; common_part < std::min(init_path_.size(), cur_path_.size()) &&
           init_path_[common_part] == cur_path_[common_part];
         ++common_part) {
    }
    size_t up = init_path_.size() - common_part;
    size_t down = cur_path_.size() - common_part;
    std::string result;
    if (up == 0) {
        result = "./";
    }
    for (size_t i = 0; i < up; ++i) {
        result += "../";
    }
    for (size_t i = 0; i < down; ++i) {
        result += cur_path_[i + common_part];
        result.push_back('/');
    }
    result.pop_back();
    return result;
}
