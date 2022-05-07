#include <string>
#include <QString>


// a header consists of 2 or 1 of these types 
enum MessageHeader {

    //message Types
    saveMessage,

    returnMessage,

    returnMessageArray,

    //entity Types e.g. Appointment etc;  to be completed

    Appointment


    


};



class AppointmentEntity{
    public:
    AppointmentEntity(){};
    ~AppointmentEntity(){};
    QString date;
    QString doctor;
    QString text;

};


