/********************************************************************************
** Form generated from reading UI file 'doctors.ui'
**
** Created by: Qt User Interface Compiler version 6.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DOCTORS_H
#define UI_DOCTORS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Doctors
{
public:
    QGridLayout *gridLayout;
    QLineEdit *city;
    QLabel *label_4;
    QLabel *label_2;
    QLabel *label_6;
    QLabel *label_5;
    QPushButton *saveButton;
    QLabel *label;
    QLineEdit *drName;
    QLineEdit *phone;
    QSpinBox *housenumber;
    QLabel *label_3;
    QLineEdit *street;
    QLineEdit *plz;
    QLabel *label_7;

    void setupUi(QWidget *Doctors)
    {
        if (Doctors->objectName().isEmpty())
            Doctors->setObjectName(QString::fromUtf8("Doctors"));
        Doctors->resize(448, 331);
        gridLayout = new QGridLayout(Doctors);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        city = new QLineEdit(Doctors);
        city->setObjectName(QString::fromUtf8("city"));

        gridLayout->addWidget(city, 3, 1, 1, 1);

        label_4 = new QLabel(Doctors);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 2, 2, 1, 1);

        label_2 = new QLabel(Doctors);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        label_6 = new QLabel(Doctors);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setMaximumSize(QSize(16777215, 22));

        gridLayout->addWidget(label_6, 0, 0, 1, 4);

        label_5 = new QLabel(Doctors);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 5, 0, 1, 1);

        saveButton = new QPushButton(Doctors);
        saveButton->setObjectName(QString::fromUtf8("saveButton"));

        gridLayout->addWidget(saveButton, 6, 2, 1, 1);

        label = new QLabel(Doctors);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 1, 0, 1, 1);

        drName = new QLineEdit(Doctors);
        drName->setObjectName(QString::fromUtf8("drName"));

        gridLayout->addWidget(drName, 1, 1, 1, 1);

        phone = new QLineEdit(Doctors);
        phone->setObjectName(QString::fromUtf8("phone"));

        gridLayout->addWidget(phone, 5, 1, 1, 1);

        housenumber = new QSpinBox(Doctors);
        housenumber->setObjectName(QString::fromUtf8("housenumber"));
        housenumber->setMaximum(10000);

        gridLayout->addWidget(housenumber, 2, 3, 1, 1);

        label_3 = new QLabel(Doctors);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 3, 0, 1, 1);

        street = new QLineEdit(Doctors);
        street->setObjectName(QString::fromUtf8("street"));

        gridLayout->addWidget(street, 2, 1, 1, 1);

        plz = new QLineEdit(Doctors);
        plz->setObjectName(QString::fromUtf8("plz"));

        gridLayout->addWidget(plz, 4, 1, 1, 1);

        label_7 = new QLabel(Doctors);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 4, 0, 1, 1);


        retranslateUi(Doctors);

        QMetaObject::connectSlotsByName(Doctors);
    } // setupUi

    void retranslateUi(QWidget *Doctors)
    {
        Doctors->setWindowTitle(QCoreApplication::translate("Doctors", "Form", nullptr));
        label_4->setText(QCoreApplication::translate("Doctors", "Hausnummer", nullptr));
        label_2->setText(QCoreApplication::translate("Doctors", "Stra\303\237e", nullptr));
        label_6->setText(QCoreApplication::translate("Doctors", "<html><head/><body><p align=\"center\"><span style=\" font-size:12pt; font-weight:700;\">Arzt anlegen</span></p></body></html>", nullptr));
        label_5->setText(QCoreApplication::translate("Doctors", "Telefonnummer", nullptr));
        saveButton->setText(QCoreApplication::translate("Doctors", "Speichern", nullptr));
        label->setText(QCoreApplication::translate("Doctors", "Name", nullptr));
        label_3->setText(QCoreApplication::translate("Doctors", "Stadt", nullptr));
        label_7->setText(QCoreApplication::translate("Doctors", "Postleitzahl", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Doctors: public Ui_Doctors {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DOCTORS_H
