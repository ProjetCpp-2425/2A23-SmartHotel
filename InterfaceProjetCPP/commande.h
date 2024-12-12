#ifndef COMMANDE_H
#define COMMANDE_H
#include "connexion.h"
#include <QMessageBox>
#include<QString>
#include<QDateTime>
#include<QSqlQuery>
#include<QSqlQueryModel>
#include <QInputDialog>
#include <QSqlTableModel>
#include <QPrinter>
#include <QTableView>
#include <QStandardItemModel>
#include <QImage>
#include <QPainter>
#include <QHeaderView>
#include <QFileDialog>
#include <QrCodeGenerator.h>
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

using namespace std;

class Commande
{
private:
    int QuantiteProduit, MatriculeFiscale, NumeroTelephone;
    QString NomEntreprise, TypeCommande,Adresse, EtatCommande;
    float PrixCommande;
    QDateTime DateLivraison;


public:
    QrCodeGenerator Generator;
    Commande(){};
    Commande(QString NomEntreprise, QString TypeCommande, int QuantiteProduit, int MatriculeFiscale, int NumeroTelephone, QString Adresse,
             float PrixCommande, QString EtatCommande, QDateTime DateLivraison);

    int GetQuantiteProduit(){return QuantiteProduit;};
    int GetMatriculeFiscale(){return MatriculeFiscale;};
    int GetNumeroTelephone(){return NumeroTelephone;};
    QString GetNomEntreprise(){return NomEntreprise;};
    QString GetTypeCommande(){return TypeCommande;};
    QString GetAdresse(){return Adresse;};
    QString GetEtatCommande(){return EtatCommande;};
    float GetPrixCommande(){return PrixCommande;};
    QDateTime GetDateLivraison(){return DateLivraison;};


    bool AjouterCommande();
    QSqlQueryModel* AfficherCommande();
    bool ModifierCommande(int ID, int Column, QString ch);
    bool SupprimerCommande(int Code);

    bool VerifChaine(const std::string& ch);
    QrCodeGenerator GenererQR;

    QSqlQueryModel * TriDateLivraisonR();
    QSqlQueryModel * TriDateLivraisonA();
    QSqlQueryModel * TriPrix();
    QSqlQueryModel * TriNomEntreprise();

    QSqlQueryModel * CategorieEquipement();
    QSqlQueryModel * CategorieAlimentation();
    QSqlQueryModel * CategorieUniformes();
    QSqlQueryModel * CategorieDecorations();
    QSqlQueryModel * CategorieFournitures();
    QSqlQueryModel * EtatPayee();
    QSqlQueryModel * EtatNonPayee();

    QString SommeEquipement();
    QString SommeAlimentation();
    QString SommeUniforme();
    QString SommeFournitures();
    QString SommeDecorations();
    QString SommeCommandeTraitee();
    QString SommeCommandeNonTraitee();
    int countTotalCommandes();

    QSqlQueryModel * RechercheNomEntreprise(QString ch);

    QString CommandeSemaine();


};

#endif // COMMANDE_H
