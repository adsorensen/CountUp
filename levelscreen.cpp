/**
* Backend of the screen in the UI that allows users to select a level
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

#include "levelscreen.h"
#include "ui_levelscreen.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDesktopWidget>
#include <QRect>
#include <QApplication>
#include <QDebug>
#include <QMouseEvent>
#include <QtGui>
#include <QAbstractItemView>
#include <QObject>
#include <QList>
#include <QFile>
#include <QCoreApplication>
#include <QTextStream>
#include <QVector>

/*
 * Constructor
 */
Levelscreen::Levelscreen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Levelscreen)
{
    // Set-up
    ui->setupUi(this);
    w.hide();
    w.level = 1;
    w.difficulty = 1;
    ui->warning->hide();
    ui->adminButton->hide();

    // Sets background of the level selection screen
    this->setStyleSheet("Levelscreen {border-image: url(:/background/Resources/loginbg.png); };");

    // Centers level selection screen on startup
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);

    // Connects slots and signals
    QObject::connect(&w, SIGNAL(menu_pressed()), this, SLOT(on_menu_pressed()));
}

/*
 * Destructor
 */
Levelscreen::~Levelscreen()
{
    delete ui;
}

/*
 * The admin button is shown if the user is an administrator
 */
void Levelscreen::showAdminButton()
{
    ui->adminButton->show();
}

/*
 * Determines which levels and difficulty to hide from users based on their
 * current level and difficulty
 */
void Levelscreen::hideButtons(int lvl, int diff)
{
    // Determine which difficulty buttons to hide
    switch(diff)
    {

    // Hide all but easy button
    case 1:
        ui->mediumButton->hide();
        ui->hardButton->hide();
        ui->veryhardButton->hide();
        ui->extremeButton->hide();
        level = 0;
        break;

        // Hide all but easy and medium buttons
    case 2:
        ui->hardButton->hide();
        ui->veryhardButton->hide();
        ui->extremeButton->hide();
        level = 5;
        break;

        // Hide veryHard and extreme buttons
    case 3:
        ui->veryhardButton->hide();
        ui->extremeButton->hide();
        level = 10;
        break;

        // Hide extreme button
    case 4:
        ui->extremeButton->hide();
        level = 15;
        break;

        // No difficulties are hidden in this case
    case 5:
        level = 20;
        break;

    default:
        break;
    }

    // Determine which level buttons to hide
    switch(lvl)
    {
    // Hide all but level 1 button
    case 1:
        ui->Level2Button->hide();
        ui->Level3Button->hide();
        ui->Level4Button->hide();
        ui->Level5Button->hide();
        level += 1;
        break;

        // Hide all but levels 1 and 2 buttons
    case 2:
        ui->Level3Button->hide();
        ui->Level4Button->hide();
        ui->Level5Button->hide();
        level += 2;
        break;

        // Hide levels 3 and 4 buttons
    case 3:
        ui->Level4Button->hide();
        ui->Level5Button->hide();
        level += 3;
        break;

        // Hide level 5 button
    case 4:
        ui->Level5Button->hide();
        level += 4;
        break;

        // In this case, don't hide any buttons
    case 0:
        level += 5;
        break;

    default:
        break;
    }
}

/*
 * Sets main window's level and difficulty based on user selection
 */
