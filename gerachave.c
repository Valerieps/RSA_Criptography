#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

void mdc_estendido(mpz_t g, mpz_t x, mpz_t y, const mpz_t a, const mpz_t b){

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
	mdc_estendido(g, auxX, auxY, b, resto);
	
	mpz_set(x, auxY);
	mpz_mul(aux, quociente, auxY);
	mpz_sub(y, auxX, aux);

	mpz_clears(resto, auxX, auxY, quociente, aux, NULL);
}

int inverso_modular(mpz_t r, const mpz_t a, const mpz_t n){
	mpz_t g, x, y;
	mpz_inits(g, x, y, NULL);

	mdc_estendido(g, x, y, n, a);

	if (mpz_cmp_ui(g, 1)==0){
		mpz_mod(r, y, n);
		return 1;
	}
	
	mpz_clears(g, x, y, NULL);	

	return 0;
}

void exp_binaria(mpz_t r, const mpz_t b, const mpz_t e, const mpz_t n){
	
	mpz_t aux;
	mpz_init(aux);

	if(mpz_cmp_ui (e, 0) == 0){
		mpz_set_ui(r, 1);
		return;
	}

	else {
		//r = expmod(b, e/2, n);
		mpz_fdiv_q_ui(aux, e, 2);
		exp_binaria(r, b, aux, n);

		//r = (r*r) % n;
		mpz_mul(aux, r, r);
		mpz_mod(aux, aux, n);
		mpz_set(r, aux);

		//if(e%2 == 1)
		//	r = (r*b) % n;
  		//return r;
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

int talvez_primo(const mpz_t a,	const mpz_t n, const mpz_t n1, unsigned int t, const mpz_t q){

	unsigned int i;
	mpz_t aux, e;
	mpz_inits(aux, e, NULL);
	
	if (mpz_cmp_ui(n,1)==0 || mpz_cmp_ui(n,2)==0)
		return 1;
	
	mpz_mod_ui(aux, n, 2);
	if (mpz_cmp_ui(aux,0)==0)
		return 0;

	
	else{ // faz miller:
		
		//calcula primeiro termo aux = a^q (mod n)		
		exp_binaria(aux, a, q, n);
		if(mpz_cmp_ui(aux,1)==0 || mpz_cmp(aux, n1)==0)
			return 1;

		for(i=1; i<t; i++){ //vai até t ou t-1??
			
			//calcula expoente (e=2^i*q)
			mpz_ui_pow_ui(aux, 2, i);
			mpz_mul(aux, aux, q);
			mpz_set(e, aux);

			exp_binaria(aux, a, e, n);
	
			if(mpz_cmp(aux, n1)==0)
				return 1;
		}
	}

	mpz_clears(aux, e, NULL);
}

void numero_aleatorio(mpz_t r, const mpz_t n, gmp_randstate_t rnd) {
	
	mp_bitcnt_t num_bits = mpz_sizeinbase(n, 2);
	do {
		mpz_urandomb(r, rnd, num_bits);
	} while (!(mpz_cmp_ui(r, 1) >= 0 && mpz_cmp(r, n) <= 0));
}

int provavelmente_primo(const mpz_t n, unsigned int iter, gmp_randstate_t rnd){
	mpz_t a, aux, q, n1;
	unsigned int t, miller;
	mpz_inits(a, aux, q, n1, NULL);
	unsigned int i;

	//Se n=1 ou 2
	if (mpz_cmp_ui(n,1)==0 || mpz_cmp_ui(n,2)==0)
		return 1;
	
	//se n eh par
	mpz_mod_ui(aux, n, 2);
	if (mpz_cmp_ui(aux,0)==0)
		return 0;

	calcula_tqn(n, n1, &t, q);
	// gmp_printf("n: %Zd\n", n);
	

	for(i=0; i<iter; i++){
		// gerar uma base aleatória 'a' entre 2 e n − 1
		// aux = n-2
		mpz_sub_ui(aux, n, 2);
		numero_aleatorio(a, aux, rnd);
		mpz_add_ui(a, a, 1);
		gmp_printf("testar base: %Zd\n", a);		

		// usar a função talvez_primo para verificar a primalidade de n.
		miller = talvez_primo(a, n, n1, t, q);
		// verifica retorno da funcao
		if(miller == 0)
			return 0;

	}

	mpz_clears(a, aux, q, n1, NULL);
	return 1;
}

void primo_aleatorio(mpz_t r,unsigned int b,gmp_randstate_t rnd){

	int provPrimo=0;
	unsigned int iter = 20;

	while(!provPrimo){
		mpz_urandomb(r, rnd, b);
		provPrimo = provavelmente_primo(r, iter, rnd);
	}
}

void gera_chaves(mpz_t n, mpz_t e, mpz_t d,gmp_randstate_t rnd){

	mpz_t p, q, g, x, y;
	mpz_inits(p, q, g, x, y, NULL);

	//gera p, q e n
	unsigned int exp=2048;
	primo_aleatorio(p, exp, rnd);
	primo_aleatorio(q, exp, rnd);
	mpz_mul(n, p, q);

	//Calcula e
	mpz_set_ui(e, 65537);
	mdc_estendido(g, x, y, n, e);
	while(mpz_cmp_ui(g,1)!=0){
		mdc_estendido(g, x, y, n, e);
		mpz_add_ui(e, e, 1);
	}
	// gmp_printf("g: %Zd\n", g);
	// gmp_printf("e: %Zd\n", e);

	//calcula phi(n)
	mpz_t phi, aux;
	mpz_inits(phi, aux, NULL);
	mpz_sub_ui(aux, p, 1);
	mpz_set(phi, aux);
	mpz_sub_ui(aux, q, 1);
	mpz_mul(phi, phi, aux);
	// gmp_printf("phi: %Zd\n", phi);

	//calcula d - inverso de 'e' (mod phi)
	inverso_modular(d, e, phi);
	// gmp_printf("d: %Zd\n", d);

	gmp_printf("p: %Zd\n", p);
	gmp_printf("q: %Zd\n", q);
	gmp_printf("n: %Zd\n", n);
	gmp_printf("e: %Zd\n", e);
	gmp_printf("d: %Zd\n", d);

	mpz_clears(p, q, g, x, y, phi, aux, NULL);
}


int main(){

	mpz_t n, d, e;
	mpz_inits(n, d, e, NULL);

	gmp_randstate_t rnd;
	gmp_randinit_default(rnd);
	gmp_randseed_ui(rnd, 583092);
	

	gera_chaves(n, e, d,rnd);

	mpz_clears(n, d, e, NULL);

	return 0;
}