#ifndef CONNECTION_H
#define CONNECTION_H
#include <QSqlDatabase>


class Connexion
{

    QSqlDatabase db;

public:
    Connexion(){};
    bool CreateConnection();
};

#endif // CONNECTION_H
