
#include "network.h"
#include <usr/include/cppconn/driver.h>
#include <usr/include/cppconn/exception.h>
#include <usr/include/cppconn/resultset.h>
#include <usr/include/cppconn/statement.h>
#include <usr/include/mysql_connection.h>
#include <QDebug>

Network::Network()
{
//    try{
//    sql::Driver *driver;
//    sql::Connection *con;
//    sql::Statement *stmt;
//    sql::ResultSet *res;

//    driver = get_driver_instance();


//    qDebug() << "trying";
//    con = driver->connect("georgia.eng.utah.edu","cs5530u108","6pa21pkl");
//    qDebug() << "trying2";
//    qDebug() << "trying3";
//    stmt = con->createStatement();
//    //stmt->execute("DROP TABLE IF EXISTS test");
//    std::string execute = "SELECT * FROM `cs5530db108`.`RegisteredUser` WHERE `LoginName` = 'tswizzle';";

//      res = stmt->executeQuery(execute);
//      while (res->next())
//      {
//        qDebug() << "\t... MySQL replies: ";
//        std::string temp = res->getString(2);
//        QString ex = toQString(temp);
//        qDebug() << ex;


//      }
//      delete res;
//      delete stmt;
//      delete con;
//    }
//    catch(sql::SQLException &e)
//    {
//        qDebug() << "error";
//    }
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


bool Network::registerUser(QString username, QString password, QString admin, QString userclass)
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
    std::string pwS = fromQString(password);
    std::string adminS = fromQString(admin);
    std::string currentlevelS = "1";
    std::string avgscoreS = "0";
    std::string userclassS = fromQString(userclass);


    stmt = con->createStatement();
    std::string execute = "INSERT INTO `cs5530db108`.`MathCrunchUsers` (`Username`, `Password`, `AdminStatus`, `CurrentLevel`, `AverageScore`, `UserClass`) VALUES ('" + nameS + "','" + pwS + "','" + adminS + "','" + currentlevelS + "','" + avgscoreS +"','" + userclassS +"');";

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

bool Network::removeUser(QString username)
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

    stmt = con->createStatement();
    std::string execute = "DELETE FROM `cs5530db108`.`MathCrunchUsers` WHERE 'Username' = '" + nameS + "';";

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

    averageScore = (total / count);

    execute = "UPDATE `cs5530db108`.`MathCrunchUser` SET AverageScore = " + averageScore + " WHERE 'Username' = '" + nameS + "';";
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

bool Network::checkUserLogin(QString username, QString password)
{
    bool success = false;
    QVector<QString> playerInfo = getPlayerInfo(username);

    if(!playerInfo.isEmpty())
    {
        //0 is username, 1 is password
        QString pw = playerInfo.at(1);
        if(pw == password)
        {
            success = true;
        }
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

