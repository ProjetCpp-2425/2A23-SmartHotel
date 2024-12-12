
#include "gs_employe.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QRegularExpression>
#include <QMessageBox>

GS_EMPLOYE::GS_EMPLOYE(int id, QString n, QString p, float s, QDate d, int tel, QString pos, QString etat, QString m, QString ps, QString rf)
    : ID_employe(id), nom(n), prenom(p), salaire(s), date_embauche(d), num_telephone(tel), post(pos), etat(etat), mail(m) , password(ps), rfid(rf)  {}

bool GS_EMPLOYE::verifierSaisie() {
    if (ID_employe <= 0) {
        QMessageBox::critical(nullptr, "Erreur", "L'identifiant doit être un entier positif.");
        return false;
    }

    QRegularExpression regexAlphabetic("^[a-zA-Z]+$");
    if (!regexAlphabetic.match(nom).hasMatch()) {
        QMessageBox::critical(nullptr, "Erreur", "Le nom doit contenir uniquement des lettres.");
        return false;
    }
    if (!regexAlphabetic.match(prenom).hasMatch()) {
        QMessageBox::critical(nullptr, "Erreur", "Le prénom doit contenir uniquement des lettres.");
        return false;
    }


    if (QString::number(num_telephone).length() != 8 || num_telephone < 10000000) {
        QMessageBox::critical(nullptr, "Erreur", "Le numéro de téléphone doit être composé de 8 chiffres.");
        return false;
    }

    if (salaire <= 0) {
        QMessageBox::critical(nullptr, "Erreur", "Le salaire doit être un nombre flottant positif.");
        return false;
    }

    return true;
}



bool GS_EMPLOYE::ajouter() {
    if (!verifierSaisie()) return false;

    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM GS_EMPLOYE WHERE ID_EMPLOYE = :ID_EMPLOYE");
    query.bindValue(":ID_EMPLOYE", ID_employe);


    query.prepare("INSERT INTO GS_EMPLOYE (ID_EMPLOYE, NOM, PRENOM, SALAIRE, DATE_EMBAUCHE, NUM_TELEPHONE, POST, ETAT, MAIL, PASSWORD, RFID) "
                  "VALUES (:ID_EMPLOYE, :NOM, :PRENOM, :SALAIRE, TO_DATE(:DATE_EMBAUCHE, 'YYYY-MM-DD'), :NUM_TELEPHONE, :POST, :ETAT, :MAIL, :PASSWORD, :RFID)");
    query.bindValue(":ID_EMPLOYE", ID_employe);
    query.bindValue(":NOM", nom);
    query.bindValue(":PRENOM", prenom);
    query.bindValue(":SALAIRE", salaire);
    query.bindValue(":DATE_EMBAUCHE", date_embauche.toString("yyyy-MM-dd"));
    query.bindValue(":NUM_TELEPHONE", num_telephone);
    query.bindValue(":POST", post);
    query.bindValue(":ETAT", etat);
    query.bindValue(":MAIL", mail);
    query.bindValue(":PASSWORD", password);
    query.bindValue(":RFID", rfid);

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'ajout de l'employé:" << query.lastError().text();
        return false;
    }
    return true;
}





QSqlQueryModel* GS_EMPLOYE::afficher() {
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT TO_CHAR(ID_EMPLOYE) AS ID_EMPLOYE, NOM, PRENOM, SALAIRE, DATE_EMBAUCHE, TO_CHAR(NUM_TELEPHONE) AS NUM_TELEPHONE, POST, ETAT FROM GS_EMPLOYE");
    model->setHeaderData(0, Qt::Horizontal, "ID Employé");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Prénom");
    model->setHeaderData(3, Qt::Horizontal, "Salaire");
    model->setHeaderData(4, Qt::Horizontal, "Date Embauche");
    model->setHeaderData(5, Qt::Horizontal, "Numéro de Téléphone");
    model->setHeaderData(6, Qt::Horizontal, "Etat");
    model->setHeaderData(7, Qt::Horizontal, "Poste");

    return model;
}



