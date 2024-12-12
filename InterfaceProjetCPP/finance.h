#ifndef FINANCE_H
#define FINANCE_H
#include "connexion.h"
#include<QString>
#include<QSqlQuery>
#include <QSqlQueryModel>
#include <QDate>
#include <QSqlError>
#include <QDebug>
#include <QTableView>



class finance
{
private:
    int Matricule_Transaction;
    QString Type_Transaction;
    QString Categorie;
    QDate Date_Transaction;
    QString Mode_Paiement;
    QString Statut;
    double Montant;


public:

    finance(); // Default constructor
    finance(int matricule_transaction, QString type_transaction, QString categorie, QDate date_transaction, QString mode_paiement, QString statut, double montant); // Parameterized constructor

    //Getters
        int getMatricule_Transaction() const { return Matricule_Transaction; }
        QDate getDate_Transaction() const { return Date_Transaction; }
        double getMontant() const { return Montant; }
        QString getType_Transaction() const { return Type_Transaction; }
        QString getCategorie() const { return Categorie; }
        QString getMode_Paiement() const { return Mode_Paiement; }
        QString getStatut() const { return Statut; }



    //Setters

        void setMatricule_Transaction(int matricule) { Matricule_Transaction = matricule; }
        void setDate_Transaction(QDate date) { Date_Transaction = date; }
        void setMontant(double montant) { montant = montant; }
        void setCategorie(QString categorie) { categorie = categorie; }
        void setMode_Paiement(QString mode_Paiement) { mode_Paiement = mode_Paiement; }
        void setStatut(QString statut) { statut = statut; }
    // Fonctionnalités de Base relatives à l'entité finance


    bool ajouter();
    bool modifier(int matriculeTransaction, int column, const QString& valeur);
    QSqlQueryModel* afficher();
    QSqlQueryModel* tri_montant();
    QSqlQueryModel* tri_statut();
    void clearTable(QTableView * table);
     void Recherche(QTableView *table, QString matricule, double Montant);
    QString read();
    void write(QString,QString);
    QString time();
    void clearh();
    double sumRevenu();

};


#endif // FINANCE_H
