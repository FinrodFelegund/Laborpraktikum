#include <string>
#include <QString>
#include <iostream>
#include <QDebug>
#include <QWidget>
#include <openssl/conf.h>
#include <openssl/evp.h>


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
    Krypter()
    {
        ctx = EVP_CIPHER_CTX_new();
    };
    ~Krypter(){
        if(ctx)
            EVP_CYPHER_CTX_FREE(ctx);
    };

    private:
    EVP_CIPHER_CTX *ctx;



};


