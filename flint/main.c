#define _GNU_SOURCE

#include <flint/flint.h>
#include <flint/fmpz.h>
#include <flint/fmpz_vec.h>
#include <flint/fmpz_mod.h>
#include <flint/fmpz_mod_poly.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
const unsigned D = 1024; //must be a power of two

void multi_product(fmpz_mod_poly_t res, fmpz_mod_poly_t polys[], const unsigned SZ, fmpz_mod_ctx_t mctx){
	unsigned spacing = 2;
	while(spacing <= SZ){
		for(int i = 0; i < SZ; i += spacing){
			fmpz_mod_poly_mul(polys[i], polys[i], polys[i+(spacing/2)], mctx);
		}
		spacing *= 2;
	}
	fmpz_mod_poly_set(res, polys[0], mctx);
}

int main(int argc, char ** argv){
	char c;
	bool test_case = false;
	bool debug_mode = false;
	while((c = getopt(argc, argv, "dg")) != -1){
		switch(c){
			case 'd':
				test_case = true;		
				break;
			case 'g':
				debug_mode = true;
				break;
			default:
				flint_printf("Unknown argument -%c.\n", optopt);
		}
	}
	fmpz_t N;
	fmpz_init(N);
	if(test_case){
		fmpz_set_str(N, "94305636672702649474558417828362036272104111840058701850649865218307321963568680690791671829040697196935047546920700797655764901389354964779077761052105531990311770491570170816115182366428361604921987776912168323789911523115886233410670849960743121279976023541547204319856773877804718054623238451628699279951", 10);
	}else{
		flint_printf("Input N:\n");
		char * N_str = 0;
		size_t len = 0;
		getline(&N_str,&len, stdin);
		fmpz_set_str(N, N_str, 10);
		free(N_str);
	}
	fmpz_mod_ctx_t mctx;
	fmpz_mod_ctx_init(mctx, N);


	fmpz_t e;
	fmpz_init(e);
	if(test_case){
		fmpz_set_str(e, "11272634923307714669458380596665007991696806333536228009995611799093838831707267726918684393539352948073058518784230022689713637763177925430935918174262355311239846537267200554686597662789712933656407398448487941156212579054981690892648123294970108223394941292585403532881166242912628785776689139642459922197", 10);
	}else{
		flint_printf("Input e:\n");
		char * e_str = 0;
		size_t len = 0;
	        getline(&e_str, &len, stdin);
		fmpz_set_str(e, e_str, 10);
		free(e_str);
	}
	
	flint_printf("Attempting to find a prime factor of N with:\nN = %{fmpz}\ne = %{fmpz}\n", N, e);

	for(unsigned rnum = 42; rnum < 2991978999; rnum += 71829){
		if(debug_mode && (rnum % (71829 * 100) == 42)){
			flint_printf("Progress: %u\n", rnum);
		}
		fmpz_t x;
		fmpz_init_set_ui(x,rnum);
		fmpz_t x_pow;
		fmpz_init(x_pow);
		fmpz_mod_pow_fmpz(x_pow, x, e, mctx);

		fmpz_mod_poly_t prod;
		fmpz_mod_poly_init(prod, mctx);
		{
			//TODO use fmpz_mod_poly_struct
			fmpz_mod_poly_t p[D];
			fmpz_t x_pow_inter;
			fmpz_init(x_pow_inter);
			fmpz_one(x_pow_inter);
			for(unsigned i = 0; i < D; i++){
				fmpz_mod_poly_init(p[i], mctx);
				fmpz_mod_poly_set_fmpz(p[i], x, mctx);
				fmpz_mod_poly_neg(p[i], p[i], mctx);
				fmpz_mod_poly_set_coeff_fmpz(p[i], 1, x_pow_inter, mctx);
				fmpz_mod_mul(x_pow_inter, x_pow_inter, x_pow, mctx);
			}	
			multi_product(prod, p, D, mctx);
			fmpz_clear(x_pow_inter);
			for(unsigned i = 0; i < D; i++){
				fmpz_mod_poly_clear(p[i], mctx);
			}
		}
		fmpz * evals = _fmpz_vec_init(D);
		{
			fmpz* inputs = _fmpz_vec_init(D);
			fmpz_t x_pow_D;
			fmpz_init(x_pow_D);
			fmpz_mod_pow_ui(x_pow_D, x_pow, D, mctx);
			fmpz_one(inputs);
			for(unsigned i = 1; i < D; i++){
				fmpz_set(inputs + i, inputs + i - 1);
				fmpz_mod_mul(inputs + i, inputs + i, x_pow_D, mctx);
			}
			fmpz_clear(x_pow_D);
			fmpz_mod_poly_evaluate_fmpz_vec_fast(evals, prod, inputs, D, mctx);
			_fmpz_vec_clear(inputs, D);
		}
		fmpz_t potential_answer;
		fmpz_init(potential_answer);
		bool solved = false;
		for(unsigned i = 0; i < D; i++){
			fmpz_gcd(potential_answer, evals + i, N);
			if(fmpz_cmp_si(potential_answer, 1) > 0 && fmpz_cmp(potential_answer, N) < 0){
				flint_printf("The prime factor is %{fmpz}.\n\a", potential_answer);
				fmpz_t verify;
				fmpz_init(verify);
				fmpz_mod(verify, N, potential_answer);
				if(fmpz_cmp_ui(verify, 0) == 0){
					flint_printf("%{fmpz} %% %{fmpz} = 0.\n", N, potential_answer);
				}else{
					flint_printf("Actually, it didn't get a factor of N.\n%{fmpz} %% %{fmpz} = %{fmpz}\n", N, potential_answer, verify);
				}
				fmpz_clear(verify);
				solved = true;
				break;
			}
		}
		fmpz_clear(x);
		fmpz_clear(x_pow);
		fmpz_mod_poly_clear(prod, mctx);
		fmpz_clear(potential_answer);
		_fmpz_vec_clear(evals, D);
		if(solved){
			break;
		}
	}
	fmpz_clear(N);
	fmpz_clear(e);
	fmpz_mod_ctx_clear(mctx);
}