QSqlQueryModel* GS_EMPLOYE::chercherParNom(const QString& nom) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT TO_CHAR(ID_EMPLOYE) AS ID_EMPLOYE, NOM, PRENOM, SALAIRE, TO_CHAR(DATE_EMBAUCHE, 'DD/MM/YYYY') AS DATE_EMBAUCHE, TO_CHAR(NUM_TELEPHONE) AS NUM_TELEPHONE, ETAT, POST, MAIL, PASSWORD "
                  "FROM GS_EMPLOYE WHERE NOM LIKE :NOM");
    query.bindValue(":NOM", "%" + nom + "%");

    if (!query.exec()) {
        qDebug() << "Erreur lors de la recherche:" << query.lastError().text();
        return nullptr;
    }

    model->setQuery(std::move(query));
    return model;
}

QSqlQueryModel* GS_EMPLOYE::chercherParPoste(const QString& poste) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT TO_CHAR(ID_EMPLOYE) AS ID_EMPLOYE, NOM, PRENOM, SALAIRE, TO_CHAR(DATE_EMBAUCHE, 'DD/MM/YYYY') AS DATE_EMBAUCHE, TO_CHAR(NUM_TELEPHONE) AS NUM_TELEPHONE, ETAT, POST, MAIL, PASSWORD "
                  "FROM GS_EMPLOYE WHERE POST LIKE :POST");
    query.bindValue(":POST", "%" + poste + "%");

    if (!query.exec()) {
        qDebug() << "Erreur lors de la recherche par poste:" << query.lastError().text();
        return nullptr;
    }

    model->setQuery(std::move(query));
    return model;
}


QSqlQueryModel* GS_EMPLOYE::chercherParEtat(const QString& etat) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;


    query.prepare("SELECT TO_CHAR(ID_EMPLOYE) AS ID_EMPLOYE, NOM, PRENOM, SALAIRE, TO_CHAR(DATE_EMBAUCHE, 'DD/MM/YYYY') AS DATE_EMBAUCHE, TO_CHAR(NUM_TELEPHONE) AS NUM_TELEPHONE, ETAT, POST, MAIL, PASSWORD "
                  "FROM GS_EMPLOYE WHERE ETAT LIKE :ETAT");

    query.bindValue(":ETAT", "%" + etat + "%");


    if (!query.exec()) {
        qDebug() << "Erreur lors de la recherche par état:" << query.lastError().text();
        return nullptr;
    }

    model->setQuery(query);
    return model;
}





bool GS_EMPLOYE::supprimer() {
    QSqlQuery query;
    query.prepare("DELETE FROM GS_EMPLOYE WHERE ID_EMPLOYE = :ID_EMPLOYE");
    query.bindValue(":ID_EMPLOYE", ID_employe);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la suppression de l'employé:" << query.lastError().text();
        return false;
    }
    return true;
}

QSqlQueryModel* GS_EMPLOYE::trierParDateEmbauche() {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    //  année (croissant) | jour (décroissant)


    query.prepare("SELECT * FROM GS_EMPLOYE "
                  "ORDER BY EXTRACT(YEAR FROM DATE_EMBAUCHE) ASC, "
                  "EXTRACT(MONTH FROM DATE_EMBAUCHE) ASC, "
                  "EXTRACT(DAY FROM DATE_EMBAUCHE) DESC");

    if (!query.exec()) {
        qDebug() << "Erreur lors du tri par date d'embauche:" << query.lastError().text();
        return nullptr;
    }

    model->setQuery(query);
    return model;
}



QSqlQueryModel* GS_EMPLOYE::trierParSalaireDecroissant() {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT TO_CHAR(ID_EMPLOYE) AS ID_EMPLOYE, NOM, PRENOM, SALAIRE, TO_CHAR(DATE_EMBAUCHE, 'DD/MM/YYYY') AS DATE_EMBAUCHE, TO_CHAR(NUM_TELEPHONE) AS NUM_TELEPHONE, ETAT, POST, MAIL, PASSWORD "
                  "FROM  GS_EMPLOYE ORDER BY SALAIRE DESC");


    if (!query.exec()) {
        qDebug() << "Erreur lors du tri par salaire décroissant:" << query.lastError().text();
        return nullptr;
    }

    model->setQuery(query);
    return model;
}





