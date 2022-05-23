#include <string>
#include <QString>
#include <iostream>
#include <QDebug>
#include <QWidget>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>


// a header consists of 2 or 1 of these types 
enum MessageHeader {

    //message Types
    saveMessage,

    returnMessage,

    returnMessageArray,

    //entity Types e.g. Appointment etc;  to be completed

    AppointmentEnt


};



class AppointmentEntity
{
    public:
    AppointmentEntity(){};
    ~AppointmentEntity(){};
    QString date;
    QString doctor;
    QString text;



};

class Krypter
{
    public:
    Krypter();

    ~Krypter();



    int encrypt(unsigned char *text, int text_len, unsigned char *cipher);
    void decrypt(unsigned char *cipher, int cipher_len, unsigned char *text);

    private:
    EVP_CIPHER_CTX *ctx;
    unsigned char *key = (unsigned char *)"MyEncryption";




};


