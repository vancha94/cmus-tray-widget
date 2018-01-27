#ifndef CMUSTRAYICON_H
#define CMUSTRAYICON_H

static const int msecs = 4000;

#include <QMainWindow>
#include <QtWidgets/QSystemTrayIcon>
#include <QProcess>
#include <QtCore/QFuture>

class CmusTrayIcon : public QMainWindow
{
    Q_OBJECT
private:
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    // actions
    QAction *pauseCMUS;
    QAction *playCMUS;
    QAction *nextTrackCMUS;
    QAction *previosTrackCMUS;
    QAction *stopCMUS;
    QAction *aboutQtAction;
    QAction *quitAction;
    int actionIndex;

    QString command;
    QVector<QString> arguments;

    QProcess *cmusProc;
    QString programm;

    QFuture<void> textThread;
    QMutex mutex;
    QStringList consoleText;
    bool keepPlaying;

    bool isPaused;

public:
    CmusTrayIcon(QWidget *parent = 0);
    ~CmusTrayIcon();

    void show();
    void processingOutputConsole();

public slots:
    void play();
    void pause();
    void nextTrack();
    void previosTrack();
    void stop();


private:
    void makeCommand(int arg);
    void createAction(QAction *&action, QString name, QString arg, bool isVisible = true);
    void initStrings();
    void createCMUSActions();
    void createTrayIcon();
    void activatedTrayIcon(QSystemTrayIcon::ActivationReason reason);
    QString getSubString(QString str, QString start, QString end);

    void startCMUS();

    void closeCMUS();
};

#endif // CMUSTRAYICON_H
