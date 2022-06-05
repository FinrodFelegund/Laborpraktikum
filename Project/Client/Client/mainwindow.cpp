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
    m_appointment_timeline = new AppointmentTimeline();
    m_doctor_overview = new DoctorOverview();

    widgets.push_back(m_appointment);
    widgets.push_back(m_appointment_timeline);
    widgets.push_back(m_doctors);
    widgets.push_back(m_doctor_overview);

    m_openingModel->setGui(m_openingScreen);
    m_openingModel->connectGui();


    ui->stackedWidget->insertWidget(0, m_openingScreen);
    ui->stackedWidget->insertWidget(1, m_appointment);
    ui->stackedWidget->insertWidget(2, m_reportScreen);
    ui->stackedWidget->insertWidget(3,m_doctors);
    ui->stackedWidget->insertWidget(4,m_appointment_timeline);
    ui->stackedWidget->insertWidget(5,m_doctor_overview);

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
    connect(client,&Client::returnAppointments,m_appointment_timeline,&AppointmentTimeline::setAppointmentVector);
    connect(client,&Client::returnDoctors,m_doctor_overview,&DoctorOverview::setDoctorText);
    connect(m_doctor_overview, &DoctorOverview::getAllDoctors,client, &Client::sendMessage);
//    connect(m_openingModel, &OpeningModel::showMainWindows, this, [this](){
//        showMainWindows();

//        QByteArray header;
//        int messageType = MessageHeader::returnMessage;
//        int messageEntity = MessageHeader::AppointmentEnt;

//        header.prepend(QString::number(messageEntity).toUtf8() + ",");
//        header.prepend(QString::number(messageType).toUtf8() + ",");

//        client->sendMessage(header,"");
//    });

    ui->appointmentButton->hide();
    ui->reportButton->hide();
    ui->docButton->hide();
    ui->appTimelineButton->hide();
    ui->allDocsButton->hide();


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
    ui->reportButton->show();
    ui->docButton->show();
    ui->allDocsButton->show();
    ui->appTimelineButton->show();
    on_appTimelineButton_clicked();
}



void MainWindow::on_allDocsButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(m_doctor_overview);
}

