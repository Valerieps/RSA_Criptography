# RSA_Criptography

Generates RSA private and public keys with 2048 bits. Encrypts and decrypts messages.
It makes use of the GMP library. The GMP library allows you to work with numbers as large and precise as you like in C language. 

## Installing GMP
 Download it from https://gmplib.org/.

 After extracting, navigate to the gmp directory and run:
 
 ``` 
 ./configure
make
make check
make install 
```


This will take a while but not much.
After installing GMP, navigate to the cloned repository and run ``` make ```


## Generating private and public keys
The keygen.out code generates 2048 bit keys in C language. When you run keygen.out, two text files will be generated with the each key. Note that the publicKey is composed of two distinct numbers. 


## Encryption
The encryption.out receives three inputs from the command line: the public Key of the receiver(composed of two numbers) and the message. The encrypted message will be printed both to screen and a to a text file.


## Decryption
