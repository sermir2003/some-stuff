#pragma once

#include <iterator>

template <typename Iterator1, typename Iterator2>
class ZippedIterator {
private:
    Iterator1 first_;
    Iterator2 second_;

public:
    ZippedIterator(const Iterator1& first, const Iterator2& second);
    auto operator*() const;
    ZippedIterator& operator++();
    bool operator!=(const ZippedIterator& other) const;
};
template <typename Iterator1, typename Iterator2>
ZippedIterator<Iterator1, Iterator2>::ZippedIterator(const Iterator1& first, const Iterator2& second) : first_(first), second_(second) {
}
template <typename Iterator1, typename Iterator2>
auto ZippedIterator<Iterator1, Iterator2>::operator*() const {
    using Type1 = decltype(*first_);
    using Type2 = decltype(*second_);
    return std::pair<Type1, Type2>(*first_, *second_);
}
template <typename Iterator1, typename Iterator2>
ZippedIterator<Iterator1, Iterator2>& ZippedIterator<Iterator1, Iterator2>::operator++() {
    ++first_;
    ++second_;
    return *this;
}
template <typename Iterator1, typename Iterator2>
bool ZippedIterator<Iterator1, Iterator2>::operator!=(const ZippedIterator& other) const {
    return (first_ != other.first_) || (second_ != other.second_);
}

template <typename Iterator1, typename Iterator2>
class Zipped {
private:
    Iterator1 first_begin_;
    Iterator1 first_end_;
    Iterator2 second_begin_;
    Iterator2 second_end_;

public:
    Zipped(const Iterator1& first_begin, const Iterator1& first_end, const Iterator2& second_begin, const Iterator2& second_end);
    ZippedIterator<Iterator1, Iterator2> begin();
    ZippedIterator<Iterator1, Iterator2> end();
};
template <typename Iterator1, typename Iterator2>
Zipped<Iterator1, Iterator2>::Zipped(const Iterator1& first_begin, const Iterator1& first_end,
                                                   const Iterator2& second_begin, const Iterator2& second_end) : first_begin_(first_begin), first_end_(first_end), second_begin_(second_begin), second_end_(second_end) {
}
template <typename Iterator1, typename Iterator2>
ZippedIterator<Iterator1, Iterator2> Zipped<Iterator1, Iterator2>::begin() {
    return ZippedIterator<Iterator1, Iterator2>(first_begin_, second_begin_);
}
template <typename Iterator1, typename Iterator2>
ZippedIterator<Iterator1, Iterator2> Zipped<Iterator1, Iterator2>::end() {
    return ZippedIterator<Iterator1, Iterator2>(first_end_, second_end_);
}

template <typename Sequence1, typename Sequence2>
auto Zip(const Sequence1& sequence1, const Sequence2& sequence2) {
    using Iterator1 = decltype(std::begin(sequence1));
    using Iterator2 = decltype(std::begin(sequence2));
    Iterator1 first_begin = std::begin(sequence1);
    Iterator1 first_end = std::begin(sequence1);
    Iterator2 second_begin = std::begin(sequence2);
    Iterator2 second_end = std::begin(sequence2);
    while (first_end != std::end(sequence1) && second_end != std::end(sequence2)) {
        ++first_end;
        ++second_end;
    }
    return Zipped<Iterator1, Iterator2>(first_begin, first_end, second_begin, second_end);
}
