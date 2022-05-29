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
    QDate tmpdate = ui->date->selectedDate();
    QString date = tmpdate.toString();
    QTime tmptime = ui->appointmentTime->time();
    QString time = tmptime.toString();

    int doctor = ui->docId->value();
    QString title = ui->title->text();
    QString notes = ui->notes->toPlainText();

    //create the header and message for the server
    QByteArray header;
    int messageType = MessageHeader::saveMessage;
    int messageEntity = MessageHeader::AppointmentEnt;

    header.prepend(QString::number(messageEntity).toUtf8() + ",");
    header.prepend(QString::number(messageType).toUtf8() + ",");




    //QString message = date + ";" + doctor + ";" + text + ";" + this->userID + ";";
    QString message = date + ";" + time + ";" + title + ";"+ notes + ";"+ QString::number(doctor) + ";";

    emit messageCreated(header, message);


}

void Appointment::resetPage()
{
    QDate today= QDate::currentDate();
    ui->date->setCurrentPage(today.year(),today.month());
    ui->appointmentTime->setTime(QTime::currentTime());
    ui->docId->setValue(0);
    ui->title->setText("");
    ui->notes->setText("");

}

void Appointment::on_buttonSend_clicked()
{
    createAppointmentMessage();
}

