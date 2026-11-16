#pragma once

#include<algorithm>
#include<vector>
#include<string>

class BigInt{
    private:
    std::vector<uint64_t> _data; //32*32 = 1024 bits; I use uint64_t to avoid overflow and perform an O(1) adjustment afterwards
    std::size_t size() {return _data.size();}
    public:
    
    BigInt();
    BigInt(std::string number);
    BigInt::BigInt(std::vector<int>& a);
    
    void print() const;
    
    BigInt& operator*=(BigInt const & b) const;
    BigInt operator*(BigInt const& b) const;
    BigInt karatsuba(BigInt& a, BigInt& b);
    BigInt& fix_values();
    
};


BigInt bigint_gcd(BigInt a, BigInt b);