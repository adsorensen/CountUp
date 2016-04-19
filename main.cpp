#include "mainwindow.h"
#include <QApplication>
#include <login.h>
#include <QObject>
#include <QDebug>
#include <Box2D/Box2D.h>
#include <Box2D/Dynamics/b2World.h>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login login;

    login.show();
    b2Vec2 world;
    b2AABB world1;


    b2Vec2 gravity(0, -9.8); //normal earth gravity, 9.8 m/s/s straight down!


    bool doSleep = true;

    //b2World* myWorld = new b2World(gravity);

    return a.exec();
}
