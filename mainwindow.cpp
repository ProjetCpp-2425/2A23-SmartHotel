#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stock.h"
#include <QMessageBox>
#include <QDate>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
        Stock S;

        connect(ui->lineEdit_9, &QLineEdit::returnPressed, this, &MainWindow::rechercherProduit);
        connect(ui->pushButton_41, &QPushButton::clicked, this, &MainWindow::on_pushButton_41_clicked);
        connect(ui->pushButton_44, &QPushButton::clicked, this, &MainWindow::on_pushButton_44_clicked);
        connect(ui->pushButton_42, &QPushButton::clicked, this, &MainWindow::on_pushButton_42_clicked);
        connect(ui->pushButton_56, &QPushButton::clicked, this, &MainWindow::soumettreProduit);

        ui->tableView_2->setModel(S.afficher());  // Affiche les produits en stock
        modelSoumissions = new QSqlQueryModel(this);  // Initialise le modèle pour les soumissions
    }

    MainWindow::~MainWindow()
    {
        delete ui;
    }
// Fonction pour ajouter un produit dans la table des produits en stock
void MainWindow::on_pushButton_44_clicked()
{
    QString Nom_Produit = ui->lineEdit_74->text();
    QString Type_Produit = ui->comboBox_49->currentText();
    QString Conditions_Environnementales = ui->comboBox_48->currentText();

    bool idValid, quantiteValid, delaiValid;
    int ID_Produit = ui->lineEdit_73->text().toInt(&idValid);
    int Quantite = ui->lineEdit_80->text().toInt(&quantiteValid);
    int Delai = ui->lineEdit_78->text().toInt(&delaiValid);
    QDate Date_Darrivee = ui->dateEdit_5->date();

    if (Nom_Produit.isEmpty() || !idValid || !quantiteValid || !delaiValid) {
        QMessageBox::critical(this, QObject::tr("Erreur"), QObject::tr("Veuillez remplir tous les champs correctement."));
        return;
    }

    Stock S(ID_Produit, Nom_Produit, Type_Produit, Conditions_Environnementales, Quantite, Date_Darrivee, Delai);
    if (S.ajouter()) {
        QMessageBox::information(this, QObject::tr("OK"), QObject::tr("Ajout effectué."));
        ui->tableView_2->setModel(S.afficher()); // Rafraîchit la vue des produits en stock
    } else {
        QMessageBox::critical(this, QObject::tr("Erreur"), QObject::tr("Ajout non effectué."));
    }
}


void MainWindow::on_pushButton_41_clicked() {
    QModelIndexList selectedRows = ui->tableView_2->selectionModel()->selectedRows();

    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "Suppression", "Veuillez sélectionner un produit à supprimer.");
        return;
    }

    int id = selectedRows.first().data().toInt();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", "Voulez-vous vraiment supprimer ce produit ?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        Stock S;
        S.setID_Produit(id);
        if (S.supprimer()) {
            QMessageBox::information(this, "Suppression", "Produit supprimé avec succès.");
            ui->tableView_2->setModel(S.afficher());
        } else {
            QMessageBox::critical(this, "Erreur", "La suppression a échoué.");
        }
    }
}

void MainWindow::rechercherProduit() {
    QString Nom_Produit = ui->lineEdit_9->text();

    Stock S;
    QSqlQueryModel* model = S.chercherParNom(Nom_Produit);

    if (model) {
        ui->tableView_2->setModel(model);
    } else {
        QMessageBox::warning(this, "Erreur", "Erreur lors de la recherche.");
    }
}

void MainWindow::on_pushButton_42_clicked() {
    QModelIndex currentIndex = ui->tableView_2->selectionModel()->currentIndex();

    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Modification", "Veuillez sélectionner un attribut du produit à modifier.");
        return;
    }

    int column = currentIndex.column();
    QString IDProduit = ui->tableView_2->model()->data(currentIndex.siblingAtColumn(0)).toString();
    bool ok;
    QString ancienneValeur = ui->tableView_2->model()->data(currentIndex).toString();
    QString nouvelleValeur = QInputDialog::getText(this, tr("Modification"), tr("Saisissez la nouvelle valeur :"), QLineEdit::Normal, ancienneValeur, &ok);

    if (ok && !nouvelleValeur.isEmpty()) {
        int id = IDProduit.toInt();

        Stock S;
        if (S.modifier(id, column, nouvelleValeur)) {
            QMessageBox::information(this, "Modification réussie", "Le produit a été modifié avec succès.");
            ui->tableView_2->setModel(S.afficher());
        } else {
            QMessageBox::critical(this, "Erreur", "La modification du produit a échoué.");
        }
    }
}

void MainWindow::soumettreProduit() {
    bool idValid, quantiteValid;
    int produit_id = ui->lineEdit_84->text().toInt(&idValid);
    int quantite_soumise = ui->lineEdit_86->text().toInt(&quantiteValid);
    QDate date_soumission = ui->dateEdit_7->date();

    if (!idValid || !quantiteValid) {
        QMessageBox::critical(this, "Erreur", "Veuillez remplir tous les champs obligatoires.");
        return;
    }

    qDebug() << "Soumission : produit_id =" << produit_id << ", quantite_soumise =" << quantite_soumise << ", date_soumission =" << date_soumission;

    Stock S;
    if (S.ajouterSoumission(produit_id, quantite_soumise, date_soumission)) {
        mettreAJourQuantiteStock(produit_id, quantite_soumise);
        modelSoumissions = S.afficherSoumissions();
        ui->tableView_6->setModel(modelSoumissions);
        QMessageBox::information(this, "Soumission", "Produit soumis avec succès.");
    } else {
        QMessageBox::critical(this, "Erreur", "La soumission du produit a échoué.");
    }
}



void MainWindow::mettreAJourQuantiteStock(int id, int quantite) {
    Stock S;
    QAbstractItemModel *model = ui->tableView_2->model();

    for (int row = 0; row < model->rowCount(); ++row) {
        int idStock = model->data(model->index(row, 0)).toInt();
        if (idStock == id) {
            int ancienneQuantite = model->data(model->index(row, 4)).toInt();
            int nouvelleQuantite = ancienneQuantite - quantite;
            S.mettreAJourQuantite(id, nouvelleQuantite);

            ui->tableView_2->setModel(S.afficher());  // Rafraîchit la vue des produits en stock
            break;
        }
    }
}

