#include "commande.h"




Commande::Commande(QString NomEntreprise, QString TypeCommande, int QuantiteProduit, int MatriculeFiscale, int NumeroTelephone, QString Adresse,
         float PrixCommande, QString EtatCommande, QDateTime DateLivraison)
{
    this->NomEntreprise=NomEntreprise;
    this->TypeCommande=TypeCommande;
    this->QuantiteProduit=QuantiteProduit;
    this->MatriculeFiscale=MatriculeFiscale;
    this->NumeroTelephone=NumeroTelephone;
    this->Adresse=Adresse;
    this->PrixCommande=PrixCommande;
    this->EtatCommande=EtatCommande;
    this->DateLivraison=DateLivraison;

}

bool Commande::AjouterCommande()
{
    QSqlQuery query;

    query.prepare("INSERT INTO Commande (NomEntreprise, TypeCommande, QuantiteProduit, MatriculeFiscale, "
                      "NumeroTelephone, Adresse, PrixCommande, EtatCommande, DateLivraison) "
                      "VALUES (:NomEntreprise, :TypeCommande, :QuantiteProduit, :MatriculeFiscale, "
                      ":NumeroTelephone, :Adresse, :PrixCommande, :EtatCommande, :DateLivraison)");

        query.bindValue(":NomEntreprise", NomEntreprise);
        query.bindValue(":TypeCommande", TypeCommande);
        query.bindValue(":QuantiteProduit", QuantiteProduit);
        query.bindValue(":MatriculeFiscale", MatriculeFiscale);
        query.bindValue(":NumeroTelephone", NumeroTelephone);
        query.bindValue(":Adresse", Adresse);
        query.bindValue(":PrixCommande", PrixCommande);
        query.bindValue(":EtatCommande", EtatCommande);
        query.bindValue(":DateLivraison", DateLivraison);

    return query.exec();


}

QSqlQueryModel* Commande::AfficherCommande()
{
    QSqlQueryModel *model = new QSqlQueryModel();

    model->setQuery("SELECT NomEntreprise, IDCommande, TypeCommande, QuantiteProduit, "
                       "TO_CHAR(MatriculeFiscale) AS MatriculeFiscale, "
                       "TO_CHAR(NumeroTelephone) AS NumeroTelephone, "
                       "Adresse, PrixCommande, EtatCommande, DateLivraison "
                       "FROM Commande");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Nom Entreprise"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Type Commande"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Quantite Produit"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Matricule Fiscale"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Numero Telephone"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Prix Commande"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Etat Commande"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("Date Livraison"));

    return model;
}

bool Commande::ModifierCommande(int ID, int Column, QString ch)
{
    QSqlQuery query;
    switch (Column)
    {
     case 0:
        query.prepare("update Commande set NomEntreprise = :ch WHERE IDCommande = :ID");
        break;

     case 2:
            query.prepare("update Commande set TypeCommande = :ch WHERE IDCommande = :ID");
        break;
     case 3:

        query.prepare("update Commande set QuantiteProduit = :ch WHERE IDCommande = :ID");
        break;
    case 4:
        query.prepare("update Commande set MatriculeFiscale = :ch WHERE IDCommande = :ID");
        break;
   case 5:
        query.prepare("update Commande set NumeroTelephone = :ch WHERE IDCommande = :ID");
        break;
    case 6:
        query.prepare("update Commande set Adresse = :ch WHERE IDCommande = :ID");
        break;
   case 7:
        query.prepare("update Commande set PrixCommande = :ch WHERE IDCommande = :ID");
        break;
   case 8:
        query.prepare("update Commande set EtatCommande = :ch WHERE IDCommande = :ID");
        break;

  case 9:
       query.prepare("update Commande set DateLivraison = :ch WHERE IDCommande = :ID");
        break;

    }

    query.bindValue(":ch", ch );
    query.bindValue(":ID", ID);



      return query.exec();
}
bool Commande::SupprimerCommande(int Code)
{
    QSqlQuery Query;

    Query.prepare("delete from Commande where IDCommande = :Code");

    Query.bindValue(":Code", Code);

    return Query.exec();
}

