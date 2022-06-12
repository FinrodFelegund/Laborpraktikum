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
#include "doctors.h"
#include "OpeningScreen/openingscreen.h"
#include "OpeningScreen/openingmodel.h"
#include "appointmenttimeline.h"
#include "doctoroverview.h"
#include "applicationmodel.h"

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

private slots:

    void on_reportButton_clicked();

    void on_appointmentButton_clicked();

    void on_docButton_clicked();

    void on_appTimelineButton_clicked();

    void on_allDocsButton_clicked();

    void on_logoutButton_clicked();

    void returnToLogin();

private:
    Ui::MainWindow *ui;
    Client *client;
    Appointment *m_appointment;
    ReportScreen *m_reportScreen;
    Doctors *m_doctors;
    OpeningModel *m_openingModel;
    ApplicationModel *m_applicationModel;
    OpeningScreen * m_openingScreen;
    std::vector<QWidget*> widgets;
    AppointmentTimeline *m_appointment_timeline;
    DoctorOverview * m_doctor_overview;
};
#endif // MAINWINDOW_H
