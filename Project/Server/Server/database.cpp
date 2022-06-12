#include "database.h"

Database::Database()
{
    db =QSqlDatabase::addDatabase("QMYSQL","demoapp");
        db.setHostName("127.0.0.1");
        db.setDatabaseName("demoapp");
        db.setUserName("LPRApp");
        db.setPassword("DaCiHa22");
        if(!db.open()){
            qDebug() << db.lastError().text();
        } else{
            qDebug() <<"db loaded successfully!";
        }

}

bool Database::saveDoctorInDb(DoctorEntity ent, QString user_id)
{
    if(user_id.toInt() > -1){
        if(db.open()){

            QSqlQuery queryInsert(db);
                        queryInsert.prepare("insert into doctors(doctorname, street, streetnumber, city, plz, phone, uid) VALUES(?,?,?,?,?,?,?)");
                        queryInsert.bindValue(0,ent.getName());
                        queryInsert.bindValue(1,ent.getStreet());
                        if(ent.getStreet()!=""){
                            queryInsert.bindValue(2,ent.getStreetNumber());
                        }
                        else {
                            queryInsert.bindValue(2,"");
                        }

                        queryInsert.bindValue(3,ent.getCity());
                        queryInsert.bindValue(4,ent.getPostalCode());
                        queryInsert.bindValue(5,ent.getPhoneNumber());
                        queryInsert.bindValue(6,user_id);

                        bool executed = queryInsert.exec();
                        qDebug()<<executed;
                        QSqlError error= queryInsert.lastError();
                        std::cout<<error.databaseText().toUtf8().constData();
                        return executed;
        }

    }
    else{
        qDebug() << "User is not logged in";
    }


    return false;
}

bool Database::saveAppointmentInDb(AppointmentEntity ent, QString user_id)
{
    if(user_id.toInt() > -1){
        if(db.open()){

            QSqlQuery queryInsert(db);
                        queryInsert.prepare("insert into appointments(appdate,apptime,title,notes,did,uid) values(?,?,?,?,?,?)");
                        queryInsert.bindValue(0,QDate::fromString(ent.getDate()));
                        queryInsert.bindValue(1,QTime::fromString(ent.getTime()));
                        queryInsert.bindValue(2,ent.getTitle());
                        queryInsert.bindValue(3,ent.getText());
                        queryInsert.bindValue(4,ent.getDoctorID());
                        queryInsert.bindValue(5,user_id);

                        bool executed = queryInsert.exec();
                        qDebug()<<executed;
                        QSqlError error= queryInsert.lastError();
                        std::cout<<error.databaseText().toUtf8().constData();
                        return executed;
        }

    }
    else{
        qDebug() << "User is not logged in";
    }

    return false;
}

bool Database::saveUserInDb(User user)
{
    bool executed = false;
    int logedIn = 0;
    if(db.open())
    {
        QSqlQuery queryInsert(db);
        queryInsert.prepare("insert into Users(email, passwort, loginState) values(md5(?), md5(?), ?)");
        queryInsert.bindValue(0, user.getEmail());
        queryInsert.bindValue(1,  user.getPassword());
        queryInsert.bindValue(2, logedIn);

        executed = queryInsert.exec();
        //qDebug() << "In Save User In db: " << executed;
        QSqlError error = queryInsert.lastError();
        qDebug() << error;
        db.commit();
        return executed;
    }

    db.commit();
    return executed;
}

int Database::findUserInDb(User user)
{
    if(db.open())
    {
        QSqlQuery queryFind(db);
        queryFind.prepare("select uid, loginState from Users where email = md5('" +user.getEmail()+ "')"); // +password

        qDebug() << queryFind.exec();
        QSqlError error = queryFind.lastError();
        if(error.isValid())
            qDebug() << "In Find User Query: " << error.databaseText().toUtf8().constData();

        int userID = 0;
        int loginState = 0;
        queryFind.next();

        if(queryFind.isValid())
        {

            userID = queryFind.value(0).toInt();
            loginState = queryFind.value(1).toInt();
            qDebug() << "valid" << " " << userID << " " << loginState;
            return (loginState == 1 ? -1 : userID);

        } else
        {
            qDebug() << "invalid";
            return 0;
        }


    }

    return 0;
}

QString Database::getPasswordFromUser(User user)
{
    QString password = "";
    if(db.open())
    {
        QSqlQuery queryFind(db);
        queryFind.prepare("select passwort from Users where email = '"+user.getEmail()+"' ");
        qDebug() << "Inf function getPasswordFromUser: " << queryFind.exec();

        QSqlError error = queryFind.lastError();
        qDebug() << error;

        queryFind.next();

        if(queryFind.isValid())
        {
            password = queryFind.value(0).toString();
        }
    }
    return password;
}

