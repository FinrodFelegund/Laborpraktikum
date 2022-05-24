#include "doctors.h"
#include "ui_doctors.h"
#include "../../header.h"

Doctors::Doctors(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Doctors)
{
    ui->setupUi(this);
}

Doctors::~Doctors()
{
    delete ui;
}

void Doctors::createDoctorMessage()
{

    //get Infos
    QString name = ui->drName->text();
    QString street = ui->street->text();
    int housenumber = ui->housenumber->value();
    QString city = ui->city->text();
    int phonenumber = ui->phone->text().toInt();

    //create the header and message for the server
    QByteArray header;
    int messageType = MessageHeader::saveMessage;
    int messageEntity = MessageHeader::DoctorEnt;

    header.prepend(QString::number(messageEntity).toUtf8() + ",");
    header.prepend(QString::number(messageType).toUtf8() + ",");

    QString message = name + ";" + street + ";" + QString::number(housenumber)
            + ";" + city + ";" + QString::number(phonenumber)+ ";" ;

    QByteArray messageToSend = message.toUtf8();
    messageToSend.prepend(header);

    emit messageCreated(header, message);
}

void Doctors::on_saveButton_clicked()
{
    createDoctorMessage();
}

