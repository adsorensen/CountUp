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

Levelscreen::Levelscreen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Levelscreen)
{
    ui->setupUi(this);
    w.hide();

    w.level = 1;
    w.difficulty = 1;

    QRect screenGeometry = QApplication::desktop()->screenGeometry();

    this->setStyleSheet("Levelscreen {border-image: url(:/background/Resources/loginbg.png); };");
    ui->warning->hide();
    ui->adminButton->hide();

    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);
    //qDebug() << "created " << currentUser;
    //QObject::connect(&game, SIGNAL(InvalidFormulaSig()), this, SLOT(dealWithInvalidFormula()));

    QObject::connect(&w, SIGNAL(menu_pressed()), this, SLOT(on_menu_pressed()));
}

Levelscreen::~Levelscreen()
{
    delete ui;
}

//Once login verifies that the user is an admin, this will show the admin button
void Levelscreen::showAdminButton()
{
    ui->adminButton->show();
}

//takes in the level and difficulty from the user and will hide the options
//not available to them.
void Levelscreen::hideButtons(int lvl, int diff)
{

    switch(diff)
    {
    case 1: ui->mediumButton->hide();
        ui->hardButton->hide();
        ui->veryhardButton->hide();
        ui->extremeButton->hide();
        level = 0;
        break;
    case 2: ui->hardButton->hide();
        ui->veryhardButton->hide();
        ui->extremeButton->hide();
        level = 5;
        break;
    case 3: ui->veryhardButton->hide();
        ui->extremeButton->hide();
        level = 10;
        break;
    case 4: ui->extremeButton->hide();
        level = 15;
        break;
    case 5: level = 20;
        break;
    default: break;
    }

    switch(lvl)
    {
    case 1: ui->Level2Button->hide();
        ui->Level3Button->hide();
        ui->Level4Button->hide();
        ui->Level5Button->hide();
        level += 1;
        break;
    case 2: ui->Level3Button->hide();
        ui->Level4Button->hide();
        ui->Level5Button->hide();
        level += 2;
        break;
    case 3: ui->Level4Button->hide();
        ui->Level5Button->hide();
        level += 3;
        break;
    case 4: ui->Level5Button->hide();
        level += 4;
        break;
    case 0: level += 5;
        break;
    default: break;
    }
}

//Checks whether the user has checked a difficulty and a level
void Levelscreen::on_playButton_pressed()
{
    qDebug() << "created " << currentUser;
    bool a, b, c, d, e, difficulty = false;
    a = ui->easyButton->isChecked();
    b = ui->mediumButton->isChecked();
    c = ui->hardButton->isChecked();
    d = ui->veryhardButton->isChecked();
    e = ui->extremeButton->isChecked();
    QString temp;

    int diff = 0;
    if (a || b || c || d || e)
    {
        if (a)
        {
            diff = 1;
            difficulty = true;
            temp = "Easy";
        }

        if (b)
        {
            diff = 2;
            difficulty = true;
            temp = "Medium";
        }

        if (c)
        {
            diff = 3;
            difficulty = true;
            temp = "Hard";
        }

        if (d)
        {
            diff = 4;
            difficulty = true;
            temp = "Very Hard";
        }

        if (e)
        {
            diff = 5;
            difficulty = true;
            temp = "Extreme";
        }
    }
    else
    {
        ui->warning->setText("Must select difficulty");
        ui->warning->show();
    }
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
            this->hide();

            w.level = level;
            w.difficulty = diff;
            w.game.difficultyString = temp;
            w.show();
            w.start();
            emit game_started(level);
        }
        else
        {
            ui->warning->setText("Must select level");
            ui->warning->show();
        }
    }
}

void Levelscreen::on_easyButton_clicked()
{

}

void Levelscreen::on_mediumButton_clicked()
{

}

void Levelscreen::on_hardButton_clicked()
{

}

void Levelscreen::on_veryhardButton_clicked()
{

}

void Levelscreen::on_extremeButton_clicked()
{

}


