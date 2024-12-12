#include "connexion.h"
#include <QMessageBox>
#include <QApplication>

bool Connexion::CreateConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("CPP_Project");
    db.setUserName("ProjetPurestay");
    db.setPassword("5910");

    return (db.open());
}



