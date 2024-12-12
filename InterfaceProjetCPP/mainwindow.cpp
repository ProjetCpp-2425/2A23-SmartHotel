#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"commande.h"
#include "stock.h"
#include "interfacelogin.h"
#include "fenetremessage.h"
#include "chambre.h"
#include <QThread>
#include <QSqlRecord>
#include <QFont>
#include <QUrlQuery>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarCategoryAxis>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QHorizontalStackedBarSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChart>
#include "gs_employe.h"
#include "gs_conge.h"
#include <QShortcut>
#include <QKeySequence>
#include <QKeyEvent>


#include <QGraphicsEllipseItem>
#include <QVBoxLayout>
#include <QWidget>

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

#include <QItemSelection>
#include <QSerialPort>

#include <QRegularExpression>

#include <QTimer>
#include <QDateTime>
#include <QDesktopServices>
#include <QDir>
#include <fstream>
#include <QLegendMarker>
int congéRestant=0;
int clique=0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
  model(new QSqlQueryModel(this)),
  historiqueModel(new QStringListModel(this)),serialPort(new QSerialPort(this)), dailyTimer(new QTimer(this))
{

    ui->setupUi(this);

    AfficherWidgetsIndice();
    ui->TableViewCommande->setModel(CommandeTemp.AfficherCommande());
    ui->TableViewCommande->setColumnHidden(1, true);
    StatistiqueCommandeTotale();
    loadHistoriqueFromFile();

    connect(ui->tableWidget_7, &QTableWidget::cellClicked, this, [=](int row, int column) {
        int ID_CHAMBRE = ui->tableWidget_7->item(row, 0)->text().toInt();
        QString oldValue = ui->tableWidget_7->item(row, column)->text();
        oldValues[ID_CHAMBRE * 10 + column] = oldValue;
    });

    statistiquesDashboard();
    ui->tableWidget_7->setColumnWidth(1, 200);
    historiqueModel = new QStringListModel(this);
    ui->listView_Historique->setModel(historiqueModel);
    ui->listView_Historique->setVisible(false);
    ui->tableWidget_7->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
    connect(ui->comboBox_28, &QComboBox::currentTextChanged, this, &MainWindow::tridisponibilite);
    connect(ui->comboBox_25, &QComboBox::currentTextChanged, this, &MainWindow::triduree);
    afficherChambres();
    afficherStatistiques();
    ui->listView_Historique->show();

    notificationLabel = new QLabel(this);
    notificationLabel->setStyleSheet("color: green; font-weight: bold; background-color: white;");
    notificationLabel->setGeometry(10, 10, 300, 30);
    notificationLabel->setAttribute(Qt::WA_TranslucentBackground);
    notificationLabel->hide();
    Stock S;
    ui->comboBox_47->clear();
    ui->comboBox_47->addItem("Nom");
    ui->comboBox_47->addItem("Délai");
    ui->comboBox_47->addItem("Quantite");

   connect(ui->tableView_2, &QTableView::clicked, this, &MainWindow::onProduitClicked);


    connect(ui->lineEdit_9, &QLineEdit::returnPressed, this, &MainWindow::rechercherProduit);
    connect(ui->pushButton_41, &QPushButton::clicked, this, &MainWindow::on_pushButton_41_clicked);
   // connect(ui->pushButton_44, &QPushButton::clicked, this, &MainWindow::on_pushButton_44_clicked);
    connect(ui->pushButton_42, &QPushButton::clicked, this, &MainWindow::on_pushButton_42_clicked);
    //connect(ui->pushButton_43, &QPushButton::clicked, this, &MainWindow::on_pushButton_43_clicked);
    connect(ui->comboBox_47, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::appliquerTri);
    ui->tableView_2->setModel(S.afficher());
    ui->tableView_6->setModel(S.afficherSoumissions());

   calculerPourcentageTypesProduits();

   calculerPourcentageProduitsAjoutesEtSoumis();
   show();



    QTimer::singleShot(0, this, &MainWindow::verifierQuantiteCritique);

    //connect(ui->finance_modify_pushButton, &QPushButton::clicked, this, &MainWindow::on_finance_modify_pushButton_clicked);
    MainWindow::connect(ui->sendmail_finanace, SIGNAL(clicked()),this, SLOT(sendMail()));
    ui->tb_Alertsss->setText(F.read());


    UpdateFinance();
    UpdateEmploye();


    /*

    QGraphicsScene *scene = new QGraphicsScene();

    // Ajoutez un élément graphique (par exemple, un cercle)
    QGraphicsEllipseItem *ellipse = new QGraphicsEllipseItem(0, 0, 100, 100);
    ellipse->setBrush(Qt::green);  // Couleur de l'ellipse
    scene->addItem(ellipse);

    // Créez un QGraphicsView pour afficher la scène
    QGraphicsView *view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing);

    // Ajoutez le QGraphicsView à la fenêtre
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(view);
    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

*/
// scheduleMinuteUpdate();








 connect(ui->BoutonAjouterE, &QPushButton::clicked, this, &MainWindow::on_BoutonAjouterE_clicked);
 connect(ui->SearchBarEmploye, &QLineEdit::returnPressed, this, &MainWindow::rechercherEmploye);
 connect(ui->BSupprimerEmployee, &QPushButton::clicked, this, &MainWindow::on_BSupprimerEmployee_clicked);
 connect(ui->ComboPosteE, SIGNAL(currentTextChanged(const QString&)), this, SLOT(rechercherParPoste(const QString&)));
 connect(ui->ComboEtatE, SIGNAL(currentTextChanged(const QString&)), this, SLOT(rechercherParEtat(const QString&)));
 connect(ui->ComboSalaireE, SIGNAL(currentTextChanged(const QString&)), this, SLOT(trierParDate(const QString&)));
 connect(ui->ComboSalaireE, &QComboBox::currentTextChanged, this, &MainWindow::on_ComboSalaireE_currentIndexChanged);




connect(ui->BModifierEmployee, &QPushButton::clicked, this, &MainWindow::on_BModifierEmployee_clicked);
 connect(ui->BExporterPDFEmployee, &QPushButton::clicked, this, &MainWindow::on_BExporterPDFEmployee_clicked);



 //connect(ui->pushButton_13, &QPushButton::clicked, this, &MainWindow::on_pushButton_13_clicked);

 connect(ui->BAjouterEmployee, &QPushButton::clicked, this, &MainWindow::on_BAjouterEmployee_clicked);

 connect(ui->BoutonDebutE, &QPushButton::clicked, this, &MainWindow::on_BoutonDebutE_clicked);
 connect(ui->BoutonFinE, &QPushButton::clicked, this, &MainWindow::on_BoutonFinE_clicked);

 connect(ui->BoutonValider, &QPushButton::clicked, this, &MainWindow::on_BoutonValider_clicked);



foreach (const QSerialPortInfo &portInfo, QSerialPortInfo::availablePorts()) {
         qDebug() << "Found port:" << portInfo.portName();
     }
foreach (const QSerialPortInfo &portInfo, QSerialPortInfo::availablePorts()) {
     qDebug() << "Port disponible :" << portInfo.portName();
 }

 serialPort->setPortName("COM3");
 serialPort->setBaudRate(QSerialPort::Baud9600);
 serialPort->setDataBits(QSerialPort::Data8);
 serialPort->setParity(QSerialPort::NoParity);
 serialPort->setStopBits(QSerialPort::OneStop);
 serialPort->setFlowControl(QSerialPort::NoFlowControl);


 connect(serialPort, &QSerialPort::readyRead, this, &MainWindow::readSerialData);


}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::AfficherWidgetsIndice()
{
    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget_5->setCurrentIndex(0);
    ui->tabWidget_2->setCurrentIndex(0);
    ui->tabWidget_10->setCurrentIndex(0);
    ui->tabWidget_9->setCurrentIndex(0);
    ui->tablewidget->setCurrentIndex(0);

}

void MainWindow::statistiquesDashboard()
{
    double revenu = F.sumRevenu();
    QString formattedRevenu = QLocale().toString(revenu, 'f', 2);  // Format with 2 decimal places
    ui->label_158->setText(QString("%1 DT").arg(formattedRevenu));
    ui->label_153->setText(QString("%1").arg(CommandeTemp.countTotalCommandes()));
    int totalEmployes = calculerTotalEmployesadate();

    ui->label_10->setText(QString("%1").arg(totalEmployes));


}


void MainWindow::ExporterPDF(QTableView *TableViewCommande, QString &path)
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(path);

    QPainter painter(&printer);
    QFont titleFont = painter.font();

    QImage logo(":/Purestay/images/interface/Logo PureStay Final.png");

    painter.drawImage(QRect(2400, 100, 1500, 1500), logo);

    titleFont.setPointSize(45);
    painter.setFont(titleFont);
    painter.setPen(QColor(161, 78, 77));
    painter.drawText(3700, 1200, "Purestay");

    QFont headerFont = painter.font();
    headerFont.setPointSize(8);
    painter.setFont(headerFont);

    int NombreLignes = TableViewCommande->model()->rowCount();
    int NombreColonnes = TableViewCommande->model()->columnCount();

    int headerOffsetY = 1900;
    int rowHeight = 575;
    int columnWidth = 1060;
    int Debut = 450;

    painter.setPen(QColor(161, 78, 77));
    for (int i = 0; i < NombreColonnes; ++i)
    {
        if(i != 1 && i != 6)
        {
            QString header = TableViewCommande->model()->headerData(i, Qt::Horizontal).toString();

            int columnOffset = i - (i > 1 ? 1 : 0) - (i > 6 ? 1 : 0);

            QFont boldFont = painter.font();
            boldFont.setBold(true);
            painter.setFont(boldFont);

            painter.drawText(360 + columnOffset * columnWidth, headerOffsetY, header);

            painter.drawLine(Debut + columnOffset * columnWidth, headerOffsetY + 55,
                             Debut + (columnOffset + 1) * columnWidth, headerOffsetY + 55);

            boldFont.setBold(false);
            painter.setFont(boldFont);
        }
    }


    painter.setPen(Qt::black);

    for (int row = 0; row < NombreLignes; ++row)
    {
        int columnOffset = 0;
        for (int col = 0; col < NombreColonnes; ++col)
        {
            if(col != 1 && col != 6)
            {
                QString data = TableViewCommande->model()->data(TableViewCommande->model()->index(row, col)).toString();
                painter.drawText(420 + columnOffset * columnWidth, headerOffsetY + (row + 1) * rowHeight, data);
                columnOffset++;
            }
        }
    }

    painter.end();
}


void MainWindow::on_BoutonExporterPDF_clicked()
{

    QString filePath = QFileDialog::getSaveFileName(this, "Save PDF", "", "PDF Files (*.pdf)");
    if (!filePath.isEmpty())
    {
        ExporterPDF(ui->TableViewCommande, filePath);
    }
}

void MainWindow::on_PushButtonAjouterCommande_clicked()
{
    bool Test;
    bool TestPrix, TestNumero, TestQuantite;

    QString NomEntreprise = ui->LineEditNomEntreprise->text();
    int QuantiteProduit = ui->LineEditQuantiteProduit->text().toInt(&TestQuantite);
    int MatriculeFiscale = ui->LineEditMatriculeFiscale->text().toInt();
    int NumeroTelephone = ui->LineEditNumeroTelephone->text().toInt(&TestNumero);
    float PrixCommande = ui->LineEditPrix->text().toFloat(&TestPrix);

    QString Pays = ui->LineEditPays->text() + " " + ui->LineEditVille->text() + " " + ui->LineEditRue->text();

    QString TypeCommande = ui->ComboBoxTypeCommande->currentText();
    QString EtatCommande = ui->ComboBoxEtatCommande->currentText();

    QDateTime DateLivraison = ui->DateTimeDateLivraison->dateTime();


    Commande Commande(NomEntreprise, TypeCommande, QuantiteProduit, MatriculeFiscale, NumeroTelephone, Pays,
                      PrixCommande, EtatCommande, DateLivraison);

    if (NomEntreprise.isEmpty())
    {

        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                              QObject::tr("Le Nom de l'entreprise ne peut pas être vide \n Cliquez sur Cancel pour quitter."),
                              QMessageBox::Cancel);

    }

    if (NomEntreprise.toInt(&Test))
    {

        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                              QObject::tr("Le Nom de l'entreprise ne peut pas être un nombre \n Cliquez sur Cancel pour quitter."),
                              QMessageBox::Cancel);

    }
    if (PrixCommande<0)
    {
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                              QObject::tr("Le prix ne peut pas etre négatif. \nCliquez sur Cancel pour quitter."),
                              QMessageBox::Cancel);
    }

    if (!TestPrix)
    {
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                              QObject::tr("Prix Invalide. \nCliquez sur Cancel pour quitter."),
                              QMessageBox::Cancel);
    }
    if (NumeroTelephone<0)
    {
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                              QObject::tr("Le numéro de téléphone ne peut pas être négatif. \nCliquez sur Cancel pour quitter."),
                              QMessageBox::Cancel);
    }
    if ((!TestNumero) || (NumeroTelephone<100))
    {
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                              QObject::tr("Numero Invalide. \nCliquez sur Cancel pour quitter."),
                              QMessageBox::Cancel);
    }

    if (Commande.AjouterCommande())
    {
        ui->TableViewCommande->setModel(CommandeTemp.AfficherCommande());
        StatistiqueCommandeTotale();
        statistiquesDashboard();
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("La commande a été ajoutée avec succès.\nAppuyez sur Cancel pour quitter."),
                                 QMessageBox::Cancel);
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                              QObject::tr("Erreur ajout", "\nCliquez sur Cancel pour quitter."),
                              QMessageBox::Cancel);
    }


}

void MainWindow::on_BoutonModifier_clicked()
{

    int Column = ui->TableViewCommande->selectionModel()->currentIndex().column();
    QString IDComm = ui->TableViewCommande->model()->data(ui->TableViewCommande->selectionModel()->currentIndex().siblingAtColumn(1)).toString();
    bool Test;
    QString AncienneValeur = ui->TableViewCommande->model()->data(ui->TableViewCommande->selectionModel()->currentIndex().siblingAtColumn(0)).toString();

    QString NouvelleValeur = QInputDialog::getText(this, tr("Modification"),
                                              tr("Saisissez la nouvelle valeur:"), QLineEdit::Normal,
                                              AncienneValeur, &Test);
    int ID=IDComm.toInt();


    if (Test)
    {
        VerifierModificationCommande(NouvelleValeur,Column);
        if (Column == 9) {
            QDateTime VerifDate = QDateTime::fromString(NouvelleValeur, "yyyy/MM/dd HH:mm");

            if (!VerifDate.isValid()) {
                QMessageBox::critical(nullptr, QObject::tr("Erreur Modification"),
                                      QObject::tr("Date Invalide \n La Date doit être sous le format: yyyy/MM/dd HH:mm \n Cliquez sur Cancel pour quitter."),
                                      QMessageBox::Cancel);
            } else {

                NouvelleValeur = VerifDate.toString("dd-MMM-yy").toUpper();
            }
        }

        if (CommandeTemp.ModifierCommande(ID,Column, NouvelleValeur))
        {
            QMessageBox::information(this, "OK", "La valeur a été modifée");
            ui->TableViewCommande->setModel(CommandeTemp.AfficherCommande());
        }

    }

}


void MainWindow::on_BoutonSupprimer_clicked()
{
    int id = ui->TableViewCommande->model()->data(ui->TableViewCommande->selectionModel()->currentIndex().siblingAtColumn(1)).toInt();
    bool Test=CommandeTemp.SupprimerCommande(id);
if (Test)
    {

        QMessageBox::information(nullptr, QObject::tr("OK"),QObject::tr("Suppression Effectuée \n" "Click Cancel to exit."), QMessageBox::Cancel);
        ui->TableViewCommande->setModel(CommandeTemp.AfficherCommande());
        StatistiqueCommandeTotale();
        statistiquesDashboard();

    }
else
    QMessageBox::critical(nullptr, QObject::tr("Not OK"),QObject::tr("Suppression non Effectuée \n" "Click Cancel to exit."), QMessageBox::Cancel);
}


void MainWindow::on_ComboBoxTri_currentTextChanged(const QString &arg1)
{

    qDebug()<<"L'option chosie est"<<arg1;
    if (arg1 == "Date Recente") {
            QSqlQueryModel *TableauTrie = CommandeTemp.TriDateLivraisonR();
            ui->TableViewCommande->setModel(TableauTrie);
        }
    else if (arg1=="Date Ancienne")
    {
        QSqlQueryModel *TableauTrie = CommandeTemp.TriDateLivraisonA();
        ui->TableViewCommande->setModel(TableauTrie);
    }
    else if (arg1=="Prix")
    {
        QSqlQueryModel *TableauTrie = CommandeTemp.TriPrix();
        ui->TableViewCommande->setModel(TableauTrie);
    }
    else if (arg1=="Nom Entreprise")
    {
        QSqlQueryModel *TableauTrie = CommandeTemp.TriNomEntreprise();
        ui->TableViewCommande->setModel(TableauTrie);
    }
    else
    {
        ui->TableViewCommande->setModel(CommandeTemp.AfficherCommande());
    }
}


void MainWindow::on_ComboBoxCategorie_currentTextChanged(const QString &arg2)
{
    if (arg2 == "Equipement")
    {
            QSqlQueryModel *TableauTrie = CommandeTemp.CategorieEquipement();
            ui->TableViewCommande->setModel(TableauTrie);
    }
    else if (arg2 == "Alimentation")
    {
            QSqlQueryModel *TableauTrie = CommandeTemp.CategorieAlimentation();
            ui->TableViewCommande->setModel(TableauTrie);
    }
    else if (arg2 == "Uniformes")
    {
            QSqlQueryModel *TableauTrie = CommandeTemp.CategorieUniformes();
            ui->TableViewCommande->setModel(TableauTrie);
    }
    else if (arg2 == "Decorations")
    {
            QSqlQueryModel *TableauTrie = CommandeTemp.CategorieDecorations();
            ui->TableViewCommande->setModel(TableauTrie);
    }
    else if (arg2 == "Fournitures")
    {
            QSqlQueryModel *TableauTrie = CommandeTemp.CategorieFournitures();
            ui->TableViewCommande->setModel(TableauTrie);
    }
    else
    {
        ui->TableViewCommande->setModel(CommandeTemp.AfficherCommande());
    }


}


void MainWindow::on_comboBox_13_currentTextChanged(const QString &arg1)
{
    if (arg1 == "Payee")
    {
            QSqlQueryModel *TableauTrie = CommandeTemp.EtatPayee();
            ui->TableViewCommande->setModel(TableauTrie);
    }
    else if (arg1 == "Non Payee")
    {
            QSqlQueryModel *TableauTrie = CommandeTemp.EtatNonPayee();
            ui->TableViewCommande->setModel(TableauTrie);
    }
    else
    {
        ui->TableViewCommande->setModel(CommandeTemp.AfficherCommande());
    }

}


void MainWindow::on_LineEditNomEntreprise_2_textChanged(const QString &arg1)
{
    if  (arg1.isEmpty())
    {
         ui->TableViewCommande->setModel(CommandeTemp.AfficherCommande());
    }
    else
    {
        QSqlQueryModel *TableauRecherche=CommandeTemp.RechercheNomEntreprise(arg1);
        ui->TableViewCommande->setModel(TableauRecherche);

    }

}
QSqlQueryModel * Commande::RechercheNomEntreprise(QString ch)
{
    QSqlQueryModel *Model = new QSqlQueryModel();
    QSqlQuery Query;

    Query.prepare("SELECT NomEntreprise, IDCommande, TypeCommande, QuantiteProduit, "
                  "TO_CHAR(MatriculeFiscale) AS MatriculeFiscale, "
                  "TO_CHAR(NumeroTelephone) AS NumeroTelephone, "
                  "Adresse, PrixCommande, EtatCommande, DateLivraison ""FROM Commande WHERE (lower(NomEntreprise) LIKE lower(:ch) "
                     "OR lower(MatriculeFiscale) LIKE lower(:ch))");
    Query.bindValue(":ch", ch.toLower() + "%");

    Query.exec();

    Model->setQuery(Query);

    return Model;
}

