#include "gs_conge.h"
#include "gs_employe.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QRegularExpression>
#include <QMessageBox>


GS_CONGE :: GS_CONGE(int id,  QDateTime d,  QDateTime f, QString type)
    : ID_EMPLOYE(id), debut(d), fin(f), type(type) {}
/*
void GS_CONGE::ajouterConge(const QDateTime& debut, const QDateTime& fin, const QString& type) {
    // Vérifier si l'ID de l'employé existe dans la table GS_EMPLOYE
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM GS_EMPLOYE WHERE ID_EMPLOYE = :ID_EMPLOYE");
    checkQuery.bindValue(":ID_EMPLOYE", ID_EMPLOYE);

    if (!checkQuery.exec()) {
        qDebug() << "Erreur lors de la vérification de l'ID employé:" << checkQuery.lastError().text();
        return;
    }

    checkQuery.next();
    int count = checkQuery.value(0).toInt();

    if (count == 0) {
        qDebug() << "Erreur : L'ID employé n'existe pas dans la table GS_EMPLOYE.";
        QMessageBox::critical(nullptr, "Erreur", "L'ID de l'employé n'existe pas dans la base de données.");
        return;
    }

    // Ajout du congé
    QSqlQuery query;
    query.prepare("INSERT INTO CONGE (ID_EMPLOYE, DEBUT, FIN, TYPE) "
                  "VALUES (:ID_EMPLOYE, TO_DATE(:DEBUT, 'DD-MM-YYYY HH24:MI'), TO_DATE(:FIN, 'DD-MM-YYYY HH24:MI'), :TYPE)");

    query.bindValue(":ID_EMPLOYE", ID_EMPLOYE);
    query.bindValue(":DEBUT", debut.toString("dd-MM-yyyy HH:mm"));
    query.bindValue(":FIN", fin.toString("dd-MM-yyyy HH:mm"));
    query.bindValue(":TYPE", type);

    qDebug() << "Requête préparée :"
             << query.executedQuery()
             << "Paramètres :"
             << "ID_EMPLOYE:" << ID_EMPLOYE
             << "DEBUT:" << debut.toString("dd-MM-yyyy HH:mm")
             << "FIN:" << fin.toString("dd-MM-yyyy HH:mm")
             << "TYPE:" << type;

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'ajout du congé:" << query.lastError().text();
        return;
    }

    qDebug() << "Le congé a été ajouté avec succès.";
}

bool GS_CONGE::verifierCongeParPosteEtDate(int idEmploye, const QDateTime& debut, const QDateTime& fin) {
    // Créer une instance de GS_EMPLOYE pour récupérer le poste
    GS_EMPLOYE employe;
    QString post = employe.getPosteById(idEmploye);

    if (post.isEmpty()) {
        qDebug() << "Erreur : L'employé avec l'ID" << idEmploye << "n'a pas de poste associé.";
        return false;
    }

    // Créer une requête SQL pour vérifier si un autre employé avec le même poste et les mêmes dates existe déjà
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM CONGE c "
                  "JOIN GS_EMPLOYE e ON c.ID_EMPLOYE = e.ID_EMPLOYE "
                  "WHERE e.POST = :post "
                  "AND ((c.DEBUT BETWEEN TO_DATE(:debut, 'DD-MM-YYYY HH24:MI') AND TO_DATE(:fin, 'DD-MM-YYYY HH24:MI')) "
                  "OR (c.FIN BETWEEN TO_DATE(:debut, 'DD-MM-YYYY HH24:MI') AND TO_DATE(:fin, 'DD-MM-YYYY HH24:MI')))");

    // Formater les dates en 'yyyy-MM-dd' pour correspondre à la base de données Oracle
    QString debutStr = debut.toString("DD-MM-YYYY HH24:MI");
    QString finStr = fin.toString("DD-MM-YYYY HH24:MI");

    // Ajouter les dates formatées à la requête
    query.bindValue(":post", post);
    query.bindValue(":debut", debutStr);
    query.bindValue(":fin", finStr);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la vérification des dates de congé:" << query.lastError().text();
        return false;
    }

    query.next();
    int count = query.value(0).toInt();

    // Si count > 0, cela signifie qu'un employé avec le même poste a déjà un congé dans la période donnée
    return count > 0;
}
*/


bool GS_CONGE::ajouterConge(const QDateTime& debut, const QDateTime& fin, const QString& type) {
    // Vérifier si l'ID de l'employé existe dans la table GS_EMPLOYE
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM GS_EMPLOYE WHERE ID_EMPLOYE = :ID_EMPLOYE");
    checkQuery.bindValue(":ID_EMPLOYE", ID_EMPLOYE);

    if (!checkQuery.exec()) {
        qDebug() << "Erreur lors de la vérification de l'ID employé:" << checkQuery.lastError().text();
        QMessageBox::critical(nullptr, "Erreur", "Erreur SQL lors de la vérification de l'employé.");
        return false;
    }

    checkQuery.next();
    if (checkQuery.value(0).toInt() == 0) {
        qDebug() << "Erreur : L'ID employé n'existe pas.";
        QMessageBox::critical(nullptr, "Erreur", "L'ID employé est introuvable dans la base de données.");
        return false;
    }

    // Préparation de la requête pour l'ajout
    QSqlQuery query;
    query.prepare("INSERT INTO CONGE (ID_EMPLOYE, DEBUT, FIN, TYPE) "
                  "VALUES (:ID_EMPLOYE, TO_DATE(:DEBUT, 'DD-MM-YYYY HH24:MI'), TO_DATE(:FIN, 'DD-MM-YYYY HH24:MI'), :TYPE)");

    query.bindValue(":ID_EMPLOYE", ID_EMPLOYE);
    query.bindValue(":DEBUT", debut.toString("dd-MM-yyyy HH:mm"));
    query.bindValue(":FIN", fin.toString("dd-MM-yyyy HH:mm"));
    query.bindValue(":TYPE", type);

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'ajout du congé:" << query.lastError().text();
        QMessageBox::critical(nullptr, "Erreur", "Impossible d'ajouter le congé. Erreur SQL.");
        return false;
    }

    qDebug() << "Congé ajouté avec succès pour l'employé ID:" << ID_EMPLOYE;
    return true;
}



bool GS_CONGE::verifierCongeParPosteEtDate(int idEmploye, const QDateTime& debut, const QDateTime& fin) {
    // Récupérer le poste de l'employé
    GS_EMPLOYE employe;
    QString post = employe.getPosteById(idEmploye);

    if (post.isEmpty()) {
        qDebug() << "Erreur : Poste introuvable pour l'employé ID:" << idEmploye;
        return false;
    }

    // Vérification des congés qui se chevauchent
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM CONGE c "
                  "JOIN GS_EMPLOYE e ON c.ID_EMPLOYE = e.ID_EMPLOYE "
                  "WHERE e.POST = :post "
                  "AND ("
                  "     (c.DEBUT <= TO_DATE(:fin, 'DD-MM-YYYY HH24:MI') AND c.FIN >= TO_DATE(:debut, 'DD-MM-YYYY HH24:MI'))"
                  "   )");

    query.bindValue(":post", post);
    query.bindValue(":debut", debut.toString("dd-MM-yyyy HH:mm"));
    query.bindValue(":fin", fin.toString("dd-MM-yyyy HH:mm"));

    if (!query.exec()) {
        qDebug() << "Erreur SQL lors de la vérification des chevauchements de congé:" << query.lastError().text();
        return false;
    }

    query.next();
    return query.value(0).toInt() > 0;  // Retourne true s'il y a un conflit
}
