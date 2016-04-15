#include "mainwindow.h"
#include <QApplication>
#include <login.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    Login login;
    login.show();
    if (login.exec() == QDialog::Accepted) {
        login.hide();
        w.show();

    }



    return a.exec();
}
