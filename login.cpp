#include "login.h"
#include "ui_login.h"
#include <QDebug>
#include <QPushButton>

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;

}

//Attempt to login
void Login::on_loginButton_clicked()
{

}
