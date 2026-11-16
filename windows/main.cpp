#include "Big_Integer/bigint.hpp"
#include "Big_Integer/modint.hpp"
#include "Polynomial/polynomial.hpp"
#include <array>
#include <span>

const int D = 1024;
const BigInt N{"9999999999999999"};
const ModInt<N> e{"6666669999999999"};


template <std::size_t SZ>
void go_down_subprod_tree(std::span<ModInt<N>,SZ> output, std::span<Polynomial<ModInt<N>>> subprod_tree, 
                                Polynomial<ModInt<N>>& p){
    if constexpr (SZ == 1) {
        output[0] = p.get_constant();
        return;
    }
    //Compute remainders at the second level of the subproduct tree.
    //Then recursively compute the first and second half of the evaluations via these remainders.
    Polynomial<ModInt<N>> r0 = p % subprod_tree[2*SZ-4];
    Polynomial<ModInt<N>> r1 = p % subprod_tree[2*SZ-3];
    go_down_subprod_tree<SZ/2>(output.first(SZ/2),subprod_tree.first(SZ-1),r0);
    go_down_subprod_tree<SZ/2>(output.last(SZ/2),subprod_tree.last(SZ-1),r1);
}

template <std::size_t SZ>
std::array<ModInt<N>,SZ> multi_eval(Polynomial<ModInt<N>>& p, std::array<ModInt<N>,SZ>& inputs){
    static_assert((SZ != 0) && ((SZ & (SZ - 1)) == 0), "D must be a power of 2.");
    //Step 1: Create a subproduct tree
    std::array<Polynomial<ModInt<N>>,2*SZ-1> subprod_tree;
    //create_subprod_tree<SZ>(subprod_tree,inputs);
    for(int i = 0; i < SZ; i++) subprod_tree[2*i] = Polynomial<ModInt<N>>({-inputs[i],1});
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
    //Step 2: a recursive algorithm that performs the actual evaluation
    std::array<ModInt<N>,SZ> output;
    go_down_subprod_tree<SZ>(output,subprod_tree,p); 
    return output;
}

template<std::size_t SZ>
Polynomial<ModInt<N>> multi_product(std::array<Polynomial<ModInt<N>>,SZ> & p){
    int spacing = 2;
    while(SZ /spacing > 0){
        for(int i = 0; i < SZ; i+= spacing) p[i] *= p[i+(spacing/2)];
    }
    return p[0];    
}


int main(){
    ModInt<N> x {"42"}; //random number
    ModInt<N> x_pow = pow_mod(x_pow, e);
    Polynomial<ModInt<N>> prod ;
    {
        std::array<Polynomial<ModInt<N>>,D> p;
        for(int i = 0; i < D; i++){
            p[i] = Polynomial<ModInt<N>>({x,pow_mod(x_pow,i)});
        }
        prod = multi_product(p);
    }
    std::array<ModInt<N>,D> evals;
    {
        std::array<ModInt<N>,D> inputs;
        for(int i = 0; i < D; i++){
            inputs[i] = pow_mod(x_pow,i*D);
        }
        evals = multi_eval<D>(prod,inputs,D);
    }
    for(int i = 0; i < D; i++){
        BigInt potential_answer = bigint_gcd(static_cast<evals[i],N);
        if(potentialanswer > 1 && potentialanswer < N && (N % potential_answer) != 0){
            potential_answer.print();
        }   
    }  
}