QSqlQueryModel * Commande::TriDateLivraisonR()
{
    QSqlQueryModel *Model = new QSqlQueryModel();
    Model->setQuery("SELECT NomEntreprise, IDCommande, TypeCommande, QuantiteProduit, "
                    "TO_CHAR(MatriculeFiscale) AS MatriculeFiscale, "
                    "TO_CHAR(NumeroTelephone) AS NumeroTelephone, "
                    "Adresse, PrixCommande, EtatCommande, DateLivraison "
                    "FROM Commande order by DateLivraison desc");

    return Model;

}
QSqlQueryModel * Commande::TriDateLivraisonA()
{
    QSqlQueryModel *Model = new QSqlQueryModel();
    Model->setQuery("SELECT NomEntreprise, IDCommande, TypeCommande, QuantiteProduit, "
                    "TO_CHAR(MatriculeFiscale) AS MatriculeFiscale, "
                    "TO_CHAR(NumeroTelephone) AS NumeroTelephone, "
                    "Adresse, PrixCommande, EtatCommande, DateLivraison "
                    "FROM Commande order by DateLivraison asc");

    return Model;

}
QSqlQueryModel * Commande::TriPrix()
{
    QSqlQueryModel *Model = new QSqlQueryModel();
    Model->setQuery("SELECT NomEntreprise, IDCommande, TypeCommande, QuantiteProduit, "
                    "TO_CHAR(MatriculeFiscale) AS MatriculeFiscale, "
                    "TO_CHAR(NumeroTelephone) AS NumeroTelephone, "
                    "Adresse, PrixCommande, EtatCommande, DateLivraison "
                    "FROM Commande order by PrixCommande desc");

    return Model;

}
QSqlQueryModel * Commande::TriNomEntreprise()
{
    QSqlQueryModel *Model = new QSqlQueryModel();
    Model->setQuery("SELECT NomEntreprise, IDCommande, TypeCommande, QuantiteProduit, "
                    "TO_CHAR(MatriculeFiscale) AS MatriculeFiscale, "
                    "TO_CHAR(NumeroTelephone) AS NumeroTelephone, "
                    "Adresse, PrixCommande, EtatCommande, DateLivraison "
                    "FROM Commande order by NomEntreprise");

    return Model;

}
QSqlQueryModel * Commande::CategorieEquipement()
{
    QSqlQueryModel *Model = new QSqlQueryModel();
    Model->setQuery("SELECT NomEntreprise, IDCommande, TypeCommande, QuantiteProduit, "
                    "TO_CHAR(MatriculeFiscale) AS MatriculeFiscale, "
                    "TO_CHAR(NumeroTelephone) AS NumeroTelephone, "
                    "Adresse, PrixCommande, EtatCommande, DateLivraison ""from Commande where TypeCommande = 'Equipement'");

    return Model;

}
QSqlQueryModel * Commande::CategorieAlimentation()
{
    QSqlQueryModel *Model = new QSqlQueryModel();
    Model->setQuery("SELECT NomEntreprise, IDCommande, TypeCommande, QuantiteProduit, "
                    "TO_CHAR(MatriculeFiscale) AS MatriculeFiscale, "
                    "TO_CHAR(NumeroTelephone) AS NumeroTelephone, "
                    "Adresse, PrixCommande, EtatCommande, DateLivraison ""from Commande where TypeCommande = 'Alimentation'");

    return Model;

}
QSqlQueryModel * Commande::CategorieUniformes()
{
    QSqlQueryModel *Model = new QSqlQueryModel();
    Model->setQuery("SELECT NomEntreprise, IDCommande, TypeCommande, QuantiteProduit, "
                    "TO_CHAR(MatriculeFiscale) AS MatriculeFiscale, "
                    "TO_CHAR(NumeroTelephone) AS NumeroTelephone, "
                    "Adresse, PrixCommande, EtatCommande, DateLivraison ""from Commande where TypeCommande = 'Uniformes'");

    return Model;

}
QSqlQueryModel * Commande::CategorieDecorations()
{
    QSqlQueryModel *Model = new QSqlQueryModel();
    Model->setQuery("SELECT NomEntreprise, IDCommande, TypeCommande, QuantiteProduit, "
                    "TO_CHAR(MatriculeFiscale) AS MatriculeFiscale, "
                    "TO_CHAR(NumeroTelephone) AS NumeroTelephone, "
                    "Adresse, PrixCommande, EtatCommande, DateLivraison ""from Commande where TypeCommande = 'Decorations'");

    return Model;

}
QSqlQueryModel * Commande::CategorieFournitures()
{
    QSqlQueryModel *Model = new QSqlQueryModel();
    Model->setQuery("SELECT NomEntreprise, IDCommande, TypeCommande, QuantiteProduit, "
                    "TO_CHAR(MatriculeFiscale) AS MatriculeFiscale, "
                    "TO_CHAR(NumeroTelephone) AS NumeroTelephone, "
                    "Adresse, PrixCommande, EtatCommande, DateLivraison ""from Commande where TypeCommande = 'Fournitures'");

    return Model;

}

