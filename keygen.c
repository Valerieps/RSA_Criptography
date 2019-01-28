#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

void extended_gcd(mpz_t g, mpz_t x, mpz_t y, const mpz_t a, const mpz_t b){

	mpz_t resto, auxX, auxY, quociente, aux;
	mpz_inits(resto, auxX, auxY, quociente, aux, NULL);
	mpz_set(auxX , x);
	mpz_set(auxY , y);
	
	if(mpz_cmp_ui(b, 0)==0){
		mpz_set_ui(x, 1);
		mpz_set_ui(y, 0);
		mpz_set(g,a);
		return;
	}

	mpz_fdiv_qr(quociente, resto, a, b);
	extended_gcd(g, auxX, auxY, b, resto);
	
	mpz_set(x, auxY);
	mpz_mul(aux, quociente, auxY);
	mpz_sub(y, auxX, aux);

	mpz_clears(resto, auxX, auxY, quociente, aux, NULL);
}

int modular_inverse(mpz_t r, const mpz_t a, const mpz_t n){
	mpz_t g, x, y;
	mpz_inits(g, x, y, NULL);

	extended_gcd(g, x, y, n, a);

	if (mpz_cmp_ui(g, 1)==0){
		mpz_mod(r, y, n);
		return 1;
	}
	
	mpz_clears(g, x, y, NULL);	

	return 0;
}

void binary_exp(mpz_t r, const mpz_t b, const mpz_t e, const mpz_t n){
	
	mpz_t aux;
	mpz_init(aux);

	if(mpz_cmp_ui (e, 0) == 0){
		mpz_set_ui(r, 1);
		return;
	}

	else {
		mpz_fdiv_q_ui(aux, e, 2);
		binary_exp(r, b, aux, n);
		mpz_mul(aux, r, r);
		mpz_mod(aux, aux, n);
		mpz_set(r, aux);
		mpz_mod_ui(aux, e, 2);
		if(mpz_cmp_ui (aux, 1) == 0){
			mpz_mul(aux, r, b);
			mpz_mod(aux, aux, n);
			mpz_set(r, aux);
		}
  	}

	mpz_clear(aux);
}

void calcula_tqn(const mpz_t n, mpz_t n1, unsigned int *t, mpz_t q){

	*t=0;
	mpz_t aux, auxN1;
	mpz_inits(aux, auxN1, NULL);
	mpz_sub_ui(aux, n, 1);
	mpz_set(n1, aux);
	mpz_set(auxN1, n1);
	
	mpz_mod_ui(aux, auxN1,2);
	while(mpz_cmp_ui(aux,0)==0){
		mpz_fdiv_q_ui(auxN1, auxN1, 2);
		*t += 1;
		mpz_mod_ui(aux, auxN1,2);
	}

	mpz_set(q, auxN1);
	
	mpz_clears(aux, auxN1, NULL);
}

int maybe_prime(const mpz_t a,	const mpz_t n, const mpz_t n1, unsigned int t, const mpz_t q){

	unsigned int i;
	mpz_t aux, e;
	mpz_inits(aux, e, NULL);
	
	if (mpz_cmp_ui(n,1)==0 || mpz_cmp_ui(n,2)==0)
		return 1;
	
	mpz_mod_ui(aux, n, 2);
	if (mpz_cmp_ui(aux,0)==0)
		return 0;

	
	else{ // Calculates Miller:
		
		binary_exp(aux, a, q, n);
		if(mpz_cmp_ui(aux,1)==0 || mpz_cmp(aux, n1)==0)
			return 1;

		for(i=1; i<t; i++){ //vai até t ou t-1??
			
			mpz_ui_pow_ui(aux, 2, i);
			mpz_mul(aux, aux, q);
			mpz_set(e, aux);

			binary_exp(aux, a, e, n);
	
			if(mpz_cmp(aux, n1)==0)
				return 1;
		}
	}

	mpz_clears(aux, e, NULL);
}

