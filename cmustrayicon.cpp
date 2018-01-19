#include <QtCore/QProcess>
#include "cmustrayicon.h"
#include <QAction>
#include <QMenu>



CmusTrayIcon::CmusTrayIcon(QWidget *parent)
    : QMainWindow(parent)
{
   // this->hide();
//    cmusPros = new QProcess(this);
//    cmusPros->start("cmus");

    initStrings();
    createCMUSActions();

    createTrayIcon();


}

void CmusTrayIcon::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(playCMUS);
    trayIconMenu->addAction(pauseCMUS);
    trayIconMenu->addAction(nextTrackCMUS);
    trayIconMenu->addAction(previosTrackCMUS);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(pauseCMUS->icon());
    trayIcon->show();
}

void CmusTrayIcon::createCMUSActions()
{
    createAction(playCMUS, "play", 0);
    connect(playCMUS, &QAction::triggered, this, &CmusTrayIcon::play);
    createAction(pauseCMUS, "pause", 1, false);
    connect(pauseCMUS, &QAction::triggered, this, &CmusTrayIcon::pause);
    createAction(nextTrackCMUS, "next", 2);
    connect(nextTrackCMUS, &QAction::triggered, this, &CmusTrayIcon::nextTrack);
    createAction(previosTrackCMUS, "back", 3);
    connect(previosTrackCMUS,&QAction::triggered, this, &CmusTrayIcon::previosTrack);
}

void CmusTrayIcon::initStrings()
{
    command = "cmus-remote";
    arguments.append("-p"); // play
    arguments.append("-u"); // pause
    arguments.append("-n"); // next
    arguments.append("-r"); //previos
    arguments.append("-Q"); // info track
}

void CmusTrayIcon::createAction(QAction *&action, const QString name, int data, bool isVisible)
{
    QString iconAdress = QString(":icons/") + name + QString("-button.png");
    action = new QAction(QIcon(iconAdress), name, this);
    action->setData(data);
    action->setVisible(isVisible);
}

CmusTrayIcon::~CmusTrayIcon()
{

}

void CmusTrayIcon::play()
{
    auto  tmp = playCMUS->data();
    int arg = tmp.toInt();
    makeCommand(arg);
    pauseCMUS->setVisible(true);
    playCMUS->setVisible(false);
    trayIcon->setIcon(playCMUS->icon());
}

void CmusTrayIcon::pause()
{
    auto  tmp = pauseCMUS->data();
    int arg = tmp.toInt();
    makeCommand(arg);
    pauseCMUS->setVisible(false);
    playCMUS->setVisible(true);
    trayIcon->setIcon(pauseCMUS->icon());
}
void CmusTrayIcon::nextTrack()
{
    auto  tmp = nextTrackCMUS->data();
    int arg = tmp.toInt();
    makeCommand(arg);
}
void CmusTrayIcon::previosTrack()
{
    auto  tmp = previosTrackCMUS->data();
    int arg = tmp.toInt();
    makeCommand(arg);
}

void CmusTrayIcon::makeCommand(int arg)
{
    QProcess *process = new QProcess();
    QStringList argument;
    argument << arguments[arg];
    process->start(command,argument);
}

void CmusTrayIcon::show()
{
    QWidget::show();
    hide();
}




