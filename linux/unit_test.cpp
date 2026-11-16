#include <gmpxx.h>
#include <iostream>
#include "modint.hpp"
#include "polynomial.hpp"
const mpz_class N ("5");

void go_down_subprod_tree(std::span<ModInt> output, std::span<Polynomial<ModInt>> subprod_tree, 
                                Polynomial<ModInt>& p, const std::size_t SZ){
    if (SZ == 1) {
        output[0] = p.get_constant();
        return;
    }
    //Compute remainders at the second level of the subproduct tree.
    //Then recursively compute the first and second half of the evaluations via these remainders.
    Polynomial<ModInt> r0 = p % subprod_tree[SZ - 1 - SZ/2];
    Polynomial<ModInt> r1 = p % subprod_tree[SZ - 1 + SZ/2];
    std::span<ModInt> a = output.first(SZ/2);
    std::span<ModInt> b = output.last(SZ/2);
    std::span<Polynomial<ModInt>> c = subprod_tree.first(SZ-1);
    std::span<Polynomial<ModInt>> d = subprod_tree.last(SZ-1);
    go_down_subprod_tree(a,c,r0, SZ/2);
    go_down_subprod_tree(b,d,r1, SZ/2);
}

std::vector<ModInt> multi_eval(Polynomial<ModInt>& p, std::vector<ModInt>& inputs){
    const auto SZ = inputs.size();
    if (!((SZ != 0) && ((SZ & (SZ - 1)) == 0))){
        throw "FAIL.";
    }
    //Step 1: Create a subproduct tree
    std::vector<Polynomial<ModInt>> subprod_tree(2*SZ-1);
    //create_subprod_tree<SZ>(subprod_tree,inputs);
    for(int i = 0; i < SZ; i++) subprod_tree[2*i] = Polynomial<ModInt>({-inputs[i],1});
    int first = 1;
    int spacing = 4;
    int depth = SZ/2;
    while(depth > 0){
        for(int i = 0; i < depth; i++){
            int curpos = first + i*spacing;
            subprod_tree[curpos] = subprod_tree[curpos - (spacing/4)] * subprod_tree[curpos + (spacing/4)];
        }
        depth /= 2;
        first += spacing/2;
        spacing *= 2;
    }
    /*
    for(int i = 0; i < 2*SZ-1; i++) {
        std::cout << "POLYNOMIAL " << i << " DEGREE " << subprod_tree[i].degree() << ": ";
        for(int j = 0; j <= subprod_tree[i].degree(); j++)
            std::cout << subprod_tree[i][j] << " ";
        std::cout << std::endl;
    }*/
    //Step 2: a recursive algorithm that performs the actual evaluation
    std::vector<ModInt> output(SZ);
    go_down_subprod_tree(std::span<ModInt>(output),std::span<Polynomial<ModInt>>(subprod_tree),p, SZ); 
    return output;
}

Polynomial<ModInt> multi_product(std::vector<Polynomial<ModInt>> & p){
    const auto SZ = p.size();
    int spacing = 2;
    while(SZ /spacing > 0){
        for(int i = 0; i < SZ; i+= spacing) p[i] *= p[i+(spacing/2)];
        spacing *= 2;
    }
    return p[0];    
}

using namespace std;
int main(){
    ModInt::set_modulus(N);
    //There is a bug here with division (?). Probably wrong with the truncation.
    Polynomial<ModInt> b({1,1,3,4,2});
    Polynomial<ModInt> c({-1,1});
    (b * c).print();
    (b / c).print();
    (b % c).print();
    
}