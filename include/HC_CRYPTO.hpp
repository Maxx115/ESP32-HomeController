/*
*
*
*
*
*
*/

#ifndef HC_CRYPTO
#define HC_CRYPTO

#include <Arduino.h>
#include "mbedtls/rsa.h"
#include "mbedtls/pk.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include "mbedtls/base64.h"

//extern char private_key[2048];
extern char public_key[1024];

void crypto_createPair(void);
//void cryptoDecrypt(const char* encrypted, size_t encrypted_length);
String cryptoDecrypt_Server(const char* encrypted_base64);
//void cryptoDecrypt_OAEP(const unsigned char* encrypted_message);

//void cryptoTest(char* PUBLIC_KEY, char* PRIVATE_KEY);


#endif /* HC_CRYPTO */