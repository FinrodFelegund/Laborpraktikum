#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QObject>
#include "../../header.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //setAttribute(Qt::WA_DeleteOnClose);
    m_appointment = new Appointment();
    m_doctors = new Doctors();
    m_openingScreen = new OpeningScreen();
    m_openingModel = new OpeningModel();
    m_appointment_timeline = new AppointmentTimeline();
    m_doctor_overview = new DoctorOverview();
    m_applicationModel = new ApplicationModel();

    widgets.push_back(m_appointment);
    widgets.push_back(m_appointment_timeline);
    widgets.push_back(m_doctors);
    widgets.push_back(m_doctor_overview);

    m_openingModel->setGui(m_openingScreen);
    m_openingModel->connectGui();

    m_applicationModel->setAppointment(m_appointment);
    m_applicationModel->setAppointment_timeline(m_appointment_timeline);
    m_applicationModel->setDoctors(m_doctors);
    m_applicationModel->setDoctor_overview(m_doctor_overview);

    m_applicationModel->connectGui();


    ui->stackedWidget->insertWidget(0, m_openingScreen);
    ui->stackedWidget->insertWidget(1, m_appointment);
    ui->stackedWidget->insertWidget(2,m_doctors);
    ui->stackedWidget->insertWidget(3,m_appointment_timeline);
    ui->stackedWidget->insertWidget(4,m_doctor_overview);

    ui->stackedWidget->setCurrentWidget(m_openingScreen);

    client = new Client();
    qDebug() << "client created";
    connect(m_applicationModel, &ApplicationModel::sendMessage, client, &Client::sendMessage);
    connect(m_openingModel, &OpeningModel::messageCreated, client, &Client::sendMessage);
    connect(client, &Client::pendingOpeningRequest, m_openingModel, &OpeningModel::receiveMessage);
    connect(m_openingModel, &OpeningModel::sendLoginProgress, this, &MainWindow::showProgress);
    connect(m_openingModel, &OpeningModel::currentUser, client, &Client::setCurrentUser);
    connect(m_openingModel, &OpeningModel::showMainWindows, this, &MainWindow::showMainWindows);
    connect(client,&Client::returnAppointments,m_applicationModel,&ApplicationModel::getReturnedAppointments);
    connect(client,&Client::returnDoctors,m_applicationModel,&ApplicationModel::getReturnedDoctors);
    connect(m_applicationModel, &ApplicationModel::sendMessage,client, &Client::sendMessage);
    connect(client, &Client::pendingLogoutRequest, m_applicationModel, &ApplicationModel::logoutRequest);
    connect(m_applicationModel, &ApplicationModel::sendApplicationProgress, this, &MainWindow::showProgress);
    connect(m_applicationModel, &ApplicationModel::logoutUser, this, &MainWindow::returnToLogin);
    connect(this, &MainWindow::closeSignalToClient, client, &Client::endApplication);
    connect(client, &Client::sendProgress, this, &MainWindow::showProgress);
    connect(client, &Client::closeApplication, this, &MainWindow::closeApplication);
    connect(m_openingModel, &OpeningModel::showMainWindows, this, &MainWindow::showMainWindows);

    ui->appointmentButton->hide();
    ui->docButton->hide();
    ui->appTimelineButton->hide();
    ui->allDocsButton->hide();
    ui->logoutButton->hide();


}

MainWindow::~MainWindow()
{

}

void MainWindow::closeApplication()
{
    QApplication::quit();
}

void MainWindow::on_appointmentButton_clicked()
{
    m_appointment->resetPage();
    ui->stackedWidget->setCurrentWidget(m_appointment);
}

void MainWindow::on_docButton_clicked()
{
    m_doctors->resetPage();
    ui->stackedWidget->setCurrentWidget(m_doctors);
}

void MainWindow::on_appTimelineButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(m_appointment_timeline);
}

void MainWindow::showProgress(QString progress)
{
    ui->statusBar->showMessage(progress);
}

void MainWindow::showMainWindows()
{
    ui->appointmentButton->show();
    ui->docButton->show();
    ui->allDocsButton->show();
    ui->appTimelineButton->show();
    ui->logoutButton->show();

    m_doctor_overview->updatePage();
    on_appTimelineButton_clicked();
}



void MainWindow::on_allDocsButton_clicked()
{
    m_doctor_overview->updatePage();
    ui->stackedWidget->setCurrentWidget(m_doctor_overview);
}


void MainWindow::on_logoutButton_clicked()
{
    client->sendLogoutRequest();
}

void MainWindow::returnToLogin()
{
    ui->appointmentButton->hide();
    ui->docButton->hide();
    ui->appTimelineButton->hide();
    ui->allDocsButton->hide();
    ui->logoutButton->hide();

    ui->stackedWidget->setCurrentWidget(m_openingScreen);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    emit closeSignalToClient();

    event->accept();
}

