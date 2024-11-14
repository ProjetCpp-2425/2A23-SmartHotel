#include "connexion.h"
#include <QDebug>
#include <QMessageBox>

Connection::Connection() {}

bool Connection::createconnect()
{
    bool test = false;
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("CPP_Project");
    db.setUserName("ProjetPurestay");
    db.setPassword("5910");

    if (!db.open()) {
        qDebug() << "Database Error: " << db.lastError().text();
        QMessageBox::critical(nullptr, QObject::tr("Database Connexion Error"),
                              db.lastError().text(), QMessageBox::Ok);
    } else {
        test = true;
    }
    return test;
}

void Connection::closeConnection()
{
    db.close();
}
