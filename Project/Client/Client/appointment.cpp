#include "appointment.h"
#include "ui_appointment.h"
#include "../../header.h"


Appointment::Appointment(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Appointment)
{
    ui->setupUi(this);


}

Appointment::~Appointment()
{
    delete ui;
}

void Appointment::createAppointmentMessage()
{

}

QStringList Appointment::getGuiInput()
{
    QStringList list;
    list.append(date);
    list.append(time);
    list.append(doctorName);
    list.append(title);
    list.append(text);

    return list;
}

void Appointment::setDocMap(std::vector<std::pair<int, QString>> doctorMap)
{
    qDebug() << "rechead setdocmap";
    doctorMapInAppointment.clear();
    doctorMapInAppointment = doctorMap;


    for(unsigned long i = 0; i <  doctorMapInAppointment.size(); i++)
    {

        ui->comboBox_2->addItem(doctorMapInAppointment[i].second);


    }


}

void Appointment::print()
{





}

void Appointment::resetPage()
{
    QDate today= QDate::currentDate();
    ui->date->setCurrentPage(today.year(),today.month());
    ui->appointmentTime->setTime(QTime::currentTime());
    //ui->docsBox->clear();
    ui->title->setText("");
    ui->notes->setText("");

}

void Appointment::on_buttonSend_clicked()
{
    date.clear();
    time.clear();
    title.clear();
    text.clear();


    date = ui->date->selectedDate().toString();
    time = ui->appointmentTime->time().toString();
    title = ui->title->text();
    text = ui->notes->toPlainText();
    doctorName = ui->comboBox_2->currentText();
    emit sendAppointmentEntity();



    resetPage();
}

