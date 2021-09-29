#include <cctype>
#include <cmath>
#include "search.h"
#include <set>

namespace {
std::vector<std::string_view> SplitTextIntoLines(std::string_view text) {
    std::vector<std::string_view> lines;
    size_t line_begin = 0;
    for (size_t i = 0; i < text.size(); ++i) {
        if (text[i] == '\n') {
            if (line_begin < i) {
                lines.push_back(std::string_view(text.data() + line_begin, i - line_begin));
            }
            line_begin = i + 1;
        }
    }
    if (line_begin < text.size()) {
        lines.push_back(std::string_view(text.data() + line_begin, text.size() - line_begin));
    }
    return lines;
}

std::vector<std::string_view> SplitLineIntoWords(std::string_view line) {
    std::vector<std::string_view> words;
    size_t word_begin = 0;
    bool is_it_word_now = false;
    for (size_t i = 0; i < line.size(); ++i) {
        if (isalpha(line[i])) {
            if (!is_it_word_now) {
                is_it_word_now = true;
                word_begin = i;
            }
        } else if (is_it_word_now) {
            is_it_word_now = false;
            words.push_back(std::string_view(line.data() + word_begin, i - word_begin));
        }
    }
    if (is_it_word_now) {
        words.push_back(std::string_view(line.data() + word_begin, line.size() - word_begin));
    }
    return words;
}

struct CmpCaseInsensitive {
    bool operator()(std::string_view st1, std::string_view st2) const {
        for (size_t i = 0; i < std::min(st1.size(), st2.size()); ++i) {
            if (tolower(st1[i]) < tolower(st2[i])) {
                return true;
            } else if (tolower(st1[i]) > tolower(st2[i])) {
                return false;
            }
        }
        return st1.size() < st2.size();
    }
};
}  // namespace

void SearchEngine::BuildIndex(std::string_view text) {
    std::vector<std::string_view> lines = SplitTextIntoLines(text);
    line_data_.resize(lines.size());
    for (size_t line_num = 0; line_num < lines.size(); ++line_num) {
        line_data_[line_num].line = lines[line_num];
        std::vector<std::string_view> words = SplitLineIntoWords(lines[line_num]);
        for (std::string_view word : words) {
            ++line_data_[line_num].cnt_words[word];
            ++line_data_[line_num].total_cnt_words;
        }
    }
}

std::vector<std::string_view> SearchEngine::Search(std::string_view query, size_t results_count) const {
    std::vector<std::string_view> query_words = SplitLineIntoWords(query);
    std::set<std::string_view, CmpCaseInsensitive> query_words_set;
    for (std::string_view word : query_words) {
        query_words_set.insert(word);
    }
    std::vector<std::vector<double>> tf(query_words_set.size(), std::vector<double>(line_data_.size()));
    std::vector<uint32_t> cnt_lines_with_this_word(query_words_set.size());
    size_t word_num = 0;
    for (std::string_view word : query_words_set) {
        for (size_t line_num = 0; line_num < line_data_.size(); ++line_num) {
            auto& line = line_data_[line_num];
            auto it_word_in_dictionary = line.cnt_words.find(word);
            if (it_word_in_dictionary != line.cnt_words.end()) {
                tf[word_num][line_num] =
                    static_cast<double>(it_word_in_dictionary->second) / static_cast<double>(line.total_cnt_words);
                ++cnt_lines_with_this_word[word_num];
            }
        }
        ++word_num;
    }
    std::vector<double> idf(query_words_set.size());
    for (word_num = 0; word_num < query_words_set.size(); ++word_num) {
        idf[word_num] =
            std::log(static_cast<double>(line_data_.size()) / static_cast<double>(cnt_lines_with_this_word[word_num]));
    }
    std::vector<std::pair<double, std::string_view>> relevance_line(line_data_.size());
    for (size_t line_num = 0; line_num < line_data_.size(); ++line_num) {
        double relevance = 0;
        for (word_num = 0; word_num < query_words_set.size(); ++word_num) {
            relevance += tf[word_num][line_num] * idf[word_num];
        }
        relevance_line[line_num] = {relevance, line_data_[line_num].line};
    }
    std::sort(relevance_line.begin(), relevance_line.end(),
              [](std::pair<double, std::string_view> elem1, std::pair<double, std::string_view> elem2) {
                  return elem1.first > elem2.first;
              });
    std::vector<std::string_view> answer;
    for (size_t i = 0; i < line_data_.size() && i < results_count && relevance_line[i].first > 0; ++i) {
        answer.push_back(relevance_line[i].second);
    }
    return answer;
}
