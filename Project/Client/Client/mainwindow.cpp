#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    m_appointment = new Appointment();
    m_reportScreen = new ReportScreen();
    widgets.push_back(m_appointment);
    widgets.push_back(m_appointment);


    ui->stackedWidget->insertWidget(0, m_appointment);
    ui->stackedWidget->insertWidget(1, m_reportScreen);

    client = new Client();
    qDebug() << "client created";
    connect(m_appointment, &Appointment::messageCreated, client, &Client::sendMessage);
    //connect(m_reportScreen, &ReportScreen::messageCreated, client, &Client::sendMessage);


}

MainWindow::~MainWindow()
{

}



void MainWindow::on_appointmentButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(m_appointment);


}



void MainWindow::on_reportButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(m_reportScreen);
}




