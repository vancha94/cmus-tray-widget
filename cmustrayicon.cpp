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
    actionIndex = 0;

    initStrings();


    createTrayIcon();
    createCMUSActions();

    stop();



}

void CmusTrayIcon::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
//    trayIconMenu->addAction(playCMUS);
//    trayIconMenu->addAction(pauseCMUS);
//    trayIconMenu->addAction(nextTrackCMUS);
//    trayIconMenu->addAction(previosTrackCMUS);
//    trayIconMenu->addAction(stopCMUS);


    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(QIcon(":icons/stop-button.png"));
    trayIcon->show();

    connect(trayIcon,&QSystemTrayIcon::activated,this,&CmusTrayIcon::activatedTrayIcon);
}

void CmusTrayIcon::createCMUSActions()
{
    createAction(playCMUS, "play");
    connect(playCMUS, &QAction::triggered, this, &CmusTrayIcon::play);
    createAction(pauseCMUS, "pause", false);
    connect(pauseCMUS, &QAction::triggered, this, &CmusTrayIcon::pause);
    createAction(nextTrackCMUS, "next");
    connect(nextTrackCMUS, &QAction::triggered, this, &CmusTrayIcon::nextTrack);
    createAction(previosTrackCMUS, "back");
    connect(previosTrackCMUS,&QAction::triggered, this, &CmusTrayIcon::previosTrack);
    createAction(stopCMUS,"stop");
    connect(stopCMUS,&QAction::triggered,this,&CmusTrayIcon::stop);
}

void CmusTrayIcon::initStrings()
{
    command = "cmus-remote";
    arguments.append("-p"); // play
    arguments.append("-u"); // pause
    arguments.append("-n"); // next
    arguments.append("-r"); //previos
    arguments.append("-s"); // stop
    arguments.append("-Q"); // info track
}

void CmusTrayIcon::createAction(QAction *&action, const QString name, bool isVisible)
{

    QString iconAdress = QString(":icons/") + name + QString("-button.png");
    action = new QAction(QIcon(iconAdress), name, this);
    action->setData(actionIndex);
    action->setVisible(isVisible);
    trayIconMenu->addAction(action);
    actionIndex++;
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
    stopCMUS->setVisible(true);
    trayIcon->setIcon(playCMUS->icon());

}

void CmusTrayIcon::pause()
{
    auto  tmp = pauseCMUS->data();
    int arg = tmp.toInt();
    makeCommand(arg);
    pauseCMUS->setVisible(false);
    playCMUS->setVisible(true);
    stopCMUS->setVisible(false);
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

void CmusTrayIcon::activatedTrayIcon(QSystemTrayIcon::ActivationReason reason)
{
    switch  (reason)
    {
        case QSystemTrayIcon::Trigger:
            if(pauseCMUS->isVisible())
                pause();
            else
                play();
            break;
        case QSystemTrayIcon::MiddleClick:
            nextTrack();
            break;
        default:
            break;
    }
}

void CmusTrayIcon::stop()
{
    auto  tmp = stopCMUS->data();
    int arg = tmp.toInt();
    makeCommand(arg);
    playCMUS->setVisible(true);
    pauseCMUS->setVisible(false);
    stopCMUS->setVisible(false);
    trayIcon->setIcon(stopCMUS->icon());

}





