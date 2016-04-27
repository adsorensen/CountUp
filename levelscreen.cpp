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

    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);
}

Levelscreen::~Levelscreen()
{
    delete ui;
}

void Levelscreen::on_playButton_pressed()
{
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
