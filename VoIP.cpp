#include "VoIP.h"
#include <QDebug>
#include <QCoreApplication>
#include <QSettings>
#include <QString>

VoIP::VoIP(QString protocol, int port, QObject *parent) : QObject(parent)
{
    try {
        loaded = false;
        connectDuration = 0;

        ep.libCreate();
        ep.libInit(epCfg);
        tCfg.port = port;
        call = nullptr;

        if(protocol=="TCP")
            ep.transportCreate(PJSIP_TRANSPORT_TCP, tCfg);
        else
            ep.transportCreate(PJSIP_TRANSPORT_UDP, tCfg);

        ep.libStart();

        loaded = true;

        qDebug()<< "*** PJSUA2 STARTED ***";

    } catch (Error &err) {
        loaded = false;

        qDebug() << "Lib starting failed" << err.info().c_str();
    }
}

AudioDevInfoVector VoIP::EnumAudioDevices()
{
    int pbd = ep.audDevManager().getCaptureDev();
    ep.audDevManager().setCaptureDev( 0 );
    pbd = ep.audDevManager().getCaptureDev();

    return ep.audDevManager().enumDev();
}

int VoIP::SetAudioInput( int dev )
{
    ep.audDevManager().setCaptureDev( dev );
    return ep.audDevManager().getCaptureDev();
}

int VoIP::SetAudioOutput( int dev )
{
    ep.audDevManager().setPlaybackDev( dev );
    return ep.audDevManager().getPlaybackDev();
}

VoIP::~VoIP()
{
    try {
        // Delete the account. This will unregister from server
        delete account;
    } catch (Error &err) {
        qDebug() << "Lib deleting failed" << err.info().c_str();
    }
}

void VoIP::createAccount(QString idUri, QString registrarUri, QString user, QString password)
{
    try {
        // Configure an AccountConfig
        aCfg.idUri = idUri.toStdString();
        aCfg.regConfig.registrarUri = registrarUri.toStdString();
        AuthCredInfo cred("digest", "*", user.toStdString(), 0, password.toStdString());
        aCfg.sipConfig.authCreds.push_back(cred);
        aCfg.callConfig.timerMinSESec = 90;
        aCfg.callConfig.timerSessExpiresSec = 1800;

        account = new MyAccount(this);
        account->create(aCfg);

        qDebug() << "Account creation successful";

    } catch(Error& err) {
        qDebug() << "Account creation error: " << err.info().c_str();
    }
}

void VoIP::registerAccount()
{
    if(account)
    {
        try
        {
            account->setRegistration(true);

            qDebug() << "Register account successfull";

        } catch(Error& err) {
            qDebug() << "Register error: " << err.info().c_str() << endl;
        }
    }
}

void VoIP::unregisterAccount()
{
    if(account)
    {
        try
        {
            account->setRegistration(false);

            qDebug() << "Unregister account successfull";

        } catch(Error& err) {
            qDebug() << "Unregister error: " << err.info().c_str() << endl;
        }
    }
}

void VoIP::makeCall(QString number)
{
    if(account)
    {
        qDebug() << "Attempting to create call";

        call = new MyCall(this, *account);
        CallOpParam prm(true); // Use default call settings

        try {
            qDebug() << "calling API with account" << account->getId() << account->getInfo().regIsActive << account->isValid();

            call->makeCall(number.toStdString(), prm);

            qDebug() << "makeCall was called with" << number;

        } catch(Error& err) {
            qDebug() << "Call could not be made" << err.info().c_str();
        }
    }
}

void VoIP::ring(int callId)
{
    if(account != nullptr)
    {
        try {
            qDebug() << "set state to ringing for callId" << callId;

            call = new MyCall(this, *account, callId);
            CallOpParam prm;
            prm.statusCode = PJSIP_SC_RINGING;
            call->answer(prm);
        } catch(Error& err) {
            qDebug() << "Ringing failed" << err.info().c_str();
        }
    }
}

void VoIP::acceptCall(int callId)
{
    if(account)
    {
        qDebug() << "accepting call with callId" << callId;

        try {
            call = new MyCall(this, *account, callId);
            CallOpParam prm;
            prm.statusCode = PJSIP_SC_OK;
            call->answer(prm);
        } catch(Error& err) {
            qDebug() << "Accepting failed" << err.info().c_str();
        }
    }
}

bool VoIP::hangupCall(int callId)
{
    qDebug() << "hang up on callId" << callId;

    if(account && call != nullptr)
    {
        try {
            CallInfo ci = call->getInfo();

            CallOpParam prm;

            if(ci.lastStatusCode == PJSIP_SC_RINGING)
            {
                prm.statusCode = PJSIP_SC_BUSY_HERE;
            }
            else
            {
                prm.statusCode = PJSIP_SC_OK;
            }

            call->hangup(prm);
            call = nullptr;

            return 1;
        } catch(Error& err) {
            qDebug() << "HangupCall failed" << err.info().c_str();
        }
    }

    return 0;
}

bool VoIP::holdCall(int callId)
{
    if(account && call != nullptr)
    {
        try {
            qDebug() << "Hold call with callId" << callId;

            CallOpParam prm(true);

            if(!call->isOnHold())
            {
                qDebug() << "Call will be hold";

                call->setHoldTo(true);

                prm.statusCode = PJSIP_SC_QUEUED;
                call->setHold(prm);
            }
            else
            {
                qDebug() << "Call will be re-invited";

                call->setHoldTo(false);

                prm.opt.flag = PJSUA_CALL_UNHOLD;
                call->reinvite(prm);
            }

            return 1;
        } catch(Error& err) {
            qDebug() << "Accepting failed" << err.info().c_str();
        }
    }

    return 0;
}

bool VoIP::transferCall(QString destination)
{
    if(account && call != nullptr)
    {
        try {
            qDebug() << "transfer call to" << destination;

            CallOpParam prm;
            prm.statusCode = PJSIP_SC_CALL_BEING_FORWARDED;
            call->xfer(destination.toStdString(), prm);

            return 1;
        } catch(Error& err) {
            qDebug() << "Accepting failed" << err.info().c_str();
        }
    }

    return 0;
}

void VoIP::emitRegStateStarted(bool status)
{
    emit regStateStarted(status);
}

void VoIP::emitRegStateChanged(bool status)
{
    emit regStateChanged(status);
}

void VoIP::emitCallStateChanged(int role, int callId, int state, int status, QString id)
{
    qDebug() << "emitting data to slot " << callId;
    emit callStateChanged(role, callId, state, status, id);
}

void VoIP::sendDtmf(QString num)
{
    if(call != nullptr)
    {
        try {
            call->dialDtmf(num.toStdString());
            qDebug() << "Dtmf" << num << " sent";
        } catch(Error& err) {
            qDebug() << "Dtmf failed" << err.info().c_str();
        }
    }
}

void VoIP::setLineOutVolume( float value )
{
    if( call != nullptr )
    {
        call->setLineOutVolume( value );
    }
}

void VoIP::setSIPOutVolume( float value )
{
    if( call != nullptr )
    {
        call->setSIPOutVolume( value );
    }
}

