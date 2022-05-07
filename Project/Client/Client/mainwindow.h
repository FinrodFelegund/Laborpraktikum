#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QMessageBox>
#include <QAbstractSocket>
#include <QDebug>
#include "appointment.h"
#include "reportscreen.h"
#include "client.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:


    void on_reportButton_clicked();

    void on_appointmentButton_clicked();



private:
    Ui::MainWindow *ui;
    Client *client;
    Appointment *m_appointment;
    ReportScreen *m_reportScreen;
    std::vector<QWidget*> widgets;
};
#endif // MAINWINDOW_H
