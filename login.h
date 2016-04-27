/**
* Backend of the screen in the UI that allows users/admins to log in
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

#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QAbstractButton>
#include <mainwindow.h>
#include <levelscreen.h>
#include <network.h>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();
     Levelscreen levelselector;

private slots:
    void on_loginbutton_pressed();
    void on_newaccountbutton_pressed();

signals:
    void validLogin();
    void invalidLogin();

private:
    Ui::Login *ui;
    bool newUser;
    Network myNetwork;
};

#endif // LOGIN_H
