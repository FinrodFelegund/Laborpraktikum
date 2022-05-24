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
#include <QtWidgets/QCalendarWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QTimeEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Appointment
{
public:
    QGridLayout *gridLayout_2;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *buttonSend;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QLabel *label;
    QTimeEdit *appointmentTime;
    QLabel *label_5;
    QSpinBox *docId;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer_3;
    QTextEdit *notes;
    QLineEdit *title;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QCalendarWidget *date;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_4;

    void setupUi(QWidget *Appointment)
    {
        if (Appointment->objectName().isEmpty())
            Appointment->setObjectName(QString::fromUtf8("Appointment"));
        Appointment->resize(416, 447);
        gridLayout_2 = new QGridLayout(Appointment);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_4, 3, 0, 1, 1);

        buttonSend = new QPushButton(Appointment);
        buttonSend->setObjectName(QString::fromUtf8("buttonSend"));

        gridLayout_2->addWidget(buttonSend, 3, 1, 1, 2);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_2 = new QLabel(Appointment);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label = new QLabel(Appointment);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        appointmentTime = new QTimeEdit(Appointment);
        appointmentTime->setObjectName(QString::fromUtf8("appointmentTime"));

        gridLayout->addWidget(appointmentTime, 0, 1, 1, 1);

        label_5 = new QLabel(Appointment);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 2, 0, 1, 1);

        docId = new QSpinBox(Appointment);
        docId->setObjectName(QString::fromUtf8("docId"));

        gridLayout->addWidget(docId, 1, 1, 1, 1);

        label_3 = new QLabel(Appointment);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 3, 0, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 0, 2, 1, 1);

        notes = new QTextEdit(Appointment);
        notes->setObjectName(QString::fromUtf8("notes"));

        gridLayout->addWidget(notes, 3, 1, 1, 2);

        title = new QLineEdit(Appointment);
        title->setObjectName(QString::fromUtf8("title"));

        gridLayout->addWidget(title, 2, 1, 1, 2);


        gridLayout_2->addLayout(gridLayout, 2, 0, 1, 3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        date = new QCalendarWidget(Appointment);
        date->setObjectName(QString::fromUtf8("date"));

        horizontalLayout->addWidget(date);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        gridLayout_2->addLayout(horizontalLayout, 1, 0, 1, 3);

        label_4 = new QLabel(Appointment);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 0, 0, 1, 3);


        retranslateUi(Appointment);

        QMetaObject::connectSlotsByName(Appointment);
    } // setupUi

    void retranslateUi(QWidget *Appointment)
    {
        Appointment->setWindowTitle(QCoreApplication::translate("Appointment", "Form", nullptr));
        buttonSend->setText(QCoreApplication::translate("Appointment", "Speichern", nullptr));
        label_2->setText(QCoreApplication::translate("Appointment", "Arzt", nullptr));
        label->setText(QCoreApplication::translate("Appointment", "Uhrzeit", nullptr));
        label_5->setText(QCoreApplication::translate("Appointment", "Titel", nullptr));
        label_3->setText(QCoreApplication::translate("Appointment", "Notizen", nullptr));
        label_4->setText(QCoreApplication::translate("Appointment", "<html><head/><body><p align=\"center\"><span style=\" font-size:14pt; font-weight:700;\">Termin einstellen</span></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Appointment: public Ui_Appointment {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_APPOINTMENT_H
