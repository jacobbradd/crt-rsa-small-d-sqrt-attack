#pragma once

#include <gmpxx.h>


class ModInt: public mpz_class{
private:
    //Unfortunately, I can't think of any other way to do this other than to use a global
    //
    static mpz_class modulus;
    
public:
    static void set_modulus(const mpz_class& mod);
    ModInt();
    ModInt(int a);
    ModInt(const mpz_class&);
    ModInt operator+(const ModInt&) const;
    ModInt operator-(const ModInt&) const;
    ModInt operator*(const ModInt&) const;
    ModInt& operator+=(const ModInt&);
    ModInt& operator-=(const ModInt&);
    ModInt& operator*=(const ModInt&);
    ModInt operator-();
    ModInt pow(const ModInt&) const;
    ModInt pow(const int) const;
    ModInt& operator=(const int);
};
