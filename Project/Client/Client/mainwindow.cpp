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
    m_openingScreen = new OpeningScreen();
    m_openingModel = new OpeningModel();
    widgets.push_back(m_appointment);
    widgets.push_back(m_appointment);
    widgets.push_back(m_doctors);

    m_openingModel->setGui(m_openingScreen);
    m_openingModel->connectGui();


    ui->stackedWidget->insertWidget(0, m_openingScreen);
    ui->stackedWidget->insertWidget(1, m_appointment);
    ui->stackedWidget->insertWidget(2, m_reportScreen);
    ui->stackedWidget->insertWidget(3,m_doctors);

    ui->stackedWidget->setCurrentWidget(m_openingScreen);

    client = new Client();
    qDebug() << "client created";
    connect(m_appointment, &Appointment::messageCreated, client, &Client::sendMessage);
    //connect(m_reportScreen, &ReportScreen::messageCreated, client, &Client::sendMessage);
    connect(m_doctors, &Doctors::messageCreated, client, &Client::sendMessage);
    connect(m_openingModel, &OpeningModel::messageCreated, client, &Client::sendMessage);
    connect(client, &Client::pendingOpeningRequest, m_openingModel, &OpeningModel::receiveMessage);
    connect(m_openingModel, &OpeningModel::sendLoginProgress, this, &MainWindow::showProgress);
    connect(m_openingModel, &OpeningModel::currentUser, client, &Client::setCurrentUser);
    connect(m_openingModel, &OpeningModel::showMainWindows, this, &MainWindow::showMainWindows);

    ui->appointmentButton->hide();
    ui->reportButton->hide();
    ui->docButton->hide();


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

void MainWindow::showProgress(QString progress)
{
    ui->statusBar->showMessage(progress);
}

void MainWindow::showMainWindows()
{
    on_appointmentButton_clicked();
}

