#include "notification.h"

Notification::Notification(const QString &message, QWidget *parent)
    : QWidget(parent)
{
    // Set the window to be frameless and not focusable
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground); // For a translucent background

    // Create widgets
    messageLabel = new QLabel(message, this);
    closeButton = new QPushButton("Close", this);

    // Style the components (optional)
    messageLabel->setStyleSheet("font-size: 16px; color: white;");
    closeButton->setStyleSheet("background-color: red; color: white;");

    // Layout the widgets
    layout = new QVBoxLayout(this);
    layout->addWidget(messageLabel);
    layout->addWidget(closeButton);

    connect(closeButton, &QPushButton::clicked, this, &Notification::hideNotification);

    // Timer to auto-hide the notification after a set time (optional)
    hideTimer = new QTimer(this);
    hideTimer->setSingleShot(true);
    connect(hideTimer, &QTimer::timeout, this, &Notification::hideNotification);

    // Set the size and position of the notification
    setFixedSize(300, 100);  // You can adjust this size
    move(QApplication::desktop()->screen()->rect().bottomLeft() - QPoint(width(), 0));
}

Notification::~Notification()
{
}

void Notification::showNotification()
{
    show(); // Show the widget
    hideTimer->start(5000); // Optionally hide after 5 seconds
}

void Notification::hideNotification()
{
    hide(); // Hide the notification
}

void Notification::setMessage(const QString &message)
{
    messageLabel->setText(message); // Update the message text
}
