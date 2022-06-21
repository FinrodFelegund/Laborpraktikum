#include "doctors.h"
#include "ui_doctors.h"
#include "../../header.h"

Doctors::Doctors(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Doctors)
{
    ui->setupUi(this);
    ui->drName->setMaxLength(100);
    ui->street->setMaxLength(50);
    ui->housenumber->setMaximum(99999);
    ui->city->setMaxLength(30);
    ui->plz->setMaxLength(5);
    ui->phone->setMaxLength(20);
}

Doctors::~Doctors()
{
    delete ui;
}

void Doctors::resetPage()
{
    ui->drName->setText("");
    ui->street->setText("");
    ui->housenumber->setValue(0);
    ui->city->setText("");
    ui->plz->setText("");
    ui->phone->setText("");
}



void Doctors::on_saveButton_clicked()
{
    name.clear();
    street.clear();
    streetNumber.clear();
    city.clear();
    postalCode.clear();
    phoneNumber.clear();

    name = ui->drName->text();
    street = ui->street->text();
    streetNumber = QString::number(ui->housenumber->value());
    city = ui->city->text();
    postalCode = ui->plz->text();
    phoneNumber = ui->phone->text();

    emit sendDoctorEntity();
    resetPage();
}

QStringList Doctors::getGuiInput()
{
    QStringList list;
    list.append(name);
    list.append(street);
    list.append(streetNumber);
    list.append(city);
    list.append(postalCode);
    list.append(phoneNumber);

    return list;
}
