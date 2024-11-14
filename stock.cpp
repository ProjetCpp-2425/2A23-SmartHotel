#include "stock.h"

Stock::Stock(int ID_Produit, QString Nom_Produit, QString Type_Produit, QString Conditions_Environnementales, int Quantite, QDate Date_Darrivee, int Delai)
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
    query.prepare("INSERT INTO stock (ID_PRODUIT, NOM_PRODUIT, TYPE_PRODUIT, CONDITIONS_ENVIRONNEMENTALES, QUANTITE, DATE_DARRIVEE, DELAI) "
                  "VALUES (:ID_PRODUIT, :NOM_PRODUIT, :TYPE_PRODUIT, :CONDITIONS_ENVIRONNEMENTALES, :QUANTITE, :DATE_DARRIVEE, :DELAI)");
    query.bindValue(":ID_PRODUIT", ID_Produit);
    query.bindValue(":NOM_PRODUIT", Nom_Produit);
    query.bindValue(":TYPE_PRODUIT", Type_Produit);
    query.bindValue(":CONDITIONS_ENVIRONNEMENTALES", Conditions_Environnementales);
    query.bindValue(":QUANTITE", Quantite);
    query.bindValue(":DATE_DARRIVEE", Date_Darrivee);
    query.bindValue(":DELAI", Delai);

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'ajout dans Stock:" << query.lastError().text();
        return false;
    }
    return true;
}

QSqlQueryModel* Stock::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT ID_PRODUIT, NOM_PRODUIT, TYPE_PRODUIT, CONDITIONS_ENVIRONNEMENTALES, QUANTITE, DATE_DARRIVEE, DELAI FROM stock");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Produit"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom Produit"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Type Produit"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Conditions Environnementales"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Quantité"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date d'Arrivée"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Délai"));

    return model;
}

QSqlQueryModel* Stock::chercherParNom(const QString& Nom_Produit) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT ID_PRODUIT, NOM_PRODUIT, TYPE_PRODUIT, CONDITIONS_ENVIRONNEMENTALES, QUANTITE, DATE_DARRIVEE, DELAI "
                  "FROM stock WHERE NOM_PRODUIT LIKE :NOM_PRODUIT");

    query.bindValue(":NOM_PRODUIT", "%" + Nom_Produit + "%");

    if (!query.exec()) {
        qDebug() << "Erreur lors de la recherche:" << query.lastError().text();
        return nullptr;
    }

    model->setQuery(std::move(query));
    return model;
}

bool Stock::modifier(int ID, int column, const QString& valeur) {
    QSqlQuery query;
    QString columnName;

    switch (column) {
        case 1: columnName = "NOM_PRODUIT"; break;
        case 2: columnName = "TYPE_PRODUIT"; break;
        case 3: columnName = "CONDITIONS_ENVIRONNEMENTALES"; break;
        case 4: columnName = "QUANTITE"; break;
        case 5: columnName = "DATE_DARRIVEE"; break;
        case 6: columnName = "DELAI"; break;
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

bool Stock::supprimer() {
    QSqlQuery query;
    query.prepare("DELETE FROM stock WHERE ID_PRODUIT = :ID_PRODUIT");
    query.bindValue(":ID_PRODUIT", ID_Produit);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la suppression du produit:" << query.lastError().text();
        return false;
    }
    return true;
}



bool Stock::ajouterSoumission(int produit_id, int quantite_soumise, QDate date_soumission) {
    QSqlQuery query;
    query.prepare("INSERT INTO soumissions (produit_id, quantite_soumise, date_soumission) "
                  "VALUES (:produit_id, :quantite_soumise, :date_soumission)");
    query.bindValue(":produit_id", produit_id);
    query.bindValue(":quantite_soumise", quantite_soumise);
    query.bindValue(":date_soumission", date_soumission);

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'ajout de la soumission:" << query.lastError().text(); // Ajout du message d'erreur détaillé
        return false;
    }
    return true;
}

QSqlQueryModel* Stock::afficherSoumissions() {
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT s.id, p.NOM_PRODUIT, s.quantite_soumise, s.date_soumission "
                    "FROM soumissions s JOIN stock p ON s.produit_id = p.ID_PRODUIT");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Soumission"));
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

