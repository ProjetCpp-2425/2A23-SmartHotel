#include "interfacelogin.h"
#include "connexion.h"
#include "mainwindow.h"
#include <QThread>

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Connexion c;

    bool test=c.CreateConnection();

    interfacelogin login;



    if(test)
        login.show();

    return a.exec();
}
