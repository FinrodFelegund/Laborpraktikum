#include "appointment.h"
#include "ui_appointment.h"
//#include "../../header.h"


Appointment::Appointment(QWidget *parent, QString userID) :
    QWidget(parent),
    ui(new Ui::Appointment)
{
    ui->setupUi(this);

    this->userID = userID;


}

Appointment::~Appointment()
{
    delete ui;
}

void Appointment::createAppointmentMessage()
{

    //get the infos from the ui;
    QDate tmp = ui->date->date();
    QString date = tmp.toString();

    QString doctor = ui->doctorEdit->text();

    QString text = ui->freeText->toPlainText();

    //create the header and message for the server
    QByteArray header;
    int messageType = MessageHeader::saveMessage;
    int messageEntity = MessageHeader::AppointmentEnt;

    header.prepend(QString::number(messageEntity).toUtf8() + ",");
    header.prepend(QString::number(messageType).toUtf8() + ",");




    //QString message = date + ";" + doctor + ";" + text + ";" + this->userID + ";";
    QString message = date + ";" + doctor + ";" + text + ";";
    QByteArray messageToSend = message.toUtf8();
    messageToSend.prepend(header);

    emit messageCreated(header, message);


}

void Appointment::on_buttonSend_clicked()
{
    createAppointmentMessage();
}

