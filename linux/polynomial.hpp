#pragma once
#include <vector>
#include <span>
#include <iostream>
#include "modint.hpp"

template<typename T>
class Polynomial{
    private:
    std::vector<T> _data;
    Polynomial<T> slice(int,int) const;
    Polynomial<T>& shift_add(const Polynomial<T>&, int);
    std::size_t size() const { return _data.size(); }
    
    Polynomial<T> reverse() const;
    Polynomial<T> truncated_inverse(int len);
    public:
    Polynomial();
    Polynomial(std::size_t);
    Polynomial(T);
    Polynomial(const std::vector<T>&);
    Polynomial(const std::span<T>&);
    Polynomial<T> truncate(int length) const;
    T operator[](int) const;  
    T& operator[](int);  
    Polynomial<T> operator+(const Polynomial<T>&) const;
    Polynomial<T>& operator+=(const Polynomial<T>&);
    Polynomial<T> operator*(const Polynomial<T>&) const;
    Polynomial<T>& operator*=(const Polynomial<T>&);
    Polynomial<T>& operator-=(const Polynomial<T>&);
    Polynomial<T> operator-(const Polynomial<T>&) const;
    Polynomial<T> operator%(const Polynomial<T>&) const;
    Polynomial<T>& operator%=(const Polynomial<T>&) ;
    Polynomial<T> operator/(const Polynomial<T>&) const;
    Polynomial<T>& operator/=(const Polynomial<T>&) ;
    Polynomial<T> reverse(int len) const;
    T get_constant();
    std::size_t degree() const { return _data.size()-1; }
    void print() const;
    void fix();
    
};


template class Polynomial<ModInt>;
