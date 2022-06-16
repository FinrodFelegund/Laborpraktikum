#include "doctoroverview.h"
#include "ui_doctoroverview.h"
#include "../../header.h"

DoctorOverview::DoctorOverview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DoctorOverview)
{
    ui->setupUi(this);
}

DoctorOverview::~DoctorOverview()
{
    delete ui;
}

void DoctorOverview::setDoctorText(QString doctors)
{
    qDebug()<<"In Doc Overview: setDoctorText";
    qDebug()<<"doctors: " + doctors;

    showDocTable(doctors);
}

void DoctorOverview::updatePage()
{
    QByteArray header;
    int messageType = MessageHeader::returnMessage;
    int messageEntity = MessageHeader::DoctorEnt;

    header.prepend(QString::number(messageEntity).toUtf8() + ",");
    header.prepend(QString::number(messageType).toUtf8() + ",");

    emit getAllDoctors(header,"");
}

void DoctorOverview::showDocTable(QString doctors)
{

    std::vector<std::shared_ptr<DoctorEntityId>> docEntVector;

    QStringList allDoctors = doctors.split(";");

    for(const auto &currDoc:allDoctors){
        if(currDoc!=""){
        QStringList currDocList = currDoc.split(",");

        std::shared_ptr<DoctorEntityId> docEnt=std::make_shared<DoctorEntityId>();
        docEnt->setPropertiesAsEntity(currDocList);
        docEntVector.push_back(docEnt);
        }
    }

    QStringList header;
    header << "Name" << "Straße" << "Hausnummer" << "Postleitzahl" << "Stadt"<<"Telefonnummer";
    ui->docTable->setHorizontalHeaderLabels(header);
    ui->docTable->setRowCount(docEntVector.size());

    int i = 0;

    for(const auto &currDoc:docEntVector){
        ui->docTable->setItem(i,0,new QTableWidgetItem(currDoc->getName()));
        ui->docTable->setItem(i,1,new QTableWidgetItem(currDoc->getStreet()));
        ui->docTable->setItem(i,2,new QTableWidgetItem(currDoc->getStreetNumber()));
        ui->docTable->setItem(i,3,new QTableWidgetItem(currDoc->getPostalCode()));
        ui->docTable->setItem(i,4,new QTableWidgetItem(currDoc->getCity()));
        ui->docTable->setItem(i,5,new QTableWidgetItem(currDoc->getPhoneNumber()));
        emit docInfo(currDoc->getId().toInt(),currDoc->getName());
        i++;
    }

    ui->docTable->setEditTriggers(QHeaderView::NoEditTriggers);

    ui->docTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->docTable->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);

    emit allDocsLoaded();
}

