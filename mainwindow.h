#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScreen>
#include <QDesktopWidget>
#include <QSoundEffect>
#include <QString>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTcpSocket>
#include <QCloseEvent>
#include <QTableWidget>
#include <QTableWidgetItem>

#include "VoIP.h"

namespace Ui {
class MainWindow;
}

enum CLI_STATE {
    IDLE,
    LOGIN,
    CONFERENCE,
    TRUNK,
    DTMF
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSoundEffect ringtone, outgoingRing;
    QScreen *screen;
    QPixmap pxmCall, pxmHangup, pxmInetOn, pxmInetOff,
        pxmPbxOn, pxmPbxOff, pxmTrunkOn, pxmTrunkOff,
        pxmDbOn, pxmDbOff, pxmCastOn, pxmCastOff,
        pxmExtOn, pxmExtOff,
        confHangup, confCall;
    QIcon confUnmute, confMute;
    VoIP *pVoIP;
    int screenX, screenY, callId, timerId;
    int inetTimeout;
    int dtmfTimeout;
    bool inetRq, inetOk;
    bool pbxRq, pbxOk;
    bool trunkOk;
    CLI_STATE state;
    QString sipState, sipUser, sipServer, sipPass, sipPort, sipExtTrunk, sipProto, sipConf, sipConfPin, sipInputDev, sipOutputDev;
    bool sipAutoConf, sipShowCount, sipComments, sipMultiComments, allowVolume;
    bool unmuteFlag;
    int svolume, avolume;
    QString psqlUser, psqlServer, psqlPass, psqlBase, pbxChannel;
    QString pbxType;
    QSqlDatabase db;
    QTcpSocket *webSocket, *pbxSocket ;
    bool parse, first_call;
    int admin;

    void showListenersCount( int );
    void Log( QString );
    void registerSip();
    void registerDB();
    void getSettings();
    void error(QString error);
    void sipCommand(QString cmd, QString value);
    void checkInet();
    void checkPBX();
    virtual void timerEvent( QTimerEvent *event );
    virtual void closeEvent( QCloseEvent *event );

private slots:
    void getScreen();
    void on_callState_changed(int role, int callId, int state, int status, QString remoteUri);
    void on_regState_changed( bool state );
    void on_callButton_clicked();
    void on_hangButton_clicked();
    void on_logButton_clicked(bool checked);
    void inetConnected();
    void inetError();
    void pbxConnected();
    void pbxError();
    void pbxDataReceived();
    void on_listenersButton_clicked(bool checked);

    void mute();
    void unmute();
    void setSVolume( float volume );
    void setAVolume( float volume );
    void checkMuting();
    void markAllChannels( bool value );
    void removeUnusedChannels();
    QTableWidgetItem *findCall( QString SIPChannel );
    virtual void resizeEvent(QResizeEvent* event);
    void valueSChanged( int value );
    void valueAChanged( int value );
    void on_listeners_cellClicked(int row, int column);
};

#endif // MAINWINDOW_H
