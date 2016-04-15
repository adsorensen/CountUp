#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QAbstractButton>

//VALIDATE USER LOG IN HERE, REQUIRES DATABASE

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT


public:
    explicit Login(QWidget *parent = 0);
    ~Login();
    bool loginSuccess = false;

private slots:

    void on_loginButton_clicked();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
