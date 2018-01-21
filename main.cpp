#include "cmustrayicon.h"
#include <QApplication>

// TODO: сделать диаалог о QT
// TODO: Сделать запуск CMUS вместе с программой
// TODO: добавить окно about
// TODO: выложить на гитхаб
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CmusTrayIcon w;
    w.show();
    return a.exec();
}
