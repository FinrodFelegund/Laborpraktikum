#ifndef APPLICATIONMODEL_H
#define APPLICATIONMODEL_H

#include <QObject>

class ApplicationModel : public QObject
{
    Q_OBJECT
public:
    explicit ApplicationModel(QObject *parent = nullptr);

signals:
    void sendApplicationProgress(QString progress);
    void logoutUser();

public slots:
    void receiveMessage(QString buffer, int messageType);


};

#endif // APPLICATIONMODEL_H
