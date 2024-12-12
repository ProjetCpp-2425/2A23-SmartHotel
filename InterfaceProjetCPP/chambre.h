#ifndef CHAMBRE_H
#define CHAMBRE_H
#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
class Chambre
{
    int ID_CHAMBRE;
    QString type_chambre;
    float PPN;
    int dispo;
    int DR;

public:
    Chambre() {}
    Chambre(int, QString, float, int, int);
    int getID_CHAMBRE() const { return ID_CHAMBRE; }

    void setID_CHAMBRE(int ID_CHAMBRE) { this->ID_CHAMBRE = ID_CHAMBRE; }

    bool ajouter();
    bool supprimer(int id);
    double calculatePercentageAvailableRooms();

    QSqlQuery afficher(); // Méthode pour l'affichage
    static bool genererRapportPDFToutesLesChambres(const QList<Chambre> &chambres, const QString &filePath);
    void enregistrerHistorique(int idChambre, const QString &action, const QString &champ = "",
                               const QString &ancienneValeur = "", const QString &nouvelleValeur = "",
                               const QString &utilisateur = "admin");

};

#endif
