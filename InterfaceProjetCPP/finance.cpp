#include "finance.h"
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

finance::finance()
{
    Matricule_Transaction = 0;
    Type_Transaction = "";
    Categorie = "";
    Date_Transaction = QDate();
    Mode_Paiement = "";
    Statut = "";
    Montant = 0.0;
}

finance::finance(int matricule_transaction, QString type_transaction, QString categorie, QDate date_transaction, QString mode_paiement, QString statut, double montant)
{
    Matricule_Transaction = matricule_transaction;
    Type_Transaction = type_transaction;
    Categorie = categorie;
    Date_Transaction = date_transaction;
    Mode_Paiement = mode_paiement;
    Statut = statut;
    Montant = montant;
}



bool finance::ajouter() {
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "Erreur: la base de données n'est pas ouverte avant l'insertion.";
        return false;
    }

    QSqlQuery query;

    query.prepare("INSERT INTO GS_FINANCE (MATRICULE_TRANSACTION, TYPE_TRANSACTION, CATEGORIE, "
                  "DATE_TRANSACTION, MODE_PAIEMENT, STATUT, MONTANT) "
                  "VALUES (:matricule_transaction, :type_transaction, :categorie, "
                  ":date_transaction, :mode_paiement, :statut, :montant)");

    query.bindValue(":matricule_transaction", Matricule_Transaction);
    query.bindValue(":type_transaction", Type_Transaction);
    query.bindValue(":categorie", Categorie);
    query.bindValue(":date_transaction", Date_Transaction);
    query.bindValue(":mode_paiement", Mode_Paiement);
    query.bindValue(":statut", Statut);
    query.bindValue(":montant", Montant);

    qDebug() << "Executing query...";
    if (query.exec()) {
        qDebug() << "Insertion successful.";
        return true;
    } else {
        qDebug() << "Erreur lors de l'ajout :" << query.lastError().text();
        qDebug() << "Executed Query:" << query.executedQuery();
        return false;
    }
}
bool finance::modifier(int matriculeTransaction, int column, const QString& valeur) {
    QSqlQuery query;
    QString columnName;

    switch (column) {
        case 0:
            columnName = "MATRICULE_TRANSACTION";
            break;
        case 1:
            columnName = "TYPE_TRANSACTION";
            break;
        case 2:
            columnName = "CATEGORIE";
            break;
        case 3:
            columnName = "DATE_TRANSACTION";
            break;
        case 4:
            columnName = "MODE_PAIEMENT";
            break;
        case 5:
            columnName = "STATUT";
            break;
        case 6:
            columnName = "MONTANT";
            break;
        default:
            qDebug() << "Colonne non valide pour la modification.";
            return false;
    }

    if (columnName == "DATE_TRANSACTION") {
        query.prepare(QString("UPDATE GS_FINANCE SET %1 = TO_DATE(:valeur, 'YYYY-MM-DD') WHERE MATRICULE_TRANSACTION = :matricule")
                          .arg(columnName));
    } else {
        query.prepare(QString("UPDATE GS_FINANCE SET %1 = :valeur WHERE MATRICULE_TRANSACTION = :matricule")
                          .arg(columnName));
    }

    query.bindValue(":valeur", valeur);
    query.bindValue(":matricule", matriculeTransaction);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la modification de la finance :" << query.lastError().text();
        return false;
    }
    return true;
}


QSqlQueryModel* finance::afficher() {
    QSqlQueryModel* model = new QSqlQueryModel();

    qDebug() << "Running query to fetch data...";
    model->setQuery("SELECT * FROM GS_FINANCE");

    if (model->lastError().isValid()) {
        qDebug() << "Query execution error:" << model->lastError().text();
        return nullptr;
    } else {
        qDebug() << "Query executed successfully.";
    }

    qDebug() << "Number of rows fetched:" << model->rowCount();

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Matricule_Transaction"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Type_Transaction"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Categorie"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date_Transaction"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Mode_Paiement"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Montant"));

    return model;
}
void finance::clearTable(QTableView *table){
    QSqlQueryModel* modelFeragh= new QSqlQueryModel();
    modelFeragh->clear();
    table->setModel(modelFeragh);
}

