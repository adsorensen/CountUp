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
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <QMouseEvent>
#include <QtGui>
#include <QAbstractItemView>

// Setup
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tableWidget->horizontalHeader()->setDefaultSectionSize(40);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(40);
    ui->tableWidget->horizontalHeader()->sectionResizeMode(QHeaderView::Fixed);
    ui->tableWidget->verticalHeader()->sectionResizeMode(QHeaderView::Fixed);
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->horizontalHeader()->hide();
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

}

// Destructor
MainWindow::~MainWindow()
{
    delete ui;
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

}

//Same thing here but for drags
void MainWindow::on_tableWidget_cellEntered(int row, int column)
{
}

//TODO: End string on mouse release



