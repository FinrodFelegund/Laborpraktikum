#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include <QWidget>
#include <iostream>
#include "../../header.h"


namespace Ui {
class Appointment;
}

class Appointment : public QWidget
{
    Q_OBJECT

public:
    explicit Appointment(QWidget *parent = nullptr, QString userID = "1234");
    ~Appointment();
    void createAppointmentMessage();
    void resetPage();

signals:
    void messageCreated(QByteArray header, QString message);

private slots:
    void on_buttonSend_clicked();

private:
    Ui::Appointment *ui;
    QString userID = "1234";


};

#endif // APPOINTMENT_H
