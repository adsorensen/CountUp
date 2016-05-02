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

#include "network.h"
#include <usr/include/cppconn/driver.h>
#include <usr/include/cppconn/exception.h>
#include <usr/include/cppconn/resultset.h>
#include <usr/include/cppconn/statement.h>
#include <usr/include/mysql_connection.h>
#include <QDebug>

/*
 * Constructor
 */
Network::Network()
{
}

/*
 * Returns a QVector of QStrings containing the currentLevel and averageScore of the denoted user in that order
 */
QVector<QString> Network::getPlayerInfo(QString username)
{
    // Declare variables
    std::string temp;
    QVector<QString> playerInfo;

    try
    {
        // Declare and initialize variables
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

        // Enter if condition if res->next() is true
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

        // Delete objects
        delete res;
        delete stmt;
        delete con;
    }

    // Catch and display any errors
    catch(sql::SQLException &e)
    {
        qDebug() << "error";
    }

    // Return the QVector
    return playerInfo;
}

/*
 * Returns a QVector of QStrings containing the username, level, difficulty, and highschore of the denoted user in that order
 */
QVector<QString> Network::getPlayerLevel(QString username)
{
    // Declare variables
    QVector<QString> playerInfo;

    try
    {
        // Declare and initialize variables
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

        // Enter if condition if res->next() is true
        if (res->next())
        {
            std::string temp = res->getString(2);
            QString ex = toQString(temp);
            playerInfo.append(ex);
            temp = res->getString(3);
            ex = toQString(temp);
            playerInfo.append(ex);
        }

        // Delete objects
        delete res;
        delete stmt;
        delete con;
    }

    // Catch and display errors
    catch(sql::SQLException &e)
    {
        qDebug() << e.what();
    }

    // Return QVector
    return playerInfo;
}

/*
 * Returns an int based on results of registering a new user in the database
 * @returns 1 - registration successful
 *          2 - user already registered
 *          3 - error connecting to database
 */
int Network::registerUser(QString username, QString password, bool admin, QString userclass)
{
    // Declare and initialize variables
    bool success = false;
    int flag = 0;

    try
    {
        // Declare and initialize variables
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        driver = get_driver_instance();
        con = driver->connect("georgia.eng.utah.edu","cs5530u108","6pa21pkl");
        std::string nameS = fromQString(username);
        std::string pwS = fromQString(password);
        std::string adminS = "";

        // Set adminS based on whether or not admin is true
        if (admin)
        {
            adminS = "true";
        }
        else
        {
            adminS = "false";
        }

        // Set string variables
        std::string currentlevelS = "1";
        std::string avgscoreS = "0";
        std::string userclassS = fromQString(userclass);
        std::string diff = "1";

        // Set stmt to connection's createStatement result
        stmt = con->createStatement();

        // Set strings to execute
        std::string execute = "INSERT INTO `cs5530db108`.`MathCrunchUsers` (`Username`, `Password`, `AdminStatus`, `CurrentLevel`, `AverageScore`, `UserClass`) VALUES ('" + nameS + "','" + pwS + "','" + adminS + "','" + currentlevelS + "','" + avgscoreS +"','" + userclassS +"');";
        std::string execute2 = "INSERT INTO `cs5530db108`.`MathCrunchLevel` (`Username`, `Level`, `Difficulty`) VALUES ('" + nameS + "','" + currentlevelS + "','" + diff + "');";

        // Set success
        success = stmt->execute(execute);
        stmt->execute(execute2);
        flag = 1;

        // Delete objects
        delete stmt;
        delete con;
    }

    // Catch any erros and disply
    catch(sql::SQLException &e)
    {
        qDebug() << "error" << e.what();
        flag = 2;
    }

    // Return flag
    return flag;
}

/*
 * Returns true if the denoted user was succesfully removed from the database, false if otherwise
 */
