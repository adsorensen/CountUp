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
#include <gamemodel.h>
#include <mathnode.h>
#include <QMouseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    GameModel game;


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void fillGrid(MathNode model[][10]);

private slots:
    void on_tableWidget_cellClicked(int row, int column);

    void on_tableWidget_cellEntered(int row, int column);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
