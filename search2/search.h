#pragma once

#include <string_view>
#include <map>
#include <vector>

struct LineData {
    std::string_view line;
    uint32_t total_cnt_words = 0;
    std::map<std::string_view, uint32_t> cnt_words;
};

class SearchEngine {
private:
    std::vector<LineData> line_data_;

public:
    void BuildIndex(std::string_view text);
    std::vector<std::string_view> Search(std::string_view query, size_t results_count) const;
};
