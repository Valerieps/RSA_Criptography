#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

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

void encodes(mpz_t r, const char *str){
	int n = strlen(str);
	int i;
	mpz_t aux;
	mpz_init(aux);
	char c='a';
	for(i=0; i<n; i++){
		mpz_ui_pow_ui(aux, 256, i);
		mpz_mul_ui(aux, aux, str[i]);
		mpz_add(r, r, aux);
	}

	mpz_clear(aux);
}

void encrypts(mpz_t C, mpz_t M, mpz_t n, mpz_t e){
	binary_exp(C, M, e, n);
}

int main(){

	char msg[501];
	mpz_t C, M, n, e;
	mpz_inits(C, M, n, e, NULL);
	
	// Gets inputs
	char num[1000000];
	gmp_printf("Please, input the LONG number from public Key: ");
	scanf("%s", num);
	getchar();

	char exp[10000];
	gmp_printf("Please, input the SHORT number from public Key: ");
	scanf("%s", exp);
	getchar();

	gmp_printf("Message: ", n);
	fgets(msg, 500, stdin);
	

	encodes(M, msg);
	encrypts(C, M, n, e);
	gmp_printf("=== Encrypted message===\n%Zd\n", C);
	
	FILE *fileEncrypted = fopen("encryptedMessage", "w");
	mpz_out_str(fileEncrypted, 10, C);
	fclose(fileEncrypted);

	mpz_clears(M, C, n, e, NULL);
	
	return 0;
}