void finance::Recherche(QTableView *table, QString matricule, double Montant) {
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery *query = new QSqlQuery();

    if (!matricule.isEmpty()) {
        if (Montant != 0) {
            query->prepare("SELECT TO_CHAR(MATRICULE_TRANSACTION) AS MATRICULE_TRANSACTION, "
                           "TYPE_TRANSACTION, CATEGORIE, "
                           "TO_CHAR(DATE_TRANSACTION, 'DD/MM/YYYY') AS DATE_TRANSACTION, "
                           "MODE_PAIEMENT, STATUT, TO_CHAR(MONTANT)AS MONTANT "
                           "FROM GS_FINANCE "
                           "WHERE regexp_like(TO_CHAR(MATRICULE_TRANSACTION), :MATRICULE_TRANSACTION) "
                           "AND (MONTANT = :MONTANT OR MONTANT IS NULL)");
            query->bindValue(":MATRICULE_TRANSACTION", matricule);
            query->bindValue(":MONTANT", Montant);
        } else {
            query->prepare("SELECT TO_CHAR(MATRICULE_TRANSACTION) AS MATRICULE_TRANSACTION, "
                           "TYPE_TRANSACTION, CATEGORIE, "
                           "TO_CHAR(DATE_TRANSACTION, 'DD/MM/YYYY') AS DATE_TRANSACTION, "
                           "MODE_PAIEMENT, STATUT, TO_CHAR(MONTANT) AS MONTANT "
                           "FROM GS_FINANCE "
                           "WHERE regexp_like(TO_CHAR(MATRICULE_TRANSACTION), :MATRICULE_TRANSACTION)");
            query->bindValue(":MATRICULE_TRANSACTION", matricule);
        }
    } else if (Montant != 0) {
        query->prepare("SELECT TO_CHAR(MATRICULE_TRANSACTION) AS MATRICULE_TRANSACTION, "
                       "TYPE_TRANSACTION, CATEGORIE, "
                       "TO_CHAR(DATE_TRANSACTION, 'DD/MM/YYYY') AS DATE_TRANSACTION, "
                       "MODE_PAIEMENT, STATUT, TO_CHAR(MONTANT) AS MONTANT "
                       "FROM GS_FINANCE "
                       "WHERE MONTANT = :MONTANT OR MONTANT IS NULL");
        query->bindValue(":MONTANT", Montant);
    } else {
        query->prepare("SELECT TO_CHAR(MATRICULE_TRANSACTION) AS MATRICULE_TRANSACTION, "
                       "TYPE_TRANSACTION, CATEGORIE, "
                       "TO_CHAR(DATE_TRANSACTION, 'DD/MM/YYYY') AS DATE_TRANSACTION, "
                       "MODE_PAIEMENT, STATUT, TO_CHAR(MONTANT) AS MONTANT "
                       "FROM GS_FINANCE");
    }

    if (query->exec()) {
        model->setQuery(*query);
        table->setModel(model);
        table->show();
    } else {
        qDebug() << "Error executing query: " << query->lastError().text();
    }
}
double finance::sumRevenu() {
    QSqlQuery query;
    double totalRevenu = 0.0;

    query.prepare("SELECT SUM(MONTANT) "
                  "FROM GS_FINANCE "
                  "WHERE TYPE_TRANSACTION = 'revenu'");

    // Execute the query
    if (query.exec()) {
        if (query.next()) {
            totalRevenu = query.value(0).toDouble();
        }
    } else {
        qDebug() << "Error executing query: " << query.lastError().text();
    }

    return totalRevenu;
}



QSqlQueryModel* finance::tri_montant(){
    QSqlQueryModel* model= new QSqlQueryModel();
    model->setQuery("SELECT * FROM GS_FINANCE ORDER BY MONTANT");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Matricule_Transaction"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Type_Transaction"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Categorie"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date_Transaction"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Mode_Paiement"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Montant"));
    return model;
}
QSqlQueryModel* finance::tri_statut(){
    QSqlQueryModel* model= new QSqlQueryModel();
    model->setQuery("SELECT * FROM GS_FINANCE ORDER BY STATUT");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Matricule_Transaction"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Type_Transaction"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Categorie"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date_Transaction"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Mode_Paiement"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Montant"));
    return model;
}

QString finance::read()
{
    QFile file("alertsmailing.txt");
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(0, "info", file.errorString());
        return "";
    }
    QTextStream in(&file);
    return in.readAll();
}
void finance::write(QString time, QString txt)
{
    QFile file("alertsmailing.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << txt << '\n';
        file.close();
    }

}
QString finance::time()
{
    QDateTime time=time.currentDateTime();
    return  time.toString();
}
void finance::clearh()
{
    QFile file("alertsmailing.txt");
    file.open(QFile::WriteOnly|QFile::Truncate);
}
