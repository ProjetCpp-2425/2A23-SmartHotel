#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("CPP_Project");//inserer le nom de la source de données
db.setUserName("ProjetPurestay");//inserer nom de l'utilisateur
db.setPassword("5910");//inserer mot de passe de cet utilisateur

if (db.open())
test=true;





    return  test;
}
