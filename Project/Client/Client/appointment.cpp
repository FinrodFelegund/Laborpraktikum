#include "appointment.h"
#include "ui_appointment.h"
#include "../../header.h"


Appointment::Appointment(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Appointment)
{
    ui->setupUi(this);

    ui->title->maxLength();
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

    AppointmentEntity appointment;
    appointment.setProperties(date,time,QString::number(doctor),title,notes);

    //create the header and message for the server
    QByteArray header;
    int messageType = MessageHeader::saveMessage;
    int messageEntity = MessageHeader::AppointmentEnt;

    header.prepend(QString::number(messageEntity).toUtf8() + ",");
    header.prepend(QString::number(messageType).toUtf8() + ",");

    emit messageCreated(header, appointment.getPropertiesAsString());


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

