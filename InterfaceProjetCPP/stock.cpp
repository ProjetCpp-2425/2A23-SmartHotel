#include "stock.h"
#include <QSqlError>
#include <QDebug>
#include <QSystemTrayIcon>
Stock::Stock(int ID_Produit, QString Nom_Produit, QString Type_Produit, QString Conditions_Environnementales,
             int Quantite, QDate Date_Darrivee, int Delai)
{
    this->ID_Produit = ID_Produit;
    this->Nom_Produit = Nom_Produit;
    this->Type_Produit = Type_Produit;
    this->Conditions_Environnementales = Conditions_Environnementales;
    this->Quantite = Quantite;
    this->Date_Darrivee = Date_Darrivee;
    this->Delai = Delai;
}

bool Stock::ajouter()
{
    QSqlQuery query;

    query.prepare("INSERT INTO stock (ID_PRODUIT, NOM_PRODUIT, TYPE_PRODUIT, CONDITIONS_ENVIRONNEMENTALES, QUANTITE, DATE_ARRIVEE, DELAI) "
                  "VALUES (:ID_PRODUIT, :NOM_PRODUIT, :TYPE_PRODUIT, :CONDITIONS_ENVIRONNEMENTALES, :QUANTITE, :DATE_ARRIVEE, :DELAI)");
    query.bindValue(":ID_PRODUIT", ID_Produit);
    query.bindValue(":NOM_PRODUIT", Nom_Produit);
    query.bindValue(":TYPE_PRODUIT", Type_Produit);
    query.bindValue(":CONDITIONS_ENVIRONNEMENTALES", Conditions_Environnementales);
    query.bindValue(":QUANTITE", Quantite);
    query.bindValue(":DATE_ARRIVEE", Date_Darrivee);
    query.bindValue(":DELAI", Delai);

    qDebug() << "Ajout dans la table Stock:";
    qDebug() << "ID_Produit:" << ID_Produit;
    qDebug() << "Nom_Produit:" << Nom_Produit;
    qDebug() << "Type_Produit:" << Type_Produit;
    qDebug() << "Conditions_Environnementales:" << Conditions_Environnementales;
    qDebug() << "Quantite:" << Quantite;
    qDebug() << "Date_Darrivee:" << Date_Darrivee;
    qDebug() << "Delai:" << Delai;

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'exécution de la requête INSERT:" << query.lastError().text();
        return false;
    }

    // Vérification explicite si aucune ligne n'a été affectée
    if (query.numRowsAffected() == 0) {
        qDebug() << "Aucune ligne ajoutée malgré la requête exécutée.";
        return false;
    }

    qDebug() << "Produit ajouté avec succès.";
    return true;
}


QSqlQueryModel* Stock::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT ID_PRODUIT, NOM_PRODUIT, TYPE_PRODUIT, CONDITIONS_ENVIRONNEMENTALES, QUANTITE, DATE_ARRIVEE, DELAI FROM stock");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Produit"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom Produit"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Type Produit"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Conditions Environnementales"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Quantité"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date d'Arrivée"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Délai"));

    return model;
}

QSqlQueryModel* Stock::chercherParNom(const QString& Nom_Produit)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT ID_PRODUIT, NOM_PRODUIT, TYPE_PRODUIT, CONDITIONS_ENVIRONNEMENTALES, QUANTITE, DATE_ARRIVEE, DELAI "
                  "FROM stock WHERE NOM_PRODUIT LIKE :NOM_PRODUIT");
    query.bindValue(":NOM_PRODUIT", "%" + Nom_Produit + "%");

    if (!query.exec()) {
        qDebug() << "Erreur lors de la recherche par nom:" << query.lastError().text();
        return nullptr;
    }

    model->setQuery(std::move(query));
    return model;
}

QSqlQueryModel* Stock::chercherParID(int ID_Produit)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT ID_PRODUIT, NOM_PRODUIT, TYPE_PRODUIT, CONDITIONS_ENVIRONNEMENTALES, QUANTITE, DATE_ARRIVEE, DELAI "
                  "FROM stock WHERE ID_PRODUIT = :ID_PRODUIT");
    query.bindValue(":ID_PRODUIT", ID_Produit);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la recherche par ID:" << query.lastError().text();
        return nullptr;
    }

    model->setQuery(std::move(query));
    if (model->rowCount() == 0) {
        qDebug() << "Aucun résultat trouvé pour l'ID :" << ID_Produit;
        return nullptr;
    }

    return model;
}

