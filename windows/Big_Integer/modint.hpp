#pragma once

#include <gmpxx.h>
#include "bigint.hpp"


template <mpz_class T>
class ModInt<T>: public mpz_class{
    public:
    ModInt<T>& operator+=(ModInt<T>&);
    ModInt<T> operator+(ModInt<T>&, ModInt<T>&) const;
    ModInt<T>& operator-=(ModInt<T>&);
    ModInt<T> operator-(ModInt<T>&, ModInt<T>&) const;
    ModInt<T>& operator-() const;
    ModInt<T>& operator*=(ModInt<T>&);
    ModInt<T> operator*(ModInt<T>&, ModInt<T>&) const;
    ModInt<T>& operator%=(ModInt<T>&);
    ModInt<T> operator%(ModInt<T>&, ModInt<T>&) const;
    ModInt<T>& operator/=(ModInt<T>&);
    ModInt<T> operator/(ModInt<T>&, ModInt<T>&) const;
}