void MainWindow::on_BoutonExporterPDF_2_clicked()
{
    auto CommandeID = ui->TableViewCommande->model()->data(ui->TableViewCommande->selectionModel()->currentIndex().siblingAtColumn(1)).toString();

    auto image = CommandeTemp.Generator.generateQr(CommandeID);

    QDialog* qrDialog = new QDialog(this);
    qrDialog->setWindowTitle("QRCode");
    qrDialog->setFixedSize(500, 500);

    QVBoxLayout* layout = new QVBoxLayout(qrDialog);

    QLabel* qrCodeLabel = new QLabel(qrDialog);
    QPixmap ScaledPixmap = QPixmap::fromImage(image).scaled(500, 500, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    qrCodeLabel->setPixmap(ScaledPixmap);
    layout->addWidget(qrCodeLabel);

    qrDialog->setLayout(layout);

    qrDialog->exec();

    delete qrDialog;
}


void MainWindow::StatistiqueChart()
{
    QPieSeries *Chart = new QPieSeries();
    Chart->setHoleSize(0.4);
    Chart->append("Equipement", CommandeTemp.SommeEquipement().toFloat());
    Chart->append("Alimentation", CommandeTemp.SommeAlimentation().toFloat());
    Chart->append("Uniformes", CommandeTemp.SommeUniforme().toFloat());
    Chart->append("Decorations", CommandeTemp.SommeDecorations().toFloat());
    Chart->append("Fournitures", CommandeTemp.SommeFournitures().toFloat());


    Chart->setLabelsVisible(true);
    Chart->setLabelsPosition(QPieSlice::LabelOutside);

    QPieSlice *Slice1 = Chart->slices().at(0);
    QPieSlice *Slice2 = Chart->slices().at(1);
    QPieSlice *Slice3 = Chart->slices().at(2);
    QPieSlice *Slice4 = Chart->slices().at(3);
    QPieSlice *Slice5 = Chart->slices().at(4);

    Slice1->setBrush(QBrush(QColor(151, 60, 59)));
    Slice2->setBrush(QBrush(QColor(188, 84, 82)));
    Slice3->setBrush(QBrush(QColor(204, 126, 126)));
    Slice4->setBrush(QBrush(QColor(221, 169, 169)));
    Slice5->setBrush(QBrush(QColor(238, 212, 212)));

    QChart *statistique = new QChart();
    statistique->addSeries(Chart);
    statistique->legend()->setVisible(false);

    statistique->setBackgroundBrush(Qt::transparent);
    statistique->setBackgroundRoundness(0);

    QChartView *chartview = new QChartView(statistique);
    chartview->setRenderHint(QPainter::Antialiasing);

    chartview->setFixedSize(550, 400);
    chartview->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    chartview->setBackgroundBrush(Qt::transparent);

    QVBoxLayout *layout = new QVBoxLayout(ui->StatCategorie);
    ui->StatCategorie->setLayout(layout);
    layout->addWidget(chartview);
    ui->StatCategorie->setStyleSheet("background-color: rgba(0,0,0,0);");
}
void MainWindow::StatistiqueCommandeTraitee()
{
    QPieSeries *Chart = new QPieSeries();
    Chart->setHoleSize(0.4);
    Chart->append("Traitée", CommandeTemp.SommeCommandeTraitee().toFloat());
    Chart->append("Non Traitée", CommandeTemp.SommeCommandeNonTraitee().toFloat());

    QPieSlice *Slice1 = Chart->slices().at(0);
    QPieSlice *Slice2 = Chart->slices().at(1);

    Slice1->setBrush(QBrush(QColor(76, 130, 89)));
    Slice2->setBrush(QBrush(QColor(33, 74, 61)));

    Chart->setLabelsVisible(true);
    Chart->setLabelsPosition(QPieSlice::LabelOutside);

    QChart *statistique = new QChart();
    statistique->addSeries(Chart);
    statistique->legend()->setVisible(false);

    statistique->setBackgroundBrush(Qt::transparent);
    statistique->setBackgroundRoundness(0);

    QChartView *chartview = new QChartView(statistique);
    chartview->setRenderHint(QPainter::Antialiasing);

    chartview->setFixedSize(550, 400);
    chartview->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    chartview->setBackgroundBrush(Qt::transparent);

    QVBoxLayout *layout = new QVBoxLayout(ui->StatCommandeTraite);
    ui->StatCommandeTraite->setLayout(layout);
    layout->addWidget(chartview);
    ui->StatCommandeTraite->setStyleSheet("background-color: rgba(0,0,0,0);");
}

void MainWindow::StatistiqueText()
{
    ui->StatistiqueEquipement->setText(QString("%1%").arg(CommandeTemp.SommeEquipement()));
    ui->StatistiqueAlimentation->setText(QString("%1%").arg(CommandeTemp.SommeAlimentation()));
    ui->StatistiqueUniforme->setText(QString("%1%").arg(CommandeTemp.SommeUniforme()));
    ui->StatistiqueDecoration->setText(QString("%1%").arg(CommandeTemp.SommeDecorations()));
    ui->StatistiqueFourniture->setText(QString("%1%").arg(CommandeTemp.SommeFournitures()));
    ui->StatCommandeTraitee->setText(QString("%1%").arg(CommandeTemp.SommeCommandeTraitee()));
    ui->StatCommandeNonTraitee->setText(QString("%1%").arg(CommandeTemp.SommeCommandeNonTraitee()));
}

void MainWindow::StatistiqueCommandeTotale()
{
    StatistiqueChart();
    statistiquesDashboard();
    StatistiqueText();
    StatistiqueCommandeTraitee();
}

void MainWindow::VerifierModificationCommande(QString NouvelleValeur, int Column)
{
    bool Test1, Test2,Test3,Test4,Test6;
    if (NouvelleValeur.isEmpty())
    {
        QMessageBox::critical(nullptr, QObject::tr("Erreur Modification"),
                              QObject::tr("La nouvelle valeur ne peut pas être vide \n Cliquez sur Cancel pour quitter."),
                              QMessageBox::Cancel);
        return;

    }
    if ((NouvelleValeur.toInt(&Test1)) && (Column==0))
    {
        QMessageBox::critical(nullptr, QObject::tr("Erreur Modification"),
                              QObject::tr("Le Nom de l'entreprise ne peut pas être un nombre \n Cliquez sur Cancel pour quitter."),
                              QMessageBox::Cancel);
        return;

    }
    if ((NouvelleValeur!="Equipement") && (NouvelleValeur!="Alimentation") && (NouvelleValeur!="Uniformes") && (NouvelleValeur!="Decorations") && (NouvelleValeur!="Fournitures") && (Column==2))
    {
        QMessageBox::critical(nullptr, QObject::tr("Erreur Modification"),
                              QObject::tr("Type Commande Invalide \n Cliquez sur Cancel pour quitter."),
                              QMessageBox::Cancel);
        return;
    }
    if((!NouvelleValeur.toInt(&Test2)) && (Column==3))
    {
        QMessageBox::critical(nullptr, QObject::tr("Erreur Modification"),
                              QObject::tr("La quantité n'est pas valide \n Cliquez sur Cancel pour quitter."),
                              QMessageBox::Cancel);
        return;

    }
    if((NouvelleValeur.toInt()<0) && (Column==3))
    {
        QMessageBox::critical(nullptr, QObject::tr("Erreur Modification"),
                              QObject::tr("La quantité ne peut pas être négative \n Cliquez sur Cancel pour quitter."),
                              QMessageBox::Cancel);
        return;

    }
    if((NouvelleValeur.toInt()<0) && (Column==3))
    {
        QMessageBox::critical(nullptr, QObject::tr("Erreur Modification"),
                              QObject::tr("La quantité ne peut pas être négative \n Cliquez sur Cancel pour quitter."),
                              QMessageBox::Cancel);
        return;

    }
    if (((!NouvelleValeur.toInt(&Test3)) || (NouvelleValeur.length()<3)) && (Column==5))
    {
        QMessageBox::critical(nullptr, QObject::tr("Erreur Modification"),
                              QObject::tr("Le numéro de téléphone est invalide \n Cliquez sur Cancel pour quitter."),
                              QMessageBox::Cancel);
        return;

    }
    if ((NouvelleValeur.toInt()<0) && (Column==5))
    {
        QMessageBox::critical(nullptr, QObject::tr("Erreur Modification"),
                              QObject::tr("Le numéro de téléphone ne peut pas être négative \n Cliquez sur Cancel pour quitter."),
                              QMessageBox::Cancel);
        return;

    }
    if ((NouvelleValeur.toInt(&Test6)) && (Column==6))
    {
        QMessageBox::critical(nullptr, QObject::tr("Erreur Modification"),
                              QObject::tr("L'Adresse ne peut pas être un nombre \n Cliquez sur Cancel pour quitter."),
                              QMessageBox::Cancel);
        return;

    }
    if ((!NouvelleValeur.toInt(&Test4)) && (Column==7))
    {
        QMessageBox::critical(nullptr, QObject::tr("Erreur Modification"),
                              QObject::tr("Le Prix est invalide \n Cliquez sur Cancel pour quitter."),
                              QMessageBox::Cancel);
        return;

    }
    if ((NouvelleValeur.toInt()<0) && (Column==7))
    {
        QMessageBox::critical(nullptr, QObject::tr("Erreur Modification"),
                              QObject::tr("Le prix ne peut pas être négative \n Cliquez sur Cancel pour quitter."),
                              QMessageBox::Cancel);
        return;

    }
    if ((NouvelleValeur!="Payee") && (NouvelleValeur!="Non Payee") && (NouvelleValeur!="payee") && (NouvelleValeur!="non payee") && (Column==8))
    {
        QMessageBox::critical(nullptr, QObject::tr("Erreur Modification"),
                              QObject::tr("Etat Commande Invalide \n L'état doit être 'payee' ou 'non Payee' \n Cliquez sur Cancel pour quitter."),
                              QMessageBox::Cancel);
        return;

    }


}

void MainWindow::on_BoutonSMS_clicked()
{
    fenetremessage *messageWindow = new fenetremessage(this);

    messageWindow->setAttribute(Qt::WA_DeleteOnClose);
    messageWindow->show();
}
void MainWindow::onCellChanged(int row, int column)
{
    // Récupérer l'ID de la chambre à partir de la première colonne
    int ID_CHAMBRE = ui->tableWidget_7->item(row, 0)->text().toInt();

    // Identifier le champ à modifier
    QString fieldName;
    switch (column) {
        case 1:
            fieldName = "type_chambre";
            break;
        case 2:
            fieldName = "dispo";
            break;
        case 3:
            fieldName = "PPN";
            break;
        case 4:
            fieldName = "DR";
            break;
        default:
            return; // Ignorer la colonne 0 (ID)
    }

    // Récupérer les anciennes et nouvelles valeurs
    QString oldValue = oldValues.value(ID_CHAMBRE * 10 + column, ""); // Ancienne valeur stockée
    QString newValue = ui->tableWidget_7->item(row, column)->text(); // Nouvelle valeur

    // Vérifier si la valeur a réellement changé
    if (oldValue == newValue) {
        return; // Pas de modification
    }

    // Exécuter la requête SQL
    QSqlQuery query;
    query.prepare(QString("UPDATE chambre SET %1 = :newValue WHERE ID_CHAMBRE = :ID_CHAMBRE").arg(fieldName));
    query.bindValue(":newValue", newValue);
    query.bindValue(":ID_CHAMBRE", ID_CHAMBRE);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Échec de la mise à jour de la chambre.");
        qDebug() << "Erreur lors de la mise à jour:" << query.lastError().text();
    }

    // Ajouter une entrée dans l'historique
    QString historiqueEntry = QString("Modification : ID = %1, Champ = %2, Ancienne = %3, Nouvelle = %4")
                              .arg(ID_CHAMBRE)
                              .arg(fieldName)
                              .arg(oldValue)
                              .arg(newValue);

    historiqueList.append(historiqueEntry);

    // Limiter la taille de l'historique à 20 entrées
    if (historiqueList.size() > 20) {
        historiqueList.clear(); // Réinitialiser l'historique
    }

    // Sauvegarder l'historique dans un fichier
    saveHistoriqueToFile();

    // Mettre à jour l'affichage de l'historique
    historiqueModel->setStringList(historiqueList);

    // Supprimer l'ancienne valeur de la mémoire temporaire
    oldValues.remove(ID_CHAMBRE * 10 + column);
}


void MainWindow::on_pushButton_1000_clicked()
{
    int id = ui->lineEdit_60->text().toInt();
    QString type_chambre = ui->comboBox_33->currentText();
    int dispo = ui->lineEdit_57->text().toInt();
    float PPN = ui->lineEdit_58->text().toFloat();
    int DR = ui->lineEdit_59->text().toInt();
    if (PPN <= 0) {
            QMessageBox::critical(this, "Erreur", "Le prix par nuit (PPN) doit être supérieur à 0.");
            return; // Arrêter l'exécution si la saisie est invalide
        }

        if (DR <= 0) {
            QMessageBox::critical(this, "Erreur", "La durée de réservation (DR) doit être supérieure à 0.");
            return; // Arrêter l'exécution si la saisie est invalide
        }
    if (dispo != 0 && dispo != 1) {
            QMessageBox::critical(this, "Erreur", "Le champ 'dispo' doit être 0 (non disponible) ou 1 (disponible).");
            return; // Arrêter l'exécution si la saisie est invalide
        }
    Chambre chambre(id, type_chambre, PPN, dispo, DR);

    bool test = chambre.ajouter();
    if (test) {
        QMessageBox::information(this, "Succès", "La chambre a été ajoutée avec succès !");
        afficherChambres();
        //afficherStatistiques();// Actualiser l'affichage après l'ajout
    } //else {
       // QMessageBox::critical(this, "Erreur", "Échec de l'ajout de la chambre.");
   // }
}


void MainWindow::on_pushButton_10_clicked()
{

    // Vérifier qu'une ligne est sélectionnée
    int currentRow = ui->tableWidget_7->currentRow();
    if (currentRow == -1) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une chambre à supprimer.");
        return;
    }

    // Récupérer l'ID de la chambre depuis la première colonne de la ligne sélectionnée
    int ID_CHAMBRE = ui->tableWidget_7->item(currentRow, 0)->text().toInt();

    Chambre chambre;
    bool test = chambre.supprimer(ID_CHAMBRE);

    if (test) {
        QMessageBox::information(this, "Succès", "La chambre a été supprimée avec succès !");
        afficherChambres();
        // Actualiser l'affichage après la suppression
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression de la chambre.");
    }

    // Ajouter un événement de suppression
        historiqueList.append(QString("Suppression : ID = %1").arg(ID_CHAMBRE));

        // Mettre à jour le modèle
        historiqueModel->setStringList(historiqueList);
}

void MainWindow::afficherChambres()
{
    Chambre chambre;
    QSqlQuery query = chambre.afficher();


    ui->tableWidget_7->clearContents();
    ui->tableWidget_7->setRowCount(0);

    int row = 0;
    while (query.next()) {
        ui->tableWidget_7->insertRow(row);


        ui->tableWidget_7->setItem(row, 0, new QTableWidgetItem(query.value("ID_CHAMBRE").toString()));
        ui->tableWidget_7->setItem(row, 1, new QTableWidgetItem(query.value("type_chambre").toString()));
        ui->tableWidget_7->setItem(row, 2, new QTableWidgetItem(query.value("dispo").toString()));
        ui->tableWidget_7->setItem(row, 3, new QTableWidgetItem(query.value("PPN").toString()));
        ui->tableWidget_7->setItem(row, 4, new QTableWidgetItem(query.value("DR").toString()));

        row++;
    }
}
void MainWindow::tridisponibilite()
{
    QString selectedValue = ui->comboBox_28->currentText();
    QSqlQuery query;

    if (selectedValue == "Disponible")
    {
        query.prepare("SELECT * FROM chambre WHERE dispo = 1");
    } else if (selectedValue == "Non Disponible")
    {
        query.prepare("SELECT * FROM chambre WHERE dispo = 0");
    } else {
        query.prepare("SELECT * FROM chambre");
    }

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'affichage des chambres selon la disponibilité:" << query.lastError().text();
        QMessageBox::critical(this, "Erreur", "Échec de l'affichage des chambres.");
        return;
    }

    ui->tableWidget_7->clearContents();
    ui->tableWidget_7->setRowCount(0);

    int row = 0;
    while (query.next()) {
        ui->tableWidget_7->insertRow(row);
        ui->tableWidget_7->setItem(row, 0, new QTableWidgetItem(query.value("ID_CHAMBRE").toString()));
        ui->tableWidget_7->setItem(row, 1, new QTableWidgetItem(query.value("type_chambre").toString()));
        ui->tableWidget_7->setItem(row, 2, new QTableWidgetItem(query.value("dispo").toString()));
        ui->tableWidget_7->setItem(row, 3, new QTableWidgetItem(query.value("PPN").toString()));
        ui->tableWidget_7->setItem(row, 4, new QTableWidgetItem(query.value("DR").toString()));
        row++;
    }
}
void MainWindow::rechercherParID(const QString &idText)
{
    if (idText.isEmpty()) {
        // Si le champ est vide, afficher toutes les chambres
        afficherChambres();
        return;
    }

    int id = idText.toInt();
    QSqlQuery query;
    query.prepare("SELECT * FROM chambre WHERE ID_CHAMBRE = :ID_CHAMBRE");
    query.bindValue(":ID_CHAMBRE", id);

    if (!query.exec() || !query.next()) {
        // Si aucune chambre trouvée, effacer le tableau et ne rien afficher
        ui->tableWidget_7->clearContents();
        ui->tableWidget_7->setRowCount(0);
        return;
    }

    // Réinitialiser l'affichage avant de montrer la chambre trouvée
    ui->tableWidget_7->clearContents();
    ui->tableWidget_7->setRowCount(1);

    // Afficher la chambre trouvée
    ui->tableWidget_7->setItem(0, 0, new QTableWidgetItem(query.value("ID_CHAMBRE").toString()));
    ui->tableWidget_7->setItem(0, 1, new QTableWidgetItem(query.value("type_chambre").toString()));
    ui->tableWidget_7->setItem(0, 2, new QTableWidgetItem(query.value("dispo").toString()));
    ui->tableWidget_7->setItem(0, 3, new QTableWidgetItem(query.value("PPN").toString()));
    ui->tableWidget_7->setItem(0, 4, new QTableWidgetItem(query.value("DR").toString()));
}

