#include "reportscreen.h"
#include "ui_reportscreen.h"
#include "../../header.h"

ReportScreen::ReportScreen(QWidget *parent, QString userID) :
    QWidget(parent),
    ui(new Ui::ReportScreen)
{
    ui->setupUi(this);

    this->userID = userID;
}

ReportScreen::~ReportScreen()
{
    delete ui;
}
