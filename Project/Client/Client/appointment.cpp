#include "appointment.h"
#include "ui_appointment.h"
#include "../../header.h"


Appointment::Appointment(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Appointment)
{
    ui->setupUi(this);
    ui->title->setMaxLength(50);

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
    ui->comboBox_2->clear();

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
    //QDate today= QDate::currentDate();
    //ui->date->setCurrentPage(today.year(),today.month());
    //ui->appointmentTime->setTime(QTime::currentTime());
    //ui->docsBox->clear();
    ui->title->clear();
    ui->notes->clear();

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


void Appointment::on_notes_textChanged()
{

    QString text = ui->notes->document()->toPlainText();


    int overLap = 0;
    if(text.size() > maxTextLength)
    {
        overLap =  text.size() - maxTextLength;

        text.chop(overLap);

        ui->notes->document()->setPlainText(text);

        QTextCursor cursor(ui->notes->textCursor());
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
        ui->notes->setTextCursor(cursor);
    }


}

