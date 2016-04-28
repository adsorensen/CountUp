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
    QString currentUser;
    int level;
    void showAdminButton();
    void hideButtons(int, int);
    QString toQString(std::string const &s);



signals:
    void game_started(int);

private slots:
    void on_playButton_pressed();

    void on_easyButton_clicked();

    void on_mediumButton_clicked();

    void on_hardButton_clicked();

    void on_veryhardButton_clicked();

    void on_extremeButton_clicked();

    void on_extremeButton_pressed();

    void on_easyButton_pressed();

    void on_mediumButton_pressed();

    void on_hardButton_pressed();

    void on_veryhardButton_pressed();

    void on_menu_pressed();

    void on_adminButton_clicked();

private:
    Ui::Levelscreen *ui;
};

#endif // LEVELSCREEN_H
