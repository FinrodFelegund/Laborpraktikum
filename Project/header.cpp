#include "header.h"


Krypter::Krypter(){
    ctx = EVP_CIPHER_CTX_new();

}

Krypter::~Krypter(){
    if(ctx)
        EVP_CIPHER_CTX_free(ctx);

}

QByteArray Krypter::encrypt(QString buffer, int* cipherLength)
{
    std::string sequence = buffer.toStdString();
    unsigned char *text = (unsigned char*)sequence.c_str();
    int textLength = strlen((const char*)text);
    unsigned char cipherText[300];

    *cipherLength = this->encrypt(text, textLength, cipherText);

    QByteArray returnMessage;

    buffer.clear();
    for(int i = 0; i < *cipherLength; i++){
        buffer.append((const char) cipherText[i]);
    }


    returnMessage = buffer.toUtf8();


    //qDebug() << "Client Utf8 encoded " <<returnMessage;
    //qDebug() << "Cipher Length in encryption: " << *cipherLength;

    return returnMessage;
}


QString Krypter::decrypt(QByteArray buffer, int cipherLength)
{
    unsigned char buf[300];
    unsigned char plainText[300];
    //qDebug() << "Server Utf8 encoded: " << buffer;
    //qDebug() << "Cipher Length in decryption: " << cipherLength;
    QString cipher = QString::fromUtf8(buffer);

    for(int i = 0; i < cipherLength; i++)
    {
        buf[i] = cipher[i].toLatin1();
    }


    this->decrypt(buf, cipherLength, plainText);

    QString message = QString::fromUtf8((char*)plainText);


    return message;

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
/*
    printf("Encryption in Krypter Class\n");
    for(int i = 0; i < cipher_len; i++){
        printf("%d ", cipher[i]);
    }
    printf("\n");*/

    return cipher_len;

}

void Krypter::decrypt(unsigned char *cipher, int cipher_len, unsigned char *text)
{

    int len = 0;
    int text_len = 0;
/*
    printf("Decryption in Krypter Class\n");
    for(int i = 0; i < cipher_len; i++){
        printf("%d ", cipher[i]);
    }
    printf("\n");*/


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


void AppointmentEntity::setProperties(QString date, QString time, QString doctorID, QString title, QString text)
{
    this->date = date;
    this->time = time;
    this->doctorID = doctorID;
    this->title = title;
    this->text = text;
}

QString AppointmentEntity::getDate()
{
    return date;
}

QString AppointmentEntity::getTime()
{
    return time;
}

QString AppointmentEntity::getDoctorID()
{
    return doctorID;
}

QString AppointmentEntity::getTitle()
{
    return title;
}

QString AppointmentEntity::getText()
{
    return text;
}

void AppointmentEntity::setPropertiesAsEntity(QStringList list)
{
    date = list[0];
    time = list[1];
    doctorID = list[2];
    title = list[3];
    text = list[4];
}

QString AppointmentEntity::getPropertiesAsString()
{
    QString str;
    str = date + "," + time + "," + doctorID + "," + title + "," + text + ",";
    return str;

}

void AppointmentEntity::print()
{
    qDebug() << date << " " << time << " " << doctorID << " " << title << " " << text;
}


void DoctorEntity::setProperties(QString name, QString street, QString streetNumber, QString city, QString postalCode, QString phoneNumber)
{
    this->name = name;
    this->street = street;
    this->streetNumber = streetNumber;
    this->city = city;
    this->postalCode = postalCode;
    this->phoneNumber = phoneNumber;
}

QString DoctorEntity::getName()
{
    return name;
}

QString DoctorEntity::getStreet()
{
    return street;
}

QString DoctorEntity::getStreetNumber()
{
    return streetNumber;
}

QString DoctorEntity::getCity()
{
    return city;
}

QString DoctorEntity::getPostalCode()
{
    return postalCode;
}

QString DoctorEntity::getPhoneNumber()
{
    return phoneNumber;
}

void DoctorEntity::setPropertiesAsEntity(QStringList list)
{
    name = list[0];
    street = list[1];
    streetNumber = list[2];
    city = list[3];
    postalCode = list[4];
    phoneNumber = list[5];
}

QString DoctorEntity::getPropertiesAsString()
{
    QString str;
    str = name + "," + street + "," + streetNumber + "," + city + "," + postalCode + "," + phoneNumber + ",";
    return str;
}

void DoctorEntity::print()
{
    qDebug() << name << " " << street << " " << streetNumber << " " << city << " " << postalCode << " " << phoneNumber;
}

User::User()
{

}

User::~User()
{

}


void User::setProperties(QString uID, QString eMail, QString password)
{
    this->uID = uID;
    this->eMail = eMail;
    this->password = password;
}

void User::setPropertiesAsEntity(QStringList list)
{
    uID = list[0];
    eMail = list[1];
    password = list[2];
}

void User::print()
{
    qDebug() << uID << " " << eMail << " " << password;
}

QString User::getUID()
{
    return uID;
}

QString User::getEmail()
{
    return eMail;
}

QString User::getPassword()
{
    return password;
}

QString User::getPropertiesAsString()
{
    QString str;
    str = uID + "," + eMail + "," + password + ",";
    return str;
}

void User::setUID(QString UID)
{
    this->uID = UID;
}

void User::setPassword(QString password)
{
    this->password = password;
}