void MainWindow::triduree()
{
    QString selectedValue = ui->comboBox_25->currentText(); // Adapter selon le nom de votre QComboBox
    QSqlQuery query;

    if (selectedValue == "DureeReser_croissant") {
        query.prepare("SELECT * FROM chambre ORDER BY DR ASC"); // Trier par ordre croissant
    } else {
        query.prepare("SELECT * FROM chambre"); // Afficher toutes les chambres sans tri particulier
    }

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'affichage des chambres triées par durée de réservation:" << query.lastError().text();
        QMessageBox::critical(this, "Erreur", "Échec de l'affichage des chambres.");
        return;
    }

    // Efface le contenu existant du QTableWidget
    ui->tableWidget_7->clearContents();
    ui->tableWidget_7->setRowCount(0);

    int row = 0;
    while (query.next()) {
        ui->tableWidget_7->insertRow(row);
        ui->tableWidget_7->setItem(row, 0, new QTableWidgetItem(query.value("ID_CHAMBRE").toString()));
        ui->tableWidget_7->setItem(row, 1, new QTableWidgetItem(query.value("type_chambre").toString()));
        ui->tableWidget_7->setItem(row, 2, new QTableWidgetItem(query.value("dispo").toString()));
        ui->tableWidget_7->setItem(row, 3, new QTableWidgetItem(query.value("PPN").toString()));
        ui->tableWidget_7->setItem(row, 4, new QTableWidgetItem(query.value("DR").toString()));
        row++;
    }
}
void MainWindow::on_pushButton_12_clicked()
{
    // Ouvrir une boîte de dialogue pour que l'utilisateur choisisse l'emplacement du fichier
    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer le PDF", "", "PDF Files (*.pdf)");
    if (filePath.isEmpty()) {
        QMessageBox::warning(this, "Annulé", "Enregistrement du PDF annulé.");
        return; // Arrêter l'exécution si aucun fichier n'est choisi
    }

    // Ajouter l'extension .pdf si elle n'est pas spécifiée
    if (!filePath.endsWith(".pdf", Qt::CaseInsensitive)) {
        filePath += ".pdf";
    }

    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setPageMargins(QMargins(30, 30, 30, 30)); // Marges pour aérer le contenu

    QPainter painter(&pdfWriter);

    QFont titleFont("Helvetica", 16, QFont::Bold);
    painter.setFont(titleFont);
    painter.drawText(100, 100, "Liste des Chambres");

    QFont contentFont("Helvetica", 12);
    painter.setFont(contentFont);

    int yPosition = 400; // Position de départ pour le contenu

    QSqlQuery query("SELECT * FROM chambre");
    while (query.next()) {
        // Dessiner chaque champ avec plus d'espace entre les lignes
        painter.drawText(100, yPosition, QString("ID: %1").arg(query.value("ID_CHAMBRE").toString()));
        yPosition += 300; // Espace entre les lignes

        painter.drawText(100, yPosition, QString("Type: %1").arg(query.value("type_chambre").toString()));
        yPosition += 300; // Espace entre les lignes

        QString dispo = query.value("dispo").toString() == "1" ? "Disponible" : "Non disponible";
        painter.drawText(100, yPosition, QString("Disponibilité: %1").arg(dispo));
        yPosition += 300; // Espace entre les lignes

        painter.drawText(100, yPosition, QString("PPN: %1").arg(query.value("PPN").toString()));
        yPosition += 300; // Espace entre les lignes

        painter.drawText(100, yPosition, QString("DR: %1").arg(query.value("DR").toString()));
        yPosition += 500; // Plus grand espace après chaque chambre

        // Vérifier si la fin de la page est atteinte
        if (yPosition > pdfWriter.height() - 150) {
            pdfWriter.newPage();
            yPosition = 100;
        }
    }

    painter.end();
    QMessageBox::information(this, "Succès", "Le PDF a été généré avec succès à l'emplacement choisi !");
}
void MainWindow::on_pushButton_33_clicked()
{
    // Vérifier qu'une ligne est sélectionnée
    int currentRow = ui->tableWidget_7->currentRow();
    if (currentRow == -1) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une chambre nécessitant un entretien.");
        return;
    }

    // Récupérer l'ID de la chambre sélectionnée
    QString chambreId = ui->tableWidget_7->item(currentRow, 0)->text();

    // Vos informations Twilio
    QString accountSid = "AC5c58385381e9721ebf061cca58df2700";
    QString authToken = "bec4d8440b0384e8d694ec5db3c6984c";
    QString fromNumber = "+15137158078";
    QString toNumber = "+21656530017";

    // Créer l'URL d'envoi de SMS de Twilio
    QUrl url("https://api.twilio.com/2010-04-01/Accounts/" + accountSid + "/Messages.json");

    // Créer les données du message à envoyer
    QByteArray postData;
    postData.append("To=" + toNumber.toUtf8() + "&");
    postData.append("From=" + fromNumber.toUtf8() + "&");
    postData.append("Body=" + ("La chambre ID " + chambreId + " a besoin d'entretien.").toUtf8());

    // Configurer la requête HTTP
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    // Authentification de base (SID + Token)
    QString credentials = accountSid + ":" + authToken;
    QString auth = "Basic " + QByteArray(credentials.toUtf8()).toBase64();
    request.setRawHeader("Authorization", auth.toUtf8());

    // Créer le gestionnaire de réseau
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    // Envoyer la requête POST
    QNetworkReply *reply = manager->post(request, postData);

    // Connecter la réponse à la fonction de traitement
    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QMessageBox::information(this, "SMS envoyé", "Le message a été envoyé avec succès pour la chambre ID " + chambreId + " !");
        } else {
            QMessageBox::critical(this, "Erreur d'envoi", "Erreur lors de l'envoi du SMS: " + reply->errorString());
        }
        reply->deleteLater();
        manager->deleteLater();
    });
}


void MainWindow::afficherStatistiques()
{
    QWidget *tabStats = ui->tab_36;

    QLayout *existingLayout = tabStats->layout();
    if (existingLayout) {
        QLayoutItem *item;
        while ((item = existingLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete existingLayout;
    }

    QVBoxLayout *mainLayout = new QVBoxLayout();
    QHBoxLayout *hLayout = new QHBoxLayout();

    QPieSeries *pieSeries = new QPieSeries();
    QSqlQuery dispoQuery = ch.afficher();
    int dispoCount = 0;
    int nonDispoCount = 0;

    while (dispoQuery.next()) {
        int dispo = dispoQuery.value("dispo").toInt();
        if (dispo == 1) {
            dispoCount++;
        } else {
            nonDispoCount++;
        }
    }

    pieSeries->append("Disponible", dispoCount);
    pieSeries->append("Non Disponible", nonDispoCount);

    ui->label_8->setText(QString("%1").arg(nonDispoCount));
    ui->label_16->setText(QString("%1").arg(dispoCount));
    QList<QPieSlice *> slices = pieSeries->slices();
    if (slices.size() >= 2)
    {
        slices[0]->setBrush(QBrush(QColor(76, 130, 89)));
        slices[1]->setBrush(QBrush(QColor(188, 84, 82)));
    }

    // Pie Chart View Creation
    QChart *pieChart = new QChart();
    pieChart->addSeries(pieSeries);
    pieChart->setAnimationOptions(QChart::SeriesAnimations);
    pieChart->setBackgroundBrush(Qt::transparent);
    pieChart->setTitle("Répartition des chambres disponibles");


    QChartView *pieChartView = new QChartView(pieChart);
    pieChartView->setRenderHint(QPainter::Antialiasing);
    pieChartView->setMinimumSize(400, 300);
    pieChartView->setMaximumSize(450, 350);
    pieChartView->setBackgroundBrush(Qt::transparent);

    hLayout->addWidget(pieChartView);

    QBarSet *set = new QBarSet("Nombre de chambres");
    QSqlQuery typeQuery = ch.afficher();
    QMap<QString, int> typeCounts;

    while (typeQuery.next()) {
        QString type = typeQuery.value("type_chambre").toString();
        typeCounts[type]++;
    }

    for (auto it = typeCounts.begin(); it != typeCounts.end(); ++it) {
        *set << it.value();
    }

    set->setBrush(QBrush(QColor(76, 130, 89)));


    QBarSeries *barSeries = new QBarSeries();
    barSeries->append(set);


    // Create a bar chart
    QChart *barChart = new QChart();
    barChart->addSeries(barSeries);
    barChart->setAnimationOptions(QChart::SeriesAnimations);
    barChart->setTitle("Répartition des chambres par type");
    barChart->setBackgroundBrush(Qt::transparent);

    QStringList categories;
    for (auto it = typeCounts.begin(); it != typeCounts.end(); ++it) {
        categories << it.key();
    }

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Nombre de chambres");

    barChart->addAxis(axisX, Qt::AlignBottom);
    barChart->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisX);
    barSeries->attachAxis(axisY);

    // Create the ChartView for the bar chart
    QChartView *barChartView = new QChartView(barChart);
    barChartView->setRenderHint(QPainter::Antialiasing);
    barChartView->setMinimumSize(325, 225);
    barChartView->setBackgroundBrush(Qt::transparent);

    hLayout->addWidget(barChartView);

    // Add layout to main layout
    mainLayout->addLayout(hLayout);

    // Set the main layout to the tab
    tabStats->setLayout(mainLayout);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        ui->listView_Historique->hide();
    }
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        QModelIndex selectedIndex = ui->tableEmployees->currentIndex();
        if (selectedIndex.isValid()) {
            int id_employe = selectedIndex.sibling(selectedIndex.row(), 0).data().toInt();
            mettreAJourCalendrier(id_employe);
            showCalendar();
        } else {
            QMessageBox::warning(this, "Aucune sélection", "Veuillez sélectionner un employé dans la table.");
        }
    } else {
        QMainWindow::keyPressEvent(event);
    }
}
void MainWindow::saveHistoriqueToFile()
{
    QFile file("historique.txt"); // Nom du fichier où l'historique est sauvegardé
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        // Sauvegarder chaque entrée de l'historique dans le fichier
        for (const QString &entry : historiqueList) {
            out << entry << "\n";
        }
        file.close();
    } else {
        qDebug() << "Impossible d'ouvrir le fichier pour sauvegarder l'historique";
    }
}
void MainWindow::loadHistoriqueFromFile()
{
    QFile file("historique.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString line;
        while (!in.atEnd()) {
            line = in.readLine();
            historiqueList.append(line); // Ajouter chaque ligne du fichier à la liste
        }
        file.close();
    } else {
        qDebug() << "Impossible d'ouvrir le fichier pour charger l'historique";
    }

    // Mettre à jour l'affichage de l'historique
    historiqueModel->setStringList(historiqueList);
}


/*void MainWindow::connecterArduino()
{
    serialPort->setPortName("COM3"); // Remplacer par le port série réel d'Arduino
    serialPort->setBaudRate(QSerialPort::Baud9600);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (!serialPort->open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Erreur", "Impossible de se connecter à Arduino !");
    }
}

void MainWindow::lireDonneesArduino()
{
    // Lire les données depuis le port série
    QByteArray data = serialPort->readAll();
    QString donnees = QString(data);
    QStringList listeDonnees = donnees.split(',');

    if (listeDonnees.size() >= 3) {
        QString etat = listeDonnees[0]; // "Disponible" ou "Occupée"
        float temperature = listeDonnees[1].toFloat();
        float humidite = listeDonnees[2].toFloat();

        // Afficher les données dans l'interface Qt
        ui->labelEtat->setText("État : " + etat);
        ui->labelTemperature->setText("Température : " + QString::number(temperature) + " °C");
        ui->labelHumidite->setText("Humidité : " + QString::number(humidite) + " %");

        // Enregistrer les données dans la base de données
        QSqlQuery query;
        query.prepare("INSERT INTO historique (ID_CHAMBRE, etat, temperature, humidite) "
                      "VALUES (:ID_CHAMBRE, :etat, :temperature, :humidite)");
        query.bindValue(":ID_CHAMBRE", 1); // Remplacer par l'ID réel de la chambre
        query.bindValue(":etat", etat);
        query.bindValue(":temperature", temperature);
        query.bindValue(":humidite", humidite);

        if (!query.exec()) {
            QMessageBox::critical(this, "Erreur", "Échec de l'enregistrement dans la base de données !");
        }
    }
}
*/

void MainWindow::onProduitClicked(const QModelIndex &index) {
    if (!index.isValid()) {
        return;
    }

    int idProduit = index.data().toInt();
    QAbstractItemModel *modelTableView6 = ui->tableView_6->model();
    QAbstractItemModel *modelTableView2 = ui->tableView_2->model();
    QDate currentDate = QDate::currentDate();


    QString nomProduit;
    QDate dateDarrivee;
    int delai = -1;


    for (int row = 0; row < modelTableView2->rowCount(); ++row) {
        if (modelTableView2->data(modelTableView2->index(row, 0)).toInt() == idProduit) {
            nomProduit = modelTableView2->data(modelTableView2->index(row, 1)).toString();
            QString rawDateDarrivee = modelTableView2->data(modelTableView2->index(row, 5)).toString();
            delai = modelTableView2->data(modelTableView2->index(row, 6)).toInt();

            qDebug() << "Raw Arrival Date String:" << rawDateDarrivee;
            break;
        }
    }


    QDate derniereSoumission = QDate(0, 1, 1);
    bool soumis = false;


    for (int row6 = 0; row6 < modelTableView6->rowCount(); ++row6) {
        int idProduitSoumission = modelTableView6->data(modelTableView6->index(row6, 0)).toInt();
        if (idProduitSoumission == idProduit) {
            QString rawDateSoumission = modelTableView6->data(modelTableView6->index(row6, 3)).toString();

            qDebug() << "Raw Submission Date String:" << rawDateSoumission;


            QDateTime soumissionDateTime = QDateTime::fromString(rawDateSoumission, Qt::ISODateWithMs);
            if (!soumissionDateTime.isValid()) {
                soumissionDateTime = QDateTime::fromString(rawDateSoumission, Qt::ISODate);
            }

            QDate soumissionDate = soumissionDateTime.date();
            if (soumissionDate.isValid() && soumissionDate > derniereSoumission) {
                derniereSoumission = soumissionDate;
                soumis = true;
            }
        }
    }


    int dureeActivite = -1;
    if (soumis && derniereSoumission.isValid() && derniereSoumission >= dateDarrivee) {
        dureeActivite = dateDarrivee.daysTo(derniereSoumission);
    }

    QString dureeActiviteMessage;
    if (dureeActivite >= 0) {
        dureeActiviteMessage = QString::number(dureeActivite) + " jours";
    } else {
        dureeActiviteMessage = "Non soumis";
    }


    QDate dateDeReference = soumis ? derniereSoumission : dateDarrivee;
    QString joursInactifsMessage;
    bool necessiteRemplacement = false;


    if (dateDeReference > currentDate) {
        joursInactifsMessage = "Date de référence dans le futur";
    } else {
        int joursInactifs = dateDeReference.daysTo(currentDate);
        joursInactifsMessage = QString::number(joursInactifs) + " jours";


        if (delai >= 0 && joursInactifs > delai) {
            necessiteRemplacement = true;
        }
    }


    QString message = QString("Détails pour le produit:\n"
                              "ID: %1\n"
                              "Nom: %2\n"
                              "Durée d'activité: %3\n"
                              "Jours inactifs: %4\n")
                      .arg(idProduit)
                      .arg(nomProduit)
                      .arg(dureeActiviteMessage)
                      .arg(joursInactifsMessage);


    if (necessiteRemplacement) {
        message += "\nAttention : Ce produit a dépassé son délai et doit être remplacé.";
    }


    notificationLabel->setText("Produit : " + nomProduit + " sélectionné !");
    notificationLabel->show();


    QPropertyAnimation *animation = new QPropertyAnimation(notificationLabel, "windowOpacity");
    animation->setDuration(1000);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    animation->start(QAbstractAnimation::DeleteWhenStopped);


    QMessageBox::information(this, "Détails du Produit", message);
}

void MainWindow::verifierQuantiteCritique()
{
    produitsInsuffisants.clear();

    QAbstractItemModel* model = ui->tableView_2->model();
    if (!model) return;

    int rowCount = model->rowCount();
    for (int i = 0; i < rowCount; ++i) {
        int Quantite = model->data(model->index(i, 4)).toInt(); // Colonne de la quantité
        QString nomProduit = model->data(model->index(i, 1)).toString(); // Colonne du nom
        QString typeProduit = model->data(model->index(i, 2)).toString(); // Colonne du type
        QString condition = model->data(model->index(i, 3)).toString(); // Colonne des conditions

        if (Quantite <= 2) {
            // Ajouter le produit à la liste des produits insuffisants
            produitsInsuffisants.append({nomProduit, typeProduit, condition, QString::number(Quantite)});

            // Afficher la notification
            QMessageBox::warning(this, "Quantité Critique",
                                 QString("Quantité de %1 est insuffisante, veuillez l'examiner").arg(nomProduit));
        }
    }
}






void MainWindow::on_pushButton_44_clicked()
{
    QString Nom_Produit = ui->lineEdit_74->text();
    QString Type_Produit = ui->comboBox_49->currentText();
    QString Conditions_Environnementales = ui->comboBox_48->currentText();

    // Contrôle de saisie
    bool idValid, quantiteValid, delaiValid;
    int ID_Produit = ui->lineEdit_73->text().toInt(&idValid);
    int Quantite = ui->lineEdit_80->text().toInt(&quantiteValid);
    int Delai = ui->lineEdit_78->text().toInt(&delaiValid);

    QDate Date_Darrivee = ui->dateEdit_5->date();

    // Validation pour le nom du produit
    QRegularExpression regex("^[a-zA-ZÀ-ÿ\\s]+$");
    if (!regex.match(Nom_Produit).hasMatch()) {
        QMessageBox::critical(this, "Erreur", "Le nom doit contenir uniquement des lettres et des espaces.");
        return;
    }

    // Validation des champs numériques
    if (!idValid || !quantiteValid || !delaiValid) {
        QMessageBox::critical(this, "Erreur", "Vérifiez que l'ID, la quantité et le délai sont valides.");
        return;
    }

    // Création de l'objet Stock
    Stock S(ID_Produit, Nom_Produit, Type_Produit, Conditions_Environnementales, Quantite, Date_Darrivee, Delai);

    // Ajout dans la base de données
    bool ajoutReussi = S.ajouter();

    if (ajoutReussi) {
        QMessageBox::information(this, "Succès", "Ajout effectué avec succès.");
        ui->tableView_2->setModel(S.afficher()); // Rafraîchir la vue des produits
    } else {
        QMessageBox::critical(this, "Erreur", "Ajout échoué. Vérifiez les données et réessayez.");
    }
}


void MainWindow::on_pushButton_42_clicked()
{
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
        Stock S; // Déclarez l'objet Stock ici

        if (S.modifier(id, column, nouvelleValeur)) {
            QMessageBox::information(this, "Modification réussie", "Le produit a été modifié avec succès.");
            ui->tableView_2->setModel(S.afficher());
        } else {
            QMessageBox::critical(this, "Erreur", "La modification du produit a échoué.");
        }
    }
}



void MainWindow::ExporterProduitsInsuffisantsPDF(const QString &path)
{
    if (produitsInsuffisants.isEmpty()) {
        QMessageBox::information(this, "Aucun produit", "Aucun produit avec une quantité insuffisante à exporter.");
        return;
    }

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(path);
    QPainter painter(&printer);

    const int margin = 20;            // Marge globale
    const int cellHeight = 300;       // Hauteur des cellules
    const int cellWidth = 2200;       // Largeur des cellules
    const int headerHeight = 300;     // Hauteur des en-têtes

    int startX = margin;              // Position X initiale
    int titleY = margin + 400;        // Position Y pour le titre
    int startY = titleY + 500;        // Position Y pour le tableau après le titre

    // Titre du PDF
    QFont titleFont = painter.font();
    titleFont.setPointSize(14);          // Taille du texte
    titleFont.setUnderline(true);        // Souligner uniquement le titre
    painter.setFont(titleFont);

    QPen titlePen;
    titlePen.setColor(QColor(186, 85, 211)); // Couleur rose-violet
    painter.setPen(titlePen);

    painter.drawText(startX, titleY, "Produits avec Quantité Insuffisante");

    // Ajouter la date et l'heure en haut à droite
    QDateTime now = QDateTime::currentDateTime();
    QString dateTimeString = now.toString("dd/MM/yyyy HH:mm:ss");
    QFont dateFont = painter.font();
    dateFont.setPointSize(10);          // Taille réduite pour la date/heure
    dateFont.setItalic(true);           // Style italique pour la date/heure
    painter.setFont(dateFont);
    painter.setPen(Qt::black);          // Texte noir pour la date
    painter.drawText(printer.width() - 2000, margin +200, dateTimeString);

    // Colonnes
    const QStringList headers = {"Nom du Produit", "Type", "Condition Environnementale", "Quantité"};
    QPen pen;
    pen.setWidth(2);
    pen.setColor(Qt::black);          // Rétablir la couleur noire après le titre
    painter.setPen(pen);

    QFont font = painter.font();
    font.setPointSize(10);            // Taille normale pour le reste du texte
    font.setUnderline(false);         // Retirer le soulignement pour le reste du texte
    painter.setFont(font);

    // Dessiner les en-têtes avec un fond rouge
    for (int col = 0; col < headers.size(); ++col) {
        QRect rect(startX + col * cellWidth, startY, cellWidth, headerHeight);
        painter.fillRect(rect, Qt::gray); // Fond rouge
        painter.drawRect(rect);         // Contour
        painter.drawText(rect, Qt::AlignCenter, headers[col]); // Texte noir dans les cellules d'en-tête
    }

    startY += headerHeight;

    // Dessiner les lignes des produits insuffisants
    for (int row = 0; row < produitsInsuffisants.size(); ++row) {
        const QList<QString> &produit = produitsInsuffisants[row];
        for (int col = 0; col < produit.size(); ++col) {
            QRect rect(startX + col * cellWidth, startY + row * cellHeight, cellWidth, cellHeight);
            painter.drawRect(rect); // Dessiner le contour de la cellule
            painter.drawText(rect, Qt::AlignCenter, produit[col]); // Ajouter le texte de la cellule
        }

        // Passer à une nouvelle page si nécessaire
        if (startY + (row + 1) * cellHeight > printer.height() - margin) {
            printer.newPage();
            startY = margin;
        }
    }

    painter.end();
    QMessageBox::information(this, "Succès", "PDF exporté avec succès !");
}

