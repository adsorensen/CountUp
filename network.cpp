/**
* Representation of the database backend
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

#include "network.h"
#include <usr/include/cppconn/driver.h>
#include <usr/include/cppconn/exception.h>
#include <usr/include/cppconn/resultset.h>
#include <usr/include/cppconn/statement.h>
#include <usr/include/mysql_connection.h>
#include <QDebug>

Network::Network() {
}

//returns username, password, admin status, currentlevel, averagescore, userclass in that order
QVector<QString> Network::getPlayerInfo(QString username)
{

    QVector<QString> playerInfo;

    try{
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;

        driver = get_driver_instance();

        con = driver->connect("georgia.eng.utah.edu","cs5530u108","6pa21pkl");

        std::string name = fromQString(username);


        stmt = con->createStatement();
        std::string execute = "SELECT * FROM `cs5530db108`.`MathCrunchUsers` WHERE `Username` = '" + name + "';";

        int col = 1;

        res = stmt->executeQuery(execute);
        while (res->next())
        {
            qDebug() << "\t... MySQL replies: ";
            std::string temp = res->getString(col);
            QString ex = toQString(temp);
            qDebug() << ex;
            playerInfo.append(ex);
            col++;

        }
        delete res;
        delete stmt;
        delete con;
    }
    catch(sql::SQLException &e)
    {
        qDebug() << "error";
    }


    return playerInfo;
}

//returns username, level, difficulty, highscore in that order
QVector<QString> Network::getPlayerLevel(QString username)
{
    QVector<QString> playerInfo;

    try{
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;

        driver = get_driver_instance();

        con = driver->connect("georgia.eng.utah.edu","cs5530u108","6pa21pkl");

        std::string name = fromQString(username);


        stmt = con->createStatement();
        std::string execute = "SELECT * FROM `cs5530db108`.`MathCrunchLevel` WHERE `Username` = '" + name + "';";

        int col = 1;

        res = stmt->executeQuery(execute);
        while (res->next())
        {
            qDebug() << "\t... MySQL replies: ";
            std::string temp = res->getString(col);
            QString ex = toQString(temp);
            qDebug() << ex;
            playerInfo.append(ex);
            col++;

        }
        delete res;
        delete stmt;
        delete con;
    }
    catch(sql::SQLException &e)
    {
        qDebug() << "error";
    }
    return playerInfo;
}


int Network::registerUser(QString username, QString password, bool admin, QString userclass)
{
    bool success = false;
    int flag = 0;

    try{
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;

        driver = get_driver_instance();

        con = driver->connect("georgia.eng.utah.edu","cs5530u108","6pa21pkl");

        std::string nameS = fromQString(username);
        std::string pwS = fromQString(password);
        std::string adminS = "";
        if (admin)
        {
            adminS = "true";
        }
        else
        {
            adminS = "false";
        }

        std::string currentlevelS = "1";
        std::string avgscoreS = "0";
        std::string userclassS = fromQString(userclass);

        stmt = con->createStatement();

        std::string execute = "INSERT INTO `cs5530db108`.`MathCrunchUsers` (`Username`, `Password`, `AdminStatus`, `CurrentLevel`, `AverageScore`, `UserClass`) VALUES ('" + nameS + "','" + pwS + "','" + adminS + "','" + currentlevelS + "','" + avgscoreS +"','" + userclassS +"');";

        success = stmt->execute(execute);
        flag = 1;

        //delete res;
        delete stmt;
        delete con;
    }
    catch(sql::SQLException &e)
    {
        qDebug() << "error" << e.what();
        flag = 2;
    }
    return flag;
}

bool Network::removeUser(QString username, QString userclass)
{
    bool success = false;

    try{
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;

        driver = get_driver_instance();

        con = driver->connect("georgia.eng.utah.edu","cs5530u108","6pa21pkl");

        std::string nameS = fromQString(username);
        std::string userclassS = fromQString(userclass);

        stmt = con->createStatement();
        std::string execute = "DELETE FROM `cs5530db108`.`MathCrunchUsers` WHERE 'Username' = '" + nameS + "' 'UserClass' = '" + userclassS + "';";

        success = stmt->execute(execute);


        delete res;
        delete stmt;
        delete con;
    }
    catch(sql::SQLException &e)
    {
        qDebug() << "error";
    }

    return success;

}

bool Network::updateHighscore(QString username, QString level, QString difficulty, QString highscore, QString currentlevel)
{
    bool success1 = false;
    int sum;
    int count = 0;

    try{
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;

        driver = get_driver_instance();

        con = driver->connect("georgia.eng.utah.edu","cs5530u108","6pa21pkl");

        std::string nameS = fromQString(username);
        std::string levelS = fromQString(level);
        std::string difficultyS = fromQString(difficulty);
        std::string highscoreS = fromQString(highscore);
        std::string currentlevelS = fromQString(currentlevel);

        stmt = con->createStatement();
        std::string execute = "SELECT CurrentLevel FROM `cs5530db108`.`MathCrunchUsers` WHERE 'Username' = '" + nameS + "';";

        res = stmt->executeQuery(execute);
        while (res->next())
        {
            int temp = res->getInt(1);
            if (level.QString::toInt() > temp) {
                stmt = con->createStatement();
                std::string execute1 = "INSERT INTO `cs5530db108`.`MathCrunchLevel` (`Username`, `Level`, `Difficulty`, `HighScore`) VALUES ('" + nameS + "'," + levelS + "," + difficultyS + ", " + highscoreS + "');";

                success1 = stmt->execute(execute1);
            }
            else {
                stmt = con->createStatement();
                std::string execute2 = "UPDATE `cs5530db108`.`MathCrunchLevel` SET HighScore = " + highscoreS + " WHERE 'Username' = '" + nameS + "' AND 'Difficulty' = " + difficultyS + " AND 'Level' = " + currentlevelS + " AND 'HighScore' < " + highscoreS + ";";

                success1 = stmt->execute(execute2);
            }
        }

        stmt = con->createStatement();
        execute = "SELECT HighScore FROM `cs5530db108`.`MathCrunchLevel` WHERE 'Username' = '" + nameS + "';";
        res = stmt->executeQuery(execute);

        while (res->next())
        {
            int temp = res->getInt(1);
            sum += temp;
            count++;
        }

        //averageScore = (total / count);

        //execute = "UPDATE `cs5530db108`.`MathCrunchUser` SET AverageScore = " + averageScore + " WHERE 'Username' = '" + nameS + "';";
        res = stmt->executeQuery(execute);

        delete res;
        delete stmt;
        delete con;
    }
    catch(sql::SQLException &e)
    {
        qDebug() << "error";
    }

    return success1;

}

int Network::checkUserLogin(QString username, QString password)
{
    int success = 0;
    QVector<QString> playerInfo = getPlayerInfo(username);
    std::string name = fromQString(username);
    std::string passwordS = fromQString(password);

    std::string query = "SELECT * from `cs5530db108`.`MathCrunchUsers` WHERE Username = '" + name + "' AND Password = '" + passwordS + "';";
    std::string query2 = "SELECT * FROM `cs5530db108`.`MathCrunchUsers` WHERE Username = '" + name + "';";
    try
    {
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;

        driver = get_driver_instance();

        con = driver->connect("georgia.eng.utah.edu","cs5530u108","6pa21pkl");

        stmt = con->createStatement();

        res = stmt->executeQuery(query);

        if (res->next())
        {
            success = 1;
        }
        else
        {
            res = stmt->executeQuery(query2);
            if (res->next())
            {
                //wrong password
                success = 2;
            }
            else
            {
                //user doesn't exist
                success = 3;
            }
        }
        delete res;
        delete con;
        delete stmt;

    }
    catch(sql::SQLException &e)
    {
        qDebug() << "error" << e.what();
        success = -1;
    }

    return success;
}

//helper methods
QString Network::toQString(std::string const &s)
{
    return QString::fromUtf8(s.c_str());
}

std::string Network::fromQString(QString const &s)
{
    return std::string(s.toUtf8().data());
}

