#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

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

void codifica(mpz_t r, const char *str){
	// printf("entrou codifica\n");
	int n = strlen(str);
	int i;
	mpz_t aux;
	mpz_init(aux);
	char c='a';
	for(i=0; i<n; i++){
		// printf("analisando char: %c\n", str[i]);
		mpz_ui_pow_ui(aux, 256, i);
		mpz_mul_ui(aux, aux, str[i]);
		// gmp_printf("esse char: %Zd\n", aux);
		mpz_add(r, r, aux);
	}

	mpz_clear(aux);
}

void criptografa(mpz_t C, mpz_t M, mpz_t n, mpz_t e){
	//C=M^e (mod n)
	exp_binaria(C, M, e, n);
}

int main(){

	char msg[501];
	mpz_t C, M, n, e;
	mpz_inits(C, M, n, e, NULL);
	
	//==== RECEBE CHAVE PÚBLICA ===
	char num[1000000];
	scanf("%s", num);
	getchar();
	mpz_set_str(n, num, 10);
	gmp_printf("n: %Zd\n", n);

	char exp[10000];
	scanf("%s", exp);
	getchar();
	mpz_set_str(e, exp, 10);
	gmp_printf("e: %Zd\n", e);


	//==== RECEBE MSG DO USUARIO===
	fgets(msg, 500, stdin);
	printf("msg: %s\n", msg);
	
	// //== 1) CODIFICA ================
	codifica(M, msg);
	gmp_printf("msg codificada: %Zd\n", M);


	// //== 2) CRIPTOGRAFA ===============
	criptografa(C, M, n, e);
	gmp_printf("msg criptografada: %Zd\n", C);
	

	mpz_clears(M, C, n, e, NULL);
	

	return 0;
}