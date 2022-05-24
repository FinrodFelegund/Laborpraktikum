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

signals:
    void messageCreated(QByteArray header, QString message);

private slots:
    void on_saveButton_clicked();

private:
    void createDoctorMessage();

    Ui::Doctors *ui;
};

#endif // DOCTORS_H
