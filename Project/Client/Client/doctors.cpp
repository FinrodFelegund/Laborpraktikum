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

void Doctors::resetPage()
{
    ui->drName->setText("");
    ui->street->setText("");
    ui->housenumber->setValue(0);
    ui->city->setText("");
    ui->plz->setText("");
    ui->phone->setText("");
}

void Doctors::createDoctorMessage()
{

    //get Infos
    QString name = ui->drName->text();
    QString street = ui->street->text();
    QString housenumber = QString::number(ui->housenumber->value());
    QString city = ui->city->text();
    QString plz = ui->plz->text();
    QString phonenumber = ui->phone->text();

    //create the header and message for the server
    QByteArray header;
    int messageType = MessageHeader::saveMessage;
    int messageEntity = MessageHeader::DoctorEnt;

    header.prepend(QString::number(messageEntity).toUtf8() + ",");
    header.prepend(QString::number(messageType).toUtf8() + ",");

    QString message = name + "," + street + "," + housenumber
            + "," + city + "," + plz + "," + phonenumber + "," + ";";

    QByteArray messageToSend = message.toUtf8();
    messageToSend.prepend(header);

    emit messageCreated(header, message);
}

void Doctors::on_saveButton_clicked()
{
    createDoctorMessage();
}

