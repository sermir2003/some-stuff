#include "poly.h"

int64_t Poly::Power(int64_t base, int64_t degree) {
    if (degree == 0) {
        return 1;
    }
    if (degree % 2 == 1) {
        return Power(base, degree - 1) * base;
    }
    base = Power(base, degree / 2);
    return base * base;
}

Poly::Poly() {}

Poly::Poly(std::vector<int64_t> init_data) {
    for (size_t i = 0; i < init_data.size(); ++i) {
        if (init_data[i] != 0) {
            coefficients_.push_back(Monomial{.index = static_cast<int32_t>(i), .ratio = init_data[i]});
        }
    }
}

Poly::Poly(std::vector<IndexAndRatioPair> init_data) {
    std::sort(init_data.begin(), init_data.end());
    for (size_t i = 0; i < init_data.size(); ++i) {
        if (init_data[i].second != 0) {
            coefficients_.push_back(Monomial{.index = static_cast<int32_t>(init_data[i].first), .ratio = init_data[i].second});
        }
    }
}

int64_t Poly::operator()(int64_t argument) const {
    int64_t value = 0;
    for (const Monomial& monomial : coefficients_) {
        value += monomial.ratio * Power(argument, monomial.index);
    }
    return value;
}

bool Poly::operator==(const Poly &other) const {
    return coefficients_ == other.coefficients_;
}

bool Poly::operator!=(const Poly &other) const {
    return coefficients_ != other.coefficients_;
}

Poly Poly::operator+(const Poly &other) const {
    std::vector<IndexAndRatioPair> new_coefficients;
    auto self_it = coefficients_.begin();
    auto other_it = other.coefficients_.begin();
    auto self_it_end = coefficients_.end();
    auto other_it_end = other.coefficients_.end();
    while (self_it != self_it_end || other_it != other_it_end) {
        if (self_it != self_it_end && other_it != other_it_end && self_it->index == other_it->index) {
            if (self_it->ratio + other_it->ratio != 0) {
                new_coefficients.emplace_back(self_it->index, self_it->ratio + other_it->ratio);
            }
            ++self_it;
            ++other_it;
        }
        else if ((self_it != self_it_end && other_it == other_it_end) || self_it->index < other_it->index) {
            new_coefficients.emplace_back(self_it->index, self_it->ratio);
            ++self_it;
        }
        else if ((self_it == self_it_end && other_it != other_it_end) || self_it->index > other_it->index) {
            new_coefficients.emplace_back(other_it->index, other_it->ratio);
            ++other_it;
        }
    }
    return Poly(new_coefficients);
}

Poly Poly::operator-() const {
    Poly result(*this);
    for (Monomial& monomial : result.coefficients_) {
        monomial.ratio = -monomial.ratio;
    }
    return result;
}

Poly Poly::operator-(const Poly &other) const {
    return *this + -other;
}

void Poly::operator+=(const Poly &other) {
    *this = *this + other;
}

void Poly::operator-=(const Poly &other) {
    *this = *this - other;
}

Poly Poly::operator*(const Poly &other) const {
    std::vector<IndexAndRatioPair> coefficients;
    coefficients.reserve(coefficients_.size() + other.coefficients_.size());
    for (auto[index1, ratio1] : coefficients_) {
        for (auto[index2, ratio2] : other.coefficients_) {
            coefficients.emplace_back(index1 + index2, ratio1 * ratio2);
        }
    }
    std::sort(coefficients.begin(), coefficients.end());
    std::vector<IndexAndRatioPair> unique_not_zero_coefficients;
    int32_t cur_index = -1, cur_ratio = 0;
    for (const auto& elem : coefficients) {
        if (cur_index != elem.first) {
            if (cur_ratio != 0) {
                unique_not_zero_coefficients.emplace_back(cur_index, cur_ratio);
            }
            cur_index = elem.first;
            cur_ratio = elem.second;
        } else {
            cur_ratio += elem.second;
        }
    }
    if (cur_ratio != 0) {
        unique_not_zero_coefficients.emplace_back(cur_index, cur_ratio);
    }
    return Poly(unique_not_zero_coefficients);
}

std::ostream& operator<< (std::ostream &out, const Poly& self) {
    out << "y = ";
    if (self.coefficients_.empty()) {
        out << "0";
    }
    else {
        for (auto it = self.coefficients_.rbegin(); it != self.coefficients_.rend(); ++it) {
            out << std::to_string(it->ratio);
            if (it->index == 1) {
                out << "x";
            } else if (it->index > 1) {
                out << "x^" << std::to_string(it->index);
            }
            if (it != self.coefficients_.rend() - 1) {
                out << " + ";
            }
        }
    }
    return out;
}
