#include "polynomial.hpp"
#include <vector>
#include <span>
#include <algorithm>

template<typename T>
T Polynomial<T>::get_constant(){
    return this->_data.begin();
}

template<typename T>
Polynomial<T>::Polynomial() _data({0}), _view(_data.data(),1){}

template<typename T>
Polynomial<T>::Polynomial(std::size_t len) _data(len), _view(_data.data(),len){}

template<typename T>
Polynomial<T>::Polynomial(T a) _data({a}), _view(_data.data(),1){}

template<typename T>
Polynomial<T>::Polynomial(vector<T> a); _data{a},_view(_data.data(),a.size()) {}

//CAUTION: This is pure scuff. This creates a ghost polynomial. Access occurs by view.
//A Polynomial is a glorified pointer: the span is the interface, with a backend vector for storage.
template<typename T>
Polynomial<T>::Polynomial(span<T> a); _view{a} {}

template<typename T>
Polynomial<T> Polynomial<T>::slice(std::size_t start, std::size_t len){
    return Polynomial<T>(_view.subspan(offset, len));
}

template<typename T>
Polynomial<T> Polynomial<T>::operator*(const Polynomial<T>& b) const{.
    auto a = *this;
    a *= b;
    return a;    
}

template<typename T>
Polynomial<T>& Polynomial<T>::operator*=(const Polynomial<T>& b){
    auto& a = *this;
    auto adeg = this->degree();
    auto bdeg = b.degree();
    auto smaller = (adeg < bdeg) ? adeg : bdeg;
    if(smaller <= 5){
        Polynomial<T> res(adeg + bdeg-1);
        for(int k = 0; k <= adeg + bdeg - 1; k++){
            for(int i = adeg; i >= k - bdeg; i--){
                res[k] += a[i] * b[k - i];
            }
        }
        a = res;
        return a;
    }
    Polynomial<T> A0 = a.slice(0, smaller/2);
    Polynomial<T> A1 = a.slice(smaller/2+1, smaller/2);
    Polynomial<T> B0 = b.slice(0, smaller/2);
    Polynomial<T> B1 = b.slice(smaller/2+1, smaller/2);
    auto m1 = A0*B0;
    auto m2 = A1*B1;
    auto sum = A0 + B0;
    sum *= A1 + B1;
    sum -= m1 + m2;
    
    Polynomial<T> res(adeg + bdeg -1);
    res.shift_add(m1,0);
    res.shift_add(m2,(smaller/2)*(smaller/2))
    res.shift_add(sum,(smaller/2));
    
    a = res; 
    return a;
}


template<typename T>
Polynomial<T>& Polynomial<T>::operator+=(const Polynomial<T>& b){
    auto& a = *this;
    if(a.degree() >= b.degree()){
        for(int i = 0; i <= a.degree(); i++){
            a[i] += b[i];
        }
        while(a._data().back() == 0) a._data().pop_back();
        return a;
    }else{
        //SCUFF: completely wrong if this ever happens with the slices
        auto c = b;
        c += a;
        a = c;
        return a;
    }
}
    
template<typename T>
Polynomial<T> Polynomial<T>::operator+(const Polynomial<T>& b) const{
    auto res = *this;
    res += b;
    return res;
}


template<typename T>
Polynomial<T>& Polynomial<T>::operator-=(const Polynomial<T>& b){
    auto& a = *this;
    if(a.degree() >= b.degree()){
        for(int i = 0; i <= a.degree(); i++){
            a[i] -= b[i];
        }
        while(a._data().back() == 0) a._data().pop_back();
        return a;
    }else{
        //SCUFF: completely wrong if this ever happens with the slices
        auto c = b;
        c -= a;
        a = c;
        return a;
    }
}
    
template<typename T>
Polynomial<T> Polynomial<T>::operator-(const Polynomial<T>& b) const{
    auto res = *this;
    res -= b;
    return res;
}

template <typename T>
Polynomial<T> Polynomial<T>::truncate(int len){
    auto& a = *this;
    Polynomial<T> b(std::min(a.size(),len));
    for(int i = 0; i < len; i++) b[i] = a[i];
    return b;
}

template <typename T>
Polynomial<T> Polynomial<T>::reverse(){
    auto& a = *this;
    Polynomial<T> b(a.size());
    for(int i = 0; i < a.size(); i++) b[i] = a[a.size() - 1 - i];
    return b
}
    
    

template<typename T>
Polynomial<T> Polynomial<T>::truncated_inverse(int len){
    //newton iteration
    auto& f = *this;
    Polynomial<T> g(1);
    g[0] = 1;
    for(int i = 1; i <= len; i *= 2){
        g = 2*g - f * g * g;
        g = g.truncate(2*i);
    }
    g = 2*g - f * g * g;
    g = g.truncate(len);
    return g;
}
    
    
template<typename T>
Polynomial<T>& Polynomial<T>::operator/=(const Polynomial<T>& b){
    auto& a = *this;
    if(b.degree() > a.degree()){
        Polynomial<T> c(1);
        a = c;
        return a;
    }
    int m = a.degree() - b.degree();
    a = (a.truncate(m+1).reverse() * b.truncate(m+1).reverse().truncated_inverse(m+1)).truncate(m+1).reverse(); 
    return a;
    
    
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
    q = a/b;
    a -= q*b;
    return a;
}
    
template<typename T>
Polynomial<T> Polynomial<T>::operator%(const Polynomial<T>& b) const{
    auto res = *this;
    res %= b;
    return res;
}
    
//helper function: assumes b fits
template<typename T>
void Polynomial<T>::shift_add(const Polynomial<T>& b, int pos){
    for(int i = 0; i < b.size(); i++){
        *this[pos+i] += b[i];
    }
}