void MainWindow::on_pushButton_43_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer PDF", "", "Fichiers PDF (*.pdf)");
    if (!filePath.isEmpty()) {
       ExporterProduitsInsuffisantsPDF(filePath); // Exporter uniquement les produits insuffisants
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
    reply = QMessageBox::question(this, "Confirmation", "Voulez-vous vraiment supprimer ce produit ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        Stock S;
        S.setID_Produit(id);
        bool test = S.supprimer();

        if (test) {
            QMessageBox::information(this, "Suppression", "Produit supprimé avec succès.");
            ui->tableView_2->setModel(S.afficher());
        } else {
            QMessageBox::critical(this, "Erreur", "La suppression a échoué.");
        }
    }
}






void MainWindow::on_pushButton_57_clicked() {
    // Contrôles de saisie
    bool idValid, quantiteValid;
    int produit_id = ui->lineEdit_88->text().toInt(&idValid); // Champ pour saisir l'ID produit
    int quantite_soumise = ui->lineEdit_87->text().toInt(&quantiteValid); // Champ pour la quantité soumise
    QDate date_soumission = ui->dateEdit_8->date(); // Champ pour sélectionner la date de soumission

    // Vérification des données saisies
    if (!idValid || produit_id <= 0) {
        QMessageBox::critical(this, QObject::tr("Erreur"),
                              QObject::tr("Veuillez entrer un ID de produit valide (entier positif)."));
        return;
    }

    if (!quantiteValid || quantite_soumise <= 0) {
        QMessageBox::critical(this, QObject::tr("Erreur"),
                              QObject::tr("Veuillez entrer une quantité soumise valide (entier positif)."));
        return;
    }

    if (date_soumission < QDate::currentDate()) {
        QMessageBox::critical(this, QObject::tr("Erreur"),
                              QObject::tr("La date de soumission doit être égale ou postérieure à la date actuelle."));
        return;
    }

    // Vérifier que l'ID_PRODUIT existe dans la table Stock
    Stock S;
    QSqlQueryModel* model = S.chercherParID(produit_id);
    if (!model || model->rowCount() == 0) {
        QMessageBox::critical(this, QObject::tr("Erreur"),
                              QObject::tr("L'ID du produit n'existe pas dans la base de données Stock."));
        return;
    }

    // Récupérer la quantité actuelle du produit
    int quantiteActuelle = model->data(model->index(0, 4)).toInt(); // Index 4 étant la colonne 'Quantité'

    // Vérifier si la nouvelle quantité est suffisante
    if (quantite_soumise > quantiteActuelle) {
        QMessageBox::critical(this, "Erreur", "Quantité insuffisante dans le stock.");
        return;
    }

    // Ajouter la soumission
    if (S.ajouterSoumission(produit_id, quantite_soumise, date_soumission)) {
        // Calculer la nouvelle quantité
        int nouvelleQuantite = quantiteActuelle - quantite_soumise;

        // Mettre à jour la quantité dans la base de données
        if (S.mettreAJourQuantite(produit_id, nouvelleQuantite)) {
            ui->tableView_2->setModel(S.afficher()); // Rafraîchir la vue des produits
            ui->tableView_6->setModel(S.afficherSoumissions()); // Rafraîchir la vue des soumissions

            // Afficher un message seulement si la nouvelle quantité est critique
            if (nouvelleQuantite <= 2) {
                QString nomProduit = model->data(model->index(0, 1)).toString(); // Index 1 pour le nom du produit
                QMessageBox::warning(this, "Quantité Critique",
                                     QString("Quantité de %1 est insuffisante, veuillez l'examiner").arg(nomProduit));
            }

            QMessageBox::information(this, "Succès", "La soumission a été ajoutée et la quantité mise à jour.");
        } else {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la mise à jour de la quantité.");
        }
    } else {
        QMessageBox::critical(this, QObject::tr("Erreur"),
                              QObject::tr("Erreur lors de l'ajout de la soumission. Vérifiez les données saisies."));
    }
}


void MainWindow::rechercherProduit()
{
    Stock S;
    QString input = ui->lineEdit_9->text();
    QSqlQueryModel* model = nullptr;

    if (input.isEmpty()) {
        QMessageBox::warning(this, "Recherche", "Veuillez saisir un critère de recherche.");
        return;
    }

    // Vérifier si l'entrée est un entier (recherche par ID)
    bool idValid;
    int ID_Produit = input.toInt(&idValid);

    if (idValid) {
        // Recherche par ID
        model = S.chercherParID(ID_Produit);
        if (!model || model->rowCount() == 0) {
            QMessageBox::information(this, "Résultat", "Aucun produit trouvé avec cet ID.");
            return;
        }
    } else {
        // Essayer la recherche par nom
        model = S.chercherParNom(input);
        if (model && model->rowCount() > 0) {
            ui->tableView_2->setModel(model);
            return;
        }

        // Si aucune correspondance par nom, essayer par conditions environnementales
        model = S.chercherParConditions(input);
        if (!model || model->rowCount() == 0) {
            QMessageBox::information(this, "Résultat", "Aucun produit trouvé avec ce nom ou ces conditions environnementales.");
            return;
        }
    }

    // Si une recherche a réussi, mettre à jour le tableau
    ui->tableView_2->setModel(model);
}

void MainWindow::appliquerTri()
{
    Stock S;
    QString critere = ui->comboBox_47->currentText();

    QSqlQueryModel* model = nullptr;
    qDebug() << "Critère sélectionné pour le tri :" << critere;

    if (critere == "Nom") {
        model = S.trierParNomProduit();
    } else if (critere == "Délai") { // Remplacer "Date" par "Délai"
        model = S.trierParDelai();
    } else if (critere == "Quantite") {
        model = S.trierParQuantite();
    }

    if (model) {
        if (model->lastError().isValid()) {
            qDebug() << "Erreur dans le modèle de tri :" << model->lastError().text();
        } else {
            ui->tableView_2->setModel(nullptr); // Réinitialiser la vue
            ui->tableView_2->setModel(model);
        }
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de trier selon le critère sélectionné.");
    }
}


void MainWindow::calculerPourcentageTypesProduits()
{
    // Récupérer le modèle de la tableView_2
    QAbstractItemModel* model = ui->tableView_2->model();
    if (!model) {
        QMessageBox::warning(this, "Erreur", "Le modèle de la table est introuvable.");
        return; // Exit if the model is not available
    }

    int rowCount = model->rowCount();
    if (rowCount == 0) {
        QMessageBox::information(this, "Aucune donnée", "Pas de produits à afficher.");
        return; // Exit if there are no products
    }

    // Compteurs pour chaque type de produit
    int countAlimentation = 0;
    int countEquipement = 0;
    int countDecoration = 0;
    int countFourniture = 0;

    // Parcourir les lignes pour compter chaque type de produit
    for (int i = 0; i < rowCount; ++i) {
        QString typeProduit = model->data(model->index(i, 2)).toString(); // Index 2 est supposé être la colonne de type

        if (typeProduit == "Alimentation") {
            countAlimentation++;
        } else if (typeProduit == "Equipement") {
            countEquipement++;
        } else if (typeProduit == "Decoration") {
            countDecoration++;
        } else if (typeProduit == "Fourniture") {
            countFourniture++;
        }
    }

    // Calculer les pourcentages
    double total = static_cast<double>(rowCount);
    double pourcentageAlimentation = (countAlimentation / total) * 100;
    double pourcentageEquipement = (countEquipement / total) * 100;
    double pourcentageDecoration = (countDecoration / total) * 100;
    double pourcentageFourniture = (countFourniture / total) * 100;

    // Créer les ensembles de barres
    QBarSet *setAlimentation = new QBarSet("Alimentation");
    QBarSet *setEquipement = new QBarSet("Equipement");
    QBarSet *setDecoration = new QBarSet("Decoration");
    QBarSet *setFourniture = new QBarSet("Fourniture");

    // Ajouter les valeurs
    *setAlimentation << pourcentageAlimentation;
    *setEquipement << pourcentageEquipement;
    *setDecoration << pourcentageDecoration;
    *setFourniture << pourcentageFourniture;

    // Définir les couleurs des barres
    setAlimentation->setColor(QColor(76, 130, 89));   // Couleur pour "Alimentation"
    setEquipement->setColor(QColor(187, 135, 126));    // Couleur pour "Equipement"
    setDecoration->setColor(QColor(245, 245, 220));    // Couleur pour "Décoration"
    setFourniture->setColor(QColor(0, 100, 0));     // Couleur pour "Fourniture"

    // Ajouter les ensembles à une série
    QBarSeries *series = new QBarSeries();
    series->append(setAlimentation);
    series->append(setEquipement);
    series->append(setDecoration);
    series->append(setFourniture);

    // Créer un graphique
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setBackgroundBrush(Qt::transparent);

    // Définir les catégories (une seule barre par catégorie ici)
    QStringList categories;
    categories << "Types de Produits";
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Définir l'axe Y pour les pourcentages
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 100);
    axisY->setTitleText("Pourcentage (%)");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Affichage dans un QChartView
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setBackgroundBrush(Qt::transparent);

    // Afficher le graphique dans le conteneur StatistiquesProduits2
    QLayout *layout = ui->StatistiquesProduits2->layout();
    if (!layout) {
        layout = new QVBoxLayout(ui->StatistiquesProduits2);
        ui->StatistiquesProduits2->setLayout(layout);
    }

    // Supprimer les anciens widgets avant d'ajouter le nouveau graphique
    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    layout->addWidget(chartView);
}



void MainWindow::setupArduino()
{
    arduino_available = false;
    arduino = new QSerialPort(this);

    // Search for Arduino port
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()) {
        if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()) {
            if(serialPortInfo.vendorIdentifier() == arduino_uno_vendor_id &&
                serialPortInfo.productIdentifier() == arduino_uno_product_id) {
                arduino->setPort(serialPortInfo);
                arduino_available = true;
            }
        }
    }

    if(arduino_available) {
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setBaudRate(QSerialPort::Baud9600);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        arduino->setFlowControl(QSerialPort::NoFlowControl);

        if(arduino->open(QIODevice::ReadOnly)) {
            connect(arduino, &QSerialPort::readyRead, this, &MainWindow::readSerial);
            qDebug() << "Connected to Arduino";
        } else {
            qDebug() << "Failed to open serial port";
            arduino_available = false;
        }
    } else {
        qDebug() << "Couldn't find the Arduino";
    }
}

void MainWindow::readSerial()
{
    while (arduino->canReadLine()) {
        serialData = arduino->readLine();
        serialBuffer += QString::fromUtf8(serialData);

        if (serialBuffer.contains('\n')) {
            QStringList lines = serialBuffer.split('\n');
            serialBuffer = lines.last(); // Keep incomplete line in buffer

            for (int i = 0; i < lines.size() - 1; ++i) {
                processSerialData(lines[i].trimmed());
            }
        }
    }
}

void MainWindow::processSerialData(const QString &data)
{
    // Format expected from Arduino: "TEMP:23.5,45.6"
    if (data.startsWith("TEMP:")) {
        QString values = data.mid(5); // Remove "TEMP:"
        QStringList parts = values.split(",");

        if (parts.size() == 2) {
            bool tempOk, humOk;
            float temperature = parts[0].toFloat(&tempOk);
            float humidity = parts[1].toFloat(&humOk);

            if (tempOk && humOk) {
                // Update the labels
                ui->humidity_value->setText(QString::number(humidity, 'f', 1) + "%");
                ui->temperature_value32->setText(QString::number(temperature, 'f', 1) + "°C");
            }
        }
    }
}





void MainWindow::calculerPourcentageProduitsAjoutesEtSoumis()
{
    // Récupérer les modèles des tables
    QAbstractItemModel *modelProduitsAjoutes = ui->tableView_2->model();
    QAbstractItemModel *modelProduitsSoumis = ui->tableView_6->model();

    // Check if models are valid
    if (!modelProduitsAjoutes || !modelProduitsSoumis) {
        QMessageBox::warning(this, "Erreur", "Les modèles des tables sont introuvables.");
        return; // Exit function gracefully
    }

    int totalProduitsAjoutes = modelProduitsAjoutes->rowCount();
    int totalProduitsSoumis = modelProduitsSoumis->rowCount();
    int totalGeneral = totalProduitsAjoutes + totalProduitsSoumis;

    if (totalGeneral == 0) {
        QMessageBox::information(this, "Aucune donnée", "Aucun produit ajouté ou soumis pour afficher les pourcentages.");
        return;
    }

    // Calculate proportions
    qreal pourcentageAjoutes = static_cast<qreal>(totalProduitsAjoutes) / totalGeneral;
    qreal pourcentageSoumis = static_cast<qreal>(totalProduitsSoumis) / totalGeneral;

    // Create a pie series for the chart
    QPieSeries *series = new QPieSeries();
    series->append("Produits Ajoutés", pourcentageAjoutes);
    series->append("Produits Soumis", pourcentageSoumis);

    // Set slice labels and colors
    for (QPieSlice *slice : series->slices()) {
        slice->setLabelVisible();
        slice->setLabel(QString("%1: %2%").arg(slice->label()).arg(slice->percentage() * 100, 0, 'f', 2));

        // Define colors for slices
        if (slice->label().contains("Produits Ajoutés")) {
            slice->setBrush(QColor(76, 130, 89)); // Color for "Produits Ajoutés"
        } else if (slice->label().contains("Produits Soumis")) {
            slice->setBrush(QColor(187, 135, 126)); // Color for "Produits Soumis"
        }
    }

    // Set up the chart
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setBackgroundBrush(Qt::transparent);

    // Display in a QChartView
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(400, 400);
    chartView->setBackgroundBrush(Qt::transparent); // Set background to transparent

    // Clear previous widgets in StatistiquesProduit1
    QLayout *layout = ui->StatistiquesProduit1->layout();
    if (layout) {
        QLayoutItem *item;
        while ((item = layout->takeAt(0)) != nullptr) {
            delete item->widget(); // Cleanup existing widgets
            delete item;
        }
    } else {
        layout = new QVBoxLayout(ui->StatistiquesProduit1);
        ui->StatistiquesProduit1->setLayout(layout);
    }

    // Add the new chart view to the layout
    layout->addWidget(chartView);
    ui->StatistiquesProduit1->setStyleSheet("background-color: rgba(0, 0, 0, 0);"); // Set container background to transparent

    // Avoid memory leaks
    QObject::connect(series, &QPieSeries::destroyed, chartView, &QChartView::deleteLater);
    QObject::connect(chart, &QChart::destroyed, series, &QPieSeries::deleteLater);
}



