#include <gmpxx.h>
#include "modint.hpp"
#include "polynomial.hpp"
#include <iostream>
#include <vector>
#include <span>

const std::size_t D = 1024;
const mpz_class N("94305636672702649474558417828362036272104111840058701850649865218307321963568680690791671829040697196935047546920700797655764901389354964779077761052105531990311770491570170816115182366428361604921987776912168323789911523115886233410670849960743121279976023541547204319856773877804718054623238451628699279951");



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


int main(){
    ModInt::set_modulus(N);
    
    const ModInt e(mpz_class("11272634923307714669458380596665007991696806333536228009995611799093838831707267726918684393539352948073058518784230022689713637763177925430935918174262355311239846537267200554686597662789712933656407398448487941156212579054981690892648123294970108223394941292585403532881166242912628785776689139642459922197"));
for(int rnum = 42; rnum < 2991978999; rnum += 71829){
    ModInt x (rnum); //random number
    ModInt x_pow = x.pow(e);
    
    Polynomial<ModInt> prod ;
    {
        std::vector<Polynomial<ModInt>> p(D);
        for(int i = 0; i < D; i++){
            p[i] = Polynomial<ModInt>(std::vector<ModInt>({-x,x_pow.pow(i)}));
        }
        prod = multi_product(p);
    }
    std::vector<ModInt> evals(D);
    {
        std::vector<ModInt> inputs(D);
        for(int i = 0; i < D; i++){
            inputs[i] = x_pow.pow(i*D);
        }
       // std::cout << x_pow.pow(740*D) << std::endl << inputs[740] << std::endl;//Why does inputs[i] NOT have the right number???
        std::cout << "Initiate Multi-Eval." <<  " " << prod.degree() << std::endl;
        evals = multi_eval(prod,inputs);
        std::cout << "DONE" << std::endl;
        //(prod % Polynomial<ModInt>(std::vector<ModInt>({-inputs[740],1}))).print();
    }
    //for(auto c: evals) std::cout << c << std::endl<< std::endl<< std::endl;
    for(int i = 0; i < D; i++){
        mpz_class potential_answer;
        mpz_gcd(potential_answer.get_mpz_t(),evals[i].get_mpz_t(),N.get_mpz_t());
        std::cout << potential_answer << " ";
        if(potential_answer > 1 && potential_answer < N){
            std::cout << "ANSWER: " << potential_answer << "\a" << std::endl;
            return 0;
        }   
    }  
    std::cout << std::endl << std::endl;
}
}