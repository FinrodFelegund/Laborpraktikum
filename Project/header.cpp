#include "header.h"


Krypter::Krypter(){
    ctx = EVP_CIPHER_CTX_new();

}

Krypter::~Krypter(){
    if(ctx)
        EVP_CIPHER_CTX_free(ctx);

}

int Krypter::encrypt(unsigned char *text, int text_len, unsigned char *cipher)
{

    int cipher_len = 0;
    int len = 0;

    if(!ctx)
        return -1;

    if(!EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL))
    {
        return -1;
    }

    if(!EVP_EncryptUpdate(ctx, cipher, &len, text, text_len))
    {
        return-1;
    }

    cipher_len += len;

    if(!EVP_EncryptFinal_ex(ctx, cipher + len, &len))
        return-1;

    cipher_len += len;

    return cipher_len;

}

void Krypter::decrypt(unsigned char *cipher, int cipher_len, unsigned char *text)
{

    int len = 0;
    int text_len = 0;


    if(!ctx)
    {
        std::cout << "Decryption failed 1" << std::endl;
        return;
    }

    if(!EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL))
    {
        std::cout << "Decryption failed 2" << std::endl;
        return;
    }


    if(!EVP_DecryptUpdate(ctx, text, &len, cipher, cipher_len))
    {
        std::cout << "Decryption failed 3" << std::endl;
        return;
    }



    text_len += len;


    if(!EVP_DecryptFinal_ex(ctx, text + len, &len))
    {
        std::cout << "Decryption failed 4" << std::endl;
        ERR_print_errors_fp(stderr);
        return;
    }

    text_len += len;


}


