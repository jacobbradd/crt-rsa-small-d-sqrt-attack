

//We allow big integers up to 1024 bits.
//Addition beyond this results in modular arithmetic by some modulus.
//By default, this modulus is 2^1024.
#pragma once
#include <algorithm>
#include <string>
#include <iostream>
#include "bigint_lookup.hpp"

void string_int_add(std::string & a, std::string const& b){
    //Assumes the string a is represented in backwards order
    while(a.size() < b.size()) a += '0';
    bool carry = 0;
    for(int i = 0; i < b.size(); i++){
        int temp = (a[i] - '0') + (b[b.size()-1-i]-'0') + carry;
        carry = temp/10;
        a[i] = '0' + temp%10;
    }        
    if(carry != 0) a += '1';
}

void string_int_halve(std::string& input){
    //Precondition: 
    //  input represents an even number
    //  "digits" are literally \0-\9, not the characters '0'-'9'
    //Halves an integer representated by a string base 10
    for(int i = input.size()-1; i >=0; i--){
        if(input[i]%2 != 0) input[i+1] += 5;
        input[i] /= 2;
    } 
    if(input.size() > 1 && input[0] == 0) input = input.substr(1,input.size());
}

BigInt::BigInt(): _data(1){}

BigInt::BigInt(std::string number) : _data(1){
    for(auto &c: number) c-='0';
    int index = 0;
    while(!(number.size() == 1 && number[0] == 0)){
        if(number.back() % 2 == 0){
            index++;
            _data.resize(index/32 + 1);
            string_int_halve(number);
        }else{
            data[index/32] += (uint32_t)(1 << (index%32));
            number.back()--;
        }
    }
}

BigInt::BigInt(std::vector<int>& a): _data(a){}

void BigInt::print() const{
    //Have to convert back to integer...
    std::string res = "";
    for(int i = 0; i < 1024; i++){
        if((data[i/32] >> (i%32)) & 1){
            //Have to now convert 1 >> i to a string 
            //This is in a lookup table
            string_int_add(res,bigint_lookup[i]);
        }
    }
    std::reverse(res.begin(),res.end());
    std::cout << res << std::endl;
}

std::vector<int> karatsuba(std::span<int>& a, std::span<int>& b){
    auto adeg = a.size();
    auto bdeg = b.size();
    auto smaller = min(adeg,bdeg);
    if(smaller <= 5){
        vector<uint64_t> res(adeg + bdeg-1);
        for(int k = 0; k <= adeg + bdeg - 1; k++){
            for(int i = adeg; i >= k - bdeg; i--){
                res[k] += a[i] * b[k - i];
            }
        }
        a = res;
        return a;
    }
    span<uint64_t> aspan(a);
    span<uint64_t> bspan(b);
    auto A0 = a.slice(0, smaller/2);
    auto A1 = a.slice(smaller/2+1, smaller/2);
    auto B0 = b.slice(0, smaller/2);
    auto B1 = b.slice(smaller/2+1, smaller/2);
    auto m1 = karatsuba(A0,B0);
    auto m2 = karatsuba(A1,B1);
    auto sum = karatsuba(vector_sliced_add(A0, A1), vector_sliced_add(B0,B1));
    sum = vector_sliced_subtract(sum, m1);
    sum = vector_sliced_subtract(sum, m2);
    
    Polynomial<T> res(adeg + bdeg -1);
    res.shift_add(m1,0);
    res.shift_add(m2,(smaller/2)*(smaller/2))
    res.shift_add(sum,(smaller/2));
    
}

BigInt& BigInt::operator*=(BigInt const & b) {
        *this = BigInt(karatsuba(this->_data,b._data));
        
        this->fix_values();
        return *this;
}


BigInt BigInt::operator*(BigInt const& b) const{
    BigInt res = *this;
    res *= b;
    return res;
}

BigInt& BigInt::fix_values(){
    for(int i = 0; i < min(15,this->size); i++){
        this->data[i+1] += this->data[i]>>32;
        this->data[i] = (this->data[i] << 32)>>32;
        if(this->data[i]) this->size = i;
    }
    if(this->data[min(15,this->size+1)]) this->size = 15;
    return *this
}



BigInt bigint_gcd(BigInt a, BigInt b){
    if(a < b) swap(a,b); //WLOG a >= b
    while(b > 0){
        a %= b;
        swap(a,b);
    }
    return a;
}