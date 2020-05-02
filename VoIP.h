#ifndef METAVOIP_H
#define METAVOIP_H

#include "MyAccount.h"
#include "MyCall.h"

#include <pjsua2.hpp>

#include <QString>
#include <QDebug>
#include <QObject>

extern "C" {
#include <pjlib.h>
#include <pjlib-util.h>
#include <pjmedia.h>
#include <pjmedia-codec.h>
#include <pjsip.h>
#include <pjsip_simple.h>
#include <pjsip_ua.h>
#include <pjsua-lib/pjsua.h>
}

class MyAccount;
class MyCall;

class VoIP: public QObject
{
    Q_OBJECT

public:
    VoIP(QString protocol, int port, QObject *parent = 0);
    AudioDevInfoVector EnumAudioDevices();
    int SetAudioInput( int dev );
    int SetAudioOutput( int dev );
    ~VoIP();

    void createAccount(QString idUri, QString registrarUri, QString user, QString password);
    void registerAccount();
    void unregisterAccount();
    void makeCall(QString number);
    bool hangupCall(int callId);
    void acceptCall(int callId);
    bool holdCall(int callId);
    bool transferCall(QString destination);
    void ring(int callId);
    void sendDtmf(QString num);
    void setLineOutVolume( float value );
    void setSIPOutVolume( float value );
    inline bool isLoaded()
    {
        return loaded;
    }

    inline long getConnectDuration()
    {
        return connectDuration;
    }

    inline void setConnectDuration(long connectDuration)
    {
        this->connectDuration = connectDuration;
    }

    void emitRegStateStarted(bool status);
    void emitRegStateChanged(bool status);
    void emitCallStateChanged(int role, int callId, int state, int status, QString remoteUri);

signals:
    void regStateStarted(bool status);
    void regStateChanged(bool status);
    void callStateChanged(int role, int callId, int state, int status, QString remoteUri);

private:
    EpConfig epCfg;
    Endpoint ep;
    TransportConfig tCfg;
    AccountConfig aCfg;
    MyAccount *account;
    MyCall *call;
    bool loaded;
    int connectDuration;
};

#endif // METAVOIP_H
