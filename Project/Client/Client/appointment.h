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
    QStringList getGuiInput();
    void setDocMap(std::vector<std::pair<int, QString>>doctorMap);
    void print();

signals:
    void sendAppointmentEntity();

private slots:
    void on_buttonSend_clicked();

    void on_notes_textChanged();

private:
    Ui::Appointment *ui;
    QString date;
    QString time;
    QString doctorName;
    QString title;
    QString text;
    int docId;
    std::vector<std::pair<int, QString>> doctorMapInAppointment;
    int maxTextLength = 200;
};

#endif // APPOINTMENT_H