void random_n(mpz_t r, const mpz_t n, gmp_randstate_t rnd) {
	
	mp_bitcnt_t num_bits = mpz_sizeinbase(n, 2);
	do {
		mpz_urandomb(r, rnd, num_bits);
	} while (!(mpz_cmp_ui(r, 1) >= 0 && mpz_cmp(r, n) <= 0));
}

int probably_prime(const mpz_t n, unsigned int iter, gmp_randstate_t rnd){
	mpz_t a, aux, q, n1;
	unsigned int t, miller;
	mpz_inits(a, aux, q, n1, NULL);
	unsigned int i;

	if (mpz_cmp_ui(n,1)==0 || mpz_cmp_ui(n,2)==0)
		return 1;
	
	mpz_mod_ui(aux, n, 2);
	if (mpz_cmp_ui(aux,0)==0)
		return 0;

	calcula_tqn(n, n1, &t, q);	

	for(i=0; i<iter; i++){
		// Generates e random base 'a' between 2 and n − 1
		// aux = n-2
		mpz_sub_ui(aux, n, 2);
		random_n(a, aux, rnd);
		mpz_add_ui(a, a, 1);

		// Tests for primality
		miller = maybe_prime(a, n, n1, t, q);

		if(miller == 0)
			return 0;

	}

	mpz_clears(a, aux, q, n1, NULL);
	return 1;
}

void random_prime(mpz_t r,unsigned int b,gmp_randstate_t rnd){

	int provPrimo=0;
	unsigned int iter = 20;

	while(!provPrimo){
		mpz_urandomb(r, rnd, b);
		provPrimo = probably_prime(r, iter, rnd);
	}
}

void keygen(gmp_randstate_t rnd){
	
	mpz_t n, d, e;
	mpz_inits(n, d, e, NULL);

	gmp_printf("Generating public key\n");
	mpz_t p, q, g, x, y;
	mpz_inits(p, q, g, x, y, NULL);

	unsigned int exp=2048;
	gmp_printf("...Finding first prime\n");
	random_prime(p, exp, rnd);
	gmp_printf("...Finding second prime\n");
	random_prime(q, exp, rnd);
	mpz_mul(n, p, q);

	//Calculates exponent
	gmp_printf("...Finding exponent\n");
	mpz_set_ui(e, 65537);
	extended_gcd(g, x, y, n, e);
	while(mpz_cmp_ui(g,1)!=0){
		extended_gcd(g, x, y, n, e);
		mpz_add_ui(e, e, 1);
	}

	//Calculates phi(n)
	gmp_printf("Generating private key\n");
	mpz_t phi, aux;
	mpz_inits(phi, aux, NULL);
	mpz_sub_ui(aux, p, 1);
	mpz_set(phi, aux);
	mpz_sub_ui(aux, q, 1);
	mpz_mul(phi, phi, aux);

	//Calculates d (private key) - modular inverse of e(mod phi)
	modular_inverse(d, e, phi);

	// Uncomment for verbose mode
	// gmp_printf("prime number 1: %Zd\n", p);
	// gmp_printf("prime number 2: %Zd\n", q);
	// gmp_printf("Public Key (prime 1 x prime 2): %Zd\n", n);
	// gmp_printf("exponent: %Zd\n", e);
	// gmp_printf("Private Key: %Zd\n", d);

	FILE *filePublic = fopen("publicKey", "w");
	mpz_out_str(filePublic, 10, n);
	mpz_out_str(filePublic, 10, e);
	fclose(filePublic);
	
	FILE *filePrivate = fopen("privateKey", "w");
	mpz_out_str(filePrivate, 10, d);
	fclose(filePrivate);

	mpz_clears(p, q, g, x, y, phi, aux, NULL);
	mpz_clears(n, d, e, NULL);
}


int main(){

	gmp_randstate_t rnd;
	gmp_randinit_default(rnd);
	gmp_randseed_ui(rnd, 583092);
	
	keygen(rnd);

	return 0;
}
