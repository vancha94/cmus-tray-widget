#include <QtCore/QProcess>
#include "cmustrayicon.h"
#include <QAction>
#include <QMenu>
#include <QtConcurrent/QtConcurrentRun>



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

}
void CmusTrayIcon::previosTrack()
{
    auto  tmp = previosTrackCMUS->data();
    int arg = tmp.toInt();
    makeCommand(arg);
    textThread.cancel();
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
    textThread.cancel();

}

void CmusTrayIcon::processingOutputConsole()
{
    QString str;
    QString artist, song, songOld = "";
    int duration, durationOld = 0;
    consoleText.clear();
    while (true)
    {
        makeCommand(arguments.length() - 1);
        int index = consoleText.lastIndexOf(QRegExp(".+"));
        str = consoleText[index];
        duration = getSubString(str, "duration", "\n").toInt();
        song = getSubString(str, "tag title", "\n");
        if (duration != durationOld || song != songOld)
        {
            artist = getSubString(str, "tag artist", "\n");
            trayIcon->showMessage(artist, song, QSystemTrayIcon::NoIcon, msecs);
            durationOld = duration;
            songOld = song;
            consoleText.clear();

        }

//        float pauseTimeSecond = 0.8;
//        bool isFirst = true;
//        consoleText.clear();
//        makeCommand(arguments.length() - 1);
//        QThread::msleep(qCeil(pauseTimeSecond*1000));
//        makeCommand(arguments.length() - 1);
//
//        int startN, endN, countN;
//                foreach(str, consoleText)
//            {
//                if (str != QString())
//                {
//                    if (isFirst)
//                    {
//                        song = getSubString(str, "tag title", "\n");
//                        artist = getSubString(str, "tag artist", "\n");
//                        countN = getSubString(str, "duration", "\n").toInt();
//                        startN = getSubString(str, "position", "\n").toInt();
//
//                        isFirst = false;
//                    }
//                    endN = getSubString(str, "position", "tag artist").toInt();
//                }
//
//            }
//        trayIcon->showMessage(artist, song, QSystemTrayIcon::NoIcon, msecs);
//        int timePause = qCeil( pauseTimeSecond * (countN - endN) / ((float) (endN - startN)));
//        QThread::sleep(timePause);
    }
}

QString CmusTrayIcon::getSubString(QString str, QString start, QString end)
{
    int indexStart = str.indexOf(start) + start.size();
    int indexEnd = str.indexOf(end, indexStart);
    int len = indexEnd - indexStart;
    return str.mid(indexStart, len);;
}





