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
    ui->docOverview->setText(doctors);
}

void DoctorOverview::on_loadDocsButton_clicked()
{
    QByteArray header;
    int messageType = MessageHeader::returnMessage;
    int messageEntity = MessageHeader::DoctorEnt;

    header.prepend(QString::number(messageEntity).toUtf8() + ",");
    header.prepend(QString::number(messageType).toUtf8() + ",");

    emit getAllDoctors(header,"");
}

