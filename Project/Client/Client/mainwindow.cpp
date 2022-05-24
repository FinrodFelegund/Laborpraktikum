#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    m_appointment = new Appointment();
    m_reportScreen = new ReportScreen();
    m_doctors = new Doctors();
    widgets.push_back(m_appointment);
    widgets.push_back(m_appointment);
    widgets.push_back(m_doctors);


    ui->stackedWidget->insertWidget(0, m_appointment);
    ui->stackedWidget->insertWidget(1, m_reportScreen);
    ui->stackedWidget->insertWidget(2,m_doctors);

    client = new Client();
    qDebug() << "client created";
    connect(m_appointment, &Appointment::messageCreated, client, &Client::sendMessage);
    //connect(m_reportScreen, &ReportScreen::messageCreated, client, &Client::sendMessage);
    connect(m_doctors, &Doctors::messageCreated, client, &Client::sendMessage);


}

MainWindow::~MainWindow()
{

}

void MainWindow::on_appointmentButton_clicked()
{
    m_appointment->resetPage();
    ui->stackedWidget->setCurrentWidget(m_appointment);
}

void MainWindow::on_reportButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(m_reportScreen);
}

void MainWindow::on_docButton_clicked()
{
    m_doctors->resetPage();
    ui->stackedWidget->setCurrentWidget(m_doctors);
}

