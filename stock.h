#ifndef STOCK_H
#define STOCK_H

#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDateTime>
#include <QDebug>
#include <QSqlError>

class Stock {
private:
    int ID_Produit, Quantite, Delai;
    QString Nom_Produit, Type_Produit, Conditions_Environnementales;
    QDate Date_Darrivee;

public:
    Stock() = default;
    Stock(int ID_Produit, QString Nom_Produit, QString Type_Produit, QString Conditions_Environnementales, int Quantite, QDate Date_Darrivee, int Delai);

    bool ajouter();
    QSqlQueryModel* afficher();
    QSqlQueryModel* chercherParNom(const QString& Nom_Produit);
    bool supprimer();
    bool modifier(int ID, int column, const QString& valeur);
    bool mettreAJourQuantite(int ID_Produit, int nouvelleQuantite);

    // Nouvelle méthode pour gérer les soumissions
   bool ajouterSoumission(int produit_id, int quantite_soumise, QDate date_soumission);  // Inclut la date de soumission
   QSqlQueryModel* afficherSoumissions();

    void setID_Produit(int id) { ID_Produit = id; }
};

#endif // STOCK_H
