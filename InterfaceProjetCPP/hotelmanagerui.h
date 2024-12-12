#ifndef HOTELMANAGERUI_H
#define HOTELMANAGERUI_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QDateTime>
#include <QTimer>

namespace Ui {
class HotelManagerUI;
}

class HotelManagerUI : public QWidget
{
    Q_OBJECT

public:
    explicit HotelManagerUI(QWidget *parent = nullptr);
    ~HotelManagerUI();

private slots:
    void on_pushButton_scan_clicked();
    void on_pushButton_connect_clicked();
    void on_pushButton_send_clicked();
    void on_pushButton_clear_clicked();
    void on_comboBox_days_currentIndexChanged(int index);
    void handleReadData();
    void updatePortList();

private:
    Ui::HotelManagerUI *ui;
    QSerialPort *serialPort;
    QString dayPrograms[7];
    QTimer *tempTimer;

    void setupUI();
    void initializeConnections();
    bool connectArduino(const QString& portName);
    void disconnectArduino();
    bool sendProgram(int day, const QString& program);
    void processIncomingData(const QString& data);
    void updateStatusLabel(const QString& message);
    bool validateProgram(const QString& program);
};

#endif // HOTELMANAGERUI_H
