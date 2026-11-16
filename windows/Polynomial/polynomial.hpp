#pragma once
#include <vector>
#include <span>

template<typename T>
class Polynomial<T>{
    private:
    std::vector<T> _data;
    std::span<T> _view;
    T& operator[](std::size_t i) { return _view[i]; }
    const T& operator[](std::size_t i) const { return _view[i]; }
    Polynomial<T> _slice(std::size_t, std::size_t);
    Polynomial<T>& shift_add(const Polynomial<T>&, int);
    std::size_t size() const { return _data.size(); }
    
    Polynomial<T> truncate(int len);
    Polynomial<T> reverse();
    Polynomial<T> truncated_inverse(int len);
    public:
    Polynomial();
    Polynomial(std::size_t):
    Polynomial(T);
    Polynomial(vector<T>);
    Polynomial(span<T>);
    Polynomial<T> truncate(int length);
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
    T get_constant();
    std::size_t degree() const { return _view.size()-1; }
    
}