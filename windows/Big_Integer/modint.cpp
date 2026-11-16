#pragma once

#include "modint.hpp"



template<BigInt T>
ModInt<T> pow_mod(ModInt<T>& x, int p){
    BigInt big_p {p};
    return pow_mod(x,p);
}
template<BigInt T>
ModInt<T> pow_mod(ModInt<T>& x, ModInt<T>& p){
    return pow_mod(x, p.as_bigint());
}
template<BigInt T>
ModInt<T> pow_mod(ModInt<T>& x, BigInt p){
    ModInt<T> answer {1};
    ModInt<T> base {x};
    while(p > 0){
        if(p%2 == 1){
            answer *= base;
            p -= 1;
        }
        base *= base;
        p /= 2;
    }
    return answer;
}