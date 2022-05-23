/********************************************************************************
** Form generated from reading UI file 'appointment.ui'
**
** Created by: Qt User Interface Compiler version 6.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_APPOINTMENT_H
#define UI_APPOINTMENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Appointment
{
public:
    QDateEdit *date;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *doctorEdit;
    QLabel *label_3;
    QTextEdit *freeText;
    QPushButton *buttonSend;

    void setupUi(QWidget *Appointment)
    {
        if (Appointment->objectName().isEmpty())
            Appointment->setObjectName(QString::fromUtf8("Appointment"));
        Appointment->resize(400, 300);
        date = new QDateEdit(Appointment);
        date->setObjectName(QString::fromUtf8("date"));
        date->setGeometry(QRect(10, 50, 110, 22));
        label = new QLabel(Appointment);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 20, 58, 16));
        label_2 = new QLabel(Appointment);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 110, 58, 16));
        doctorEdit = new QLineEdit(Appointment);
        doctorEdit->setObjectName(QString::fromUtf8("doctorEdit"));
        doctorEdit->setGeometry(QRect(10, 140, 113, 21));
        label_3 = new QLabel(Appointment);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 180, 58, 16));
        freeText = new QTextEdit(Appointment);
        freeText->setObjectName(QString::fromUtf8("freeText"));
        freeText->setGeometry(QRect(10, 210, 104, 71));
        buttonSend = new QPushButton(Appointment);
        buttonSend->setObjectName(QString::fromUtf8("buttonSend"));
        buttonSend->setGeometry(QRect(210, 170, 100, 32));

        retranslateUi(Appointment);

        QMetaObject::connectSlotsByName(Appointment);
    } // setupUi

    void retranslateUi(QWidget *Appointment)
    {
        Appointment->setWindowTitle(QCoreApplication::translate("Appointment", "Form", nullptr));
        label->setText(QCoreApplication::translate("Appointment", "Datum", nullptr));
        label_2->setText(QCoreApplication::translate("Appointment", "Arzt", nullptr));
        label_3->setText(QCoreApplication::translate("Appointment", "Grund", nullptr));
        buttonSend->setText(QCoreApplication::translate("Appointment", "Speichern", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Appointment: public Ui_Appointment {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_APPOINTMENT_H