void Levelscreen::on_playButton_pressed()
{
    // Declare and initialize variables;
    bool difficulty = false;
    bool a = ui->easyButton->isChecked();
    bool b = ui->mediumButton->isChecked();
    bool c = ui->hardButton->isChecked();
    bool d = ui->veryhardButton->isChecked();
    bool e = ui->extremeButton->isChecked();
    QString diffString;
    int diff = 0;

    // Check if user has made a difficulty selection
    if (a || b || c || d || e)
    {
        if (a)
        {
            diff = 1;
            diffString = "Easy";
        }

        if (b)
        {
            diff = 2;
            diffString = "Medium";
        }

        if (c)
        {
            diff = 3;
            diffString = "Hard";
        }

        if (d)
        {
            diff = 4;
            diffString = "Very Hard";
        }

        if (e)
        {
            diff = 5;
            diffString = "Extreme";
        }
        difficulty = true;
    }

    // Display warning if user hasn't selected difficulty
    else
    {
        ui->warning->setText("Must select difficulty");
        ui->warning->show();
    }

    // Otherwise deal with level selection
    if (difficulty)
    {
        int level;
        a = ui->Level1Button->isChecked();
        b = ui->Level2Button->isChecked();
        c = ui->Level3Button->isChecked();
        d = ui->Level4Button->isChecked();
        e = ui->Level5Button->isChecked();
        if (a || b || c || d || e)
        {
            if (a)
            {
                level = 1;
            }
            if (b)
            {
                level = 2;
            }
            if (c)
            {
                level = 3;
            }
            if (d)
            {
                level = 4;
            }
            if (e)
            {
                level = 5;
            }
            // Hide the level selection screen
            this->hide();

            // Set main window's properties based on user selection
            w.level = level;
            w.difficulty = diff;
            w.game.difficultyString = diffString;

            // Show the main window and start the game
            w.show();
            w.start();

            // Pass the current level through the game_started signal
            emit game_started(level);
        }
        // Display warning if level hasn't been selected
        else
        {
            ui->warning->setText("Must select level");
            ui->warning->show();
        }
    }
}

/*
 * Sets visibility of level radio buttons after user clicks easyButton
 */
void Levelscreen::on_easyButton_pressed()
{
    unhideHelper(5);
}

/*
 * Sets visibility of level radio buttons after user clicks mediumButton
 */
void Levelscreen::on_mediumButton_pressed()
{
    unhideHelper(10);
}

/*
 * Sets visibility of level radio buttons after user clicks hardButton
 */
void Levelscreen::on_hardButton_pressed()
{
    unhideHelper(15);
}

/*
 * Sets visibility of level radio buttons after user clicks veryHardButton
 */
void Levelscreen::on_veryhardButton_pressed()
{
    unhideHelper(20);
}

/*
 * Sets visibility of level radio buttons after user clicks extremeButton
 */
void Levelscreen::on_extremeButton_pressed()
{
    unhideHelper(25);
}

/*
 * Sets visibility of level radio buttons based on user's difficulty selection
 */
void Levelscreen::unhideHelper(int currentLevel)
{
    int temp = level % 5;
    if (level >= currentLevel)
    {
        ui->Level2Button->show();
        ui->Level3Button->show();
        ui->Level4Button->show();
        ui->Level5Button->show();
    }
    else
    {
        switch(temp)
        {
        case 1: ui->Level2Button->hide();
            ui->Level3Button->hide();
            ui->Level4Button->hide();
            ui->Level5Button->hide();
            break;
        case 2: ui->Level2Button->show();
            ui->Level3Button->hide();
            ui->Level4Button->hide();
            ui->Level5Button->hide();
            break;
        case 3: ui->Level2Button->show();
            ui->Level3Button->show();
            ui->Level4Button->hide();
            ui->Level5Button->hide();
            break;
        case 4: ui->Level2Button->show();
            ui->Level3Button->show();
            ui->Level4Button->show();
            ui->Level5Button->hide();
            break;
        default: break;
        }
    }
}

/*
 * Deals with when the user presses the menu button in the game over screen
 */
void Levelscreen::on_menu_pressed()
{
    this->show();
}

/*
 * Deals with when the user clicks the adminButton button
 */
