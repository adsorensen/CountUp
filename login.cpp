#include "login.h"
#include "ui_login.h"
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDesktopWidget>
#include <QRect>
#include <QApplication>
#include <QDebug>
#include <QMouseEvent>
#include <QtGui>
#include <QAbstractItemView>

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    levelselector.hide();
    QRect screenGeometry = QApplication::desktop()->screenGeometry();

    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);
    this->show();
}

Login::~Login()
{
    delete ui;
}


//validate user login here
void Login::on_loginbutton_pressed()
{
    this->hide();
    levelselector.show();
}

//create new user
void Login::on_newaccountbutton_pressed()
{

}
