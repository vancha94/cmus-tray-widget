#include "cmustrayicon.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CmusTrayIcon w;
    w.show();



    return a.exec();
}
