#include "cmustrayicon.h"
#include <QApplication>

// TODO: сделать диаалог о QT
// TODO: добавить окно about
// TODO: Сделать окно настроек
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CmusTrayIcon w;
    w.show();
    return a.exec();
}
