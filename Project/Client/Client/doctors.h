#ifndef DOCTORS_H
#define DOCTORS_H

#include <QWidget>


namespace Ui {
class Doctors;
}

class Doctors : public QWidget
{
    Q_OBJECT

public:
    explicit Doctors(QWidget *parent = nullptr);
    ~Doctors();
    void resetPage();
    QStringList getGuiInput();

signals:
    void sendDoctorEntity();

private slots:
    void on_saveButton_clicked();

public:


private:

    QString name;
    QString street;
    QString streetNumber;
    QString city;
    QString postalCode;
    QString phoneNumber;

    Ui::Doctors *ui;
};

#endif // DOCTORS_H
