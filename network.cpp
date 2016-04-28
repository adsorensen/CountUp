#include "network.h"
#include <usr/include/cppconn/driver.h>
#include <usr/include/cppconn/exception.h>
#include <usr/include/cppconn/resultset.h>
#include <usr/include/cppconn/statement.h>
#include <usr/include/mysql_connection.h>
#include <QDebug>

Network::Network() {
}

//returns currentlevel, averagescore in that order
QVector<QString> Network::getPlayerInfo(QString username)
{
    std::string temp;
    QVector<QString> playerInfo;

    try{
    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;
    int count = 1;

    driver = get_driver_instance();

    con = driver->connect("georgia.eng.utah.edu","cs5530u108","6pa21pkl");

    std::string name = fromQString(username);


    stmt = con->createStatement();
    std::string execute = "SELECT * FROM `cs5530db108`.`MathCrunchUsers` WHERE `Username` = '" + name + "';";

      res = stmt->executeQuery(execute);
      if (res->next())
      {

        temp = res->getString(4);
        QString ex = toQString(temp);

        playerInfo.append(ex);

        temp = res->getString(5);
        ex = toQString(temp);

        playerInfo.append(ex);
        count++;
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


      res = stmt->executeQuery(execute);
      if (res->next())
      {
        std::string temp = res->getString(2);
        QString ex = toQString(temp);

        playerInfo.append(ex);
        temp = res->getString(3);
        ex = toQString(temp);

        playerInfo.append(ex);
      }


      delete res;
      delete stmt;
      delete con;
    }
    catch(sql::SQLException &e)
    {
        qDebug() << "error with getting player level";
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
    std::string diff = "1";
    int level = 1;

    stmt = con->createStatement();

    std::string execute = "INSERT INTO `cs5530db108`.`MathCrunchUsers` (`Username`, `Password`, `AdminStatus`, `CurrentLevel`, `AverageScore`, `UserClass`) VALUES ('" + nameS + "','" + pwS + "','" + adminS + "','" + currentlevelS + "','" + avgscoreS +"','" + userclassS +"');";
    std::string execute2 = "INSERT INTO `cs5530db108`.`MathCrunchLevel` (`Username`, `Level`, `Difficulty`) VALUES ('" + nameS + "','" + currentlevelS + "','" + diff + "');";
    success = stmt->execute(execute);
    stmt->execute(execute2);
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
    std::string execute = "DELETE FROM `cs5530db108`.`MathCrunchUsers` WHERE Username = '" + nameS + "' UserClass = '" + userclassS + "';";

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

bool Network::updateHighscore(QString username, QString level, QString difficulty, QString highscore)
{
    bool success1 = false;
    int sum = 0;
    int count = 0;
    int averageScore;
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
    std::string nextLevelS = fromQString(QString::number((difficulty.toInt() * level.toInt()) + 1));


    stmt = con->createStatement();
    std::string execute = "SELECT * FROM `cs5530db108`.`MathCrunchUsers` WHERE Username = '" + nameS + "';";

    qDebug() << "0";

    res = stmt->executeQuery(execute);

    qDebug() << "1";
    while (res->next())
    {
        qDebug() << "2";
      int temp = res->getInt("CurrentLevel");
      qDebug() << "3";
      if ((level.QString::toInt() * difficulty.QString::toInt()) == temp) {

          stmt = con->createStatement();
          std::string execute1 = "INSERT INTO `cs5530db108`.`MathCrunchLevel` (`Username`, `Level`, `Difficulty`, `HighScore`) VALUES ('" + nameS + "'," + levelS + "," + difficultyS + ", " + highscoreS + ");";
          success1 = stmt->execute(execute1);

          execute1 = "UPDATE `cs5530db108`.`MathCrunchUsers` SET CurrentLevel = " + nextLevelS + " WHERE Username = '" + nameS + "';";
          success1 = stmt->execute(execute1);
      }
      else {

          stmt = con->createStatement();
          std::string execute2 = "UPDATE `cs5530db108`.`MathCrunchLevel` SET HighScore = " + highscoreS + " WHERE Username = '" + nameS + "' AND Difficulty = " + difficultyS + " AND Level = " + levelS + " AND HighScore < " + highscoreS + ";";
          success1 = stmt->execute(execute2);
      }

    }

    stmt = con->createStatement();
    execute = "SELECT * FROM `cs5530db108`.`MathCrunchLevel` WHERE Username = '" + nameS + "';";
        qDebug() << "4";
    res = stmt->executeQuery(execute);

    while (res->next())
    {
        qDebug() << "5";

      int temp = res->getInt("HighScore");
      qDebug() << temp;

      sum += temp;
      count++;
    }
    qDebug() << "COUNT:" << count;

    averageScore = (int) (sum/count);
    std::string averageScoreS = fromQString(QString::number(averageScore));

    qDebug() << "7";
    qDebug() << averageScore;


    execute = "UPDATE `cs5530db108`.`MathCrunchUsers` SET AverageScore = " + averageScoreS + " WHERE Username = '" + nameS + "';";\

    res = stmt->executeQuery(execute);

        qDebug() << "8";

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

//Method will check to see if the login name is an admin. Returns true if they are
//an admin (teacher) and false otherwise
bool Network::checkAdmin(QString name)
{
    bool success = false;
    std::string name2 = fromQString(name);
    std::string query = "SELECT * from `cs5530db108`.`MathCrunchUsers` WHERE Username = '" + name2 + "' AND AdminStatus = 'true';";
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
\
        if (res->next())
        {
            success = true;
        }
        delete res;
        delete con;
        delete stmt;
        return success;
    }
    catch(sql::SQLException &e)
    {
        qDebug() << "ERROR " << e.what();
        return false;
    }
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