void MainWindow::afficherProduitsParCondition(const QString& condition, const QStringList& produits) {
    QTableView* tableView = ui->tableView_2;
    if (!tableView) return;

    // Create a new model for the data
    QStandardItemModel* model = new QStandardItemModel(this);

    // Set headers
    QStringList headers;
    headers << "ID" << "Nom" << "Type" << "Conditions" << "Quantité" << "Date d'arrivée" << "Délai";
    model->setHorizontalHeaderLabels(headers);

    // Add products to the model
    for (const QString& produit : produits) {
        QList<QStandardItem*> rowItems;
        QStringList fields = produit.split(",");

        for (const QString& field : fields) {
            rowItems.append(new QStandardItem(field.trimmed()));
        }

        model->appendRow(rowItems);
    }

    // Apply model to view
    tableView->setModel(model);

    // Adjust column sizes
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::addProductToTable(QTableWidget* table, const QString& nomProduit) {
    if (!table) return;

    int row = table->rowCount();
    table->insertRow(row);

    // Add the product name
    QTableWidgetItem* item = new QTableWidgetItem(nomProduit);
    table->setItem(row, 0, item);
}

void MainWindow::setupTableWidget(QTableWidget* table) {
    if (!table) return;

    // Set up headers
    QStringList headers;
    headers << "ID" << "Nom" << "Type" << "Conditions" << "Quantité" << "Date d'arrivée" << "Délai";

    table->setColumnCount(headers.size());
    table->setHorizontalHeaderLabels(headers);

    // Configure table properties
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Adjust column sizes
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}


void MainWindow::Equip_choix_pie2()
{
    if (!ui->statsContainer) {
        QMessageBox::critical(this, "Erreur", "Le conteneur statsContainer est introuvable.");
        return;
    }

    QPieSeries *series = new QPieSeries();
    series->append("Option 1", 40);
    series->append("Option 2", 60);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Deuxième graphique circulaire");
    chart->setAnimationOptions(QChart::SeriesAnimations); // Optional: for animations
    chart->setBackgroundBrush(Qt::transparent);

    // Create the chart view and make background transparent
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setBackgroundBrush(Qt::transparent); // Set chart background to transparent

    QLayout *layout = ui->statsContainer->layout();
    if (!layout) {
        layout = new QVBoxLayout(ui->statsContainer);
        ui->statsContainer->setLayout(layout);
    }

    // Clear old widgets before adding the new chart
    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    layout->addWidget(chartView);
    ui->statsContainer->setStyleSheet("background-color: rgba(0, 0, 0, 0);"); // Make the container background transparent

    // Avoid memory leaks
    QObject::connect(series, &QPieSeries::destroyed, chartView, &QChartView::deleteLater);
    QObject::connect(chart, &QChart::destroyed, series, &QPieSeries::deleteLater);
}



void MainWindow::on_pushButton_45_clicked()
{
    ui->pushButton_45->blockSignals(true); // Désactiver les signaux pour éviter les clics multiples

    // Créer une fenêtre modale avec des onglets
    QDialog *dialog = new QDialog(this);
    int totalProduits = 0; // Compteur pour le titre
    dialog->setWindowTitle("Classification des Produits");
    dialog->resize(800, 500); // Fenêtre plus large pour inclure plus de données

    QTabWidget *tabWidget = new QTabWidget(dialog);

    // Onglets pour les classifications
    QWidget *tabChaud = new QWidget();
    QWidget *tabFroid = new QWidget();
    QWidget *tabMoyen = new QWidget();

    // Ajouter les onglets au QTabWidget
    tabWidget->addTab(tabChaud, "Chambre Chaude Spécialisée");
    tabWidget->addTab(tabFroid, "Réfrigérateur");
    tabWidget->addTab(tabMoyen, "Étagères et Racks");

    // Layouts pour chaque onglet
    QVBoxLayout *layoutChaud = new QVBoxLayout(tabChaud);
    QVBoxLayout *layoutFroid = new QVBoxLayout(tabFroid);
    QVBoxLayout *layoutMoyen = new QVBoxLayout(tabMoyen);

    // Tableaux pour chaque classification
    QTableWidget *tableChaud = new QTableWidget();
    QTableWidget *tableFroid = new QTableWidget();
    QTableWidget *tableMoyen = new QTableWidget();

    // Ajouter les tableaux dans les layouts respectifs
    layoutChaud->addWidget(tableChaud);
    layoutFroid->addWidget(tableFroid);
    layoutMoyen->addWidget(tableMoyen);

    // Récupérer les produits depuis tableView_2
    QAbstractItemModel *model = ui->tableView_2->model();
    if (!model) {
        QMessageBox::critical(this, "Erreur", "Aucune donnée disponible dans la table des produits.");
        return;
    }

    // Initialiser les tableaux avec colonnes supplémentaires
    setupTableWidgetWithColumns(tableChaud);
    setupTableWidgetWithColumns(tableFroid);
    setupTableWidgetWithColumns(tableMoyen);

    // Parcourir les données et remplir les tableaux
    int rowCount = model->rowCount();
    int columnCondition = 3; // Colonne des conditions environnementales
    int columnNom = 1;       // Colonne des noms de produits
    int columnQuantite = 4;  // Colonne des quantités

    for (int i = 0; i < rowCount; ++i) {
        QString condition = model->data(model->index(i, columnCondition)).toString();
        QString nomProduit = model->data(model->index(i, columnNom)).toString();
        QString quantite = model->data(model->index(i, columnQuantite)).toString();

        if (condition == "Chaud") {
            addProductToTableWithDetails(tableChaud, nomProduit, quantite, condition);
            totalProduits++;
        } else if (condition == "Froid") {
            addProductToTableWithDetails(tableFroid, nomProduit, quantite, condition);
            totalProduits++;
        } else if (condition == "Moyen") {
            addProductToTableWithDetails(tableMoyen, nomProduit, quantite, condition);
            totalProduits++;
        }
    }

    // Messages si une table est vide
    afficherMessageSiVide(tableChaud, "Aucun produit dans la catégorie 'Chaud'");
    afficherMessageSiVide(tableFroid, "Aucun produit dans la catégorie 'Froid'");
    afficherMessageSiVide(tableMoyen, "Aucun produit dans la catégorie 'Moyen'");

    // Mettre à jour le titre pour inclure le nombre total de produits classifiés
    dialog->setWindowTitle(QString("Classification des Produits (%1 produits)").arg(totalProduits));

    // Afficher le QTabWidget
    QVBoxLayout *mainLayout = new QVBoxLayout(dialog);
    mainLayout->addWidget(tabWidget);
    dialog->exec();

    ui->pushButton_45->blockSignals(false); // Réactiver les signaux après la fermeture
}

// Méthode pour initialiser un QTableWidget avec colonnes supplémentaires
void MainWindow::setupTableWidgetWithColumns(QTableWidget *table)
{
    table->setColumnCount(3); // Trois colonnes : Nom, Quantité, Conditions
    table->setHorizontalHeaderLabels({"Nom du Produit", "Quantité", "Condition"});
    table->horizontalHeader()->setStretchLastSection(true); // Étendre la dernière colonne
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // Ajuster la largeur automatiquement
    table->setEditTriggers(QAbstractItemView::NoEditTriggers); // Empêcher la modification
    table->setSelectionBehavior(QAbstractItemView::SelectRows); // Sélection par ligne
    table->setStyleSheet("QTableWidget { border: 1px solid gray; }"); // Ajouter une bordure
}

// Méthode pour ajouter un produit avec des détails dans un QTableWidget
void MainWindow::addProductToTableWithDetails(QTableWidget *table, const QString &nomProduit, const QString &quantite, const QString &condition)
{
    int row = table->rowCount();
    table->insertRow(row);
    table->setItem(row, 0, new QTableWidgetItem(nomProduit));
    table->setItem(row, 1, new QTableWidgetItem(quantite));
    table->setItem(row, 2, new QTableWidgetItem(condition));

    // Centrer le texte
    for (int col = 0; col < table->columnCount(); ++col) {
        table->item(row, col)->setTextAlignment(Qt::AlignCenter);
    }
}

// Méthode pour afficher un message si une table est vide
void MainWindow::afficherMessageSiVide(QTableWidget *table, const QString &message)
{
    if (table->rowCount() == 0) {
        int row = table->rowCount();
        table->insertRow(row);
        QTableWidgetItem *item = new QTableWidgetItem(message);
        item->setTextAlignment(Qt::AlignCenter);
        item->setForeground(QBrush(Qt::gray)); // Texte en gris
        table->setItem(row, 0, item);
        table->setSpan(row, 0, 1, table->columnCount()); // Fusionner les colonnes
    }
}


void MainWindow::on_tabChanged(int index) {
    if (index == 1) { // Assuming the "ajouter" tab is at index 1
        on_pushButton_36_clicked(); // Call a method to handle the action when "ajouter" tab is selected
    }
}


void MainWindow::on_pushButton_36_clicked() {

    int Matricule_Transaction = ui->lineEdit_71->text().toInt();
    QString Type_Transaction = ui->comboBox_18->currentText();
    QString Categorie = ui->lineEdit_76->text();
    QString Mode_Paiement = ui->comboBox_27->currentText();
    //QString Statut = ui->lineEdit_77->text();
    QString Statut="";
    QDate Date_Transaction = ui->dateEdit_2->date();
     if (!Date_Transaction.isValid()) {
         Date_Transaction = QDate::currentDate();
     }
    double Montant = ui->lineEdit_72->text().toDouble();

     finance F(Matricule_Transaction, Type_Transaction, Categorie, Date_Transaction, Mode_Paiement, Statut, Montant);

     bool test = F.ajouter();
     if (test) {
         QMessageBox::information(this, QObject::tr("OK"), QObject::tr("Ajout effectué\nCliquez sur Annuler pour quitter."), QMessageBox::Cancel);
         UpdateFinance();
         statistiquesDashboard();

     } else {
         QMessageBox::critical(this, QObject::tr("Not OK"), QObject::tr("L'ajout a échoué\nCliquez sur Annuler pour quitter."), QMessageBox::Cancel);
     }


     Function_Mailing();
     ui->tb_Alertsss->setText(F.read());
 }



void MainWindow::on_finance_tri_montant_clicked()
{
    F.clearTable(ui->finance_tableView);
    ui->finance_tableView->setModel(F.tri_montant());
     qDebug() << " aaaaaaaaaaa";

}


void MainWindow::on_finance_tri_statut_clicked()
{
    F.clearTable(ui->finance_tableView);
    ui->finance_tableView->setModel(F.tri_statut());

}


void MainWindow::on_finance_PDF_pushButton_clicked()
{
    QString currentPath = QDir::currentPath();
    QString pdfPath = currentPath + "/Liste-Transactions.pdf";
    QString logoPath = currentPath + "/ressource/Logo.png";
    QPdfWriter pdf(pdfPath);
    QPainter painter(&pdf);
    int i = 4000;
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 30));
    painter.drawPixmap(QRect(100,400,2000,2000),QPixmap(logoPath));
    painter.drawText(2900,1500,"LISTE DES TRANSACTIONS");
    painter.setPen(Qt::blue);
    painter.setFont(QFont("Arial", 50));
    painter.drawRect(2700,200,7000,2600);
    painter.drawRect(0,3000,9600,500);
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 9));
    painter.drawText(300, 3300, "Matricule");
    painter.drawText(1300, 3300, "Type");
    painter.drawText(2300, 3300, "Catégorie");
    painter.drawText(3900, 3300, "Date");
    painter.drawText(5300, 3300, "Mode de paiement");
    painter.drawText(7100, 3300, "Statut");
    painter.drawText(8300, 3300, "Montant");
    QSqlQuery query;
    query.prepare("<SELECT CAST( GETDATE() AS Date ) ");
    time_t tt;
    struct tm* ti;
    time(&tt);
    ti=localtime(&tt);
    asctime(ti);
    painter.drawText(500,300, asctime(ti));
    query.prepare("select * from GS_FINANCE");
    query.exec();
    while (query.next())
    {
        painter.drawText(300, i, query.value(0).toString());
        painter.drawText(1300, i, query.value(1).toString());
        painter.drawText(2300, i, query.value(2).toString());
        painter.drawText(3300, i, query.value(3).toString());
        painter.drawText(5300, i, query.value(4).toString());
        painter.drawText(7100, i, query.value(5).toString());
        painter.drawText(8300, i, query.value(6).toString());
        i = i +500;
    }
    int reponse = QMessageBox::question(this, "PDF généré", "Afficher le PDF ?", QMessageBox::Yes |  QMessageBox::No);
    if (reponse == QMessageBox::Yes)
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(pdfPath));
        painter.end();
    }
    if (reponse == QMessageBox::No)
    {
        painter.end();
    }
}
void MainWindow::Equip1_choix_pie() {
    QChartView *chartView = nullptr;  // Initialisation à nullptr pour éviter les problèmes de mémoire
    QSqlQuery query;
    qreal tot = 0, depenseCount = 0, revenuCount = 0;

    // Get the total count of records
    query.prepare("SELECT COUNT(*) FROM GS_FINANCE");
    if (!query.exec()) {
        qDebug() << "Error executing query to get total count:" << query.lastError().text();
        return;  // Exit if the query fails
    }
    if (query.next()) {
        tot = query.value(0).toDouble();
    }
    qDebug() << "Total transactions: " << tot; // Debugging step

    if (tot == 0) {
        // If there are no transactions, display an empty chart or a message
        qDebug() << "No transactions available.";
        return;  // Exit if no transactions are available
    }

    // Get the counts for depense and revenu
    query.prepare("SELECT TYPE_TRANSACTION, COUNT(*) FROM GS_FINANCE WHERE TYPE_TRANSACTION IN ('depense', 'revenu') GROUP BY TYPE_TRANSACTION");
    if (!query.exec()) {
        qDebug() << "Error executing query to get transaction counts:" << query.lastError().text();
        return;  // Exit if the query fails
    }

    while (query.next()) {
        QString status = query.value(0).toString();
        int count = query.value(1).toInt();

        if (status == "depense") {
            depenseCount = count;
        } else if (status == "revenu") {
            revenuCount = count;
        }
    }

    qDebug() << "Dépense Count: " << depenseCount << ", Revenu Count: " << revenuCount; // Debugging step

    // Calculate proportions (make sure the total is not zero to avoid division by zero)
    qreal c1 = (tot > 0) ? (depenseCount / tot) : 0;
    qreal c2 = (tot > 0) ? (revenuCount / tot) : 0;

    qDebug() << "Proportion Dépense: " << c1 << ", Proportion Revenu: " << c2; // Debugging step

    // Create the pie chart series
    QPieSeries *series = new QPieSeries();

    // Add 'Dépense' segment, ensure a non-zero percentage even if depenseCount is 0
    QPieSlice *depenseSlice = series->append("Dépense", c1);
    depenseSlice->setBrush(QColor("#FF4500")); // Orange for 'Dépense'

    // Add 'Revenu' segment, ensure a non-zero percentage even if revenuCount is 0
    QPieSlice *revenuSlice = series->append("Revenu", c2);
    revenuSlice->setBrush(QColor("#32CD32")); // Green for 'Revenu'

    // Make sure labels are visible and display correctly
    for (QPieSlice *slice : series->slices()) {
        slice->setLabelVisible(true);
        slice->setLabel(QString("%1: %2%").arg(slice->label()).arg(slice->percentage() * 100, 0, 'f', 1));
    }

    // Remove old chartView if it exists
    if (chartView) {
        delete chartView;
    }

    // Set up the chart
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des Transactions");

    // Display the chart in the designated UI label
    chartView = new QChartView(chart, ui->finance_label_Stats);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(370, 370);
    chartView->show();
}


void MainWindow::Equip1_choix_pie2()
{
    QChartView *chartView;
    QSqlQuery query;
    int especesCount = 0, creditCount = 0, virementCount = 0, chequeCount = 0;

    // Get counts for each payment method
    query.prepare("SELECT MODE_PAIEMENT, COUNT(*) FROM GS_FINANCE "
                  "WHERE MODE_PAIEMENT IN ('especes', 'Carte de credit', 'Virement bancaire', 'Cheques') "
                  "GROUP BY MODE_PAIEMENT");
    if (query.exec()) {
        while (query.next()) {
            QString status = query.value(0).toString();
            int count = query.value(1).toInt();

            // Check the payment method and assign counts
            if (status == "especes") {
                especesCount = count;
            } else if (status == "Carte de credit") {
                creditCount = count;
            } else if (status == "Virement bancaire") {
                virementCount = count;
            } else if (status == "Cheques") {
                chequeCount = count;
            }
        }
    }

    // Ensure at least one value for display purposes
    if (especesCount == 0 && creditCount == 0 && virementCount == 0 && chequeCount == 0) {
        // Add a small value for visibility
        especesCount = 1;
        creditCount = 1;
        virementCount = 1;
        chequeCount = 1;
    }

    // Create the pie chart series
    QPieSeries *series = new QPieSeries();

    // Add all values with specified colors
    series->append("Espèces", especesCount)->setBrush(QColor(255, 105, 180)); // Pink
    series->append("Carte de crédit", creditCount)->setBrush(QColor(0, 255, 0)); // Green
    series->append("Virement bancaire", virementCount)->setBrush(QColor(255, 20, 147)); // Deep Pink
    series->append("Chèques", chequeCount)->setBrush(QColor(144, 238, 144)); // Light Green

    // Make the labels visible with percentages
    for (QPieSlice *slice : series->slices()) {
        slice->setLabelVisible();
        slice->setLabel(QString("%1%").arg(slice->percentage() * 100, 0, 'f', 1));
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignTop);
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->setTheme(QChart::ChartThemeLight);

    chart->setBackgroundBrush(QBrush(Qt::transparent));
    chart->setBackgroundRoundness(0); // Optional: Set roundness to 0 for sharper edges

    chart->setMinimumSize(360, 360);

    chartView = new QChartView(chart, ui->finance_label_Stats_paiement);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(370, 370);

    chartView->setStyleSheet("background: transparent;");

    chartView->show();

    chartView->update();
}


void MainWindow::on_finance_modify_pushButton_clicked()
{
    QModelIndex currentIndex = ui->finance_tableView->selectionModel()->currentIndex();

    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Modification", "Veuillez sélectionner un attribut de la transaction à modifier.");
        return;
    }

    int column = currentIndex.column();
    QString matriculeTransaction = ui->finance_tableView->model()->data(currentIndex.siblingAtColumn(0)).toString();
    bool ok;

    QString ancienneValeur = ui->finance_tableView->model()->data(currentIndex).toString();
    QString nouvelleValeur = QInputDialog::getText(this, tr("Modification"), tr("Saisissez la nouvelle valeur :"), QLineEdit::Normal, ancienneValeur, &ok);

    if (ok && !nouvelleValeur.isEmpty()) {
        int matricule = matriculeTransaction.toInt();
        finance F;

        if (F.modifier(matricule, column, nouvelleValeur)) {
            QMessageBox::information(this, "Modification réussie", "La transaction a été modifiée avec succès.");
            ui->finance_tableView->setModel(F.afficher());
        } else {
            QMessageBox::critical(this, "Erreur", "La modification de la transaction a échoué.");
        }
    }
}


void MainWindow::on_finance_tableView_clicked(const QModelIndex &index)
{
    qDebug() << "Row clicked:" << index.row();

    int row = index.row();
    QAbstractItemModel *model = ui->finance_tableView->model();

    if (!model || row < 0) {
        qDebug() << "Invalid selection!";
        return;
    }

    // Correctly map the fields
    QString matricule = model->data(model->index(row, 0)).toString(); // Assuming Matricule_Transaction is the 1st column
    QString typeTransaction = model->data(model->index(row, 1)).toString(); // Type_Transaction
    QString categorie = model->data(model->index(row, 2)).toString(); // Categorie
    QString dateTransaction = model->data(model->index(row, 3)).toString(); // Date_Transaction
    QString modePaiement = model->data(model->index(row, 4)).toString(); // Mode_Paiement
    QString statut = model->data(model->index(row, 5)).toString(); // Statut
    QString montant = model->data(model->index(row, 6)).toString(); // Montant

    qDebug() << "Matricule_Transaction:" << matricule;
    qDebug() << "Type_Transaction:" << typeTransaction;
    qDebug() << "Categorie:" << categorie;
    qDebug() << "Date_Transaction:" << dateTransaction;
    qDebug() << "Mode_Paiement:" << modePaiement;
    qDebug() << "Statut:" << statut;
    qDebug() << "Montant:" << montant;

    // Populate the correct form fields
    ui->lineEdit_71->setText(matricule);
    ui->comboBox_18->setCurrentText(typeTransaction);
    ui->lineEdit_76->setText(categorie);
    ui->dateEdit_2->setDate(QDate::fromString(dateTransaction, "yyyy-MM-dd")); // Adjust the format as needed
    ui->comboBox_27->setCurrentText(modePaiement);
    //ui->lineEdit_77->setText(statut);
    ui->lineEdit_72->setText(montant);


    QString qrData = QString("Matricule: %1 | Type de transaction: %2 | Categorie: %3 | Date: %4 | Mode de paiement: %5 | Statut: %6 | Montant: %7")
                        .arg(matricule)
                        .arg(typeTransaction)
                        .arg(categorie)
                        .arg(QDateTime::fromString(dateTransaction, Qt::ISODate).toString("dd/MM/yyyy")) // Format date
                        .arg(modePaiement)
                        .arg(statut)
                        .arg(montant);

   generateQRCode(qrData);


}
void MainWindow::generateQRCode(const QString &data) {
    try {
        // Convert the data to a UTF-8 encoded string
        std::string encodedData = data.toUtf8().constData();

        // Generate the QR code
        const qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(encodedData.c_str(), qrcodegen::QrCode::Ecc::LOW);

        // Create a QImage to draw the QR code
        int size = qr.getSize();
        QImage image(size, size, QImage::Format_RGB32);
        image.fill(Qt::white); // Fill the background with white

        // Draw the QR code
        for (int y = 0; y < size; y++) {
            for (int x = 0; x < size; x++) {
                if (qr.getModule(x, y)) {
                    image.setPixel(x, y, qRgb(0, 0, 0)); // Set pixel to black
                }
            }
        }

        // Convert QImage to QPixmap for displaying
        QPixmap pixmap = QPixmap::fromImage(image).scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        // Set the rendered QR code to the QLabel in the main UI, assuming the QLabel is named qrcode
        ui->qrcode->setPixmap(pixmap);

        // Optionally, you can set fixed size to the QLabel
        ui->qrcode->setFixedSize(200, 200);
        ui->qrcode->setScaledContents(true); // Ensure it scales the content to fit the QLabel

        // Debug output
        qDebug() << "QR Code generated for data:" << data;
    } catch (const std::exception &e) {
        qDebug() << "Error generating QR Code:" << e.what();
    }
}



void MainWindow::on_ajouter_finance_pushButton_clicked()
{
    ui->tabWidget_9->setCurrentWidget(ui->ajout_finance);
    clearFinanceFields();
}

void MainWindow::clearFinanceFields() {
    ui->lineEdit_71->clear();
    ui->lineEdit_76->clear();
    //ui->lineEdit_77->clear();
    ui->lineEdit_72->clear();
    ui->comboBox_18->setCurrentIndex(0);
    ui->comboBox_27->setCurrentIndex(0);
    ui->dateEdit_2->setDate(QDate::currentDate());
}
void MainWindow::on_delete_finance_pushButton_clicked()
{
    // Get the selected row
    QItemSelectionModel *selection = ui->finance_tableView->selectionModel();
    if (!selection->hasSelection()) {
        QMessageBox::warning(this, tr("No Selection"), tr("Please select a transaction to delete."));
        return;
    }

    // Get the Matricule_Transaction from the selected row (assume it's in the first column)
    int row = selection->currentIndex().row();
    QAbstractItemModel *model = ui->finance_tableView->model();
    int Matricule_Transaction = model->data(model->index(row, 0)).toInt();

    // Confirm deletion
    QMessageBox::StandardButton confirm = QMessageBox::question(this, tr("Delete Transaction"),
        tr("Are you sure you want to delete this transaction?"),
        QMessageBox::Yes | QMessageBox::No);

    if (confirm == QMessageBox::No) {
        return; // Cancel deletion
    }

    // Delete the transaction from the database
    QSqlQuery query;
    query.prepare("DELETE FROM GS_FINANCE WHERE MATRICULE_TRANSACTION = :Matricule_Transaction");
    query.bindValue(":Matricule_Transaction", Matricule_Transaction);

    if (query.exec()) {
        QMessageBox::information(this, tr("Transaction Deleted"), tr("The transaction was successfully deleted."));

        // Refresh the table view
        finance F;
        ui->finance_tableView->setModel(F.afficher());
    } else {
        QMessageBox::critical(this, tr("Deletion Failed"), tr("Failed to delete the transaction.\nError: ") + query.lastError().text());
    }
}




void MainWindow::on_sendmail_finanace_clicked()
{
    if (ui->recipient_2->text().isEmpty() || ui->subjectLineEdit_2->text().isEmpty() || ui->message_2->toPlainText().isEmpty()) {
        QMessageBox::warning(this, tr("Missing Information"), tr("Please fill in all fields: recipient, subject, and message."));
        return;
    }

    Smtp* smtp = new Smtp("ranimcherni03@gmail.com", "wdzbdvinwzsuqhjt", "smtp.gmail.com", 465);
    connect(smtp, &Smtp::status, this, [this](QString status) {

        if (status == "Message sent") {
            QMessageBox::information(this, tr("Success"), tr("Message sent successfully!"));
            ui->recipient_2->clear();
            ui->subjectLineEdit_2->clear();
            ui->message_2->clear();
        } else {
            QMessageBox::critical(this, tr("Error"), tr("Failed to send the message: %1").arg(status));
        }
    });

    smtp->sendMail("ranimcherni03@gmail.com", ui->recipient_2->text(), ui->subjectLineEdit_2->text(), ui->message_2->toPlainText());
}

