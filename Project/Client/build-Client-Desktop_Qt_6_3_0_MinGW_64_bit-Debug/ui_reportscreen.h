/********************************************************************************
** Form generated from reading UI file 'reportscreen.ui'
**
** Created by: Qt User Interface Compiler version 6.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REPORTSCREEN_H
#define UI_REPORTSCREEN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ReportScreen
{
public:

    void setupUi(QWidget *ReportScreen)
    {
        if (ReportScreen->objectName().isEmpty())
            ReportScreen->setObjectName(QString::fromUtf8("ReportScreen"));
        ReportScreen->resize(400, 300);

        retranslateUi(ReportScreen);

        QMetaObject::connectSlotsByName(ReportScreen);
    } // setupUi

    void retranslateUi(QWidget *ReportScreen)
    {
        ReportScreen->setWindowTitle(QCoreApplication::translate("ReportScreen", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ReportScreen: public Ui_ReportScreen {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REPORTSCREEN_H
