#include "mainwindow.h"
#include <QApplication>
#include <login.h>
#include <QObject>
#include <QDebug>
#include <Box2D/Box2D.h>
#include <Box2D/Dynamics/b2World.h>
#include <QString>
#include <stdio.h>
#include <string>
#include <network.h>






int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login login;

    login.show();

    Network nw;


    return a.exec();
}




//try {
//  sql::Driver *driver;
//  sql::Connection *con;
//  sql::Statement *stmt;
//  sql::ResultSet *res;

//  /* Create a connection */
//  driver = get_driver_instance();
//  con = driver->connect("tcp://127.0.0.1:3306", "root", "root");
//  /* Connect to the MySQL test database */
//  con->setSchema("test");

//  stmt = con->createStatement();
//  res = stmt->executeQuery("SELECT 'Hello World!' AS _message");
//  while (res->next()) {
//    cout << "\t... MySQL replies: ";
//    /* Access column data by alias or column name */
//    cout << res->getString("_message") << endl;
//    cout << "\t... MySQL says it again: ";
//    /* Access column fata by numeric offset, 1 is the first column */
//    cout << res->getString(1) << endl;
//  }
//  delete res;
//  delete stmt;
//  delete con;

//} catch (sql::SQLException &e) {
//      cout << "# ERR: SQLException in " << __FILE__;
//      cout << "(" << __FUNCTION__ << ") on line " »
//         << __LINE__ << endl;
//      cout << "# ERR: " << e.what();
//      cout << " (MySQL error code: " << e.getErrorCode();
//      cout << ", SQLState: " << e.getSQLState() << " )" << endl;
//}
