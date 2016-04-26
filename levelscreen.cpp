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
    int level;
    bool a, b, c, d, e, difficulty = false;
    a = ui->easyButton->isChecked();
    b = ui->mediumButton->isChecked();
    c = ui->hardButton->isChecked();
    d = ui->veryhardButton->isChecked();
    e = ui->extremeButton->isChecked();

    int diff = 0;
    if (a || b || c || d || e)
    {
        if (a)
        {
            //level = 0;
            diff = 1;
            difficulty = true;
        }

        if (b)
        {
            diff = 2;
            //level = 5;
            difficulty = true;
        }

        if (c)
        {
            diff = 3;
            //level = 10;
            difficulty = true;
        }

        if (d)
        {
            diff = 4;
            //level = 15;
            difficulty = true;
        }

        if (e)
        {
            diff = 5;
            //level = 20;
            difficulty = true;
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
                //level += 1;
            }
            if (b)
            {
                level = 2;
                //level += 2;
            }
            if (c)
            {
                level = 3;
                //level += 3;
            }
            if (d)
            {
                level = 4;
                //level += 4;
            }
            if (e)
            {
                level = 5;
                //level += 5;
            }
            this->hide();
            qDebug() << "d " << diff;
            qDebug() << "l " << level;
            w.level = level;
            w.difficulty = diff;
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
