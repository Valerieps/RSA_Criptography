#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

// void mdc_estendido(mpz_t g, mpz_t x, mpz_t y, const mpz_t a, const mpz_t b){

// 	mpz_t resto, auxX, auxY, quociente, aux;
// 	mpz_inits(resto, auxX, auxY, quociente, aux, NULL);
// 	mpz_set(auxX , x);
// 	mpz_set(auxY , y);
	
// 	if(mpz_cmp_ui(b, 0)==0){
// 		mpz_set_ui(x, 1);
// 		mpz_set_ui(y, 0);
// 		mpz_set(g,a);
// 		return;
// 	}

// 	mpz_fdiv_qr(quociente, resto, a, b);
// 	mdc_estendido(g, auxX, auxY, b, resto);
	
// 	mpz_set(x, auxY);
// 	mpz_mul(aux, quociente, auxY);
// 	mpz_sub(y, auxX, aux);

// 	mpz_clears(resto, auxX, auxY, quociente, aux, NULL);
// }

// int inverso_modular(mpz_t r, const mpz_t a, const mpz_t n){
// 	mpz_t g, x, y;
// 	mpz_inits(g, x, y, NULL);

// 	mdc_estendido(g, x, y, n, a);

// 	if (mpz_cmp_ui(g, 1)==0){
// 		mpz_mod(r, y, n);
// 		return 1;
// 	}
// 	return 0;
// }

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

// void calcula_tqn(const mpz_t n, mpz_t n1, unsigned int *t, mpz_t q){

// 	*t=0;
// 	mpz_t aux, auxN1;
// 	mpz_inits(aux, auxN1, NULL);
// 	mpz_sub_ui(aux, n, 1);
// 	mpz_set(n1, aux);
// 	mpz_set(auxN1, n1);
	
// 	mpz_mod_ui(aux, auxN1,2);
// 	while(mpz_cmp_ui(aux,0)==0){
// 		mpz_fdiv_q_ui(auxN1, auxN1, 2);
// 		*t += 1;
// 		mpz_mod_ui(aux, auxN1,2);
// 	}

// 	mpz_set(q, auxN1);
	
// 	mpz_clears(aux, auxN1, NULL);
// }


void descriptografa(mpz_t M, const mpz_t C, const mpz_t n, const mpz_t d){
	//M = C^d(mod n)
	
	exp_binaria(M, C, d, n);

}

char* decodifica(const mpz_t n){

	char *str = (char*) malloc(501*sizeof(char));
	unsigned int i, letra;

	mpz_t c, n1, aux;
	mpz_inits(c, n1, aux, NULL);
	
	mpz_set(n1, n);

 	for(i=0; i<500; i++){
		
		if(mpz_cmp_ui(n1,0)==0)
			break;
		
		mpz_ui_pow_ui(aux, 256, i);
		mpz_fdiv_q(aux, n1, aux);
		mpz_mod_ui(c, aux, 256);
		mpz_ui_pow_ui(aux, 256, i);
		mpz_mul(aux, c, aux);
		mpz_sub(n1, n1, aux);
		
		//Converte
		letra = mpz_get_ui (c);
		str[i] = (int) letra;
	}

	str[i]='\0';

	mpz_clears(c, n1, aux, NULL);

	return str;
}

int main(){

	mpz_t M, C, n, d;
	mpz_inits(M, C, n, d, NULL);

	gmp_randstate_t rnd;
	gmp_randinit_default(rnd);
	gmp_randseed_ui(rnd, 583092);
	
	
	char num[1000000];
	scanf("%s", num);
	mpz_set_str(n, num, 10);
	gmp_printf("n: %Zd\n", n);

	char chave[10000];
	scanf("%s", chave);
	mpz_set_str(d, chave, 10);
	gmp_printf("d: %Zd\n", d);

	char msg[1000000];
	scanf("%s", msg);
	mpz_set_str(C, msg, 10);
	gmp_printf("msg lida: %Zd\n", C);	

	//== 1) DESCRIPTOGRAFA ===============
	descriptografa(M, C, n, d);
	gmp_printf("M: %Zd\n", M);

	//== 2) DECODIFICA ===============
	char *msg2=(char*) malloc(501*sizeof(char));
	msg2 = decodifica(M);

	puts(msg2);

	mpz_clears(M, C, n, d, NULL);
	// free(msg2);

	return 0;
}