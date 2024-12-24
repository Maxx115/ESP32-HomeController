/*
*
*
*
*
*
*/

#include "HC_Crypto.hpp"

#include "mbedtls/rsa.h"
#include "mbedtls/pk.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include "mbedtls/base64.h"

char private_key[2048];
char public_key[1024];

const char *pers = "rsa_encrypt";
const char *pers_de = "rsa_decrypt";

void crypto_createPair(void)
{
    Serial.println("Starting key pair creation process...");
    
    // Initialize the necessary contexts
    mbedtls_pk_context pk;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;

    mbedtls_pk_init(&pk);
    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);

    if (mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)pers, strlen(pers)) != 0) {
        Serial.println("Failed to initialize random number generator");
        return;
    }

    // Generate an RSA key pair
    if (mbedtls_pk_setup(&pk, mbedtls_pk_info_from_type(MBEDTLS_PK_RSA)) != 0 ||
        mbedtls_rsa_gen_key(mbedtls_pk_rsa(pk), mbedtls_ctr_drbg_random, &ctr_drbg, 2048, 65537) != 0) {
        Serial.println("Failed to generate RSA key pair");
        return;
    }

    // Write the private key
    size_t len;
    if (mbedtls_pk_write_key_pem(&pk, (unsigned char *)private_key, sizeof(private_key)) != 0) {
        Serial.println("Failed to write private key");
        return;
    }
    Serial.println("Private Key:");
    Serial.println(private_key);

    // Write the public key
    if (mbedtls_pk_write_pubkey_pem(&pk, (unsigned char *)public_key, sizeof(public_key)) != 0) {
        Serial.println("Failed to write public key");
        return;
    }
    Serial.println("Public Key:");
    Serial.println(public_key);

    // Free the contexts
    mbedtls_pk_free(&pk);
    mbedtls_entropy_free(&entropy);
    mbedtls_ctr_drbg_free(&ctr_drbg);
}

void cryptoDecrypt(const char* encrypted, size_t encrypted_length)
{
    char decrypted[256];

    // Initialize the encryption context
    mbedtls_pk_context pk;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    
   // Initialize the decryption context
    mbedtls_pk_init(&pk);
    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);

    if ((mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)pers_de, strlen(pers_de))) != 0) 
    { 
        Serial.println("Failed to initialize random number generator");
        return;
    }

    // Load private key
    if ((mbedtls_pk_parse_key(&pk, (const unsigned char *)private_key, strlen(private_key) + 1, NULL, 0)) != 0) {
        Serial.println("Failed to parse private key");
        return;
    }

    // Ensure the key is RSA
    if (mbedtls_pk_get_type(&pk) != MBEDTLS_PK_RSA) {
        Serial.println("Key is not RSA");
        return;
    }

    // Decrypt the message
    size_t decrypted_length;
    int ret;
    if ((ret = mbedtls_pk_decrypt(&pk, (const unsigned char *)encrypted, encrypted_length, (unsigned char *)decrypted, &decrypted_length, sizeof(decrypted), mbedtls_ctr_drbg_random, &ctr_drbg)) != 0) {
        Serial.println("Failed to decrypt");
        Serial.println(encrypted_length);
        Serial.write((const char *)encrypted, encrypted_length);
        Serial.println();
        Serial.println(ret);
        return;
    }

    Serial.println("Decrypted message:");
    Serial.write((const char *)decrypted, decrypted_length);
    Serial.println();

    // Free the private key context
    mbedtls_pk_free(&pk);
    mbedtls_entropy_free(&entropy);
    mbedtls_ctr_drbg_free(&ctr_drbg);
}

