#ifndef FENETREMESSAGE_H
#define FENETREMESSAGE_H
#include "arduinosms.h"
#include "mainwindow.h"
#include <QMainWindow>

namespace Ui {
class fenetremessage;
}

class fenetremessage : public QMainWindow
{
    Q_OBJECT

public:
    explicit fenetremessage(QWidget *parent = nullptr);
    ~fenetremessage();
    bool VerificationNumero(QString numerotelephone);

private slots:
    void on_BoutonEnvoyerSMS_clicked();

private:
    Ui::fenetremessage *ui;
    arduinosms A;
};

#endif // FENETREMESSAGE_H
