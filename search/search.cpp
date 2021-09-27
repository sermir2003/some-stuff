#include <cctype>
#include <cmath>
#include <iostream>
#include <map>
#include "search.h"
#include <set>

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

std::vector<std::string_view> Search(std::string_view text, std::string_view query, size_t results_count) {
    std::cout << "control point 1" << std::endl;
    std::vector<std::string_view> query_words = SplitLineIntoWords(query);
    std::set<std::string_view, CmpCaseInsensitive> query_words_set;
    std::cout << "control point 2" << std::endl;
    for (std::string_view word : query_words) {
        query_words_set.insert(word);
    }
    std::vector<std::string_view> lines = SplitTextIntoLines(text);
    std::cout << "control point 3" << std::endl;
    std::vector<std::vector<uint32_t>> cnt_word_for_lines(lines.size(), std::vector<uint32_t>(query_words_set.size()));
    std::vector<uint32_t> cnt_lines_with_word(query_words_set.size());
    std::vector<uint32_t> cnt_all_words_in_line(query_words_set.size());
    std::cout << "control point 4" << std::endl;
    for (size_t line_num = 0; line_num < lines.size(); ++line_num) {
        std::string_view line = lines[line_num];
        std::cout << "try SplitLineIntoWords " << line_num << std::endl;
        std::vector<std::string_view> words = SplitLineIntoWords(line);
        std::cout << "finish SplitLineIntoWords " << line_num << std::endl;
        std::map<std::string_view, uint32_t, CmpCaseInsensitive> cnt_word_in_line;
        std::cout << "control point A" << std::endl;
        for (std::string_view word : words) {
            ++cnt_word_in_line[word];
        }
        std::cout << "control point B" << std::endl;
        size_t query_word_num = 0;
        for (std::string_view query_word : query_words_set) {
            cnt_word_for_lines[line_num][query_word_num] = cnt_word_in_line[query_word];
            if (cnt_word_in_line[query_word]) {
                ++cnt_lines_with_word[query_word_num];
            }
            ++query_word_num;
            std::cout << "control point C" << std::endl;
        }
        cnt_all_words_in_line[line_num] = words.size();
        std::cout << "control point D" << std::endl;
    }
    std::cout << "control point 5" << std::endl;
    std::vector<double> idf(query_words_set.size());
    for (size_t query_word_num = 0; query_word_num < query_words_set.size(); ++query_word_num) {
        idf[query_word_num] =
            std::log(static_cast<double>(lines.size()) / static_cast<double>(cnt_lines_with_word[query_word_num]));
    }
    std::cout << "control point 6" << std::endl;
    std::vector<std::pair<double, std::string_view>> relevance_line(lines.size());
    for (size_t line_num = 0; line_num < lines.size(); ++line_num) {
        double relevance = 0;
        for (size_t query_word_num = 0; query_word_num < query_words_set.size(); ++query_word_num) {
            double tf = static_cast<double>(cnt_word_for_lines[line_num][query_word_num]) /
                        static_cast<double>(cnt_all_words_in_line[line_num]);
            relevance += tf * idf[query_word_num];
        }
        relevance_line[line_num] = {relevance, lines[line_num]};
    }
    std::cout << "control point 7" << std::endl;
    std::sort(relevance_line.begin(), relevance_line.end(),
              [](std::pair<double, std::string_view> elem1, std::pair<double, std::string_view> elem2) {
                  return elem1.first > elem2.first;
              });
    std::cout << "control point 8" << std::endl;
    std::vector<std::string_view> answer;
    for (size_t i = 0; i < lines.size() && i < results_count && relevance_line[i].first > 0; ++i) {
        answer.push_back(relevance_line[i].second);
    }
    std::cout << "control point 9" << std::endl;
    return answer;
}
