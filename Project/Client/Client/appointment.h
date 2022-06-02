#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include <QWidget>
#include <iostream>

namespace Ui {
class Appointment;
}

class Appointment : public QWidget
{
    Q_OBJECT

public:
    explicit Appointment(QWidget *parent = nullptr);
    ~Appointment();
    void createAppointmentMessage();
    void resetPage();

signals:
    void messageCreated(QByteArray header, QString message);

private slots:
    void on_buttonSend_clicked();

private:
    Ui::Appointment *ui;
};

#endif // APPOINTMENT_H
