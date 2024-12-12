#ifndef GS_CONGE_H
#define GS_CONGE_H


#include <QString>
#include <QDate>

class GS_CONGE {
private:
    int ID_EMPLOYE;
    QDateTime debut;
    QDateTime fin;
    QString type;

public:
    // Constructeur


    GS_CONGE() {}
    GS_CONGE(int, QDateTime, QDateTime, QString );

    // Getter et Setter pour ID_EMPLOYE
    int getIdEmploye() const { return ID_EMPLOYE; }
    void setIdEmploye(int id) { ID_EMPLOYE = id; }

    // Getter et Setter pour dateDebut
    QDateTime getDebut() const { return debut; }
    void setDebut(const QDateTime &d) { debut = d; }

    // Getter et Setter pour dateFin
    QDateTime getFin() const { return fin; }
    void setFin(const QDateTime &f) { fin = f; }

    // Getter et Setter pour type
    QString getType() const { return type; }
    void setType(const QString &type) { this->type = type; }

//void ajouterConge(const QDateTime &debut, const QDateTime &fin, const QString& type);
bool ajouterConge(const QDateTime &debut, const QDateTime &fin, const QString& type);
bool verifierCongeParPosteEtDate(int idEmploye, const QDateTime& debut, const QDateTime& fin);

};


#endif // GS_CONGE_H
