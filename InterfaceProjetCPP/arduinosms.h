#ifndef ARDUINOSMS_H
#define ARDUINOSMS_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>

class arduinosms
{
public:
    arduinosms();
    int connect_arduino();
    int close_arduino();
    void write_to_arduino( const QString &d  );
    QByteArray read_from_arduino();
    QSerialPort* getserial();
    QString getarduino_port_name();
    private:
    QSerialPort * serial;
    static const quint16 arduino_uno_vendor_id=0x2341;
    static const quint16 arduino_uno_producy_id=0x0043;
    QString arduino_port_name;
    bool arduino_is_available;
    QByteArray data;
};

#endif // ARDUINOSMS_H
