#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QString>
#include "connexion.h"
#include <QMainWindow>
#include <QMessageBox>
#include <QInputDialog>
#include <QSqlTableModel>
#include <QDebug>
#include <QPrinter>
#include <QPainter>
#include <QTableView>
#include <QHeaderView>
#include <QFileDialog>
#include <QrCodeGenerator.h>
#include "commande.h"
#include "chambre.h"
#include "finance.h"
#include "gs_employe.h"
#include "gs_conge.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QStringListModel>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QIcon>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QLabel>
#include <QStandardItemModel>
#include <QSerialPort>        // Add this
#include <QSerialPortInfo>    // Add this
#include <QByteArray>         // Add this
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QHorizontalStackedBarSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <fstream>
#include <QLegendMarker>
#include <QKeyEvent>
#include <QCalendarWidget>
#include <QDialog>
#include <fstream>

#include <QItemSelection>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QtSvg/QSvgRenderer>
#include <QTimer>
#include <QLabel>
#include "smtp.h"
#include "hotelmanagerui.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QSystemTrayIcon *trayIcon;

private:
    Ui::MainWindow *ui;
    Commande CommandeTemp;

    void AfficherWidgetsIndice();
    void afficherChambresParDuree(const QString &dureeText);
    void statistiquesDashboard();

    void ExporterPDF(QTableView *TableViewCommande, QString &path);

    void StatistiqueChart();
    void StatistiqueCommandeTraitee();
    void StatistiqueText();
    void StatistiqueCommandeTotale();
    void VerifierModificationCommande(QString NouvelleValeur, int Column);
    QSqlQueryModel *model;
    Chambre ch;
    QStringListModel *historiqueModel;
    QStringList historiqueList;
    QMap<int, QString> oldValues;
    void loadHistoriqueFromFile();
    void saveHistoriqueToFile();

    QList<QList<QString>> produitsInsuffisants;
    void Equip_choix_pie2();
    QLabel *notificationLabel;

    // Add these new private members for Arduino
    QSerialPort *arduino;
    QString arduino_port_name;
    void setupArduino();
    static const quint16 arduino_uno_vendor_id = 9025;
    static const quint16 arduino_uno_product_id = 67;
    QByteArray serialData;
    QString serialBuffer;
    bool arduino_available;

    QLabel *alertLabel;

    void Equip_choix2_pie();
    void Equip_choix2_pie2();

    void UpdateFinance();
    void UpdateEmploye();

    finance F;
    GS_EMPLOYE E;
    QLineEdit *lineEdit_4;
    HotelManagerUI *hotelManagerUI;

    QWidget *widget_0;

 QString receivedData;
 QSerialPort *serialPort;
    QTimer *dailyTimer;
    QTime startTime; // Heure de début pour le calcul
       int currentEmployeeId;



