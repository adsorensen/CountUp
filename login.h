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
     QString currentUser;

private slots:

    void on_loginbutton_pressed();

    void on_newaccountbutton_pressed();

signals:
    void validLogin();
    void invalidLogin();
    void sendUsername(QString);

private:
    Ui::Login *ui;
    bool newUser;
    Network myNetwork;
};

#endif // LOGIN_H
