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

/*
 * Constructor
 */
Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    // General setup of UI
    ui->setupUi(this);
    ui->newUser->hide();
    ui->warning->hide();
    ui->classBox->setGeometry(82, 190, 231, 25);
    ui->teacher->setGeometry(150, 230, 231, 25);
    ui->teacher->hide();
    ui->classBox->hide();
    levelselector.hide();

    // Set background image of login screen
    this->setStyleSheet("Login {border-image: url(:/background/Resources/loginbg.png); };");

    // Center login screen
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);
    this->show();

    // Set newUser to false
    newUser = false;
}

/*
 * Destructor
 */
Login::~Login()
{
    delete ui;
}

/*
 * Validates user login when loginButton is pressed
 */
void Login::on_loginbutton_pressed()
{
    // Declare variables
    int flag, flag2;

    // Enter if condition if the username and password edit textboxes are both not empty
    if(ui->usernameEdit->text() != "" && ui->passwordEdit->text() != "")
    {
        // Sets text in edit boxes to QString variables
        QString name = ui->usernameEdit->text();
        QString password = ui->passwordEdit->text();

        // Pass name to sendUsername signal and emit
        emit sendUsername(name);

        // Handle if the user is new
        if(newUser)
        {
            // Set isAdmin based on whether the teacher box is checked
            bool isAdmin = ui->teacher->isChecked();

            // Set text in classBox to QString variable
            QString classText = ui->classBox->text();

            // Attempt to register the user in the network and set the flag based off registerUser result
            flag = myNetwork.registerUser(name, password, isAdmin, classText);

            // Call checkUserHelper for a new user
            checkUserHelper(flag, true);
        }

        // Validate existing user
        else
        {
            // Set flag2 to results of checkUserLogin from network
            flag2 = myNetwork.checkUserLogin(name, password);

            // Call checkUserHelper for a non new user
            checkUserHelper(flag2, false);
        }
    }

    // Throw warning if user has left fields blank
    else
    {
        ui->warning->setText("Must fill in all fields");
        ui->warning->show();
    }
}

/*
 * Helper method that performs different actions based on the flag returned from the network and whether the user is new or not
 */
void Login::checkUserHelper(int fromNetwork, bool newUser)
{
    // Sets text from usernameEdit to a QString variable
    QString name = ui->usernameEdit->text();

    // 1 means success in was creating login or checking login
    if(fromNetwork == 1)
    {
        // Hides login screen
        this->hide();

        // If user is an admin, show the admin button
        if(myNetwork.checkAdmin(name))
        {
            levelselector.showAdminButton();
        }

        // Set properties based on user name
        getUserLevel(name);
        levelselector.currentUser = name;
        levelselector.show();
    }
    else if (fromNetwork == 2)
    {
        // 2 for a newUser means login already exists
        if (newUser)
        {
            ui->warning->setText("Login already exists");
        }

        // 2 for existing user means the password does not match that of the user's on file
        else
        {
            ui->warning->setText("Wrong password!");
        }
        ui->warning->show();
    }
    else
    {
        // 3 for newUser means there was an error connecting to the database
        if (newUser)
        {
            ui->warning->setText("Check database connection!");
        }
        // 3 for an existing user means the login does not exist in the database
        else
        {
            ui->warning->setText("Login doesn't exist");
        }
        ui->warning->show();
    }
}

/*
 * Retrieves information about the user with the passed in name
 */
QVector<QString> Login::getUserInfo(QString name)
{
    QVector<QString> playerInfo = myNetwork.getPlayerInfo(name);
    qDebug() << playerInfo.at(0);
    return playerInfo;
}

/*
 * Takes in a user's name and returns the denoted user's level
 */
QVector<QString> Login::getUserLevel(QString name)
{
    QVector<QString> playerLevel = myNetwork.getPlayerInfo(name);
    int level = (playerLevel.at(0).toInt() % 5);
    int diff = ((playerLevel.at(0).toInt() - 1) / 5) + 1;
    levelselector.hideButtons(level, diff);
    return playerLevel;
}

/*
  * Handles when the newAccountButton is pressed
  */
void Login::on_newaccountbutton_pressed()
{
    // Enters if condition if newUser is true
    if (newUser)
    {
        // Sets newUser to false
        newUser = false;

        // Deals with UI
        ui->newUser->hide();
        ui->loginbutton->setText("Log in");
        ui->newaccountbutton->setText("Create a new account");
        ui->warning->hide();
        ui->teacher->hide();
        ui->classBox->hide();
        ui->newaccountbutton->setGeometry(120,240, 151, 26);
        ui->loginbutton->setGeometry(150,200,90,26);
        ui->warning->setGeometry(130, 180, 131, 21);
    }

    // Enters else condition if newUser is false
    else
    {
        // Sets newUser to true
        newUser = true;

        // Deals with UI
        ui->newUser->show();
        ui->usernameEdit->setPlaceholderText("New user name");
        ui->passwordEdit->setPlaceholderText("New user password");
        ui->loginbutton->setText("Create user");
        ui->newaccountbutton->setText("Cancel");
        ui->warning->hide();
        ui->classBox->show();
        ui->teacher->show();
        ui->newaccountbutton->setGeometry(120,330, 151, 26);
        ui->loginbutton->setGeometry(150,290,90,26);
        ui->warning->setGeometry(130, 260, 131, 21);
    }

    // Sets text from usernameEdit to a QString variable
    QString name = ui->usernameEdit->text();

    // Passes name to sendUsername signal and emits
    emit sendUsername(name);
}
