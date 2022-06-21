#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QMessageBox>
#include <QAbstractSocket>
#include <QDebug>
#include "client.h"
#include "OpeningScreen/openingscreen.h"
#include "OpeningScreen/openingmodel.h"
#include "applicationmodel.h"
#include <QCloseEvent>
#include <chrono>
#include <thread>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showProgress(QString progress);
    void showMainWindows();
    void closeEvent(QCloseEvent *event);

private slots:
    void closeApplication();

    void on_appointmentButton_clicked();

    void on_docButton_clicked();

    void on_appTimelineButton_clicked();

    void on_allDocsButton_clicked();

    void on_logoutButton_clicked();

    void returnToLogin();

    void on_deleteProfileButton_clicked();



signals:
    void closeSignalToClient();
    void destroyed(QObject *obj = Q_NULLPTR);
    void logedIn();

private:
    Ui::MainWindow *ui;
    Client *client;
    Appointment *m_appointment;
    Doctors *m_doctors;
    OpeningModel *m_openingModel;
    ApplicationModel *m_applicationModel = nullptr;
    OpeningScreen * m_openingScreen;
    std::vector<QWidget*> widgets;
    AppointmentTimeline *m_appointment_timeline;
    DoctorOverview * m_doctor_overview;
};
#endif // MAINWINDOW_H