void MainWindow::Function_Mailing()
{
    QSqlQuery query;
    query.prepare("SELECT * FROM GS_FINANCE WHERE MONTANT > 1000");
    Smtp* smtp = new Smtp("ranimcherni03@gmail.com", "wdzbdvinwzsuqhjt", "smtp.gmail.com", 465);
    if (query.exec()) {
        while (query.next()) {
            QString matricule = query.value(0).toString();
            QString typeTransaction = query.value(1).toString();
            QString categorie = query.value(2).toString();
            QString dateTransaction = query.value(3).toString();
            QString modePaiement = query.value(4).toString();
            QString statut = query.value(5).toString();
            double montant = query.value(6).toDouble();

            QString MSG_BODY = QString("Alerte : Transaction Importante\n\n"
                                       "Détails de la transaction :\n"
                                       "- Matricule : %1\n"
                                       "- Type de transaction : %2\n"
                                       "- Montant : %3\n\n"
                                       "Ce montant élevé nécessite votre attention immédiate.\n"
                                       "Veuillez vérifier cette transaction dès que possible.")
                               .arg(matricule)
                               .arg(typeTransaction)
                               .arg(QString::number(montant, 'f', 2)); // Ensure two decimal places for montant


            smtp->sendMail("ranimcherni03@gmail.com", "negrasyrine1@gmail.com", "Alerte: Montant élevé", MSG_BODY);

            F.write(F.time(), QString("Alerte : Matricule %1, Type de transaction : %2, Montant : %3")
                    .arg(matricule)
                    .arg(typeTransaction)
                    .arg(montant));

            ui->tb_Alertsss->setText(F.read());


        }
    } else
        qDebug() << "Erreur lors de l'exécution de la requête:" << query.lastError().text();

}

void MainWindow::on_supprimeralerte_finance_clicked()
{
    F.clearh();
    ui->tb_Alertsss->setText(F.read());
}

void MainWindow::checkAndUpdateRFID(const QString &RFID) {
    // Vérifier si le RFID existe dans GS_EMPLOYE
    QSqlQuery query;
    query.prepare("SELECT ID_EMPLOYE FROM PROJETPURESTAY.GS_EMPLOYE WHERE RFID = :RFID");
    query.bindValue(":RFID", RFID);

    if (query.exec()) {
        if (query.next()) {  // Si un enregistrement avec ce RFID est trouvé
            int employeeId = query.value("ID_EMPLOYE").toInt();
 qDebug() << employeeId;
            // Mettre à jour le statut
            QSqlQuery updateQuery;
            updateQuery.prepare("UPDATE PROJETPURESTAY.PRESENCE SET STATUS = 1 WHERE ID_EMPLOYEE = :ID_EMPLOYEE and DATE_PR = TRUNC(SYSDATE)");
            updateQuery.bindValue(":ID_EMPLOYEE", employeeId);
            qDebug() << "silkitchi??  :";

            if (updateQuery.exec()) {
                QMessageBox::information(this, "Succès",
                                          QString("RFID trouvé. ID employé : %1\nStatut mis à jour avec succès.")
                                          .arg(employeeId));
            } else {
                QMessageBox::critical(this, "Erreur",
                                      "Impossible de mettre à jour le statut : " + updateQuery.lastError().text());
            }
        } else {
            QMessageBox::warning(this, "RFID introuvable",
                                  "Le RFID n'existe pas dans la base de données.");
        }
    } else {
        QMessageBox::critical(this, "Erreur SQL",
                              "Erreur lors de la recherche du RFID : " + query.lastError().text());
    }
}




void MainWindow::readSerialData() {
    receivedData += serialPort->readAll(); // Lire les données disponibles

    if (receivedData.endsWith("\n")) { // Vérifier la fin des données
        receivedData = receivedData.trimmed(); // Nettoyer les espaces inutiles
        qDebug() << "Données reçues :" << receivedData;

        // Vérifier et mettre à jour le statut
        checkAndUpdateRFID(receivedData);
        // Afficher les données dans l'interface utilisateur
                ui->ERFID->setText(receivedData);

        receivedData.clear();
    }
}







void MainWindow::on_pushButton_SaveRFID_clicked() {
    QString RFID = ui->ERFID->text();


    if (RFID.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "L'ID RFID est vide.");
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE GS_EMPLOYE SET RFID = :RFID WHERE ID_EMPLOYE = :employeID");
    query.bindValue(":RFID", RFID);


    if (query.exec()) {
        QMessageBox::information(this, "Succès", "ID RFID enregistré avec succès.");
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible d'enregistrer l'ID RFID : " + query.lastError().text());
    }
}

/*
void MainWindow::scheduleDailyUpdate()
{
    // Calculer le temps restant jusqu'à minuit
    QDateTime now = QDateTime::currentDateTime();
    QDateTime nextMidnight = QDateTime(QDate::currentDate().addDays(1), QTime(0, 0, 0)); // Prochain minuit
    qint64 millisecondsToMidnight = now.msecsTo(nextMidnight);

    // Lancer un single-shot pour attendre jusqu'à minuit
    QTimer::singleShot(millisecondsToMidnight, this, [this]() {
        updatePresence(); // Exécuter la mise à jour à minuit
        dailyTimer->start(24 * 60 * 60 * 1000); // Répéter toutes les 24h
    });
}

*/





bool MainWindow::copyEmployeesToPresence()
{
    QSqlQuery checkQuery;
qDebug() <<"saaarrrrrrr";
    // Vérifier si des enregistrements pour aujourd'hui existent déjà dans la table PRESENCE
    checkQuery.prepare("SELECT COUNT(*) FROM PRESENCE WHERE DATE_PR = TRUNC(SYSDATE)");

    if (!checkQuery.exec()) {
        qDebug() << "Erreur lors de la vérification de la date actuelle : " << checkQuery.lastError().text();
        return false; // Échec
    }

    if (checkQuery.next() && checkQuery.value(0).toInt() > 0) {
        qDebug() << "Des enregistrements pour la date actuelle existent déjà dans la table PRESENCE.";
        return false; // Aucun ajout nécessaire
    }

    // Si aucun enregistrement pour aujourd'hui, insérer les employés
    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO PRESENCE (ID_EMPLOYEE, DATE_PR, STATUS) "
                        "SELECT ID_EMPLOYE, TRUNC(SYSDATE), 0 FROM GS_EMPLOYE");

    if (!insertQuery.exec()) {
        qDebug() <<"saaarrrrrrr";
        qDebug() << "Erreur lors de la copie des employés dans PRESENCE : " << insertQuery.lastError().text();
        qDebug() <<"saaarrrrrrr";
        return false; // Échec
    }

    qDebug() << "Les employés ont été copiés dans PRESENCE avec succès.";
     qDebug() <<"saaarrrrrrr";
    return true; // Succès
}

#include <QTime>
#include <QMessageBox>

QTime startTime; // Variable globale ou membre de la classe

/* ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * les cumule calculer enregistrer juste pour les employees present
 *
void MainWindow::on_BoutonDebutE_clicked() {
    // Vérifier si un calcul est déjà en cours
    if (startTime.isValid()) {
        QMessageBox::warning(this, "Erreur", "Le calcul est déjà en cours !");
        return;
    }

    // Enregistrer l'heure de début
    startTime = QTime::currentTime();
    QMessageBox::information(this, "Démarrage", "Le calcul des minutes a commencé !");
}

void MainWindow::on_BoutonFinE_clicked() {
    // Vérifier si le calcul a commencé
    if (!startTime.isValid()) {
        QMessageBox::warning(this, "Erreur", "Aucun calcul en cours à arrêter !");
        return;
    }

    // Calculez la durée en minutes écoulées
    QTime endTime = QTime::currentTime();
    int minutesElapsed = startTime.msecsTo(endTime) / 60000; // Convertir ms en minutes

    // Mettre à jour la colonne CUMULE pour tous les employés présents aujourd'hui
    QSqlQuery query;
    query.prepare("UPDATE PROJETPURESTAY.PRESENCE "
                  "SET CUMULE = CUMULE + :minutes "
                  "WHERE DATE_PR = TRUNC(SYSDATE)");
    query.bindValue(":minutes", minutesElapsed);

    if (query.exec()) {
        QMessageBox::information(this, "Succès",
                                 QString("Durée : %1 minutes ajoutées à tous les employés présents aujourd'hui.")
                                 .arg(minutesElapsed));
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la mise à jour : " + query.lastError().text());
    }

    // Réinitialiser l'heure de début
    startTime = QTime();
}*/

void MainWindow::on_BoutonDebutE_clicked() {
    // Vérifier si un calcul est déjà en cours
    if (startTime.isValid()) {
        QMessageBox::warning(this, "Erreur", "Le calcul est déjà en cours !");
        return;
    }

    // Enregistrer l'heure de début
    startTime = QTime::currentTime();
    QMessageBox::information(this, "Démarrage", "Le calcul des minutes a commencé !");
}


void MainWindow::on_BoutonFinE_clicked() {
    if (!startTime.isValid()) {
        QMessageBox::warning(this, "Erreur", "Aucun calcul en cours à arrêter !");
        return;
    }

    // Calculez la durée en minutes écoulées
    QTime endTime = QTime::currentTime();
    int minutesElapsed = startTime.msecsTo(endTime) / 60000;

    qDebug() << "Minutes écoulées :" << minutesElapsed;

    // ID de l'employé cible
    int targetEmployeeId = 14434880;

    // Récupérer les données de l'employé spécifique
    QSqlQuery query;
    query.prepare("SELECT P.ID_EMPLOYEE, P.CUMULE, E.DEROIT "
                  "FROM PROJETPURESTAY.PRESENCE P "
                  "JOIN PROJETPURESTAY.GS_EMPLOYE E ON P.ID_EMPLOYEE = E.ID_EMPLOYE "
                  "WHERE P.DATE_PR = TRUNC(SYSDATE) AND P.ID_EMPLOYEE = :targetEmployeeId");
    query.bindValue(":targetEmployeeId", targetEmployeeId);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Impossible de récupérer les données de l'employé : " + query.lastError().text());
        return;
    }

    if (!query.next()) {
        QMessageBox::warning(this, "Erreur", "Aucun employé trouvé avec l'ID spécifié pour aujourd'hui.");
        return;
    }

    int employeeId = query.value("ID_EMPLOYEE").toInt();
    int cumule = query.value("CUMULE").toInt();
    int deroit = query.value("DEROIT").toInt();

    qDebug() << "Employé trouvé : ID =" << employeeId << ", Cumule =" << cumule << ", DEROIT =" << deroit;

    QSqlDatabase::database().transaction();

    try {
        // Calcul des minutes manquantes par rapport à la minute obligatoire de 10min
        int newCumule = cumule + minutesElapsed;
        //int missingMinutes = 10 - newCumule;

        qDebug() << "Cumule avant mise à jour =" << cumule;
        qDebug() << "CUMULE mis à jour =" << newCumule;


    /*    if (cumule < 10) {
            // Si CUMULE < 10
            congéRestant = deroit - missingMinutes;
            ui->lineEdit_142->setText(QString::number(congéRestant));
            ui->label_90->setText("Vous avez dépassé la durée maximale de votre congé");
        } else {
            // Si CUMULE >= 10
            congéRestant = deroit - (10 - newCumule);
            ui->lineEdit_142->setText(QString::number(congéRestant));
            ui->label_90->setText("Merci pour vos efforts, votre période de vacances a été allongée");
        }
        */

        // Mettre à jour le CUMULE avec les minutes écoulées
        QSqlQuery updateCumule;
        updateCumule.prepare("UPDATE PROJETPURESTAY.PRESENCE SET CUMULE = :newCumule WHERE ID_EMPLOYEE = :employeeId AND DATE_PR = TRUNC(SYSDATE)");
        updateCumule.bindValue(":newCumule", newCumule);
        updateCumule.bindValue(":employeeId", employeeId);

        if (!updateCumule.exec()) {
            throw std::runtime_error("Erreur mise à jour CUMULE : " + updateCumule.lastError().text().toStdString());
        }
        qDebug() << "CUMULE mis à jour : " << newCumule;


        QSqlDatabase::database().commit();
        QMessageBox::information(this, "Succès", "Mise à jour réussie pour l'employé ID : " + QString::number(targetEmployeeId));

    } catch (const std::exception &e) {
        QSqlDatabase::database().rollback();
        QMessageBox::critical(this, "Erreur", "Transaction échouée : " + QString::fromStdString(e.what()));
    }

    startTime = QTime();
     calculerNombreJoursConge();
}




void MainWindow::on_BAjouterEmployee_clicked()
{
    if (copyEmployeesToPresence())
    {
        QMessageBox::information(this, "Succès", "Les employés ont été copiés dans la table PRESENCE avec succès.");
        UpdateEmploye();
        statistiquesDashboard();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la copie des employés dans la table PRESENCE.");
    }
}


void MainWindow::on_BoutonAjouterE_clicked() {
    int id = ui->EID->text().toInt();
    QString nom = ui->ENom->text();
    QString prenom = ui->EPrenomE->text();
    float salaire = ui->ESalaire->text().toFloat();
    QDate date_embauche = ui->EDateEmbauche->date();
    int num_telephone = ui->ENum->text().toInt();
    QString post = ui->EPoste->currentText();
    QString etat = ui->EEtat->currentText();
    QString mail = ui->EEmail->text();
    QString password = ui->EMotdePasse->text();
    QString rfid= ui->ERFID->text();
    if (nom.isEmpty() || prenom.isEmpty() || salaire <= 0 || num_telephone <= 0 || post.isEmpty()) {
        QMessageBox::critical(this, "Erreur", "Veuillez remplir tous les champs correctement.");
        return;
    }

    GS_EMPLOYE E(id, nom, prenom, salaire, date_embauche, num_telephone, post, etat, mail, password, rfid);
    bool test = E.ajouter();

    if (test)
    {
        QMessageBox::information(this, "Succès", "L'employé a été ajouté.");
        UpdateEmploye();
        statistiquesDashboard();
    }
    else
    {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout.L'id exist deja");
    }

}







void MainWindow::on_BSupprimerEmployee_clicked()
{
    QModelIndexList selectedRows = ui->tableEmployees->selectionModel()->selectedRows();

    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "Suppression", "Veuillez sélectionner un employé à supprimer.");
        return;
    }

    int id = selectedRows.first().data().toInt();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", "Voulez-vous vraiment supprimer cet employé ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        GS_EMPLOYE E;
        E.setID_employe(id);
        bool test = E.supprimer();

        if (test) {
            QMessageBox::information(this, "Suppression", "Employé supprimé avec succès.");
            ui->tableEmployees->setModel(E.afficher());
            UpdateEmploye();
            statistiquesDashboard();

        } else {
            QMessageBox::critical(this, "Erreur", "La suppression a échoué.");
        }
    }
}


void MainWindow::rechercherEmploye() {
    QString nom = ui->SearchBarEmploye->text();


    GS_EMPLOYE employe;
    QSqlQueryModel* model = employe.chercherParNom(nom);

    if (model) {

        ui->tableEmployees->setModel(model);
    } else {
        QMessageBox::warning(this, "Erreur", "Erreur lors de la recherche.");
    }
}




void MainWindow::rechercherParPoste(const QString& poste)
{

    GS_EMPLOYE employe;


    QSqlQueryModel* model = employe.chercherParPoste(poste);


    if (model) {
        ui->tableEmployees->setModel(model);
    } else {
        qDebug() << "Aucun résultat trouvé pour le poste " << poste;
    }
}

void MainWindow::rechercherParEtat(const QString& etat) {

    GS_EMPLOYE employe;


    QSqlQueryModel* model = employe.chercherParEtat(etat);


    if (model) {
        ui->tableEmployees->setModel(model);
    } else {
        qDebug() << "Aucun résultat trouvé pour l'état " << etat;
    }
}









void MainWindow::on_pushButton_charger_clicked() {
    int id = ui->EID->text().toInt();  // Supposons que l'ID est entré dans EID
    GS_EMPLOYE employe;
    employe.setID_employe(id);

    if (employe.chargerDonneesEmploye(id)) {
        // Charger les données dans les champs de l'interface pour modification
        chargerDonneesEmploye(employe);
    } else {
        QMessageBox::warning(this, "Erreur", "Échec du chargement des données de l'employé.");
    }
}




void MainWindow::trierParDateEmbauche() {
    GS_EMPLOYE employe;
    QSqlQueryModel* model = employe.trierParDateEmbauche();

    if (model) {
        ui->tableEmployees->setModel(model);  // Affiche le modèle trié dans la table
    } else {
        QMessageBox::critical(this, "Erreur", "Échec du tri par date d'embauche.");
    }
}


void MainWindow::trierParSalaireDecroissant() {
    GS_EMPLOYE employe;
    QSqlQueryModel* model = employe.trierParSalaireDecroissant();

    if (model) {
        ui->tableEmployees->setModel(model);
    } else {
        QMessageBox::critical(this, "Erreur", "Échec du tri par salaire décroissant.");
    }
}



void MainWindow::on_ComboSalaireE_currentIndexChanged(const QString& texte) {
    if (texte == "Salaire") {
        trierParSalaireDecroissant();
    }
    if (texte == "Date Embauche") {
        trierParDateEmbauche();
    }
}




void MainWindow::chargerDonneesEmploye(const GS_EMPLOYE& employe) {



    qDebug() << "Chargement des données de l'employé";

    // Vérification de l'ID de l'employé
    int idEmploye = employe.getID_employe();
    if (idEmploye == 0) {
        qDebug() << "ID de l'employé est invalide";
    }
    ui->EID->setText(QString::number(idEmploye));


    QString nom = employe.getNom();
    if (nom.isEmpty()) {
        qDebug() << "Nom de l'employé est vide";
    }
    ui->ENom->setText(nom);

    QString prenom = employe.getPrenom();
    if (prenom.isEmpty()) {
        qDebug() << "Prénom de l'employé est vide";
    }
    ui->EPrenomE->setText(prenom);

    double salaire = employe.getSalaire();
    if (salaire <= 0) {
        qDebug() << "Salaire de l'employé est invalide";
    }
    ui->ESalaire->setText(QString::number(salaire));

    QDate dateEmb = employe.getDate_embauche();
    if (!dateEmb.isValid()) {
        qDebug() << "Date d'embauche invalide";
    }
    ui->EDateEmbauche->setDate(dateEmb);


    int numTelephone = employe.getNum_telephone();
    if (numTelephone == 0) {
        qDebug() << "Numéro de téléphone invalide";
    }
    ui->ENum->setText(QString::number(numTelephone));



    qDebug() << "Données chargées avec succès!";
}


void MainWindow::mettreAJourTotalEmployes() {
    GS_EMPLOYE employe;
    int total = employe.calculerTotalEmployes();
    //ui->LabelNBAbsE->setText(QString::number(total));

    if (employe.ajouter()) {
        QMessageBox::information(this, "Succès", "Employé ajouté avec succès.");
        ui->tableEmployees->setModel(employe.afficher());
        mettreAJourTotalEmployes();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout de l'employé.");
    }

    if (employe.supprimer()) {
        QMessageBox::information(this, "Succès", "Employé supprimé avec succès.");
        ui->tableEmployees->setModel(employe.afficher());
        mettreAJourTotalEmployes();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression de l'employé.");
    }


}


void MainWindow::mettreAJourCalendrier(int id_employe) {
    GS_EMPLOYE employe;
    QList<QDate> absences = employe.getAbsences(id_employe);

    QTextCharFormat format;
    format.setBackground(Qt::red);
    format.setForeground(Qt::white);


    ui->CalendarWidgetE->setDateTextFormat(QDate(), QTextCharFormat());

    for (const QDate& date : absences) {
        QString dateStr = date.toString("yyyy-MM-dd"); // Conversion en chaîne dans le format souhaité
           QDate reformattedDate = QDate::fromString(dateStr, "yyyy-MM-dd"); //
        ui->CalendarWidgetE->setDateTextFormat(reformattedDate, format);
         qDebug() << "Date trouve";

         qDebug() << date;
         qDebug() << reformattedDate;
    }
}
void MainWindow::on_tableEmployees_clicked1(const QModelIndex& index) {
    int id_employe = index.sibling(index.row(), 0).data().toInt();
    mettreAJourCalendrier(id_employe);
}



void MainWindow::showCalendar() {
    qDebug() << "showCalendar() was called!";
    QMessageBox::information(this, "Calendrier", "L'employé sélectionné a été mis à jour dans le calendrier.");
}