private slots:
    void on_PushButtonAjouterCommande_clicked();
    void on_BoutonModifier_clicked();
    void on_BoutonSupprimer_clicked();
    void on_ComboBoxTri_currentTextChanged(const QString &arg1);
    void on_ComboBoxCategorie_currentTextChanged(const QString &arg2);
    void on_comboBox_13_currentTextChanged(const QString &arg1);
    void on_LineEditNomEntreprise_2_textChanged(const QString &arg1);
    void on_BoutonExporterPDF_clicked();
    void on_BoutonExporterPDF_2_clicked();

    void on_BoutonSMS_clicked();
    void on_pushButton_1000_clicked();
    void afficherChambres();
    void on_pushButton_10_clicked();
    void onCellChanged(int row, int column); // Nouveau slot pour la modification en ligne
    void tridisponibilite();
    void triduree();
    void on_pushButton_12_clicked();
    void rechercherParID(const QString &idText);
    void on_pushButton_33_clicked();
    void afficherStatistiques();

    void on_pushButton_41_clicked();
    void on_pushButton_42_clicked();
    void on_pushButton_44_clicked();
    void on_pushButton_43_clicked();
    void on_pushButton_57_clicked();
    void rechercherProduit();
    void appliquerTri();
    void ExporterProduitsInsuffisantsPDF(const QString &path);
    void setupTableWidgetWithColumns(QTableWidget *table);
    void addProductToTableWithDetails(QTableWidget *table, const QString &nomProduit, const QString &quantite, const QString &condition);
    void afficherMessageSiVide(QTableWidget *table, const QString &message);
    void verifierQuantiteCritique();
    void calculerPourcentageTypesProduits();
    void calculerPourcentageProduitsAjoutesEtSoumis();
    void on_pushButton_45_clicked();
    void afficherProduitsParCondition(const QString &condition, const QStringList &produits);
    void addProductToTable(QTableWidget *table, const QString &nomProduit);
    void setupTableWidget(QTableWidget *table);
    void onProduitClicked(const QModelIndex &index);
    // Add these new slots for Arduino
    void readSerial();
    void processSerialData(const QString &data);

    void on_pushButton_36_clicked();
    //void rechercherfinance();
  // void on_pushButton_18_clicked(); //supprimer
   // void on_pushButton_19_clicked(); //modifier
    void on_tabChanged(int index);  // Slot pour le changement d'onglet


    void on_finance_tri_montant_clicked();
    void on_finance_tri_statut_clicked();

    void on_finance_PDF_pushButton_clicked();

    void Equip1_choix_pie();
    void Equip1_choix_pie2();

    void on_finance_modify_pushButton_clicked();

    void on_finance_tableView_clicked(const QModelIndex &index);


    void on_ajouter_finance_pushButton_clicked();

    void on_delete_finance_pushButton_clicked();
    void clearFinanceFields();

    void on_sendmail_finanace_clicked();
    void generateQRCode(const QString &data);

    void on_supprimeralerte_finance_clicked();
    void Function_Mailing();

   void on_pushButton_13_clicked();


    void on_BoutonAjouterE_clicked(); // bouton ajouter un employé
    void on_BSupprimerEmployee_clicked(); // bouton supprimer un employé
    void on_pushButton_charger_clicked(); // charger les données d'employés

    void chargerDonneesEmploye(const GS_EMPLOYE& employe);

    void rechercherEmploye();
    void rechercherParPoste(const QString& poste);
    void rechercherParEtat(const QString& etat);

    void trierParDateEmbauche();
    void trierParSalaireDecroissant();
    void on_ComboSalaireE_currentIndexChanged(const QString& texte);// salaire/date_emb


    void mettreAJourTotalEmployes();

    void showCalendar(); // Ajouter une méthode pour afficher le calendrier
    void mettreAJourCalendrier(int id_employe);
    void on_tableEmployees_clicked1(const QModelIndex& index);
    void StatistiquesEmployee();
   //int calculerTotalAbsences(const QDate& premiereDateDuMois, const QDate& dernierJourDuMois);
    double calculerPourcentageAbsences(){};
    int calculerNombreAbsents();
    int calculerNombreConges();
    int calculerTotalEmployesadate();


    void setupComboBoxes();

    QPair<QDate, QDate> getDateRangeFromComboBoxes();//stat

    void onComboBox40Changed(const QString &text);// stat mois
    void onComboBox39Changed(const QString &text);// stat annes

    void ExporterTableViewPDF(const QString &path);//PDF
    void on_BExporterPDFEmployee_clicked();//pdf

    void on_BModifierEmployee_clicked();//modification
    void on_tableEmployees_clicked(const QModelIndex &index);// modification

    void afficherGraphiqueAbsences();

    bool calculerNombreJoursConge();
    //void on_pushButton_13_clicked();
    void afficherCongesEmploye(int idEmploye);
    //void onTableView14SelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
    void on_tableEmployees_clicked16(const QModelIndex& index);


    void on_lineEdit_6_textChanged(const QString &arg1);
    void on_BoutonValider_clicked();


void readSerialData();

void checkAndUpdateRFID(const QString &RFID);
void on_pushButton_SaveRFID_clicked();

//void updatePresence(); // Méthode pour mettre à jour la table PRESENCE
    //void scheduleDailyUpdate(); // Méthode pour planifier l'exécution à minuit
//void scheduleMinuteUpdate();
//void copyEmployeesToPresence();
bool copyEmployeesToPresence();

void on_BoutonDebutE_clicked();
void on_BoutonFinE_clicked();

void on_BAjouterEmployee_clicked();


void on_Hotel_Program_clicked();

void on_SMSChambre_clicked();
void on_pushButton_9_clicked();



void on_finance_recherche_textChanged(const QString &arg1);

void on_BoutonAjouter_clicked();

protected:
    void keyPressEvent(QKeyEvent *event) override; // Déclaration de la méthode
};
#endif // MAINWINDOW_H
