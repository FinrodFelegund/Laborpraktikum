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
    void getAllDoctors(QByteArray header, QString message);
    void docInfo(int doc_id, QString name);
    void allDocsLoaded();

private:
    void showDocTable(QString doctors);

    Ui::DoctorOverview *ui;
    //std::vector<std::shared_ptr<DoctorEntityId>> docEntVector;
};

#endif // DOCTOROVERVIEW_H
