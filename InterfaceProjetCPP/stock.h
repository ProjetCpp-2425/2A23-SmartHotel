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
    QSqlQueryModel*afficherSoumissions();
    QSqlQueryModel* chercherParNom(const QString& Nom_Produit);
    QSqlQueryModel* chercherParID(int ID_Produit);
    QSqlQueryModel* chercherParConditions(const QString& conditions);

    bool supprimer();
    bool modifier(int ID, int column, const QString& valeur);

bool ajouterSoumission(int ID_Produit, int Quantite_Soumise, QDate Date_Soumission);
bool mettreAJourQuantite(int ID_Produit, int nouvelleQuantite);


    QSqlQueryModel* trierParDelai();
    QSqlQueryModel* trierParQuantite();
    QSqlQueryModel* trierParNomProduit();


    void setID_Produit(int id) { ID_Produit = id; }
    void setQuantite(int quantite) { Quantite = quantite; }
    void setDelai(int delai) { Delai = delai; }
};

#endif // STOCK_H
