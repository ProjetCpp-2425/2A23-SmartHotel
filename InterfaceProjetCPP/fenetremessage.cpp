#include "fenetremessage.h"
#include "ui_fenetremessage.h"
#include <QThread>


fenetremessage::fenetremessage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::fenetremessage)
{
    ui->setupUi(this);
    int ret=A.connect_arduino();
    switch(ret)
    {
    case(0):qDebug()<< "arduino is available and connected to : "<< A.getarduino_port_name();
        break;
    case(1):qDebug() << "arduino is available but not connected to :" <<A.getarduino_port_name();
       break;
    case(-1):qDebug() << "arduino is not available";
    }
     QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(update_label()));
}

fenetremessage::~fenetremessage()
{
    delete ui;
}

void fenetremessage::on_BoutonEnvoyerSMS_clicked()
{
    QString phoneNumber = ui->num->text();
    QString message = ui->message->text();
    if (message.isEmpty())
    {
            ui->LabelNum->setText("Saisissez un message");
            return;

    }
    if (!VerificationNumero(phoneNumber))
    {
        ui->LabelNum->setText("Numero téléphone n'existe pas dans la base de données");

    }
    else
    {
        QString command = phoneNumber + ";" + message;
        A.write_to_arduino(command);

    }

}

bool fenetremessage::VerificationNumero(QString numerotelephone)
{
    QSqlQuery Query;
    Query.prepare("SELECT * FROM Commande WHERE NumeroTelephone= :numerotelephone");

    Query.bindValue(":numerotelephone", numerotelephone);

    if(numerotelephone.isEmpty())
        ui->LabelNum->setText("Saisissez un numero de telephone");
    else
    {
    if (!Query.exec())

        return false;
    if (Query.next())
        return true;

    }
    return false;
}