void MainWindow::StatistiquesEmployee()
{
    /*
    int x = ui->ComboAnneeE->currentText().toInt();
    int index = ui->ComboMoisE->currentIndex();
       index=index+1;
    QDate premiereDateDuMois = QDate(x, index, 1);
    QDate dernierJourDuMois = QDate(x, index+1, 1);
*/




    int totalEmployes = calculerTotalEmployesadate();


    ui->LabelNBE->setText(QString("%1").arg(totalEmployes));

    double pourcentageAbsences = calculerPourcentageAbsences();

    if (pourcentageAbsences == -1) {
        ui->LabelPourAbsE->setText("Erreur dans le calcul du pourcentage d'absences.");
        return;
    }

    ui->LabelPourAbsE->setText(QString("%1").arg(QString::number(calculerNombreConges())));
    ui->LabelNBAbsE->setText(QString("%1").arg(QString::number(calculerNombreAbsents())));

    afficherGraphiqueAbsences();

}

/*
int MainWindow::calculerTotalAbsences(const QDate& premiereDateDuMois, const QDate& dernierJourDuMois) {



    QString premiereDateStr = premiereDateDuMois.toString("yyyy-MM-dd");
    QString dernierJourStr = dernierJourDuMois.toString("yyyy-MM-dd");


    QSqlQuery query;
    query.prepare("SELECT COUNT(*) AS totalAbsences "
                  "FROM PRESENCE "
                  "WHERE DATE_PR BETWEEN TO_DATE(:premiereDate, 'YYYY-MM-DD') "
                  "AND TO_DATE(:dernierJour, 'YYYY-MM-DD') "
                  "AND STATUS = 0");

    // Lier les valeurs
    query.bindValue(":premiereDate", premiereDateStr);
    query.bindValue(":dernierJour", dernierJourStr);


    if (!query.exec()) {
        qDebug() << "Erreur d'exécution de la requête : " ;
        return -1;
    }


    if (query.next()) {
        return query.value("totalAbsences").toInt();
    }

    return 0;
}
*/


int MainWindow::calculerTotalEmployesadate()
{
    QSqlQuery query;

    query.prepare("SELECT COUNT(*) AS totalEmployes FROM GS_EMPLOYE");

    if (!query.exec()) {
        qDebug() << "Erreur d'exécution de la requête : " << query.lastError().text();
        return 0; // return 0 in case of error
    }

    // Retrieve the total count of employees
    if (query.next()) {
        int totalEmployes = query.value("totalEmployes").toInt();
        return totalEmployes;
    }

    return 0; // return 0 if no result
}


int MainWindow::calculerNombreConges() {
    // Count the number of employees on work leave with ETAT 'Conge'
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) AS totalConges FROM GS_EMPLOYE WHERE ETAT = 'Conge'");

    if (!query.exec()) {
        qDebug() << "Erreur d'exécution de la requête de comptage des congés : " << query.lastError().text();
        return -1; // Return -1 in case of error
    }

    int totalConges = 0;
    if (query.next()) {
        totalConges = query.value("totalConges").toInt();
    }

    qDebug() << "Total des congés:" << totalConges;

    return totalConges; // Return the number of employees on work leave
}






int MainWindow::calculerNombreAbsents() {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) AS totalAbsences FROM GS_EMPLOYE WHERE ETAT ='Absent'");

    if (!query.exec()) {
        qDebug() << "Erreur d'exécution de la requête de comptage des absences : " << query.lastError().text();
        return -1;
    }

    int totalAbsences = 0;
    if (query.next()) {
        totalAbsences = query.value("totalAbsences").toInt();
    }

    qDebug() << "Total des absents:" << totalAbsences;

    return totalAbsences; // Return the number of absent employees
}


/**/

QPair<QDate, QDate> MainWindow::getDateRangeFromComboBoxes() {
    /*
    // Récupérer les valeurs sélectionnées dans les comboBox


    // Vérifier que les deux champs ont une valeur
    if (anneeStr.isEmpty() || moisStr.isEmpty()) {
        qDebug() << "Erreur : Année ou mois non sélectionnés.";
        return QPair<QDate, QDate>();
    }

    // Convertir les valeurs en entiers
    int annee = anneeStr.toInt();
    int mois = moisStr.toInt();

    // Construire les dates du premier et dernier jour du mois
    QDate premiereDate(annee, mois, 1);
    QDate dernierJour = premiereDate.addMonths(1).addDays(-1);

    return QPair<QDate, QDate>(premiereDate, dernierJour);
    */
}







