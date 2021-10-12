#pragma once
#include <vector>
#include <ostream>

class Poly {
private:
    struct Monomial {
        int32_t index = 0;
        int64_t ratio = 0;
        bool operator==(const Monomial& other) const {
            return index == other.index && ratio == other.ratio;
        }
    };
    using IndexAndRatioPair = std::pair<int32_t, int64_t>;
    std::vector<Monomial> coefficients_;
    static int64_t Power(int64_t base, int64_t degree);

public:
    Poly();
    explicit Poly(std::vector<int64_t> init_data);
    explicit Poly(std::vector<IndexAndRatioPair> init_data);
    int64_t operator()(int64_t argument) const;
    bool operator==(const Poly& other) const;
    bool operator!=(const Poly& other) const;
    Poly operator+(const Poly& other) const;
    Poly operator-(const Poly& other) const;
    void operator+=(const Poly& other);
    void operator-=(const Poly& other);
    Poly operator*(const Poly& other) const;
    Poly operator-() const;
    friend std::ostream& operator<<(std::ostream& out, const Poly& self);
};
