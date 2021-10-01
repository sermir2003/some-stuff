#pragma once

#include <list>
#include <string>
#include <string_view>
#include <vector>

using PathTokens = std::vector<std::string_view>;

class UnixPath {
private:
    std::list<std::string> cache_;
    PathTokens init_path_;
    PathTokens cur_path_;
    static PathTokens ParsePath(std::string_view path);
    static void NormalizePath(PathTokens& current_working_dir, const PathTokens& path);

public:
    UnixPath(std::string_view initial_dir);

    void ChangeDirectory(std::string_view path);

    std::string GetAbsolutePath() const;
    std::string GetRelativePath() const;
};