void MainWindow::afficherGraphiqueAbsences() {
    QPieSeries *series = new QPieSeries();

    int totalAbsents = calculerNombreAbsents();
    int totalEmployees = calculerTotalEmployesadate();
    int totalPresences = totalEmployees - totalAbsents;

    series->append("Absences", totalAbsents);
    series->append("Présences", totalPresences);

    series->slices().at(0)->setColor(QColor(187,135,126));
    series->slices().at(1)->setColor(QColor(76,130,89));

    for (QPieSlice *slice : series->slices()) {
        slice->setLabel(QString("%1: %2%").arg(slice->label()).arg(slice->percentage() * 100, 0, 'f', 1)); // Show category name and percentage
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Statistiques des absences");
    chart->legend()->show();
    chart->setBackgroundBrush(Qt::transparent);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    chartView->setMinimumSize(450, 450);
    chartView->setMaximumSize(1000, 1000);
    chartView->setBackgroundBrush(Qt::transparent);

    QLayout *oldLayout = ui->widget_9->layout();
    if (oldLayout != nullptr) {
        QLayoutItem *item;
        while ((item = oldLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete oldLayout;
    }

    QVBoxLayout *layout = new QVBoxLayout(ui->widget_9);
    layout->addWidget(chartView);
    ui->widget_9->setLayout(layout);
}




void MainWindow::setupComboBoxes() {
/*
    for (int annee = 2000; annee <= QDate::currentDate().year(); ++annee) {
        ui->ComboAnneeE->addItem(QString::number(annee));
    }


    for (int mois = 1; mois <= 12; ++mois) {
        ui->ComboMoisE->addItem(QString::number(mois));
    }
    */
}

void MainWindow::onComboBox39Changed(const QString &text) {
    /*
    qDebug() << "ComboBox 39 value changed to: " << text;

    // Exemple: synchroniser avec ComboMoisE
    if (text == "Option 1") {
        ui->ComboMoisE->setCurrentText("Linked Option 1");
    }
    */
}

void MainWindow::onComboBox40Changed(const QString &text) {
    /*
    qDebug() << "ComboBox 40 value changed to: " << text;

    QMessageBox::information(this, "ComboBox 40", "Nouvelle valeur : " + text);
    */
}





#include <QPdfWriter>
#include <QPagedPaintDevice>
#include <QPainter>
#include <QTableView>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

// Votre fonction ExporterTableViewPDF et autres

void MainWindow::ExporterTableViewPDF(const QString &path)
{
    // Création du document PDF
    QPdfWriter writer(path);
    writer.setPageSize(QPageSize::A2);
    writer.setResolution(300);

    QPainter painter(&writer);
    if (!painter.isActive()) {
        qDebug() << "Erreur : Impossible de créer le fichier PDF.";
        return;
    }

    // Configurer les dimensions et marges
    const int margin = 30;              // Marges réduites pour plus d'espace
    const int cellHeight = 60;          // Hauteur ajustée des cellules
    const int headerHeight = 120;        // Hauteur ajustée des en-têtes
    int startX = margin;
    int startY = margin + headerHeight;

    // Titre du PDF
    QFont titleFont = painter.font();
    titleFont.setPointSize(12);         // Taille augmentée pour le titre
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.drawText(margin, margin + 10, "Exportation des données en PDF");

    // Accéder à la QTableView
    QTableView *table = ui->tableEmployees;
    if (!table) {
        qDebug() << "Erreur : Le tableau tableEmployees n'existe pas.";
        return;
    }

    QAbstractItemModel *model = table->model();
    if (!model) {
        qDebug() << "Erreur : Aucun modèle associé au tableau.";
        return;
    }

    int rowCount = model->rowCount();
    int colCount = model->columnCount();

    if (rowCount == 0 || colCount == 0) {
        qDebug() << "Erreur : Le modèle est vide.";
        return;
    }

    // Calculer les largeurs de colonnes pour occuper toute la largeur de la page
    QVector<int> columnWidths(colCount, 0);
    int totalWidthAvailable = writer.width() - 2 * margin;
    int totalDynamicWidth = 0;

    for (int col = 0; col < colCount; ++col) {
        int maxWidth = 0;
        for (int row = 0; row < rowCount; ++row) {
            QModelIndex index = model->index(row, col);
            QString cellText = model->data(index).toString();
            maxWidth = qMax(painter.fontMetrics().horizontalAdvance(cellText), maxWidth);
        }
        columnWidths[col] = maxWidth + 20; // Ajouter de l'espace pour la lisibilité
        totalDynamicWidth += columnWidths[col];
    }

    if (totalDynamicWidth < totalWidthAvailable) {
        int extraWidthPerColumn = (totalWidthAvailable - totalDynamicWidth) / colCount;
        for (int col = 0; col < colCount; ++col) {
            columnWidths[col] += extraWidthPerColumn;
        }
    }

    // Dessiner les en-têtes de colonnes
    QFont headerFont = painter.font();
    headerFont.setPointSize(12);        // Taille augmentée pour les en-têtes
    headerFont.setBold(true);
    painter.setFont(headerFont);

    for (int col = 0; col < colCount; ++col) {
        QRect rect(startX, margin + 30, columnWidths[col], headerHeight);
        painter.fillRect(rect, QColorConstants::Svg::pink); // Fond gris clair pour les en-têtes
        painter.drawRect(rect);
        painter.drawText(rect, Qt::AlignCenter, model->headerData(col, Qt::Horizontal).toString());
        startX += columnWidths[col];
    }

    startX = margin;

    // Dessiner les lignes et les colonnes avec les données
    QFont cellFont = painter.font();
    cellFont.setPointSize(12);          // Taille ajustée pour les cellules
    cellFont.setBold(false);
    painter.setFont(cellFont);

    for (int row = 0; row < rowCount; ++row) {
        for (int col = 0; col < colCount; ++col) {
            QRect rect(startX, startY, columnWidths[col], cellHeight);
            painter.fillRect(rect, Qt::white); // Fond blanc pour les cellules
            painter.drawRect(rect);

            QModelIndex index = model->index(row, col);
            QString cellText = model->data(index).toString();

            painter.drawText(rect, Qt::AlignCenter, cellText);

            startX += columnWidths[col];
        }
        startX = margin;
        startY += cellHeight;
    }

    painter.end();
    qDebug() << "Exportation terminée : " << path;
}



void MainWindow::on_BExporterPDFEmployee_clicked()
{
    // Boîte de dialogue pour demander le chemin du fichier PDF
    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer en PDF", "", "Fichiers PDF (*.pdf)");

    // Si un chemin de fichier a été sélectionné, appeler la méthode d'exportation
    if (!filePath.isEmpty()) {
        ExporterTableViewPDF(filePath);
        QMessageBox::information(this, "Exportation réussie", "Les données ont été exportées avec succès en PDF !");
    } else {
        QMessageBox::warning(this, "Exportation annulée", "Aucun fichier n'a été enregistré.");
    }
}

void MainWindow::on_tableEmployees_clicked(const QModelIndex &index) {
    if (!index.isValid()) return;

    QAbstractItemModel *model = ui->tableEmployees->model();
    int row = index.row(); // Obtenir la ligne sélectionnée

    // Remplir les champs avec les données de la ligne sélectionnée
    ui->EID->setText(model->data(model->index(row, 0)).toString()); // ID
    ui->ENom->setText(model->data(model->index(row, 1)).toString()); // Nom
    ui->EPrenomE->setText(model->data(model->index(row, 2)).toString());  // Prénom
    ui->ESalaire->setText(model->data(model->index(row, 3)).toString()); // Salaire
    ui->EDateEmbauche->setDate(QDate::fromString(model->data(model->index(row, 4)).toString(), "yyyy-MM-dd")); // Date embauche
    ui->ENum->setText(model->data(model->index(row, 5)).toString()); // Numéro de téléphone
    ui->EEtat->setCurrentText(model->data(model->index(row, 6)).toString()); // Etat
    ui->EPoste->setCurrentText(model->data(model->index(row, 7)).toString()); // Poste
    ui->EEmail->setText(model->data(model->index(row, 8)).toString()); // Mail
    ui->EMotdePasse->setText(model->data(model->index(row, 9)).toString()); // Password

    ui->lineEditIDE->setText(model->data(model->index(row, 0)).toString()); // ID
    ui->lineEditNomE->setText(model->data(model->index(row, 1)).toString()); // Nom
    ui->lineEditPrenomE->setText(model->data(model->index(row, 2)).toString()); // Prénom

}


void MainWindow::on_BModifierEmployee_clicked() {
    // Récupérer les données des champs
    int id = ui->EID->text().toInt();
    QString nom = ui->ENom->text();
    QString prenom = ui->EPrenomE->text();
    float salaire = ui->ESalaire->text().toFloat();
    QDate date_embauche = ui->EDateEmbauche->date();
    int num_telephone = ui->ENum->text().toInt();
    QString post = ui->EPoste->currentText();
    QString etat = ui->EEtat->currentText();
    QString mail = ui->EEmail->text();
    QString password = ui->EMotdePasse->text();
    QString rfid= ui->ERFID->text();

    // Validation des champs
    if (id <= 0 || nom.isEmpty() || prenom.isEmpty() || salaire <= 0 || num_telephone <= 0 || post.isEmpty()) {
        QMessageBox::critical(this, "Erreur", "Veuillez remplir tous les champs correctement.");
        return;
    }

    // Modifier l'employé
    GS_EMPLOYE E(id, nom, prenom, salaire, date_embauche, num_telephone, post, etat, mail, password, rfid );
    bool test = E.modifier();

    if (test) {
        QMessageBox::information(this, "Succès", "L'employé a été modifié avec succès.");
        ui->tableEmployees->setModel(E.afficher()); // Rafraîchir la vue
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification de l'employé.");
    }
}
/*
void MainWindow::calculerNombreJoursConge() {
    // Récupérer les dates de début et de fin du congé depuis les lineEdits
    QDate debut = ui->lineEdit_138->date();  // Date de début du congé
    QDate fin = ui->lineEdit_139->date();    // Date de fin du congé

    // Vérifier que les dates sont valides
    if (debut.isValid() && fin.isValid()) {
        // Calculer la différence en jours
        int nombreJours = debut.daysTo(fin);  // daysTo() renvoie la différence en jours entre deux dates

        // Vérifier si la date de fin est après la date de début
        if (nombreJours >= 0) {
            // Afficher le nombre de jours dans le lineEditNJE
            ui->lineEditNJE->setText(QString::number(nombreJours));
        } else {
            // Afficher un message d'erreur si la date de fin est antérieure à la date de début
            QMessageBox::critical(this, "Erreur", "La date de fin doit être postérieure à la date de début.");
        }
    } else {
        // Afficher un message d'erreur si l'une des dates est invalide
        QMessageBox::critical(this, "Erreur", "Veuillez sélectionner des dates valides.");
    }
}*/
/*
void MainWindow::calculerNombreJoursConge() {
    // Récupérer les dates de début et de fin du congé depuis les lineEdits
    QDateTime debut = ui->lineEdit_138->dateTime();  // Date et heure de début du congé
    QDateTime fin = ui->lineEdit_139->dateTime();    // Date et heure de fin du congé

    // Vérifier que les dates sont valides
    if (debut.isValid() && fin.isValid()) {
        // Calculer la différence en jours, heures et minutes
        qint64 totalMinutes = debut.msecsTo(fin) / 60000;  // Convertir les millisecondes en minutes
        int nombreJours = totalMinutes / (24 * 60);  // Nombre de jours (diviser par 1440 minutes par jour)
        int heuresRestantes = (totalMinutes % (24 * 60)) / 60;  // Heures restantes après avoir calculé les jours
        int minutesRestantes = totalMinutes % 60;  // Minutes restantes après avoir calculé les heures

        // Vérifier si la date de fin est après la date de début
        if (totalMinutes >= 0) {
            // Afficher le nombre de jours, heures et minutes dans le lineEditNJE
            QString resultat = QString("%1 jour(s), %2 heure(s) et %3 minute(s)")
                               .arg(nombreJours)
                               .arg(heuresRestantes)
                               .arg(minutesRestantes);
            ui->lineEditNJE->setText(resultat);
        } else {
            // Afficher un message d'erreur si la date de fin est antérieure à la date de début
            QMessageBox::critical(this, "Erreur", "La date de fin doit être postérieure à la date de début.");
        }
    } else {
        // Afficher un message d'erreur si l'une des dates est invalide
        QMessageBox::critical(this, "Erreur", "Veuillez sélectionner des dates valides.");
    }
}
*/

bool MainWindow::calculerNombreJoursConge() {

    qDebug() << "Marra";
    // Récupérer les dates de début et de fin du congé depuis les lineEdits
    QDateTime debut = ui->lineEditDateDE->dateTime();  // Date et heure de début du congé
    QDateTime fin = ui->lineEditDateFE->dateTime();    // Date et heure de fin du congé

    // Vérifier que les dates sont valides
    if (!debut.isValid() || !fin.isValid()) {
        QMessageBox::critical(this, "Erreur", "Veuillez sélectionner des dates valides.");
        return false;  // Retourner false si les dates sont invalides
    }

    // Calculer la différence en minutes entre les deux dates
    qint64 totalMinutes = debut.msecsTo(fin) / 60000;  // Convertir les millisecondes en minutes

    // Vérifier si la date de fin est après la date de début
    if (totalMinutes < 0) {
        QMessageBox::critical(this, "Erreur", "La date de fin doit être postérieure à la date de début.");
        return false;  // Retourner false si la différence est négative
    }

    // Récupérer le DEROIT de l'utilisateur depuis la base de données
    int targetEmployeeId = 14434880;  // Remplacez par l'ID réel si nécessaire
    QSqlQuery query;
    query.prepare("SELECT DEROIT FROM PROJETPURESTAY.GS_EMPLOYE WHERE ID_EMPLOYE = :employeeId");
    query.bindValue(":employeeId", targetEmployeeId);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Impossible de récupérer le DEROIT : " + query.lastError().text());
        return false;  // Retourner false si la requête échoue
    }

    if (!query.next()) {
        QMessageBox::critical(this, "Erreur", "Aucun employé trouvé avec l'ID spécifié.");
        return false;  // Retourner false si l'employé n'existe pas
    }

    int deroitRestant = query.value("DEROIT").toInt();  // Solde de congé restant en minutes
   ui->lineEditNJRE->setText(QString::number(deroitRestant));

    // Comparer la durée du congé demandé avec le DEROIT restant
    if (totalMinutes > deroitRestant) {
        QMessageBox::critical(this, "Erreur", "Le congé demandé dépasse votre solde de congé restant.");
        return false;  // Retourner false si le congé dépasse le solde
    }

    // Si la demande est valide, afficher le résultat dans un format lisible
    int nombreJours = totalMinutes / (24 * 60);               // Nombre de jours
    int heuresRestantes = (totalMinutes % (24 * 60)) / 60;    // Heures restantes
    int minutesRestantes = totalMinutes % 60;                // Minutes restantes

    QString resultat = QString("%1 jour(s), %2 heure(s) et %3 minute(s)")
                       .arg(nombreJours)
                       .arg(heuresRestantes)
                       .arg(minutesRestantes);
    ui->lineEditNJE->setText(resultat);
 qDebug() << "Échec de  calcule .";

    // Mise à jour du DEROIT
    int nouveauDeroit = deroitRestant - totalMinutes;
    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE PROJETPURESTAY.GS_EMPLOYE SET DEROIT = :nouveauDeroit WHERE ID_EMPLOYE = :employeeId");
    updateQuery.bindValue(":nouveauDeroit", nouveauDeroit);
    updateQuery.bindValue(":employeeId", targetEmployeeId);

    if (!updateQuery.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la mise à jour du DEROIT : " + updateQuery.lastError().text());
        return false;  // Retourner false si la mise à jour échoue
    }

    QMessageBox::information(this, "Succès", "Congé validé. Nouveau solde de congé : " + QString::number(nouveauDeroit) + " minutes.");
    return true;  // Retourner true si tout est réussi
}


void MainWindow::on_pushButton_13_clicked() {
    // Valider la durée du congé
    if (calculerNombreJoursConge()) {
        qDebug() << "Congé validé avec succès.";

        // Récupérer les informations nécessaires
        QDateTime debut = ui->lineEditDateDE->dateTime();
        QDateTime fin = ui->lineEditDateFE->dateTime();
        QString type = ui->ComboTypeConge->currentText();
        int idEmploye = ui->lineEditIDE->text().toInt();

        // Ajouter le congé via la classe GS_CONGE
        GS_CONGE g(idEmploye, debut, fin, type);
        g.ajouterConge(debut, fin, type); // Assurez-vous que cette méthode ne met pas à jour `DEROIT` deux fois.

    } else {
        qDebug() << "Échec de la validation du congé.";
    }
}


#include <QItemSelection>





void MainWindow::on_tableEmployees_clicked16(const QModelIndex &index) {
    if (index.isValid()) {
        // Récupérer l'ID de l'employé à partir de la ligne sélectionnée
        int id_employe = index.sibling(index.row(), 0).data().toInt();

        // Appeler la méthode pour afficher les congés de l'employé
        afficherCongesEmploye(id_employe);
    } else {
        QMessageBox::warning(this, "Aucune sélection", "Veuillez sélectionner une ligne valide.");
    }
}


void MainWindow::afficherCongesEmploye(int idEmploye) {
    // Préparer la requête SQL
    QSqlQuery query;
    query.prepare("SELECT c.ID_EMPLOYE, e.NOM, e.PRENOM, e.POST, c.TYPE, c.DEBUT, c.FIN "
                  "FROM CONGE c "
                  "JOIN GS_EMPLOYE e ON c.ID_EMPLOYE = e.ID_EMPLOYE "
                  "WHERE c.ID_EMPLOYE = :idEmploye");
    query.bindValue(":idEmploye", idEmploye);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la récupération des congés :" << query.lastError().text();
        return;
    }

    // Vérifiez si la requête retourne des résultats
    if (!query.next()) {
        qDebug() << "Aucun congé trouvé pour l'ID employé :" << idEmploye;
        return;
    }

    query.previous(); // Retour à la première ligne pour l'itération
    qDebug() << "Données trouvées pour l'ID employé :" << idEmploye;

    // Modèle pour afficher les données
    QStandardItemModel* model = new QStandardItemModel();
    model->setHorizontalHeaderLabels({"ID", "Nom", "Prénom", "Poste", "Type de Congé", "Date de Début", "Date de Fin"});

    // Parcourir les résultats de la requête
    int row = 0;
    while (query.next()) {
        qDebug() << "ID:" << query.value(0).toString()
                 << "Nom:" << query.value(1).toString()
                 << "Prénom:" << query.value(2).toString()
                 << "Poste:" << query.value(3).toString()
                 << "Type de Congé:" << query.value(4).toString()
                 << "Début:" << query.value(5).toDate().toString("dd-MM-yyyy")
                 << "Fin:" << query.value(6).toDate().toString("dd-MM-yyyy");

        model->setItem(row, 0, new QStandardItem(query.value(0).toString())); // ID
        model->setItem(row, 1, new QStandardItem(query.value(1).toString())); // Nom
        model->setItem(row, 2, new QStandardItem(query.value(2).toString())); // Prénom
        model->setItem(row, 3, new QStandardItem(query.value(3).toString())); // Poste
        model->setItem(row, 4, new QStandardItem(query.value(4).toString())); // Type de Congé
        model->setItem(row, 5, new QStandardItem(query.value(5).toDate().toString("dd-MM-yyyy"))); // Début
        model->setItem(row, 6, new QStandardItem(query.value(6).toDate().toString("dd-MM-yyyy"))); // Fin
        row++;
    }

    // Associer le modèle au tableau
    ui->TableConge->setModel(model);
    ui->TableConge->resizeColumnsToContents();
    ui->TableConge->horizontalHeader()->setStretchLastSection(true);
}

/*
void MainWindow::on_BoutonValider_clicked() {
if(clique==0){
    clique++;
qDebug() << "Manzoul marra bark";
    qDebug() << "Nazla";
    // Récupérer les informations saisies
    int idEmploye = ui->EID->text().toInt();  // ID de l'employé
    QDateTime debut = ui->lineEdit_138->dateTime();  // Date de début du congé
    QDateTime fin = ui->lineEdit_139->dateTime();  // Date de fin du congé
    QString type = ui->comboBox_35->currentText();  // Type de congé

    qDebug() << "hanna hounni:::::::::::";



    // Vérifier que les dates et le type de congé sont valides
    if (debut.isValid() && fin.isValid() && !type.isEmpty()) {
        // Créer l'objet GS_CONGE avec l'ID de l'employé
        GS_CONGE g(idEmploye, debut, fin, type);

        // Vérifier s'il y a un autre employé avec le même poste qui a un congé pendant la même période
        if (g.verifierCongeParPosteEtDate(idEmploye, debut, fin)) {
            // Mettre à jour le label avec le message d'erreur
            ui->labelCongeEmp->setText("Impossible de valider votre demande. \nUn autre employé du même poste a déjà un congé \npendant cette période.");
            ui->labelCongeEmp->setStyleSheet("color: white; font-weight: bold;"); // Affiche le texte en blanc et gras
            return;  // Arrêter le processus si une telle demande existe
             qDebug() << "hanna hounni  tawa :::::::::::";
        }

        // Si aucune collision de congé, on peut ajouter le congé dans la base de données
        g.ajouterConge(debut, fin, type);
 qDebug() << "voila  :::::::::::";
        // Afficher un message de succès dans le label
        ui->labelCongeEmp->setText("Le congé a été ajouté avec succès.");
        ui->labelCongeEmp->setStyleSheet("color: white; font-weight: bold;"); // Affiche le texte en blanc et gras
    } else {
        // Afficher un message d'erreur si les champs sont invalides
        ui->labelCongeEmp->setText("Veuillez remplir correctement tous les champs.");
        ui->labelCongeEmp->setStyleSheet("color: white; font-weight: bold;"); // Affiche le texte en blanc et gras
    }

    qDebug() << "Marra";


    calculerNombreJoursConge();
     qDebug() << "CaVa :::::::::::";
}

else{
 clique=0;
 }
}
*/

void MainWindow::on_BoutonValider_clicked() {

        qDebug() << "Bouton cliqué pour la première fois.";

        // Récupérer les informations saisies
        int idEmploye = ui->lineEditIDE->text().toInt();
        QDateTime debut = ui->lineEditDateDE->dateTime();
        QDateTime fin = ui->lineEditDateFE->dateTime();
        QString type = ui->ComboTypeConge->currentText();

        qDebug() << "ID Employé:" << idEmploye;
        qDebug() << "Début:" << debut.toString();
        qDebug() << "Fin:" << fin.toString();
        qDebug() << "Type de congé:" << type;

        // Vérifier la validité des dates et des champs
        if (!debut.isValid() || !fin.isValid() || type.isEmpty()) {
            ui->labelCongeEmp->setText("Veuillez remplir correctement tous les champs.");
            ui->labelCongeEmp->setStyleSheet("color: white; font-weight: bold;");
            return;
        }

        if (debut > fin) {
            ui->labelCongeEmp->setText("La date de début doit être antérieure à la date de fin.");
            ui->labelCongeEmp->setStyleSheet("color: white; font-weight: bold;");
            return;
        }

        GS_CONGE g(idEmploye, debut, fin, type);

        if (g.verifierCongeParPosteEtDate(idEmploye, debut, fin)) {
            ui->labelCongeEmp->setText("Un autre employé du même poste a déjà un congé pendant cette période.");
            ui->labelCongeEmp->setStyleSheet("color: white; font-weight: bold;");
 QMessageBox::information(this, "Erreur", "demande refusé : ");
            return;
        }

           if (!debut.isValid() || !fin.isValid()) {
               QMessageBox::critical(this, "Erreur", "Veuillez sélectionner des dates valides.");

           }
           qint64 totalMinutes = debut.msecsTo(fin) / 60000;

           if (totalMinutes < 0) {
               QMessageBox::critical(this, "Erreur", "La date de fin doit être postérieure à la date de début.");

           }

           int targetEmployeeId = 14434880;
           QSqlQuery query;
           query.prepare("SELECT DEROIT FROM PROJETPURESTAY.GS_EMPLOYE WHERE ID_EMPLOYE = :employeeId");
           query.bindValue(":employeeId", targetEmployeeId);

           if (!query.exec())
               QMessageBox::critical(this, "Erreur", "Impossible de récupérer le DEROIT : " + query.lastError().text());

           if (!query.next())
               QMessageBox::critical(this, "Erreur", "Aucun employé trouvé avec l'ID spécifié.");

           int deroitRestant = query.value("DEROIT").toInt();
          ui->labelCongeEmp->setText(QString::number(deroitRestant));

          QSqlQuery query1;
          query1.prepare("SELECT P.ID_EMPLOYEE, P.CUMULE, E.DEROIT "
                        "FROM PROJETPURESTAY.PRESENCE P "
                        "JOIN PROJETPURESTAY.GS_EMPLOYE E ON P.ID_EMPLOYEE = E.ID_EMPLOYE "
                        "WHERE P.DATE_PR = TRUNC(SYSDATE) AND P.ID_EMPLOYEE = :targetEmployeeId");
          query1.bindValue(":targetEmployeeId", targetEmployeeId);

          if (!query1.exec()) {
              QMessageBox::critical(this, "Erreur", "Impossible de récupérer les données de l'employé : " + query.lastError().text());
              return;
          }

          if (!query1.next()) {
              QMessageBox::warning(this, "Erreur", "Aucun employé trouvé avec l'ID spécifié pour aujourd'hui.");
              return;
          }


          int cumule = query1.value("CUMULE").toInt();
          qDebug() <<totalMinutes;
          qDebug() <<cumule;

        int hesba = totalMinutes + (10-cumule) ;
        qDebug() <<hesba;
        qDebug() <<deroitRestant;
           // Comparer la durée du congé demandé avec le DEROIT restant
           if (totalMinutes + (10-cumule) > deroitRestant) {
               QMessageBox::critical(this, "Erreur", "Le congé demandé dépasse votre solde de congé restant.");

           }

           else {
               // Si la demande est valide, afficher le résultat dans un format lisible
               int nombreJours = totalMinutes / (24 * 60);               // Nombre de jours
               int heuresRestantes = (totalMinutes % (24 * 60)) / 60;    // Heures restantes
               int minutesRestantes = totalMinutes % 60;                // Minutes restantes

               QString resultat = QString("%1 jour(s), %2 heure(s) et %3 minute(s)")
                                  .arg(nombreJours)
                                  .arg(heuresRestantes)
                                  .arg(minutesRestantes);
               ui->lineEditNJE->setText(resultat);
            qDebug() << "Échec de  calcule .";

               // Mise à jour du DEROIT
               int nouveauDeroit = deroitRestant - totalMinutes;
               QSqlQuery updateQuery;
               updateQuery.prepare("UPDATE PROJETPURESTAY.GS_EMPLOYE SET DEROIT = :nouveauDeroit WHERE ID_EMPLOYE = :employeeId");
               updateQuery.bindValue(":nouveauDeroit", nouveauDeroit);
               updateQuery.bindValue(":employeeId", targetEmployeeId);
               g.ajouterConge(debut, fin, type);
               if (!updateQuery.exec()) {
                   QMessageBox::critical(this, "Erreur", "Erreur lors de la mise à jour du DEROIT : " + updateQuery.lastError().text());

               }

               QMessageBox::information(this, "Succès", "Congé validé. Nouveau solde de congé : " + QString::number(nouveauDeroit) + " minutes.");

}
    }

void MainWindow::on_Hotel_Program_clicked()
{

    hotelManagerUI = new HotelManagerUI();

    hotelManagerUI->show();



}

void MainWindow::Equip_choix2_pie()
{
    QChartView *chartView = nullptr;
    QSqlQuery query;
    qreal tot = 0, depenseCount = 0, revenuCount = 0;

    query.prepare("SELECT COUNT(*) FROM GS_FINANCE");
    if (!query.exec()) {
        qDebug() << "Error executing query to get total count:" << query.lastError().text();
        return;
    }
    if (query.next()) {
        tot = query.value(0).toDouble();
    }
    qDebug() << "Total transactions: " << tot;

    if (tot == 0) {
        qDebug() << "No transactions available.";
        return;
    }

    query.prepare("SELECT TYPE_TRANSACTION, COUNT(*) FROM GS_FINANCE WHERE TYPE_TRANSACTION IN ('depense', 'revenu') GROUP BY TYPE_TRANSACTION");
    if (!query.exec()) {
        qDebug() << "Error executing query to get transaction counts:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        QString status = query.value(0).toString();
        int count = query.value(1).toInt();

        if (status == "depense") {
            depenseCount = count;
        } else if (status == "revenu") {
            revenuCount = count;
        }
    }

    qDebug() << "Dépense Count: " << depenseCount << ", Revenu Count: " << revenuCount;

    qreal c1 = (tot > 0) ? (depenseCount / tot) : 0;
    qreal c2 = (tot > 0) ? (revenuCount / tot) : 0;

    qDebug() << "Proportion Dépense: " << c1 << ", Proportion Revenu: " << c2;

    QPieSeries *series = new QPieSeries();

    QPieSlice *depenseSlice = series->append("Dépense", c1);
    depenseSlice->setBrush(QColor(187,135,126));

    QPieSlice *revenuSlice = series->append("Revenu", c2);
    revenuSlice->setBrush(QColor(76,130,89));

    for (QPieSlice *slice : series->slices()) {
        slice->setLabelVisible(true);
        slice->setLabel(QString("%1: %2%").arg(slice->label()).arg(slice->percentage() * 100, 0, 'f', 1));
    }

    if (chartView) {
        delete chartView;
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des Transactions");
    chart->setBackgroundBrush(Qt::transparent);

    chartView = new QChartView(chart, ui->finance_label_Stats);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(370, 370);
    chartView->show();
    chartView->setBackgroundBrush(Qt::transparent);
}
void MainWindow::UpdateFinance()
{
    QSqlQueryModel* model = F.afficher();

    Equip_choix2_pie();
    Equip_choix2_pie2();
    ui->finance_tableView->setModel(model);

    ui->finance_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->finance_tableView->horizontalHeader()->setFixedHeight(30);

    ui->finance_tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

}

void MainWindow::UpdateEmploye()
{
    connect(ui->tableEmployees, &QTableView::clicked, this, &MainWindow::on_tableEmployees_clicked16);

    ui->tableEmployees->setModel(E.afficher());
    ui->tableEmployees->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableEmployees->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->TableConge->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->TableConge->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->TableConge->horizontalHeader()->setStretchLastSection(true);
    StatistiquesEmployee();
    ui->LabelPourAbsE->setText(QString("%1").arg(QString::number(calculerNombreConges())));
    ui->LabelNBAbsE->setText(QString("%1").arg(QString::number(calculerNombreAbsents())));

    afficherGraphiqueAbsences();

}


void MainWindow::Equip_choix2_pie2()
{
    QChartView *chartView;
    QSqlQuery query;
    int especesCount = 0, creditCount = 0, virementCount = 0, chequeCount = 0;

    query.prepare("SELECT MODE_PAIEMENT, COUNT(*) FROM GS_FINANCE WHERE MODE_PAIEMENT IN ('especes ', 'Carte de credit ', 'Virement bancaire', 'Cheques') GROUP BY MODE_PAIEMENT");

    if (query.exec()) {
        while (query.next()) {
            QString status = query.value(0).toString();
            int count = query.value(1).toInt();

            if (status == "especes ") {
                especesCount = count;
            } else if (status == "Carte de credit ") {
                creditCount = count;
            } else if (status == "Virement bancaire") {
                virementCount = count;
            } else if (status == "Cheques") {
                chequeCount = count;
            }
        }
    }

    QPieSeries *series = new QPieSeries();

    series->append("Espèces", especesCount);
    series->append("Carte de crédit", creditCount);
    series->append("Virement bancaire", virementCount);
    series->append("Chèques", chequeCount);

    series->slices().at(0)->setBrush(QColor(76, 130, 89));
    series->slices().at(1)->setBrush(QColor(187, 135, 126));
    series->slices().at(2)->setBrush(QColor(245, 245, 220));
    series->slices().at(3)->setBrush(QColor(0, 100, 0));

    for (QPieSlice *slice : series->slices())
    {
        slice->setLabelVisible();
        slice->setLabel(QString("%1 - %2%").arg(slice->label()).arg(slice->percentage() * 100, 0, 'f', 1));
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignTop);
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->setTheme(QChart::ChartThemeLight);
    chart->setBackgroundBrush(Qt::transparent);
    chart->setMinimumSize(360, 360);

    chartView = new QChartView(chart, ui->finance_label_Stats_paiement);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(370, 370);
    chartView->setBackgroundBrush(Qt::transparent);
    chartView->show();
    chartView->update();
}


void MainWindow::on_SMSChambre_clicked()
{
    int currentRow = ui->tableWidget_7->currentRow();
    if (currentRow == -1) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une chambre nécessitant un entretien.");
        return;
    }

    QString chambreId = ui->tableWidget_7->item(currentRow, 0)->text();

    QString accountSid = "AC5c58385381e9721ebf061cca58df2700";
    QString authToken = "bec4d8440b0384e8d694ec5db3c6984c";
    QString fromNumber = "+15137158078";
    QString toNumber = "+21656530017";

    QUrl url("https://api.twilio.com/2010-04-01/Accounts/" + accountSid + "/Messages.json");

    QByteArray postData;
    postData.append("To=" + toNumber.toUtf8() + "&");
    postData.append("From=" + fromNumber.toUtf8() + "&");
    postData.append("Body=" + ("La chambre ID " + chambreId + " a besoin d'entretien.").toUtf8());

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QString credentials = accountSid + ":" + authToken;
    QString auth = "Basic " + QByteArray(credentials.toUtf8()).toBase64();
    request.setRawHeader("Authorization", auth.toUtf8());

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    QNetworkReply *reply = manager->post(request, postData);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QMessageBox::information(this, "SMS envoyé", "Le message a été envoyé avec succès pour la chambre ID " + chambreId + " !");
        } else {
            QMessageBox::critical(this, "Erreur d'envoi", "Erreur lors de l'envoi du SMS: " + reply->errorString());
        }
        reply->deleteLater();
        manager->deleteLater();
    }
    );
}


void MainWindow::on_pushButton_9_clicked()
{
    if (ui->listView_Historique->isVisible()) {
            ui->listView_Historique->hide();
        } else {
            ui->listView_Historique->show();
        }
}



void MainWindow::on_lineEdit_6_textChanged(const QString &arg1)
{
    // If the input field is empty, show all rooms
    if (arg1.isEmpty()) {
        afficherChambres();  // Assuming this function shows all rooms.
    }
    else {
        // If input is not empty, search based on the input duration (DR)
        afficherChambresParDuree(arg1);
    }
}

void MainWindow::afficherChambresParDuree(const QString &dureeText)
{
    QSqlQuery query;

    int duree = dureeText.toInt();

    query.prepare("SELECT * FROM chambre WHERE DR = :duree");
    query.bindValue(":duree", duree);

    // Execute the query
    if (!query.exec()) {
        qDebug() << "Query execution failed:" << query.lastError();

        return;
    }

    ui->tableWidget_7->clearContents();
    ui->tableWidget_7->setRowCount(0);

    int row = 0;
    while (query.next()) {
        ui->tableWidget_7->insertRow(row);

        ui->tableWidget_7->setItem(row, 0, new QTableWidgetItem(query.value("ID_CHAMBRE").toString()));
        ui->tableWidget_7->setItem(row, 1, new QTableWidgetItem(query.value("type_chambre").toString()));
        ui->tableWidget_7->setItem(row, 2, new QTableWidgetItem(query.value("dispo").toString()));
        ui->tableWidget_7->setItem(row, 3, new QTableWidgetItem(query.value("PPN").toString()));
        ui->tableWidget_7->setItem(row, 4, new QTableWidgetItem(query.value("DR").toString()));

        row++;
    }
}



void MainWindow::on_finance_recherche_textChanged(const QString &arg1)
{
    F.clearTable(ui->finance_tableView);

    F.Recherche(ui->finance_tableView, arg1, 0.0);

}


void MainWindow::on_BoutonAjouter_clicked()
{
    ui->tabWidget_2->setCurrentIndex(2);
}