bool GS_EMPLOYE::modifier() {
    QSqlQuery query;
    query.prepare("UPDATE GS_EMPLOYE SET "
                  "NOM = :NOM, "
                  "PRENOM = :PRENOM, "
                  "SALAIRE = :SALAIRE, "
                  "DATE_EMBAUCHE = TO_DATE(:DATE_EMBAUCHE, 'YYYY-MM-DD'), "
                  "NUM_TELEPHONE = :NUM_TELEPHONE, "
                  "POST = :POST, "
                  "ETAT = :ETAT, "
                  "MAIL = :MAIL, "
                  "PASSWORD = :PASSWORD "
                  "WHERE ID_EMPLOYE = :ID_EMPLOYE");

    query.bindValue(":ID_EMPLOYE", ID_employe);
    query.bindValue(":NOM", nom);
    query.bindValue(":PRENOM", prenom);
    query.bindValue(":SALAIRE", salaire);
    query.bindValue(":DATE_EMBAUCHE", date_embauche.toString("yyyy-MM-dd"));
    query.bindValue(":NUM_TELEPHONE", num_telephone);
    query.bindValue(":POST", post);
    query.bindValue(":ETAT", etat);
    query.bindValue(":MAIL", mail);
    query.bindValue(":PASSWORD", password);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la modification de l'employé:" << query.lastError().text();
        return false;
    }
    return true;
}



bool GS_EMPLOYE::chargerDonneesEmploye(int id) {
    QSqlQuery query;
    query.prepare("SELECT NOM, PRENOM, SALAIRE, DATE_EMBAUCHE, NUM_TELEPHONE, POST FROM GS_EMPLOYE WHERE ID_EMPLOYE = :ID_EMPLOYE");
    query.bindValue(":ID_EMPLOYE", id);


    if (query.exec() && query.next()) {
        nom = query.value(0).toString();
        prenom = query.value(1).toString();
        salaire = query.value(2).toFloat();
        date_embauche = query.value(3).toDate();
        num_telephone = query.value(4).toInt();
        post = query.value(5).toString();
        return true;
    } else {
        qDebug() << "Erreur lors du chargement des données de l'employé:" << query.lastError().text();
        return false;
    }
}




int GS_EMPLOYE::calculerTotalEmployes() {
    QSqlQuery query;
    query.prepare("SELECT COUNT * FROM GS_employe");

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    } else {
        qDebug() << "Erreur dans la requête de comptage des employés:" << query.lastError().text();
        return 0;
    }
}



QList<QDate> GS_EMPLOYE::getAbsences(int id_employe) {
    QList<QDate> absences;
    QSqlQuery query;

    query.prepare("SELECT DATE_PR FROM PRESENCE WHERE ID_EMPLOYEE = :id and STATUS='0'");
    query.bindValue(":id", id_employe);

    if (query.exec()) {
        while (query.next()) {
            QDate date = query.value(0).toDate();
            absences.append(date);
        }
    } else {
        qDebug() << "Erreur lors de la récupération des absences:" << query.lastError().text();
    }
    return absences;
}

QString GS_EMPLOYE::getPosteById(int idEmploye) {
    // Créer une requête SQL pour obtenir le poste de l'employé
    QSqlQuery query;
    query.prepare("SELECT POST FROM GS_EMPLOYE WHERE ID_EMPLOYE = :ID_EMPLOYE");
    query.bindValue(":ID_EMPLOYE", idEmploye);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la récupération du poste de l'employé:" << query.lastError().text();
        return "";
    }

    if (query.next()) {
        return query.value(0).toString();
    } else {
        return "";  // Si aucun employé trouvé, retourner une chaîne vide
    }
}




