#ifndef DOCTOROVERVIEW_H
#define DOCTOROVERVIEW_H

#include <QWidget>
//#include "../../header.h"

namespace Ui {
class DoctorOverview;
}

class DoctorOverview : public QWidget
{
    Q_OBJECT

public:
    explicit DoctorOverview(QWidget *parent = nullptr);
    ~DoctorOverview();


public slots:
    void setDoctorText(QString doctors);
    void updatePage();
signals:
    void getAllDoctors();
    void docInfo(int doc_id, QString name);
    void allDocsLoaded(std::vector<std::pair<int, QString>>doctorMap);
    void refreshClicked();


private slots:


private:
    void showDocTable(QString doctors);
    std::vector<std::pair<int, QString>>doctorMap;

    Ui::DoctorOverview *ui;
    //std::vector<std::shared_ptr<DoctorEntityId>> docEntVector;
};

#endif // DOCTOROVERVIEW_H
