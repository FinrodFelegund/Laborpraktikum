

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

void AppointmentTimeline::updatePage()
{
    emit getAllAppointments();
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

void AppointmentTimeline::setDocMap(std::vector<std::pair<int, QString>>doctorMap)
{
    this->doctorMap = doctorMap;
    emit getAllAppointments();
}

QString AppointmentTimeline::getNameFromDocMap(int id)
{
    for(unsigned long i = 0; i < doctorMap.size(); i++)
    {
        if(doctorMap[i].first == id)
            return doctorMap[i].second;
    }

    return "";
}

void AppointmentTimeline::showList()
{
    QStringList header;
    header << "Datum" << "Uhrzeit" << "Titel" << "Arzt" << "Notizen";
    ui->allAppointments->setHorizontalHeaderLabels(header);
    ui->allAppointments->setRowCount(appEntVector.size());

    int i = 0;
    for(const auto &currApp:appEntVector){
        QString name = getNameFromDocMap(currApp->getDoctorID().toInt());
        ui->allAppointments->setItem(i,0,new QTableWidgetItem(currApp->getDate()));
        ui->allAppointments->setItem(i,1,new QTableWidgetItem(currApp->getTime()));
        ui->allAppointments->setItem(i,2,new QTableWidgetItem(currApp->getTitle()));
        ui->allAppointments->setItem(i,3,new QTableWidgetItem(name));
        ui->allAppointments->setItem(i,4,new QTableWidgetItem(currApp->getText()));
        i++;
    }

    ui->allAppointments->setEditTriggers(QHeaderView::NoEditTriggers);

    ui->allAppointments->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->allAppointments->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Stretch);
}
