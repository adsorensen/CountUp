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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
