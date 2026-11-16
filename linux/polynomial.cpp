#include "polynomial.hpp"
#include <vector>
#include <span>
#include <algorithm>
#include <iostream>
template<typename T>
T Polynomial<T>::get_constant(){
    return *this->_data.begin();
}

template<typename T>
Polynomial<T>::Polynomial(): _data(1){}

template<typename T>
Polynomial<T>::Polynomial(std::size_t len): _data(len){}

template<typename T>
Polynomial<T>::Polynomial(T a): _data({a}){}

template<typename T>
Polynomial<T>::Polynomial(const std::vector<T>& a): _data{a}{}

template<typename T>
Polynomial<T> Polynomial<T>::slice(int start, int end) const{
    auto& a = *this;
    auto d = end - start;
    Polynomial<T> res(d);
    for(int i = 0; i < d; i++) res[i] = a[start+i];
    res.fix();
    return res;
}

template<typename T>
Polynomial<T> Polynomial<T>::operator*(const Polynomial<T>& b) const{
    auto  a = *this;
    a *= b;
    return a;    
}

template<typename T>
void Polynomial<T>::fix(){
    while(this->size() > 1 && this->_data.back() == 0) this->_data.pop_back();
}


template<typename T>
T Polynomial<T>::operator[](int i) const{
    return this->_data[i];
}
template<typename T>
T& Polynomial<T>::operator[](int i){
    return this->_data[i];
}



template<typename T>
Polynomial<T>& Polynomial<T>::operator+=(const Polynomial<T>& b){
    *this = *this + b;
    return *this;
}
    
template<typename T>
Polynomial<T> Polynomial<T>::operator+(const Polynomial<T>& b) const{
    auto a = *this;
    Polynomial<T> res(std::max(a.size(),b.size()));
    for(int i = 0; i < a.size(); i++) res[i] += a[i];
    for(int i = 0; i < b.size(); i++) res[i] += b[i];
    res.fix();
    return res;
}


template<typename T>
Polynomial<T>& Polynomial<T>::operator-=(const Polynomial<T>& b){
    *this = *this - b;
    return *this;
}
    
template<typename T>
Polynomial<T> Polynomial<T>::operator-(const Polynomial<T>& b) const{
    auto a = *this;
    Polynomial<T> res(std::max(a.degree(),b.degree())+1);
    for(int i = 0; i < a.size(); i++) res[i] += a[i];
    for(int i = 0; i < b.size(); i++) res[i] -= b[i];
    res.fix();
    return res;
}

//helper function: assumes b fits
template<typename T>
Polynomial<T>& Polynomial<T>::shift_add(const Polynomial<T>& b, int pos){
    for(int i = 0; i < b.size(); i++){
        this->_data[pos+i] += b[i];
    }
    return *this;
}



template<typename T>
Polynomial<T>& Polynomial<T>::operator*=(const Polynomial<T>& b){
    auto a = *this;
    int adeg = this->size();
    int bdeg = b.size();
    int smaller = (adeg < bdeg) ? adeg : bdeg;
    if(smaller <= 5){
        Polynomial<T> res(adeg + bdeg - 1);
        for(int i = 0; i < adeg; i++){
            for(int j = 0; j < bdeg; j++){
                res[i+j] += a[i] * b[j];
            }
        }
        res.fix();
        *this = res;
        return *this;
    }
    Polynomial<T> A0 = a.slice(0, smaller/2);
    Polynomial<T> A1 = a.slice(smaller/2, adeg);
    Polynomial<T> B0 = b.slice(0, smaller/2);
    Polynomial<T> B1 = b.slice(smaller/2, bdeg);
    
    auto m1 = A0*B0;
    auto m2 = A1*B1;
    auto sum = A0 + A1;
    sum *= B0+B1;
    sum -= m1 + m2;
    Polynomial<T> res(adeg + bdeg - 1);
    res.shift_add(m1,0);
    res.shift_add(m2,(smaller/2)*2);
    res.shift_add(sum,smaller/2);
    res.fix();
    *this = res;
    return *this;
}




template <typename T>
Polynomial<T> Polynomial<T>::truncate(int len) const{
    return this->slice(0,std::min(len,static_cast<int>(this->size())));
}

template <typename T>
Polynomial<T> Polynomial<T>::reverse() const{
    auto& a = *this;
    Polynomial<T> b(a.size());
    for(int i = 0; i < a.size(); i++) b[i] = a[a.size() - 1 - i];
    return b;
}
    
template <typename T>
Polynomial<T> Polynomial<T>::reverse(int len) const{
    auto& a = *this;
    Polynomial<T> b(len);
    for(int i = 0; i < a.size(); i++) b[i] = a[a.size() - 1 - i];
    return b;
}
    
    

template<typename T>
Polynomial<T> Polynomial<T>::truncated_inverse(int len){
    //newton iteration
    auto& f = *this;
    Polynomial<T> g;
    g[0] = 1;
    int l = 1;
    while(l < len) l*= 2;
    for(int i = 2; i <= l; i *= 2){
        auto tempf = f.truncate(i);
        g = g + g - tempf * g * g;
        g = g.truncate(i);
    }
    g = g.truncate(len);
    g.fix();
    return g;
}
    
    
template<typename T>
Polynomial<T>& Polynomial<T>::operator/=(const Polynomial<T>& c){
    
    if(c.degree() > this->degree()){
        *this = Polynomial<T>();
        return *this;
    }
    int m = this->degree() - c.degree();
    auto a = this->reverse();
    auto b = c.reverse();
    a = a.truncate(m+1);
    b = b.truncate(m+1);
    b = b.truncated_inverse(m+1);
    a *= b;
    //std::cout << m+1 << " ";
    //a.print();
    a = a.truncate(m+1);
    while(a.size() < m+1) a._data.push_back(0);
    a = a.reverse(m+1);
    *this = a;
    return *this;
    
    
}
    
template<typename T>
Polynomial<T> Polynomial<T>::operator/(const Polynomial<T>& b) const{
    auto res = *this;
    res /= b;
    return res;
}

template<typename T>
Polynomial<T>& Polynomial<T>::operator%=(const Polynomial<T>& b){
    auto& a = *this;
    if(b.degree() > a.degree()) return a;
    auto q = a/b;
    a -= q*b;
    a = a.truncate(b.size());
    while(a.size() > 1 && a[a.degree()] == 0) a._data.pop_back();
    return a;
}
    
template<typename T>
Polynomial<T> Polynomial<T>::operator%(const Polynomial<T>& b) const{
    auto res = *this;
    res %= b;
    return res;
}
    
template<typename T>
void Polynomial<T>::print()const {
    std::cout <<"| ";
    for(auto c: this->_data){
        std::cout << c << " ";
    }
    std::cout << "|" << std::endl;
}