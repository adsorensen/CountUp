/**
* Backend of the screen in the UI that allows users/admins to log in
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

#include "login.h"
#include "ui_login.h"
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDesktopWidget>
#include <QRect>
#include <QApplication>
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
    int flag, flag2;
    if(ui->lineEdit->text() != "" && ui->lineEdit_2->text() != "")
    {
        QString name = ui->lineEdit->text(), password = ui->lineEdit_2->text();

        emit sendUsername(name);

        if(newUser)
        {
            bool isAdmin = ui->teacher->isChecked();
            QString classText = ui->classBox->text();
            flag = myNetwork.registerUser(name, password, isAdmin, classText);
            //successfully created user
            if(flag == 1)
            {
                this->hide();
                //checks for admin privledges
                if(myNetwork.checkAdmin(name))
                {
                    levelselector.showAdminButton();
                }
                levelselector.currentUser = name;
                levelselector.show();
            }
            //Error to show that the login already exists
            else if (flag == 2)
            {
                ui->warning->show();
                ui->warning->setText("Login already exists");
            }
            //Error with the database
            else
            {
                ui->warning->setText("Something went wrong");
                ui->warning->show();
            }
        }
        //validate old user
        else
        {
            //emit validateLogin(name, password);
            flag2 = myNetwork.checkUserLogin(name, password);
            //success
            if(flag2 == 1)
            {
                this->hide();
                //check for admin privledges
                if (myNetwork.checkAdmin(name))
                {
                    levelselector.showAdminButton();
                }
                //QVector<QString> playerInfo = myNetwork.getPlayerInfo(name);
                //QVector<QString> playerInfo = myNetwork.getPlayerLevel(name);
                //qDebug() << "level info" << playerInfo.at(0);
                getUserLevel(name);
                //getUserInfo(name);
                levelselector.currentUser = name;
                levelselector.show();
            }
            //error code to say wrong password
            else if (flag2 == 2)
            {
                ui->warning->show();
                ui->warning->setText("Wrong password!");
            }
            //error for login doesn't exist
            else if (flag2 == 3)
            {
                ui->warning->show();
                ui->warning->setText("Login doesn't exist");
            }
            //error connecting to the database
            else
            {
                ui->warning->show();
                ui->warning->setText("Something went wrong");
            }
        }
    }
    else
    {
        ui->warning->setText("Must fill in all fields");
        ui->warning->show();
    }
}

//gets user information
QVector<QString> Login::getUserInfo(QString name)
{
    QVector<QString> playerInfo = myNetwork.getPlayerInfo(name);
    qDebug() << playerInfo.at(0);
    return playerInfo;
}

 QVector<QString> Login::getUserLevel(QString name)
{
    QVector<QString> playerLevel = myNetwork.getPlayerLevel(name);
    int level = playerLevel.at(0).toInt();
    int diff = playerLevel.at(1).toInt();
    levelselector.hideButtons(level, diff);
    return playerLevel;
}

//create new user or switch back to login
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

    QString name = ui->lineEdit->text();
    emit sendUsername(name);
}