void Levelscreen::on_adminButton_clicked()
{
    try
    {
        // Define sql variables
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;

        // Initialize sql variables
        driver = get_driver_instance();
        con = driver->connect("georgia.eng.utah.edu","cs5530u108","6pa21pkl");
        stmt = con->createStatement();
        \
        // Define SQL queries
        std::string currUser = currentUser.toStdString();
        std::string execute1 = "SELECT Username FROM cs5530db108.MathCrunchUsers WHERE UserClass IN (SELECT UserClass FROM cs5530db108.MathCrunchUsers WHERE Username =\'"+ currUser +"\')";
        std::string execute2 = "SELECT CurrentLevel FROM cs5530db108.MathCrunchUsers WHERE UserClass IN (SELECT UserClass FROM cs5530db108.MathCrunchUsers WHERE Username =\'"+ currUser +"\')";
        std::string execute3 = "SELECT AverageScore FROM cs5530db108.MathCrunchUsers WHERE UserClass IN (SELECT UserClass FROM cs5530db108.MathCrunchUsers WHERE Username =\'"+ currUser +"\')";

        // Define variables
        QVector<QString> usernameList;
        QVector<QString> levelList;
        QVector<QString> avgscoreList;
        int m = 1;
        res = stmt->executeQuery(execute1);

        while(res->next())
        {
            qDebug() << "\t... MySQL replies1: " << m;
            std::string temp1 = res->getString(m);
            QString ex = toQString(temp1);
            qDebug() << ex;
            usernameList.append(ex);
            //m++;
        }
        res = stmt->executeQuery(execute2);

        while(res->next())
        {
            qDebug() << "\t... MySQL replies2: ";
            std::string temp2 = res->getString(m);
            QString ex = toQString(temp2);
            qDebug() << ex;
            levelList.append(ex);
        }
        res = stmt->executeQuery(execute3);

        while(res->next())
        {
            qDebug() << "\t... MySQL replies3: ";
            std::string temp3 = res->getString(m);
            QString ex = toQString(temp3);
            qDebug() << ex;
            avgscoreList.append(ex);
        }

        // Save HTML lines into QStrings
        QString s1 = "<!DOCTYPE html><html lang='en'><head><title>Bootstrap Theme The Band</title><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1'><link rel='stylesheet' href='http://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css'><script src='https://ajax.googleapis.com/ajax/libs/jquery/1.12.0/jquery.min.js'></script><script src='http://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/js/bootstrap.min.js'></script>";
        QString s2 = "<style>.panel-heading {color: #fff !important;background-color: #f4511e !important;padding: 25px;border-bottom: 1px solid transparent;border-top-left-radius: 0px;border-top-right-radius: 0px;border-bottom-left-radius: 0px;border-bottom-right-radius: 0px;}.panel {border: 1px solid #f4511e; border-radius:0 !important;transition: box-shadow 0.5s;}.panel-footer {background-color: white !important;}.panel-footer h3 {font-size: 32px;}.panel-footer h4 {color: #aaa;font-size: 14px;}.panel-footer .btn {margin: 15px 0;background-color: #f4511e;color: #fff;}@media screen and (max-width: 768px) {.col-sm-4 {text-align: center;margin: 25px 0;}}</style></head>";
        QString s3 = "<div class='bg-1'><div class='container'><h3 class='text-center'> Hello!</h3><p class='text-center'>Here is your group menbers and their high scores.<br> Enjoy!</p><br></div></div><div class='container'><br><br><div class='panel-group' id='accordion'>";
        QString s4 = "<div class='panel panel-default'><div class='panel-heading'><h4 class='panel-title'><a data-toggle='collapse' data-parent='#accordion' href='#collapse";
        QString s5 = "'>";
        QString s6 = "</a></h4></div><div id='collapse";
        QString s7 = "' class='panel-collapse collapse in'><div class='panel-body'>";
        QString s8 = "</div></div></div>";

        // Save HTML to file
        QFile file("stats.html");
        file.open(QIODevice::WriteOnly| QIODevice::Text);
        QTextStream out(&file);

        if(!usernameList.empty()){
            out << s1 << s2 << s3;
            for(int i = 0; i < usernameList.size(); i++){
                out << s4 << i << s5 << usernameList.at(i) << s6 << i << s7 << "Level: " << (levelList.at(i).toInt() - 1)%5 + 1  << "    Difficulty: " << (levelList.at(i).toInt() - 1)/5 + 1 << "    Average Score: " << avgscoreList.at(i) << s8;
            }
        }

        file.close();

        delete stmt;
        delete con;
    }
    // Catch and print any errors
    catch(sql::SQLException &e)
    {
        qDebug() << "error" << e.what();
    }
}

/*
 * Converts a string into a QString
 */
QString Levelscreen::toQString(std::string const &s)
{
    return QString::fromUtf8(s.c_str());
}
