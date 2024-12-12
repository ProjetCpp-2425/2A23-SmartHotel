#ifndef INTERFACELOGIN_H
#define INTERFACELOGIN_H

#include <QMainWindow>
#include "mainwindow.h"

namespace Ui {
class interfacelogin;
}

class interfacelogin : public QMainWindow
{
    Q_OBJECT

public:
    explicit interfacelogin(QWidget *parent = nullptr);
    ~interfacelogin();
    bool VerificationLogin();

private slots:

    void on_LoginButton_clicked();

private:
    Ui::interfacelogin *ui;
    MainWindow *w;
};

#endif // INTERFACELOGIN_H
