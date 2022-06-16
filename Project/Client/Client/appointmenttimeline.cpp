#include "appointmenttimeline.h"
#include "ui_appointmenttimeline.h"
#include <QStringList>
#include "../../header.h"

AppointmentTimeline::AppointmentTimeline(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AppointmentTimeline)
{
    ui->setupUi(this);
}

AppointmentTimeline::~AppointmentTimeline()
{
    delete ui;
}

void AppointmentTimeline::setAppointmentVector(QString appointments)
{
    qDebug()<<"In App Timeline: set AppointmentVector";
    qDebug()<<"appointments" + appointments;

    appEntVector.clear();

    QStringList allAppointments = appointments.split(";");
    qDebug()<<"allAppointments";
    for(const auto &app:allAppointments){
        qDebug() << app;
    }

    for(const auto &currApp:allAppointments){
        if(currApp!=""){
        QStringList currAppList = currApp.split(",");

        std::shared_ptr<AppointmentEntity> appEnt=std::make_shared<AppointmentEntity>();
        appEnt->setPropertiesAsEntity(currAppList);
        appEntVector.push_back(appEnt);
        }
    }

    showList();
}

void AppointmentTimeline::reciveDocMap(std::map<int, QString> map)
{
    docMap=map;
    emit getAllAppointments();
}

void AppointmentTimeline::showList()
{
    QStringList header;
    header << "Datum" << "Uhrzeit" << "Titel" << "Arzt" << "Notizen";
    ui->allAppointments->setHorizontalHeaderLabels(header);
    ui->allAppointments->setRowCount(appEntVector.size());

    int i = 0;
    for(const auto &currApp:appEntVector){
        ui->allAppointments->setItem(i,0,new QTableWidgetItem(currApp->getDate()));
        ui->allAppointments->setItem(i,1,new QTableWidgetItem(currApp->getTime()));
        ui->allAppointments->setItem(i,2,new QTableWidgetItem(currApp->getTitle()));
        ui->allAppointments->setItem(i,3,new QTableWidgetItem(docMap[currApp->getDoctorID().toInt()]));
        ui->allAppointments->setItem(i,4,new QTableWidgetItem(currApp->getText()));
        i++;
    }

    ui->allAppointments->setEditTriggers(QHeaderView::NoEditTriggers);

    ui->allAppointments->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->allAppointments->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Stretch);
}
