#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTimer>

class Notification : public QWidget
{
    Q_OBJECT

public:
    explicit Notification(const QString &message, QWidget *parent = nullptr);
    ~Notification();

    void showNotification();
    void hideNotification();
    void setMessage(const QString &message);


private:
    QLabel *messageLabel;
    QPushButton *closeButton;
    QVBoxLayout *layout;
    QTimer *hideTimer;
};

#endif // NOTIFICATION_H
