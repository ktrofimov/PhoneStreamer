#include "MyCall.h"

#include <pjsua2.hpp>
#include <QDebug>

using namespace pj;

// Notification when call's state has changed.
void MyCall::onCallState(OnCallStateParam &prm)
{
    Q_UNUSED(prm);
    CallInfo ci = getInfo();

    parent->emitCallStateChanged(ci.role, ci.id, ci.state, ci.lastStatusCode, QString::fromStdString(ci.remoteUri));

    if(ci.state == PJSIP_INV_STATE_DISCONNECTED)
    {
        parent->setConnectDuration(ci.connectDuration.sec);
        qDebug() << "deleting call with callId" << ci.id;
        delete this;
    }
}

// Notification when call's media state has changed.
void MyCall::onCallMediaState(OnCallMediaStateParam &prm)
{
    Q_UNUSED(prm);
    CallInfo ci = getInfo();

    // Iterate all the call medias
    for (unsigned i = 0; i < ci.media.size(); i++) {
        if (ci.media[i].type == PJMEDIA_TYPE_AUDIO && getMedia(i)) {
            audioMedia = (AudioMedia *)getMedia(i);

            // Connect the call audio media to sound device
            AudDevManager& mgr = Endpoint::instance().audDevManager();
            audioMedia->startTransmit(mgr.getPlaybackDevMedia());
            // Adjust Audio Volume
            audioMedia->adjustRxLevel(1.5f);
            audioMedia->adjustTxLevel(0.0f);
            captureMedia = &(mgr.getCaptureDevMedia());
            captureMedia->adjustRxLevel(1.5f);
            captureMedia->adjustTxLevel(1.5f);
            captureMedia->startTransmit(*audioMedia);
        }
    }
}

void MyCall::setLineOutVolume( float value )
{
    if( audioMedia != 0 )
        audioMedia->adjustTxLevel( value );    // To Line Out
//    TEST
    if( captureMedia != 0 )
        captureMedia->adjustRxLevel( value );  // From SIP
}

void MyCall::setLineInVolume( float value )
{
    if( audioMedia != 0 )
        audioMedia->adjustRxLevel( value );    // From Line In
}

void MyCall::setSIPOutVolume( float value )
{
    if( captureMedia != 0 )
        captureMedia->adjustTxLevel( value );  // To SIP
//    TEST
    if( audioMedia != 0 )
        audioMedia->adjustRxLevel( value );    // From Line In
}

void MyCall::setSIPInVolume( float value )
{
    if( captureMedia != 0 )
        captureMedia->adjustRxLevel( value );  // From SIP
}


void MyCall::onCallTransferRequest(OnCallTransferRequestParam &prm)
{
    Q_UNUSED(prm);
    CallInfo ci = getInfo();
    parent->emitCallStateChanged(ci.role, ci.id, ci.state, ci.lastStatusCode, QString::fromStdString(ci.remoteUri));
}

void MyCall::onCallTransferStatus(OnCallTransferStatusParam &prm)
{
    Q_UNUSED(prm);
    CallInfo ci = getInfo();
    parent->emitCallStateChanged(ci.role, ci.id, ci.state, ci.lastStatusCode, QString::fromStdString(ci.remoteUri));

    if(prm.statusCode == PJSIP_SC_OK)
    {
        qDebug() << "deleting call with callId" << ci.id;

        delete this;
    }
}

void MyCall::onCallReplaceRequest(OnCallReplaceRequestParam &prm)
{
    qDebug() << "onCallReplaceRequest statusCode" << prm.statusCode;
}
