#ifndef CMUSTRAYICON_H
#define CMUSTRAYICON_H

#include <QMainWindow>
#include <QtWidgets/QSystemTrayIcon>
#include <QProcess>

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

    QString command;
    QVector<QString> arguments;

    QProcess *cmusPros;



public:
    CmusTrayIcon(QWidget *parent = 0);
    ~CmusTrayIcon();

    void show();

public slots:
    void play();
    void pause();
    void nextTrack();
    void previosTrack();

private:
    void makeCommand(int arg);
    void createAction(QAction *&action, const QString name, int data, bool isVisible = true);

    void initStrings();

    void createCMUSActions();

    void createTrayIcon();
};

#endif // CMUSTRAYICON_H
