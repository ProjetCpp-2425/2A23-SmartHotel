#include "interfacelogin.h"
#include "ui_interfacelogin.h"

interfacelogin::interfacelogin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::interfacelogin)
{
    ui->setupUi(this);
}

interfacelogin::~interfacelogin()
{
    delete ui;
}

void interfacelogin::on_LoginButton_clicked()
{
    if (VerificationLogin())
    {
        w=new MainWindow(this);

        w->show();
        this->hide();


    }
}
bool interfacelogin::VerificationLogin()
{
    QString Email = ui->LineEditEmai->text();
    QString MDP = ui->LineEditMDP->text();

    QSqlQuery Query;
    Query.prepare("SELECT * FROM GS_Employe WHERE Mail = :Email AND Password = :MDP");

    Query.bindValue(":Email", Email);
    Query.bindValue(":MDP", MDP);

    if (Email.isEmpty())
        ui->LabelEmail->setText("L'Email ne peut pas être vide");
    if(MDP.isEmpty())
        ui->LabelMDP->setText("Le mot de passe neut pas être vide");
    else
    {
    if (!Query.exec())

        return false;
    if (Query.next())
        return true;
    else
    {
        ui->LabelEmail->setText("Email Invalide");
         ui->LabelMDP->setText("Mot de passe invalide ");
        ui->LabelLogin->setText("Erreur Login");
    }
    }

    return false;
}
