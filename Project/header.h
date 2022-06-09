#include <string>
#include <QString>
#include <iostream>
#include <QDebug>
#include <QWidget>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>



// a header consists of 2 of these types + cipher length
enum MessageHeader
{    //message Types
    saveMessage = 0,
    returnMessage,
    returnMessageArray,
    loginRequest,
    signUpRequest,
    passwortRequest,
    DoNothing,
    ReturnInformation,
    logoutRequest,

    //entity Types e.g. Appointment etc;  to be completed
    AppointmentEnt,
    DoctorEnt,
    UserEnt,

    //Return Message Entity Types
    DoctorSaved,
    AppointmentSaved,
    DoctorNotSaved,
    AppointmentNotSaved,
    UserSaved,
    UserNotSaved,
    logoutSuccessful,
    logoutNotSuccessful


};




class Entity
{
    public:
    Entity(){};
    ~Entity(){};

    virtual QString getPropertiesAsString() = 0;
    virtual void setPropertiesAsEntity(QStringList list) = 0;
    virtual void print() = 0;

};

class User : public Entity
{
public:
    User();
    virtual ~User();
    void setProperties(QString uID, QString eMail, QString password);
    void setPropertiesAsEntity(QStringList list);
    void print();
    void setUID(QString UID);
    void setPassword(QString password);

    QString getPropertiesAsString();
    QString getUID();
    QString getEmail();
    QString getPassword();

private:
    QString uID = "";
    QString password = "";
    QString eMail = "";
};

class AppointmentEntity : public Entity
{
    public:

    AppointmentEntity(){};
    ~AppointmentEntity(){};
    void setProperties(QString date, QString time, QString doctorID, QString title, QString text);
    void setPropertiesAsEntity(QStringList list);
    void print();
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
    void print();
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

class DoctorEntityId: public DoctorEntity{

public:
    DoctorEntityId(){};
    ~DoctorEntityId(){};
    void setProperties(QString doctor_id, QString name, QString street, QString streetNumber, QString city, QString postalCode, QString phoneNumber);
    void setPropertiesAsEntity(QStringList list);
    void print();
    QString getPropertiesAsString();

private:
    QString doctor_id;
};

class Krypter
{
    public:
    Krypter();

    ~Krypter();

    QString decrypt(QByteArray buffer, int cipherLength);
    QByteArray encrypt(QString buffer, int* cipherLength);

    int encrypt(unsigned char *text, int text_len, unsigned char *cipher);
    void decrypt(unsigned char *cipher, int cipher_len, unsigned char *text);

    private:
    EVP_CIPHER_CTX *ctx;
    unsigned char *key = (unsigned char *)"MyEncryption";




};

/*insert into users (email, Passwort) values("emaileingabe", md5("passworteingabe");*/


