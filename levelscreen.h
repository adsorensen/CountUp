#ifndef LEVELSCREEN_H
#define LEVELSCREEN_H

#include <QMainWindow>
#include <mainwindow.h>

namespace Ui {
class Levelscreen;
}

class Levelscreen : public QMainWindow
{
    Q_OBJECT

public:
    explicit Levelscreen(QWidget *parent = 0);
    ~Levelscreen();
    MainWindow w;

private slots:

    void on_playButton_pressed();

private:
    Ui::Levelscreen *ui;
};

#endif // LEVELSCREEN_H
