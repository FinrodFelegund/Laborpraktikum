#include <string>
#include <QString>
#include <iostream>
#include <QDebug>
#include <QWidget>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>



// a header consists of 2 of these types
enum MessageHeader
{

    //message Types
    saveMessage,

    returnMessage,

    returnMessageArray,



    //entity Types e.g. Appointment etc;  to be completed

    AppointmentEnt,

    DoctorEnt

};

enum FromServerToClient
{
    Appointments,

    Doctor


};



class Entity
{
    public:
    Entity(){};
    ~Entity(){};

    virtual QString getPropertiesAsString() = 0;
    virtual void setPropertiesAsEntity(QStringList list) = 0;
};

class AppointmentEntity : public Entity
{
    public:

    AppointmentEntity(){};
    ~AppointmentEntity(){};
    void setProperties(QString date, QString time, QString doctorID, QString title, QString text);
    void setPropertiesAsEntity(QStringList list);
    QString getPropertiesAsString();
    QString getDate();
    QString getTime();
    QString getDoctorID();
    QString getTitle();
    QString getText();

    private:

    QString date;
    QString time;
    QString doctorID;
    QString title;
    QString text;



};

class DoctorEntity : public Entity
{
    public:

    DoctorEntity(){};
    ~DoctorEntity(){};
    void setProperties(QString name, QString street, QString streetNumber, QString city, QString postalCode, QString phoneNumber);
    void setPropertiesAsEntity(QStringList list);
    QString getPropertiesAsString();
    QString getName();
    QString getStreet();
    QString getStreetNumber();
    QString getCity();
    QString getPostalCode();
    QString getPhoneNumber();

    private:

    QString name;
    QString street;
    QString streetNumber;
    QString city;
    QString postalCode;
    QString phoneNumber;

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