QSqlQueryModel* Stock::chercherParConditions(const QString& conditions)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT ID_PRODUIT, NOM_PRODUIT, TYPE_PRODUIT, CONDITIONS_ENVIRONNEMENTALES, QUANTITE, DATE_ARRIVEE, DELAI "
                  "FROM stock WHERE CONDITIONS_ENVIRONNEMENTALES = :CONDITIONS_ENVIRONNEMENTALES");
    query.bindValue(":CONDITIONS_ENVIRONNEMENTALES", conditions);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la recherche par conditions environnementales:" << query.lastError().text();
        return nullptr;
    }

    model->setQuery(std::move(query));
    return model;
}

bool Stock::modifier(int ID, int column, const QString& valeur) {
    QSqlQuery query;
    QString columnName;

    switch (column) {
        case 1:
            columnName = "NOM_PRODUIT";
            break;
        case 2:
            columnName = "TYPE_PRODUIT";
            break;
        case 3:
            columnName = "CONDITIONS_ENVIRONNEMENTALES";
            break;
        case 4:
            columnName = "QUANTITE";
            break;
        case 5:
            columnName = "DATE_ARRIVEE";
            break;
        case 6:
            columnName = "DELAI";
            break;
        default:
            qDebug() << "Colonne non valide pour la modification.";
            return false;
    }

    query.prepare(QString("UPDATE stock SET %1 = :valeur WHERE ID_PRODUIT = :ID").arg(columnName));
    query.bindValue(":valeur", valeur);
    query.bindValue(":ID", ID);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la modification du produit:" << query.lastError().text();
        return false;
    }
    return true;
}


bool Stock::supprimer()
{
    QSqlQuery query;
    query.prepare("DELETE FROM stock WHERE ID_PRODUIT = :ID_PRODUIT");
    query.bindValue(":ID_PRODUIT", ID_Produit);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la suppression du produit:" << query.lastError().text();
        return false;
    }
    return true;
}



QSqlQueryModel* Stock::trierParDelai()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT ID_PRODUIT, NOM_PRODUIT, TYPE_PRODUIT, CONDITIONS_ENVIRONNEMENTALES, QUANTITE, DATE_ARRIVEE, DELAI "
                    "FROM stock ORDER BY DELAI ASC");

    if (model->lastError().isValid()) {
        qDebug() << "Erreur dans le tri par délai :" << model->lastError().text();
    }
    return model;
}






QSqlQueryModel* Stock::trierParQuantite()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT ID_PRODUIT, NOM_PRODUIT, TYPE_PRODUIT, CONDITIONS_ENVIRONNEMENTALES, QUANTITE, DATE_ARRIVEE, DELAI "
                    "FROM stock ORDER BY QUANTITE DESC");

    return model;
}

QSqlQueryModel* Stock::trierParNomProduit()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT ID_PRODUIT, NOM_PRODUIT, TYPE_PRODUIT, CONDITIONS_ENVIRONNEMENTALES, QUANTITE, DATE_ARRIVEE, DELAI "
                    "FROM stock ORDER BY NOM_PRODUIT ASC");

    return model;
}

bool Stock::ajouterSoumission(int ID_Produit, int Quantite_Soumise, QDate Date_Soumission) {
    QSqlQuery query;

    // Préparer la requête pour insérer une soumission
    query.prepare("INSERT INTO Soumission (ID_PRODUIT, QUANTITE_SOUMISE, DATE_SOUMISSION) "
                  "VALUES (:ID_PRODUIT, :QUANTITE_SOUMISE, :DATE_SOUMISSION)");
    query.bindValue(":ID_PRODUIT", ID_Produit);
    query.bindValue(":QUANTITE_SOUMISE", Quantite_Soumise);
    query.bindValue(":DATE_SOUMISSION", Date_Soumission);

    // Exécuter la requête et vérifier les erreurs
    if (!query.exec()) {
        qDebug() << "Erreur lors de l'ajout de la soumission :" << query.lastError().text();
        return false;
    }
    return true;
}


QSqlQueryModel* Stock::afficherSoumissions() {
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT s.ID_PRODUIT, st.NOM_PRODUIT, s.QUANTITE_SOUMISE, s.DATE_SOUMISSION "
                    "FROM Soumission s "
                    "JOIN Stock st ON s.ID_PRODUIT = st.ID_PRODUIT");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Produit"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom Produit"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Quantité Soumise"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date de Soumission"));
    return model;
}


bool Stock::mettreAJourQuantite(int ID_Produit, int nouvelleQuantite) {
    QSqlQuery query;
    query.prepare("UPDATE stock SET QUANTITE = :nouvelleQuantite WHERE ID_PRODUIT = :ID_PRODUIT");
    query.bindValue(":nouvelleQuantite", nouvelleQuantite);
    query.bindValue(":ID_PRODUIT", ID_Produit);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la mise à jour de la quantité:" << query.lastError().text();
        return false;
    }
    return true;
}