bool Network::removeUser(QString username, QString mainuser)
{
    // Declare and initialize the success variable
    bool success = false;

    try
    {
        // Declare and initialize variables
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;
        driver = get_driver_instance();

        // Create a connection
        con = driver->connect("georgia.eng.utah.edu","cs5530u108","6pa21pkl");
        stmt = con->createStatement();
        // Set variables
        std::string nameS = fromQString(username);
        std::string userclassS;
        std::string mainuserS = fromQString(mainuser);

        // Create a statement
        stmt = con->createStatement();
        std::string execute;
        std::string query;
        std::string result;
        query = "SELECT UserClass FROM `cs5530db108`.`MathCrunchUsers` WHERE Username = '"+mainuserS+"';";
        res = stmt->executeQuery(query);

        // Enter while condition
        while (res->next())
        {
            userclassS = res->getString("UserClass");
        }
        QString temp;
        temp = QString::fromStdString(userclassS);


        // Create a query


        execute = "DELETE FROM `cs5530db108`.`MathCrunchUsers` WHERE Username = '" + nameS + "' AND UserClass = '" + userclassS + "';";


        // Execute the query
        success = stmt->execute(execute);


        // Delete objects
        delete res;
        delete stmt;
        delete con;
    }
    // Catch any errors and print them
    catch(sql::SQLException &e)
    {
        qDebug() << e.what();
    }

    // Return success bool
    return success;
}

/*
 * Updates the high score of the user in the database
 */
bool Network::updateHighscore(QString username, QString level, QString difficulty, QString highscore)
{
    // Declare and initialize variables
    bool success = false;
    int sum = 0;
    int count = 0;
    int averageScore;


    try
    {
        // Initialize sql variables
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;
        driver = get_driver_instance();

        // Connect to the database
        con = driver->connect("georgia.eng.utah.edu","cs5530u108","6pa21pkl");

        // Declare string variables
        std::string nameS = fromQString(username);
        std::string levelS = fromQString(level);
        std::string difficultyS = fromQString(difficulty);
        std::string highscoreS = fromQString(highscore);
        std::string nextLevelS = fromQString(QString::number((((difficulty.toInt() - 1) * 5) + level.toInt()) + 1));

        // Make a statement
        stmt = con->createStatement();

        // Set a query
        std::string execute = "SELECT * FROM `cs5530db108`.`MathCrunchUsers` WHERE Username = '" + nameS + "';";

        // Execute the query
        res = stmt->executeQuery(execute);

        // Enter the while condition
        while (res->next())
        {
            int temp = res->getInt("CurrentLevel");
            if ((((difficulty.toInt() - 1) * 5) + level.toInt()) == temp)
            {
                // Create a statement
                stmt = con->createStatement();

                // Create a query
                std::string execute1 = "INSERT INTO `cs5530db108`.`MathCrunchLevel` (`Username`, `Level`, `Difficulty`, `HighScore`) VALUES ('" + nameS + "'," + levelS + "," + difficultyS + ", " + highscoreS + ");";

                // Execute the query and save the status of it
                success = stmt->execute(execute1);

                // Create a query
                execute1 = "UPDATE `cs5530db108`.`MathCrunchUsers` SET CurrentLevel = " + nextLevelS + " WHERE Username = '" + nameS + "';";

                // Execute the query and save the status
                success = stmt->execute(execute1);
            }
            else
            {
                // Create a statement
                stmt = con->createStatement();

                // Create a query
                std::string execute2 = "UPDATE `cs5530db108`.`MathCrunchLevel` SET HighScore = " + highscoreS + " WHERE Username = '" + nameS + "' AND Difficulty = " + difficultyS + " AND Level = " + levelS + " AND HighScore < " + highscoreS + ";";

                // Execute the query and save its status into success
                success = stmt->execute(execute2);
            }
        }

        // Create a statement
        stmt = con->createStatement();

        // Create a query
        execute = "SELECT * FROM `cs5530db108`.`MathCrunchLevel` WHERE Username = '" + nameS + "';";

        // Execute the query and save into res
        res = stmt->executeQuery(execute);

        // Enter while condition
        while (res->next())
        {
            int temp = res->getInt("HighScore");
            sum += temp;
            count++;
        }

        // Calculate the averageScore and save into a string
        averageScore = (int) (sum/count);
        std::string averageScoreS = fromQString(QString::number(averageScore));

        // Create a query
        execute = "UPDATE `cs5530db108`.`MathCrunchUsers` SET AverageScore = " + averageScoreS + " WHERE Username = '" + nameS + "';";\

        // Execute query and save status into res
        res = stmt->executeQuery(execute);

        // Delete objects
        delete res;
        delete stmt;
        delete con;
    }
    // Catch and print any errors
    catch(sql::SQLException &e)
    {
        qDebug() << e.what();
    }

    // Return success bool
    return success;
}

