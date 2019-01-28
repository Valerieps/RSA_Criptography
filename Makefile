all: 
	gcc keygen.c -lgmp -o keygen.out
	gcc encrypt.c -lgmp -o encrypt.out
	gcc decrypt.c -lgmp -o decrypt.out