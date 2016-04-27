/**
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
