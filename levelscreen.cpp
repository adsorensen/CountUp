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
    QRect screenGeometry = QApplication::desktop()->screenGeometry();

    this->setStyleSheet("Levelscreen {border-image: url(:/background/Resources/loginbg.png); };");


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
    this->hide();

    w.show();
    w.start();
    emit game_started();
}
