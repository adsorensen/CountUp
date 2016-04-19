/**
* Main view controller for the CountUp game
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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <Box2D/Box2D.h>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDesktopWidget>
#include <QRect>
#include <QApplication>
#include <QDebug>
#include <QMouseEvent>
#include <QtGui>
#include <QAbstractItemView>

// Setup
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Centers application on start up
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    ui->setupUi(this);
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);
    this->show();

    // Sets the background image
    this->setStyleSheet("MainWindow {border-image: url(:/background/Resources/bg.png); };");

    // Initializes the game "grid"
    ui->tableWidget->horizontalHeader()->sectionResizeMode(QHeaderView::Fixed);
    ui->tableWidget->verticalHeader()->sectionResizeMode(QHeaderView::Fixed);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setMouseTracking(true);
    ui->tableWidget->viewport()->setMouseTracking(true);
    ui->tableWidget->installEventFilter(this);
    ui->tableWidget->viewport()->installEventFilter(this);

    // Connect slots and signals
    QObject::connect(ui->shuffleButton, SIGNAL(clicked(bool)), this, SLOT(on_entry()));
}

// Destructor
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
 {
     QStyleOption opt;
     opt.init(this);
     QPainter p(this);
     style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
 }

//Sets value of the cell according to model
void MainWindow::fillGrid(MathNode model[][10])
{

    for(int x = 0; x < 10; x++)
    {
        for(int y = 0; y < 10; y++)
        {
            MathNode current = model[y][x];

            ui->tableWidget->setItem(y, x, new QTableWidgetItem);
            ui->tableWidget->item(y, x)->setText(current.value);
            ui->tableWidget->item(y, x)->setTextAlignment(Qt::AlignCenter);
        }
    }
}

//Add math node to current formula
void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    //retrieve value from math node
    //append to formula string
    qDebug() << "Start expression";

}

//Same thing here but for drag
//Do not register diagonal nodes?
void MainWindow::on_tableWidget_cellEntered(int row, int column)
{

}

//End string on mouse release
//Send current formula to game model to evaluate
//If valid expression, replace math nodes with new nodes from game model
//else return nodes to grid
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->tableWidget->viewport())
    {
        if (event->type() == QEvent::MouseButtonRelease)
            qDebug() << "End expression";
    }
    return QMainWindow::eventFilter(obj, event);
}

//
void MainWindow::on_entry()
{

}