QSqlQueryModel * Commande::EtatPayee()
{
    QSqlQueryModel *Model = new QSqlQueryModel();
    Model->setQuery("SELECT NomEntreprise, IDCommande, TypeCommande, QuantiteProduit, "
                    "TO_CHAR(MatriculeFiscale) AS MatriculeFiscale, "
                    "TO_CHAR(NumeroTelephone) AS NumeroTelephone, "
                    "Adresse, PrixCommande, EtatCommande, DateLivraison ""from Commande where EtatCommande ='Payee'");

    return Model;

}
QSqlQueryModel * Commande::EtatNonPayee()
{
    QSqlQueryModel *Model = new QSqlQueryModel();
    Model->setQuery("SELECT NomEntreprise, IDCommande, TypeCommande, QuantiteProduit, "
                    "TO_CHAR(MatriculeFiscale) AS MatriculeFiscale, "
                    "TO_CHAR(NumeroTelephone) AS NumeroTelephone, "
                    "Adresse, PrixCommande, EtatCommande, DateLivraison ""from Commande where EtatCommande = 'Non Payee'");

    return Model;

}



QString Commande::SommeEquipement()
{
    QSqlQuery Query;
    Query.prepare("SELECT (COUNT(CASE WHEN TypeCommande = 'Equipement' THEN 1 END) * 100.0) / COUNT(*) FROM Commande");

    if (Query.exec()) {
        if (Query.next()) {
            double percentage = Query.value(0).toDouble();
            return QString::number(percentage, 'f', 2);
        }
    }
    return "error";
 }
QString Commande::SommeAlimentation()
{
    QSqlQuery Query;
    Query.prepare("SELECT (COUNT(CASE WHEN TypeCommande = 'Alimentation' THEN 1 END) * 100.0) / COUNT(*) FROM Commande");

    if (Query.exec()) {
        if (Query.next()) {
            double percentage = Query.value(0).toDouble();
            return QString::number(percentage, 'f', 2);
        }
    }
    return "error";
 }
QString Commande::SommeUniforme()
{
    QSqlQuery Query;
    Query.prepare("SELECT (COUNT(CASE WHEN TypeCommande = 'Uniformes' THEN 1 END) * 100.0) / COUNT(*) FROM Commande");

    if (Query.exec()) {
        if (Query.next()) {
            double percentage = Query.value(0).toDouble();
            return QString::number(percentage, 'f', 2);
        }
    }
    return "error";
 }
QString Commande::SommeFournitures()
{
    QSqlQuery Query;
    Query.prepare("SELECT (COUNT(CASE WHEN TypeCommande = 'Fournitures' THEN 1 END) * 100.0) / COUNT(*) FROM Commande");

    if (Query.exec()) {
        if (Query.next()) {
            double percentage = Query.value(0).toDouble();
            return QString::number(percentage, 'f', 2);
        }
    }
    return "error";
 }

QString Commande::SommeDecorations()
{
    QSqlQuery Query;
    Query.prepare("SELECT (COUNT(CASE WHEN TypeCommande = 'Decorations' THEN 1 END) * 100.0) / COUNT(*) FROM Commande");

    if (Query.exec()) {
        if (Query.next()) {
            double percentage = Query.value(0).toDouble();
            return QString::number(percentage, 'f', 2);
        }
    }
    return "error";
 }

QString Commande::SommeCommandeTraitee()
{
    QSqlQuery Query;
    Query.prepare("SELECT (COUNT(CASE WHEN EtatCommande = 'Payee' THEN 1 END) * 100.0) / COUNT(*) FROM Commande");

    if (Query.exec()) {
        if (Query.next()) {
            double percentage = Query.value(0).toDouble();
            return QString::number(percentage, 'f', 2);
        }
    }
    return "error";
 }
int Commande::countTotalCommandes() {
    QSqlQuery query;
    int totalCommandes = 0;

    query.prepare("SELECT COUNT(*) FROM Commande");

    if (query.exec()) {
        if (query.next()) {
            totalCommandes = query.value(0).toInt();
        }
    } else {
        qDebug() << "Error executing query: ";
    }

    return totalCommandes;
}

QString Commande::SommeCommandeNonTraitee()
{
    QSqlQuery Query;
    Query.prepare("SELECT (COUNT(CASE WHEN EtatCommande = 'Non Payee' THEN 1 END) * 100.0) / COUNT(*) FROM Commande");

    if (Query.exec()) {
        if (Query.next()) {
            double percentage = Query.value(0).toDouble();
            return QString::number(percentage, 'f', 2);
        }
    }
    return "error";
 }


QString Commande::CommandeSemaine()
{
    QSqlQuery Query;
    Query.prepare("SELECT * FROM Commande WHERE DateLivraison >= trunc ( sysdate, 'iw')");

    if (Query.exec()) {
        if (Query.next()) {
            double percentage = Query.value(0).toDouble();
            return QString::number(percentage, 'f', 2);
        }
    }
    return "error";
 }


