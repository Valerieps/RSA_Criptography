#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

void exp_binaria(mpz_t r, const mpz_t b, const mpz_t e, const mpz_t n){
	
	mpz_t aux;
	mpz_init(aux);

	if(mpz_cmp_ui (e, 0) == 0){
		mpz_set_ui(r, 1);
		return;
	}

	else {
		mpz_fdiv_q_ui(aux, e, 2);
		exp_binaria(r, b, aux, n);

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


void descriptografa(mpz_t M, const mpz_t C, const mpz_t n, const mpz_t d){
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

	return 0;
}