bool Database::setLoginStateInDb(QString user_id, bool loginState)
{

    if(db.open())
    {
        QSqlQuery queryAlter(db);
        queryAlter.prepare("update Users set loginState = '"+QString::number(loginState)+"' where uid = '"+user_id+"'");
        //qDebug() << "In Function SetLoginState in DB: " <<
        queryAlter.exec();
        QSqlError error = queryAlter.lastError();
        if(error.text().isEmpty())
        {
            qDebug() << error;
            return true;
        }

    }

    return false;
}

std::vector<std::shared_ptr<Entity> > Database::selectAppointmentsFromDatabase(QString user_id)
{
    qDebug()<<"Select all Appointments for User: " << user_id;
    bool executed=false;
    std::vector<std::shared_ptr<Entity>> appEntVector;

    if(user_id.toInt() > -1){
        if(db.open()){
            QSqlQuery querySelect(db);
            querySelect.prepare("SELECT appdate, apptime, title, notes, did FROM appointments WHERE uid= ?");
            querySelect.bindValue(0,user_id.toInt());
            executed = querySelect.exec();
            qDebug()<< executed;
            QSqlError error= querySelect.lastError();
            std::cout<<error.databaseText().toUtf8().constData();

            QDate date;
            QTime time;
            QString doctorID;
            QString title;
            QString text;

            if(executed){
                while (querySelect.next()) {
                        date = querySelect.value(0).toDate();
                        qDebug()<<"date: " << date.toString();
                        time = querySelect.value(1).toTime();
                        title = querySelect.value(2).toString();
                        text = querySelect.value(3).toString();
                        doctorID = querySelect.value(4).toString();

                        std::shared_ptr<AppointmentEntity> appEnt=std::make_shared<AppointmentEntity>();
                        appEnt->setProperties(date.toString(),time.toString(),doctorID,title,text);
                        appEntVector.push_back(appEnt);
                        qDebug()<<"Neuer Termin: " + appEnt->getPropertiesAsString();
                    }
            }

        }
    }
    return appEntVector;
}

std::vector<std::shared_ptr<Entity> > Database::selectDoctorsFromDatabase(QString user_id)
{
    qDebug()<<"Select all Doctors for User: " << user_id;
    bool executed=false;
    std::vector<std::shared_ptr<Entity>> docEntVector;

    if(user_id.toInt() > -1){
        if(db.open()){
            QSqlQuery querySelect(db);
            querySelect.prepare("select did,doctorname,street,streetnumber,city,plz,phone from doctors where uid= ?");
            querySelect.bindValue(0,user_id.toInt());
            executed = querySelect.exec();
            qDebug()<< executed;
            QSqlError error= querySelect.lastError();
            std::cout<<error.databaseText().toUtf8().constData();

            QString doctor_id;
            QString name;
            QString street;
            QString streetNumber;
            QString city;
            QString postalCode;
            QString phoneNumber;

            if(executed){
                while (querySelect.next()) {
                        doctor_id = querySelect.value(0).toString();
                        name = querySelect.value(1).toString();
                        street = querySelect.value(2).toString();
                        streetNumber = querySelect.value(3).toString();
                        city = querySelect.value(4).toString();
                        postalCode = querySelect.value(5).toString();
                        phoneNumber = querySelect.value(6).toString();


                        std::shared_ptr<DoctorEntityId> docEnt=std::make_shared<DoctorEntityId>();
                        docEnt->setProperties(doctor_id,name,street,streetNumber,city,postalCode,phoneNumber);
                        docEntVector.push_back(docEnt);
                        qDebug()<<"Neuer Arzt: " + docEnt->getPropertiesAsString();
                    }
            }

        }
    }
    return docEntVector;
}

bool Database::selectDateTest()
{
    bool executed=false;

        if(db.open()){
            QSqlQuery querySelect(db);
            querySelect.prepare("select appdate from appointments where uid = 1");
            executed = querySelect.exec();
            qDebug()<< executed;
            QSqlError error= querySelect.lastError();
            std::cout<<error.databaseText().toUtf8().constData();

            //QString date;
            QDate date;

            if(executed){
                while (querySelect.next()) {
                        date = querySelect.value(0).toDate();
                        qDebug()<<"Neuer Termin Datum: " + date.toString();
                    }
            }

        }

    return executed;
}
