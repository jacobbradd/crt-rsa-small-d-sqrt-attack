#include <gmpxx.h>
#include "modint.hpp"

mpz_class ModInt::modulus;
void ModInt::set_modulus(const mpz_class& mod){
    modulus = mod;
}

ModInt::ModInt(){
    this->mpz_class::operator=(0);
}

ModInt::ModInt(int a){
    this->mpz_class::operator=(a);
    this->mpz_class::operator+=(modulus);
    this->mpz_class::operator%=(modulus);
}
ModInt::ModInt(const mpz_class& a){
    this->mpz_class::operator=(a);
    this->mpz_class::operator+=(modulus);
    this->mpz_class::operator%=(modulus);
}


ModInt& ModInt::operator=(const int a){
    mpz_class b(a);
    this->mpz_class::operator=(b);
    return *this;
}

ModInt& ModInt::operator+=(const ModInt& a){
    this->mpz_class::operator+=(a);
    this->mpz_class::operator%=(modulus);
    return *this;
}

ModInt ModInt::operator+(const ModInt& a) const{
    auto b = *this;
    b += a;
    return b;
}


ModInt& ModInt::operator*=(const ModInt& a){
    this->mpz_class::operator*=(a);
    this->mpz_class::operator%=(modulus);
    return *this;
}

ModInt ModInt::operator*(const ModInt& a) const{
    auto b = *this;
    b *= a;
    return b;
}


ModInt& ModInt::operator-=(const ModInt& a){
    this->mpz_class::operator+=(modulus);
    this->mpz_class::operator-=(a);
    this->mpz_class::operator%=(modulus);
    return *this;
}
ModInt ModInt::operator-(const ModInt& a) const{
    auto b = *this;
    b -= a;
    return b;
}


ModInt ModInt::operator-() {
    ModInt t = 0;
    t -= *this;
    return t;
}

ModInt ModInt::pow(const ModInt& a) const {
    ModInt ans;
    mpz_powm(ans.get_mpz_t(),this->get_mpz_t(), a.get_mpz_t(), modulus.get_mpz_t());
    return ans;
}

ModInt ModInt::pow(const int a) const {
    ModInt ans;
    mpz_powm(ans.get_mpz_t(),this->get_mpz_t(), mpz_class(a).get_mpz_t(), modulus.get_mpz_t());
    return ans;
}
