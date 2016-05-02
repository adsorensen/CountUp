/**
* Logical backend behind the database
*
* CS3505, April 2016.
* Team: It's Lit (fire, fire, fire)
*
* @author Adam Sorensen
*         Connor Douglas
*         Johnny Le
*         Michelle Nguyen
*         Steven Sun
*         Trung Le
*         Yu Song
*         Zhi Wong
*/

#ifndef NETWORK_H
#define NETWORK_H
#include <QString>
#include <stdio.h>
#include <usr/include/cppconn/driver.h>
#include <usr/include/cppconn/exception.h>
#include <usr/include/cppconn/resultset.h>
#include <usr/include/cppconn/statement.h>
#include <usr/include/mysql_connection.h>
#include <QVector>


class Network
{
public:
    Network();
    QString toQString(std::string const &s);
    std::string fromQString(QString const &s);
    QVector<QString> getPlayerInfo(QString username);
    QVector<QString> getPlayerLevel(QString username);
    int registerUser(QString username, QString password, bool admin, QString userclass);
    bool removeUser(QString username, QString mainuser);
    bool updateHighscore(QString username, QString level, QString difficulty, QString highscore);
    int checkUserLogin(QString username, QString password);
    bool checkAdmin(QString name);
};

#endif // NETWORK_H
