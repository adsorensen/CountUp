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
    this->setStyleSheet("Login {border-image: url(:/background/Resources/loginbg.png); };");
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);
    this->show();
    ui->newUser->hide();
    ui->warning->hide();
    ui->classBox->setGeometry(82, 190, 231, 25);
    ui->teacher->setGeometry(150, 230, 231, 25);
    ui->teacher->hide();
    ui->classBox->hide();
    newUser = false;

}

Login::~Login()
{
    delete ui;
}


//validate user login here
void Login::on_loginbutton_pressed()
{
    if(ui->lineEdit->text() != "" && ui->lineEdit_2->text() != "")
    {

        QString name = ui->lineEdit->text(), password = ui->lineEdit_2->text();
        if(newUser)
        {
            //check to see if the login name exists
            //add new user to the database
        }
        else
        {
            //validate user login
        }

        this->hide();
        levelselector.show();
    }
    else
    {
        ui->warning->show();
    }
}

//create new user
void Login::on_newaccountbutton_pressed()
{
    if (newUser)
    {
        newUser = false;
        ui->newUser->hide();
        ui->loginbutton->setText("Log in");
        ui->newaccountbutton->setText("Create a new account");
        ui->lineEdit->setPlaceholderText("User name");
        ui->lineEdit_2->setPlaceholderText("Password");
        ui->warning->hide();
        ui->teacher->hide();
        ui->classBox->hide();
        ui->newaccountbutton->setGeometry(120,240, 151, 26);
        ui->loginbutton->setGeometry(150,200,90,26);
        ui->warning->setGeometry(130, 180, 131, 21);
    }
    else
    {
        newUser = true;
        ui->newUser->show();
        ui->lineEdit->setPlaceholderText("New user name");
        ui->lineEdit_2->setPlaceholderText("New user password");
        ui->loginbutton->setText("Create user");
        ui->newaccountbutton->setText("Cancel");
        ui->warning->hide();
        ui->classBox->show();
        ui->teacher->show();
        ui->newaccountbutton->setGeometry(120,330, 151, 26);
        ui->loginbutton->setGeometry(150,290,90,26);
        ui->warning->setGeometry(130, 260, 131, 21);
    }
}

