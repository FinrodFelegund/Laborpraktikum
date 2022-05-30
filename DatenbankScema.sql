create database DemoApp;
use demoapp;

create table Users(
uid integer primary key auto_increment,
email varchar(50) unique not null,
passwort char(32) not null
);


create table Doctors(
did integer primary key auto_increment,
doctorname varchar(100) not null,
street varchar(50),
streetnumber varchar(5),
city varchar(30),
plz varchar(5),
phone varchar(20),
uid integer not null,
Foreign key (uid) references Users(uid)  on delete cascade
);


create table Appointments(
appdate date not null,
apptime time not null,
title varchar(50),
notes varchar(200),
aid integer auto_increment primary key,
uid integer not null,
did integer,
Foreign key (uid) references Users(uid) on delete cascade, 
foreign key (did) references Doctors(did)
);

create table Examinations(
result varchar(500),
picture varchar(100), 
aid integer not null unique,
Foreign key (aid) references Appointments(aid) on delete cascade
);

create table Medicine(
medname varchar(50),

);