/*
 * Returns an int based on results of checking user login in the database
 * @returns 1 - login successful
 *          2 - incorrect password
 *          3 - username does not exist in database
 *          -1 - error with database
 */
int Network::checkUserLogin(QString username, QString password)
{
    // Declare and initialize variables
    int success = 0;
    std::string name = fromQString(username);
    std::string passwordS = fromQString(password);
    std::string query = "SELECT * from `cs5530db108`.`MathCrunchUsers` WHERE Username = '" + name + "' AND Password = '" + passwordS + "';";
    std::string query2 = "SELECT * FROM `cs5530db108`.`MathCrunchUsers` WHERE Username = '" + name + "';";

    try
    {
        // Declare sql variables
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;

        // Set driver
        driver = get_driver_instance();

        // Connect to the database
        con = driver->connect("georgia.eng.utah.edu","cs5530u108","6pa21pkl");

        // Create a statement
        stmt = con->createStatement();

        // Execute query
        res = stmt->executeQuery(query);

        // If the if condition is met, set success to 1
        if (res->next())
        {
            success = 1;
        }
        // Otherwise, enter else condition
        else
        {
            // Execute query2 and set into res
            res = stmt->executeQuery(query2);

            // If if condition is met, set succe4ss to 2
            if (res->next())
            {
                // Wrong password
                success = 2;
            }

            // Otherwise, set success to 3
            else
            {
                // User doesn't exist
                success = 3;
            }
        }

        // Delete objects
        delete res;
        delete con;
        delete stmt;
    }

    // Catch and display any errors
    catch(sql::SQLException &e)
    {
        qDebug() << "error" << e.what();
        success = -1;
    }

    // Return success bool
    return success;
}


/*
 * Returns true if the passed in name refers to an admin (teacher), false if otherwise
 */
bool Network::checkAdmin(QString name)
{
    // Declare and initialize variables
    bool success = false;
    std::string name2 = fromQString(name);
    std::string query = "SELECT * from `cs5530db108`.`MathCrunchUsers` WHERE Username = '" + name2 + "' AND AdminStatus = 'true';";

    try
    {
        // Declare sql variables
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;

        // Set the driver
        driver = get_driver_instance();

        // Connect to the database
        con = driver->connect("georgia.eng.utah.edu","cs5530u108","6pa21pkl");

        // Create a statement
        stmt = con->createStatement();

        // Execute query and set status to res
        res = stmt->executeQuery(query);
        \
        // If if condition is met, set success to true
        if (res->next())
        {
            success = true;
        }

        // Delete objects
        delete res;
        delete con;
        delete stmt;
        return success;
    }
    // Catch and print any errors and return false
    catch(sql::SQLException &e)
    {
        qDebug() << "ERROR " << e.what();
        return false;
    }
}

/*
 * Converts string to QString
 */
QString Network::toQString(std::string const &s)
{
    return QString::fromUtf8(s.c_str());
}

/*
 * Converts QString to string
 */
std::string Network::fromQString(QString const &s)
{
    return std::string(s.toUtf8().data());
}