String cryptoDecrypt_OAEP(const unsigned char* encrypted_message)
{
    mbedtls_pk_context pk;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_entropy_context entropy;
    const char *pers = "rsa_decrypt";

    mbedtls_pk_init(&pk);
    mbedtls_ctr_drbg_init(&ctr_drbg);
    mbedtls_entropy_init(&entropy);

    // Seed the random number generator
    if (mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)pers, strlen(pers)) != 0) {
        Serial.println("Failed to initialize random number generator");
        return "error";
    }

    // Parse the private key
    if (mbedtls_pk_parse_key(&pk, (const unsigned char *)private_key, strlen(private_key) + 1, NULL, 0) != 0) {
        Serial.println("Failed to parse private key");
        return "error";
    }

    // Ensure the key is RSA
    if (mbedtls_pk_get_type(&pk) != MBEDTLS_PK_RSA) {
        Serial.println("Key is not RSA");
        return "error";
    }

    unsigned char decrypted[256];
    size_t decrypted_length;

    // Set RSA context for OAEP padding
    mbedtls_rsa_context *rsa = mbedtls_pk_rsa(pk);
    mbedtls_rsa_set_padding(rsa, MBEDTLS_RSA_PKCS_V21, MBEDTLS_MD_SHA256);

    // Decrypt the message using OAEP padding
    int ret = mbedtls_rsa_rsaes_oaep_decrypt(rsa, mbedtls_ctr_drbg_random, &ctr_drbg, MBEDTLS_RSA_PRIVATE, NULL, 0, &decrypted_length, (unsigned char*) encrypted_message, decrypted, sizeof(decrypted));
    if (ret != 0) {
        Serial.printf("Failed to decrypt: -0x%04X\n", -ret);
        return "error";
    }

    Serial.print("Decrypted message: ");
    Serial.write(decrypted, decrypted_length);
    Serial.println();

    mbedtls_pk_free(&pk);
    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);
    String str(reinterpret_cast<char*>(decrypted), decrypted_length);

    return str;
}

String cryptoDecrypt_Server(const char* encrypted_base64)
{
    unsigned char encrypted[256];
    size_t encrypted_length;
    mbedtls_base64_decode(encrypted, sizeof(encrypted), &encrypted_length, (const unsigned char*)encrypted_base64, strlen(encrypted_base64));
    return cryptoDecrypt_OAEP(encrypted);
}

void cryptoTest(char* PUBLIC_KEY, char* PRIVATE_KEY) 
{
    const char *message = "Hello, ESP32!";
    char encrypted[256];
    char decrypted[256];

    // Initialize the encryption context
    mbedtls_pk_context pk;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    
    mbedtls_pk_init(&pk);
    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);

    if ((mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)pers, strlen(pers))) != 0) 
    { 
        Serial.println("Failed to initialize random number generator");
        return;
    }

    // Load the public key
    if (mbedtls_pk_parse_public_key(&pk, (const unsigned char *)PUBLIC_KEY, strlen(PUBLIC_KEY) + 1) != 0) {
        Serial.println("Failed to parse public key");
        return;
    }

    // Encrypt the message
    size_t ecrypted_length;
    if (mbedtls_pk_encrypt(&pk, (const unsigned char *)message, strlen(message), (unsigned char *)encrypted, &ecrypted_length, sizeof(encrypted), mbedtls_ctr_drbg_random, &ctr_drbg) != 0) {
        Serial.println("Failed to encrypt");
        return;
    }

    Serial.println("Encrypted message:");
    for (size_t i = 0; i < ecrypted_length; i++) 
    {
        Serial.print(encrypted[i]);
        Serial.print(" ");
    }
    Serial.println();
    Serial.println(ecrypted_length);

    // Free the public key context
    mbedtls_pk_free(&pk);

    // Initialize the decryption context
    mbedtls_pk_init(&pk);

    // Load private key
    if ((mbedtls_pk_parse_key(&pk, (const unsigned char *)private_key, strlen(private_key) + 1, NULL, 0)) != 0) {
        Serial.println("Failed to parse private key");
        return;
    }

    // Decrypt the message
    size_t decrypted_length;
    if ((mbedtls_pk_decrypt(&pk, (const unsigned char *)encrypted, ecrypted_length, (unsigned char *)decrypted, &decrypted_length, sizeof(decrypted), mbedtls_ctr_drbg_random, &ctr_drbg)) != 0) {
        Serial.println("Failed to decrypt");
        return;
    }

    Serial.println("Decrypted message:");
    Serial.write((const char *)decrypted, decrypted_length);
    Serial.println();

    // Free the private key context
    mbedtls_pk_free(&pk);
    mbedtls_entropy_free(&entropy);
    mbedtls_ctr_drbg_free(&ctr_drbg);

    cryptoDecrypt(encrypted, 256);
}