//Will unhide and hide level buttons when the user
//selects the easy button
void Levelscreen::on_easyButton_pressed()
{
    int temp = level % 5;
    if (level >= 5)
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
//Will unhide and hide level buttons when the user
//selects the medium button
void Levelscreen::on_mediumButton_pressed()
{
    int temp = level % 5;
    if (level >= 10)
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

//Will unhide and hide level buttons when the user
//selects the hard button
void Levelscreen::on_hardButton_pressed()
{
    int temp = level % 5;
    if (level >= 15)
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

//Will unhide and hide level buttons when the user
//selects the very hard button
void Levelscreen::on_veryhardButton_pressed()
{
    int temp = level % 5;
    if (level >= 20)
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

//Will unhide and hide level buttons when the user
//selects the extreme button
void Levelscreen::on_extremeButton_pressed()
{
    int temp = level % 5;
    if (level >= 25)
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


void Levelscreen::on_adminButton_clicked()
{
    QString temp = "";
    try{
    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;

    driver = get_driver_instance();

    con = driver->connect("georgia.eng.utah.edu","cs5530u108","6pa21pkl");

    stmt = con->createStatement();
\
    std::string currUser = currentUser.toStdString();
    std::string execute1 = "SELECT Username FROM cs5530db108.MathCrunchUsers WHERE UserClass IN (SELECT UserClass FROM cs5530db108.MathCrunchUsers WHERE Username =\'"+ currUser +"\')";
    std::string execute2 = "SELECT CurrentLevel FROM cs5530db108.MathCrunchUsers WHERE UserClass IN (SELECT UserClass FROM cs5530db108.MathCrunchUsers WHERE Username =\'"+ currUser +"\')";
    std::string execute3 = "SELECT AverageScore FROM cs5530db108.MathCrunchUsers WHERE UserClass IN (SELECT UserClass FROM cs5530db108.MathCrunchUsers WHERE Username =\'"+ currUser +"\')";

    QVector<QString> usernameList;
    QVector<QString> levelList;
    QVector<QString> avgscoreList;

    int m = 1;
    res = stmt->executeQuery(execute1);
    while(res->next()){
        qDebug() << "\t... MySQL replies1: " << m;
        std::string temp1 = res->getString(m);
        QString ex = toQString(temp1);
        qDebug() << ex;
        usernameList.append(ex);
        //m++;
    }

    res = stmt->executeQuery(execute2);
    while(res->next()){
        qDebug() << "\t... MySQL replies2: ";
        std::string temp2 = res->getString(m);
        QString ex = toQString(temp2);
        qDebug() << ex;
        levelList.append(ex);
    }

    res = stmt->executeQuery(execute3);
    while(res->next()){
        qDebug() << "\t... MySQL replies3: ";
        std::string temp3 = res->getString(m);
        QString ex = toQString(temp3);
        qDebug() << ex;
        avgscoreList.append(ex);
    }



    QString s1 = "<!DOCTYPE html><html lang='en'><head><title>Bootstrap Theme The Band</title><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1'><link rel='stylesheet' href='http://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css'><script src='https://ajax.googleapis.com/ajax/libs/jquery/1.12.0/jquery.min.js'></script><script src='http://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/js/bootstrap.min.js'></script>";
    QString s2 = "<style>.panel-heading {color: #fff !important;background-color: #f4511e !important;padding: 25px;border-bottom: 1px solid transparent;border-top-left-radius: 0px;border-top-right-radius: 0px;border-bottom-left-radius: 0px;border-bottom-right-radius: 0px;}.panel {border: 1px solid #f4511e; border-radius:0 !important;transition: box-shadow 0.5s;}.panel-footer {background-color: white !important;}.panel-footer h3 {font-size: 32px;}.panel-footer h4 {color: #aaa;font-size: 14px;}.panel-footer .btn {margin: 15px 0;background-color: #f4511e;color: #fff;}@media screen and (max-width: 768px) {.col-sm-4 {text-align: center;margin: 25px 0;}}</style></head>";
    QString s3 = "<div class='bg-1'><div class='container'><h3 class='text-center'> Hello!</h3><p class='text-center'>Here is your group menbers and their high scores.<br> Enjoy!</p><br></div></div><div class='container'><br><br><div class='panel-group' id='accordion'>";
    QString s4 = "<div class='panel panel-default'><div class='panel-heading'><h4 class='panel-title'><a data-toggle='collapse' data-parent='#accordion' href='#collapse";
    QString s5 = "'>";
    QString s6 = "</a></h4></div><div id='collapse";
    QString s7 = "' class='panel-collapse collapse in'><div class='panel-body'>";
    QString s8 = "</div></div></div>";



    QFile file("/home/zhiw/Desktop/out.html");
    file.open(QIODevice::WriteOnly| QIODevice::Text);
    QTextStream out(&file);

    if(!usernameList.empty()){
        out << s1 << s2 << s3;
        for(int i = 0; i < usernameList.size(); i++){
            out << s4 << i << s5 << usernameList.at(i) << s6 << i << s7 << "Level: " << (levelList.at(i).toInt() - 1)%5 + 1  << "    Difficulty: " << (levelList.at(i).toInt() - 1)/5 + 1 << "    Average Score: " << avgscoreList.at(i) << s8;
        }
    }

    file.close();

    //delete res;
    delete stmt;
    delete con;
    }
    catch(sql::SQLException &e)
    {
        qDebug() << "error" << e.what();

    }


}

//helper methods
QString Levelscreen::toQString(std::string const &s)
{
    return QString::fromUtf8(s.c_str());
}
