#pragma once

#include <forward_list>
#include <string>

using Value = std::string;
using Iterator = std::forward_list<std::string>::const_iterator;
using ZippedPair = std::pair<const Value&, const Value&>;

class ZippedIterator {
private:
    Iterator first_;
    Iterator second_;

public:
    ZippedIterator(const Iterator& first, const Iterator& second);
    ZippedPair operator*() const;
    ZippedIterator& operator++();
    ZippedIterator operator++(int);
    bool operator!=(const ZippedIterator& other) const;
};

class Zipped {
private:
    Iterator first_begin_;
    Iterator second_begin_;
    Iterator first_end_;
    Iterator second_end_;

public:
    Zipped(const Iterator& first_begin, const Iterator& second_begin, const Iterator& first_end,
           const Iterator& second_end);
    ZippedIterator begin();
    ZippedIterator end();
};

Zipped Zip(Iterator a_begin, Iterator a_end, Iterator b_begin, Iterator b_end);
