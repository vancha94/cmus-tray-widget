#include <QtCore/QProcess>
#include "cmustrayicon.h"
#include <QAction>
#include <QMenu>
#include <QtConcurrent/QtConcurrentRun>
#include <QtCore/QtMath>
//#include <QtConcurrent>


CmusTrayIcon::CmusTrayIcon(QWidget *parent)
    : QMainWindow(parent)
{
    actionIndex = 0;
    createTrayIcon();
    createCMUSActions();
    initStrings();
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

    trayIcon->showMessage("Hello", "CMUS is started", QSystemTrayIcon::NoIcon, msecs);

    connect(trayIcon,&QSystemTrayIcon::activated,this,&CmusTrayIcon::activatedTrayIcon);
}

void CmusTrayIcon::createCMUSActions()
{
    createAction(playCMUS, "play", "-p");
    connect(playCMUS, &QAction::triggered, this, &CmusTrayIcon::play);
    createAction(pauseCMUS, "pause", "-u", false);
    connect(pauseCMUS, &QAction::triggered, this, &CmusTrayIcon::pause);
    createAction(nextTrackCMUS, "next", "-n");
    connect(nextTrackCMUS, &QAction::triggered, this, &CmusTrayIcon::nextTrack);
    createAction(previosTrackCMUS, "back", "-r");
    connect(previosTrackCMUS,&QAction::triggered, this, &CmusTrayIcon::previosTrack);
    createAction(stopCMUS, "stop", "-s");
    connect(stopCMUS,&QAction::triggered,this,&CmusTrayIcon::stop);
}

void CmusTrayIcon::initStrings()
{
    command = "cmus-remote";
    arguments.append("-Q"); // info track
}

void CmusTrayIcon::createAction(QAction *&action, const QString name, QString arg, bool isVisible)
{

    QString iconAdress = QString(":icons/") + name + QString("-button.png");
    action = new QAction(QIcon(iconAdress), name, this);
    action->setData(actionIndex);
    action->setVisible(isVisible);
    trayIconMenu->addAction(action);
    arguments.append(arg);
    actionIndex++;
}

CmusTrayIcon::~CmusTrayIcon()
{
    stop();
    textThread.cancel();
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
    textThread = QtConcurrent::run(this, &CmusTrayIcon::processingOutputConsole);

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
    textThread.cancel();
}
void CmusTrayIcon::nextTrack()
{
    auto  tmp = nextTrackCMUS->data();
    int arg = tmp.toInt();
    makeCommand(arg);
    makeCommand(arguments.length() - 1);
    textThread.cancel();
    while (!textThread.isCanceled());
    textThread = QtConcurrent::run(this, &CmusTrayIcon::processingOutputConsole);
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


    mutex.lock();
    process->waitForFinished();
    consoleText.append(QString(process->readAllStandardOutput()));
    mutex.unlock();
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

void CmusTrayIcon::processingOutputConsole()
{
    while (true)
    {
        int pauseTimeSecond = 1;
        bool isFirst = true;
        consoleText.clear();
        makeCommand(arguments.length() - 1);
        QThread::sleep(pauseTimeSecond);
        makeCommand(arguments.length() - 1);
        QString str;
        QString artist, song;
        int startN, endN, countN;
                foreach(str, consoleText)
            {
                if (str != QString())
                {
                    if (isFirst)
                    {
                        song = getSubString(str, "tag title", "\n");
                        artist = getSubString(str, "tag artist", "\n");
                        countN = getSubString(str, "duration", "\n").toInt();
                        startN = getSubString(str, "position", "\n").toInt();

                        isFirst = false;
                    }
                    endN = getSubString(str, "position", "tag artist").toInt();
                }

            }
        trayIcon->showMessage(artist, song, QSystemTrayIcon::NoIcon, msecs);
        int timePause = qCeil((float) pauseTimeSecond * (countN - endN) / ((float) (endN - startN)));
        QThread::sleep(timePause);
    }
}

QString CmusTrayIcon::getSubString(QString str, QString start, QString end)
{
    int indexStart = str.indexOf(start) + start.size();
    int indexEnd = str.indexOf(end, indexStart);
    int len = indexEnd - indexStart;
    return str.mid(indexStart, len);;
}





