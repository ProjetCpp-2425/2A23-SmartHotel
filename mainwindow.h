#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "stock.h"
#include <QMainWindow>
#include <QSqlQueryModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_44_clicked();          // Fonction pour l'ajout de produit
    void on_pushButton_42_clicked();          // Fonction pour la modification de produit
    void rechercherProduit();                 // Fonction pour la recherche de produit
    void on_pushButton_41_clicked();          // Fonction pour la suppression de produit
    void soumettreProduit();                  // Fonction pour la soumission de produit
    void mettreAJourQuantiteStock(int id, int quantite); // Mise à jour de la quantité dans le stock

private:
    Ui::MainWindow *ui;
    QSqlQueryModel *modelSoumissions;         // Modèle pour afficher les soumissions
};

#endif // MAINWINDOW_H
