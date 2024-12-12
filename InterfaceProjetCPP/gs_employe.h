#ifndef GS_EMPLOYE_H
#define GS_EMPLOYE_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDate>

class GS_EMPLOYE
{
    int ID_employe;
    QString nom;
    QString prenom;
    float salaire;
    QDate date_embauche;
    int num_telephone;
    QString post;
    QString etat; // Présent ou Absent
    QString mail;
    QString password;
    QString rfid;






public:
    GS_EMPLOYE() {}
    GS_EMPLOYE(int, QString, QString, float, QDate, int, QString, QString, QString, QString, QString );



    int getID_employe() const { return ID_employe; }
    void setID_employe(int ID_employe) { this->ID_employe = ID_employe; }

    int getNum_telephone() const { return num_telephone; }
    void setNum_telephone(int num_telephone) { this->num_telephone = num_telephone; }

    float getSalaire() const { return salaire; }
    void setSalaire(float salaire) { this->salaire = salaire; }

    QString getNom() const { return nom; }
    void setNom(const QString& n) { nom = n; }

    QString getMail() const { return mail; }
    void setMail(const QString& m) { mail = m; }

    QString getPassword() const { return password; }
    void setPassword(const QString& ps) { password = ps; }


    QString getEtat() const { return etat; }
    void setEtat(const QString& e) { etat = e; }

    QString getPrenom() const { return prenom; }
    void setPrenom(const QString& p) { prenom = p; }

    QString getRfid() const { return rfid; }
    void setRfid(const QString& rf) { rfid = rf; }

    QString getPost() const { return post; }
    void setPost(const QString& ps) { post = ps; }

    QDate getDate_embauche() const { return date_embauche; }
    void setDate_embauche(const QDate& date) { date_embauche = date; }


    bool verifierSaisie();
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer();
    bool modifier();
    bool chargerDonneesEmploye(int id);

    QSqlQueryModel* chercherParNom(const QString& nom);
    QSqlQueryModel* chercherParPoste(const QString& poste);
    QSqlQueryModel* chercherParEtat(const QString& etat);

    QSqlQueryModel* trierParDateEmbauche();
    QSqlQueryModel* trierParSalaireDecroissant();


    int calculerTotalEmployes();
    QList<QDate> getAbsences(int id_employe);

QString getPosteById(int idEmploye);


};




#endif // GS_EMPLOYE_H





