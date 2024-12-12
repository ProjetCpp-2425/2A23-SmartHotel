#include "chambre.h"
#include <QSqlQuery>
#include <QDebug>
#include <QPdfWriter>
#include <QPainter>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>

Chambre::Chambre(int id, QString type_chambre, float PPN, int dispo, int DR)
    : ID_CHAMBRE(id), type_chambre(type_chambre), PPN(PPN), dispo(dispo), DR(DR) {}

bool Chambre::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO chambre(ID_CHAMBRE, type_chambre, PPN, dispo, DR) "
                  "VALUES (:ID_CHAMBRE, :type_chambre, :PPN, :dispo, :DR)");
    query.bindValue(":ID_CHAMBRE", ID_CHAMBRE);
    query.bindValue(":type_chambre", type_chambre);
    query.bindValue(":dispo", dispo);
    query.bindValue(":PPN", PPN);
    query.bindValue(":DR", DR);

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'ajout du chambre:" << query.lastError().text();
        return false;
    }
    return true;
    if (query.exec()) {
        enregistrerHistorique(ID_CHAMBRE, "Ajout");
        return true;
    }
}
double Chambre::calculatePercentageAvailableRooms() {
    QSqlQuery query;
    int totalRooms = 0;
    int availableRooms = 0;

    query.prepare("SELECT COUNT * FROM CHAMBRE");
    if (query.exec()) {
        if (query.next()) {
            totalRooms = query.value(0).toInt();
        }
    } else {
        qDebug() << "Error executing query for total rooms: " << query.lastError().text();
        return 0.0;
    }

    query.prepare("SELECT COUNT *  FROM CHAMBRE WHERE DISPO = 1");
    if (query.exec()) {
        if (query.next()) {
            availableRooms = query.value(0).toInt();
        }
    } else {
        qDebug() << "Error executing query for available rooms: " << query.lastError().text();
        return 0.0;
    }


    double percentage = (static_cast<double>(availableRooms) / totalRooms) * 100;
    return percentage;
}


QSqlQuery Chambre::afficher()
{
    QSqlQuery query;
    query.prepare("SELECT * FROM chambre");
    if (!query.exec()) {
        qDebug() << "Erreur lors de l'affichage des chambres:" << query.lastError().text();
    }
    return query;
}

bool Chambre::supprimer(int ID_CHAMBRE)
{
    QSqlQuery query;
    query.prepare("DELETE FROM chambre WHERE ID_CHAMBRE = :ID_CHAMBRE");
    query.bindValue(":ID_CHAMBRE",ID_CHAMBRE);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la suppression de la chambre:" << query.lastError().text();
        return false;
    }
    return true;
    if (query.exec()) {
        enregistrerHistorique(ID_CHAMBRE, "Suppression");
        return true;
    }

}
bool Chambre::genererRapportPDFToutesLesChambres(const QList<Chambre> &chambres, const QString &filePath) {
    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setTitle("Rapport de Toutes les Chambres");

    QPainter painter(&pdfWriter);
    if (!painter.isActive()) {
        qWarning() << "Erreur lors de la création du PDF.";
        return false;
    }

    painter.setFont(QFont("Helvetica", 10));
    int yPosition = 100;
    painter.drawText(100, yPosition, "Liste de toutes les chambres :");
    yPosition += 50;

    for (const Chambre &chambre : chambres) {
        painter.drawText(100, yPosition, QString("ID: %1, Type: %2, PPN: %3, Disponibilité: %4, DR: %5")
                                       .arg(chambre.getID_CHAMBRE())
                                       .arg(chambre.type_chambre)
                                       .arg(chambre.PPN)
                                       .arg(chambre.dispo == 1 ? "Disponible" : "Non disponible")
                                       .arg(chambre.DR));
        yPosition += 50;
        if (yPosition > pdfWriter.height() - 100) {
            pdfWriter.newPage();
            yPosition = 100;
        }
    }

    painter.end();
    return true;
}



