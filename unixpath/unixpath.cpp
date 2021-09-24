#include "unixpath.h"
#include <vector>

std::vector<std::string_view> ParsePath(std::string_view path) {
    std::vector<size_t> slash_pos;
    for (size_t i = 0; i < path.size(); ++i) {
        if (path[i] == '/') {
            slash_pos.push_back(i);
        }
    }
    slash_pos.push_back(path.size());
    std::vector<std::string_view> parsed_path;
    size_t begin = 0;
    for (size_t end : slash_pos) {
        if (begin < end) {
            parsed_path.push_back(std::string_view(path.data() + begin, end - begin));
        }
        begin = end + 1;
    }
    return parsed_path;
}

std::string BuildString(std::vector<std::string_view> path) {
    if (path.empty()) {
        return "/";
    }
    std::string result;
    for (std::string_view token : path) {
        result.push_back('/');
        result += token.substr();
    }
    return result;
}

std::string NormalizePath(std::string_view current_working_dir, std::string_view path) {
    std::vector<std::string_view> parsed_path = ParsePath(path);
    std::vector<std::string_view> corrected_path;
    if (!path.empty() && path[0] != '/') {
        corrected_path = ParsePath(current_working_dir);
    }
    for (std::string_view token : parsed_path) {
        if (token == "..") {
            if (!corrected_path.empty()) {
                corrected_path.pop_back();
            }
        } else if (token != ".") {
            corrected_path.push_back(token);
        }
    }
    return BuildString(corrected_path);
}
