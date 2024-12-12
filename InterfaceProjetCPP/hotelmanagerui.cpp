#include "hotelmanagerui.h"
#include "ui_hotelmanagerui.h"
#include <QDebug>

HotelManagerUI::HotelManagerUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HotelManagerUI),
    serialPort(new QSerialPort(this))
{
    ui->setupUi(this);
    setupUI();
    initializeConnections();
    updatePortList();
}

HotelManagerUI::~HotelManagerUI()
{
    if (serialPort->isOpen()) {
        serialPort->close();
    }
}

void HotelManagerUI::setupUI()
{
    // Setup days combobox
    QStringList days = {"Monday", "Tuesday", "Wednesday", "Thursday",
                        "Friday", "Saturday", "Sunday"};
    ui->comboBox_days->addItems(days);

    // Initialize UI state
    ui->pushButton_send->setEnabled(false);
    ui->textEdit_program->setEnabled(false);

    // Initialize programs
    for (int i = 0; i < 7; i++) {
        dayPrograms[i] = "No program set";
    }

    updateStatusLabel("Disconnected");
}

void HotelManagerUI::initializeConnections()
{
    connect(serialPort, &QSerialPort::readyRead, this, &HotelManagerUI::handleReadData);
}

void HotelManagerUI::on_pushButton_scan_clicked()
{
    updatePortList();
}

void HotelManagerUI::updatePortList()
{
    ui->comboBox_ports->clear();
    const auto ports = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : ports) {
        ui->comboBox_ports->addItem(info.portName());
    }
}

void HotelManagerUI::on_pushButton_connect_clicked()
{
    if (!serialPort->isOpen()) {
        QString portName = ui->comboBox_ports->currentText();
        if (connectArduino(portName)) {
            ui->pushButton_connect->setText("Disconnect");
            ui->pushButton_send->setEnabled(true);
            ui->textEdit_program->setEnabled(true);
            updateStatusLabel("Connected");
        }
    } else {
        disconnectArduino();
        ui->pushButton_connect->setText("Connect");
        ui->pushButton_send->setEnabled(false);
        ui->textEdit_program->setEnabled(false);
        updateStatusLabel("Disconnected");
    }
}

bool HotelManagerUI::connectArduino(const QString& portName)
{
    serialPort->setPortName(portName);
    serialPort->setBaudRate(QSerialPort::Baud9600);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (!serialPort->open(QIODevice::ReadWrite)) {
        QMessageBox::warning(this, "Connection Error",
                             "Could not open port " + portName + ": " + serialPort->errorString());
        return false;
    }
    return true;
}

void HotelManagerUI::disconnectArduino()
{
    if (serialPort->isOpen()) {
        serialPort->close();
    }
}

void HotelManagerUI::on_pushButton_send_clicked()
{
    int day = ui->comboBox_days->currentIndex() + 1;
    QString program = ui->textEdit_program->toPlainText().trimmed();

    if (program.isEmpty()) {
        QMessageBox::warning(this, "Error", "Program cannot be empty");
        return;
    }

    if (sendProgram(day, program)) {
        updateStatusLabel("Program sent for " + ui->comboBox_days->currentText());
    }
}

bool HotelManagerUI::sendProgram(int day, const QString& program)
{
    if (!serialPort->isOpen() || day < 1 || day > 7) {
        return false;
    }

    if (!validateProgram(program)) {
        QMessageBox::warning(this, "Error", "Program is too long (max 80 characters)");
        return false;
    }

    // Format: PRG:day:program
    QString message = QString("PRG:%1:%2\n").arg(day).arg(program);
    if (serialPort->write(message.toUtf8()) != -1) {
        dayPrograms[day - 1] = program;
        return true;
    }
    return false;
}

void HotelManagerUI::handleReadData()
{
    while (serialPort->canReadLine()) {
        QByteArray data = serialPort->readLine();
        processIncomingData(QString::fromUtf8(data).trimmed());
    }
}

void HotelManagerUI::processIncomingData(const QString& data)
{
    if (data.startsWith("TEMP:")) {
        QStringList parts = data.mid(5).split(",");
        if (parts.size() == 2) {
            bool tempOk, humOk;
            float temp = parts[0].toFloat(&tempOk);
            float humidity = parts[1].toFloat(&humOk);

        }
    } else if (data.startsWith("ACK:")) {
        int day = data.mid(4).toInt();
        if (day >= 1 && day <= 7) {
            updateStatusLabel("Program confirmed for " + ui->comboBox_days->itemText(day - 1));
        }
    }
}



void HotelManagerUI::on_pushButton_clear_clicked()
{
    ui->textEdit_program->clear();
}

void HotelManagerUI::on_comboBox_days_currentIndexChanged(int index)
{
    if (index >= 0 && index < 7) {
        ui->textEdit_program->setText(dayPrograms[index]);
    }
}

void HotelManagerUI::updateStatusLabel(const QString& message)
{
    ui->label_status->setText("Status: " + message);
}

bool HotelManagerUI::validateProgram(const QString& program)
{
    return program.length() <= 80;  // LCD display limitation
}
