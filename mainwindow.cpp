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

// Setup
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

// Destructor
MainWindow::~MainWindow()
{
    delete ui;
}
