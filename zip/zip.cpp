#include "zip.h"

Zipped Zip(Iterator a_begin, Iterator a_end, Iterator b_begin, Iterator b_end) {
    Iterator first = a_begin;
    Iterator second = b_begin;
    while (first != a_end && second != b_end) {
        ++first;
        ++second;
    }
    return Zipped(a_begin, b_begin, first, second);
}
Zipped::Zipped(const Iterator& first_begin, const Iterator& second_begin, const Iterator& first_end,
               const Iterator& second_end)
    : first_begin_(first_begin), second_begin_(second_begin), first_end_(first_end), second_end_(second_end) {
}
ZippedIterator Zipped::begin() {
    return ZippedIterator(first_begin_, second_begin_);
}
ZippedIterator Zipped::end() {
    return ZippedIterator(first_end_, second_end_);
}
ZippedIterator::ZippedIterator(const Iterator& first, const Iterator& second) : first_(first), second_(second) {
}
ZippedPair ZippedIterator::operator*() const {
    return {*first_, *second_};  // I used make_pair here but is did not work. TODO: find out why
}
ZippedIterator& ZippedIterator::operator++() {
    ++first_;
    ++second_;
    return *this;
}
ZippedIterator ZippedIterator::operator++(int) {
    ZippedIterator previous(*this);
    ++first_;
    ++second_;
    return previous;
}
bool ZippedIterator::operator!=(const ZippedIterator& other) const {
    return first_ != other.first_ || second_ != other.second_;
}
