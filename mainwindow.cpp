#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QSettings>
#include <QMessageBox>

#define INET_TIMEOUT 10 // 10s
#define DTMF_TIMEOUT 5

#define ASTERISK "asterisk"
#define YATE     "yate"
#define SIP      "SIP"
#define PJSIP    "PJSIP"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->logButton->setChecked( true );
    ui->listenersButton->setChecked( true );
    ui->log->hide();
    ui->listeners->hide();
    ui->listeners->setColumnCount( 3 );

    inetTimeout=INET_TIMEOUT;
    dtmfTimeout = 0;

    first_call = true;
    inetRq = false;
    inetOk = false;
    pbxRq = false;
    pbxOk = false;
    trunkOk = false;
    unmuteFlag = false;

    webSocket = new QTcpSocket(this);
    connect( webSocket, SIGNAL(connected()), this, SLOT(inetConnected()));
    connect( webSocket, SIGNAL(error()), this, SLOT(inetError()));
    pbxSocket = new QTcpSocket(this);
    connect( pbxSocket, SIGNAL(connected()), this, SLOT(pbxConnected()));
    connect( pbxSocket, SIGNAL(error()), this, SLOT(pbxError()));
    connect( pbxSocket, SIGNAL(readyRead()), this, SLOT(pbxDataReceived()));
    //connect( ui->setVolume, SIGNAL(valueChanged(int)), this, SLOT( volumeChanged(int) ) );
    connect( ui->SVolume, SIGNAL(valueChanged(int)), this, SLOT( valueSChanged(int) ) );
    connect( ui->AVolume, SIGNAL(valueChanged(int)), this, SLOT( valueAChanged(int) ) );

    screen = QGuiApplication::screens().first();
    connect(screen, SIGNAL(virtualGeometryChanged(QRect)), this, SLOT(getScreen()));
    getScreen();
    getSettings();

    if( sipShowCount==false )
    {
        ui->info->setVisible( false);
        ui->listeners->setVisible( false );
        ui->listenersButton->setVisible( false );
    }

    ui->SVolume->setValue( svolume );
    ui->SVolume->setEnabled( allowVolume );
    ui->lblSVolumeVal->setText( QString("%1").arg((float)svolume/100) );

    ui->AVolume->setValue( avolume );
    ui->AVolume->setEnabled( allowVolume );
    ui->lblAVolumeVal->setText( QString("%1").arg((float)avolume/100) );

    ui->AVolume->hide();
    ui->lblAVolume->hide();
    ui->lblAVolumeVal->hide();

//#define DEBUG_LIST
#ifdef DEBUG_LIST
    ui->listeners->insertRow(0);
    ui->listeners->setItem( 0,0, new QTableWidgetItem( QString("+78121234567 (Name Lastname)")));
    ui->listeners->setItem( 0,1, new QTableWidgetItem( QString("")));
    ui->listeners->setItem( 0,2, new QTableWidgetItem( QString("")));
    ui->listeners->item(0,1)->setTextAlignment(Qt::AlignCenter);
    ui->listeners->item(0,1)->setIcon( confMute );
    QLabel *lbl = new QLabel();
    lbl->setPixmap( confCall );
    lbl->setAlignment( Qt::AlignCenter );
    lbl->setStyleSheet("QLabel{background-color: rgba(0, 0, 0, 0);}");
    ui->listeners->setCellWidget( 0, 2, lbl );


    ui->listeners->insertRow(1);
    ui->listeners->setItem( 1,0, new QTableWidgetItem( QString("+79111637007 (Name Familyname)")));
    ui->listeners->setItem( 1,1, new QTableWidgetItem( QString("1")));
    ui->listeners->setItem( 1,2, new QTableWidgetItem( QString("")));
    ui->listeners->item(1,1)->setTextAlignment(Qt::AlignCenter);
    ui->listeners->item(1,1)->setIcon( confUnmute );
    ui->listeners->item(0,1)->setData(Qt::UserRole,QVariant( true ));
    QLabel *lbl2 = new QLabel();
    lbl2->setPixmap( confCall );
    lbl2->setAlignment( Qt::AlignCenter );
    lbl2->setStyleSheet("QLabel{background-color: rgba(0, 0, 0, 0);}");
    ui->listeners->setCellWidget( 1, 2, lbl2 );

    ui->listeners->insertRow(2);
    ui->listeners->setItem( 2,0, new QTableWidgetItem( QString("+79219876543 (Ivan Ivanov)")));
    ui->listeners->setItem( 2,1, new QTableWidgetItem( QString("15")));
    ui->listeners->setItem( 2,2, new QTableWidgetItem( QString("")));
    ui->listeners->item(2,1)->setTextAlignment(Qt::AlignCenter);
    ui->listeners->item(2,1)->setIcon( confMute );
    QLabel *lbl3 = new QLabel();
    lbl3->setPixmap( confCall );
    lbl3->setAlignment( Qt::AlignCenter );
    lbl3->setStyleSheet("QLabel{background-color: rgba(0, 0, 0, 0);}");
    ui->listeners->setCellWidget( 2, 2, lbl3 );
#endif

    timerId = startTimer( 1000 );

    if(sipUser!="" && sipPass!="" && sipServer!="" && sipPort!="")
    {
        Log( tr("Registering SIP...") );
        registerSip();
    }
    else
        Log( tr("Wrong SIP parameters") );

    if( QString( YATE ).compare( pbxType )==0 )
    {
        if(psqlUser!="" && psqlPass!="" && psqlServer!="" && psqlBase!="")
        {
            Log( tr("Registering Database...") );
            registerDB();
        }
        else
            Log( tr("Wrong Database parameters") );

    }
    else
        ui->stDB->setEnabled( false );
}

void MainWindow::valueSChanged( int value )
{
    svolume = value;
    ui->lblSVolumeVal->setText( QString("%1").arg((float)value/100) );
    setSVolume( (float)value/100 );
}

void MainWindow::valueAChanged( int value )
{
    avolume = value;
    ui->lblAVolumeVal->setText( QString("%1").arg((float)value/100) );
    if( unmuteFlag )
        setAVolume( (float)value/100 );
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   ui->listeners->setColumnWidth(0,ui->info->width()-202);
   ui->listeners->setColumnWidth(1,100);
   ui->listeners->setColumnWidth(2,100);
}

void MainWindow::checkInet()
{
    webSocket->connectToHost( "www.example.com", 80 );
    inetRq = true;
}

void  MainWindow::inetConnected()
{
    webSocket->disconnectFromHost();
    ui->stInet->setPixmap( pxmInetOn );
    if( !inetOk )
        Log( tr("Internet connected") );
    inetRq = false;
    inetOk = true;
}

void  MainWindow::inetError()
{
    ui->stInet->setPixmap( pxmInetOff );
    if( inetOk )
        Log( tr("Internet connection error") );
    inetRq = false;
    inetOk = false;
}

void MainWindow::checkPBX()
{
    if( !pbxOk )
    {
        state = LOGIN;
        pbxSocket->connectToHost( sipServer, 5038 );
    }
    else
    {
        state = CONFERENCE;
        parse = false;
        admin = 0;

        // Mark all rows for delete
        // instead of ui->listeners->setRowCount( 0 );
        markAllChannels( false );

        QString rq = QString( "Action: ConfBridgeList\nConference: %1\n\n" ).arg( sipConf );

        //    Response: Success
        //    EventList: start
        //    Message: Confbridge user list will follow

        //    Event: ConfbridgeList
        //    Conference: 000130200
        //    CallerIDNum: 100
        //    CallerIDName: 100
        //    Channel: SIP/100-00000008
        //    Admin: Yes
        //    MarkedUser: Yes
        //    WaitMarked: No
        //    EndMarked: No
        //    Waiting: No
        //    Muted: No
        //    AnsweredTime: 168

        //    Event: ConfbridgeList
        //    Conference: 000130200
        //    CallerIDNum: +79111637007
        //    CallerIDName: +79111637007
        //    Channel: SIP/TELPHIN_000130192-00000009
        //    Admin: No
        //    MarkedUser: No
        //    WaitMarked: Yes
        //    EndMarked: Yes
        //    Waiting: No
        //    Muted: Yes
        //    AnsweredTime: 13

        //    Event: ConfbridgeList
        //    Conference: 000130200
        //    CallerIDNum: 000108716
        //    CallerIDName: 000108716
        //    Channel: SIP/TELPHIN_000130192-0000000a
        //    Admin: No
        //    MarkedUser: No
        //    WaitMarked: Yes
        //    EndMarked: Yes
        //    Waiting: No
        //    Muted: Yes
        //    AnsweredTime: 23

        //    Event: ConfbridgeListComplete
        //    EventList: Complete
        //    ListItems: 3

        pbxSocket->write( rq.toLatin1() );
    }
    pbxRq = true;
}

void  MainWindow::pbxConnected()
{
    if( QString( YATE ).compare( pbxType )==0 )
        pbxSocket->disconnectFromHost();
    ui->stPBX->setPixmap( pxmPbxOn );
    if( !pbxOk )
        Log( tr("PBX connected") );
    pbxRq = false;
    pbxOk = true;
    if( QString( ASTERISK ).compare( pbxType )==0 )
    {
        state = LOGIN;
        QString rq = QString( "Action: Login\nUsername: %1\nSecret: %2\nEvents: on\n\n" ).arg( psqlUser ).arg( psqlPass );
        pbxSocket->write( rq.toLatin1() );
    }
}

void  MainWindow::pbxError()
{
    ui->stPBX->setPixmap( pxmPbxOff );
    if( pbxOk )
        Log( tr("PBX connection error") );
    pbxRq = false;
    pbxOk = false;
}

QString confCallerIDNum,
    confCallerIDName,
    confChannel,
    confMuted,
    dtmfChannel,
    dtmfDigit;
int dtmfNumber;

void MainWindow::pbxDataReceived( void )
{

    pbxRq = false;
    QRegExp rx("[\r\n]");
    QStringList data = QString( pbxSocket->readAll()).split( rx, QString::SkipEmptyParts );
    for( int i=0; i<data.size(); i++ )
    {
        QStringList pair = data.at(i).split( ": " );
        switch( state )
        {
        case LOGIN:
            if( pair.at(0).compare("Message")==0 )
            {
                if( pair.at(1).compare("Authentication accepted")==0 )
                {
                    pbxOk = true;
                    state = CONFERENCE;
                    parse = false;
                    admin = 0;

#ifndef DEBUG_LIST
                    ui->listeners->setRowCount( 0 );
#endif
                    QString rq = QString( "Action: ConfBridgeList\nConference: %1\n\n" ).arg( sipConf );
                    pbxSocket->write( rq.toLatin1() );
                }
                else
                {
                    pbxSocket->disconnectFromHost();
                    Log( tr("PBX login failed") );
                }
            }
            break;

        case CONFERENCE:
            pbxOk = true;
            if( pair.at(0).compare("Response")==0 && pair.at(1).compare("Success")==0 )
            {
                parse = true;
                admin = 0;
            }
            if( parse )
            {
                //    Event: ConfbridgeList
                //    Conference: 000130200
                //    CallerIDNum: 100
                //    CallerIDName: 100
                //    Channel: SIP/100-00000008
                //    Admin: Yes
                //    MarkedUser: Yes
                //    WaitMarked: No
                //    EndMarked: No
                //    Waiting: No
                //    Muted: No
                //    AnsweredTime: 168

                if( pair.at(0).compare("CallerIDNum")==0 )
                {
                    confCallerIDNum = pair.at(1);
                }
                if( pair.at(0).compare("CallerIDName")==0 )
                {
                    confCallerIDName = pair.at(1);
                }
                if( pair.at(0).compare("Channel")==0 )
                {
                    confChannel = pair.at(1);
                }
                if( pair.at(0).compare("Muted")==0 )
                {
                    confMuted = pair.at(1);
                    if( confCallerIDNum.compare( sipUser )==0 )
                    {
                        admin = 1;
                    }
                    else
                    {
                        QTableWidgetItem *twi = findCall( confChannel );
                        if( twi )
                        {
                            if( confMuted.compare("Yes")==0 )
                            {
                                twi->setData( Qt::UserRole, QVariant( false ) );
                                ui->listeners->item( ui->listeners->row( twi ), 1)->setIcon( confMute );
                            }
                            else
                            {
                                twi->setData( Qt::UserRole, QVariant( true ) );
                                ui->listeners->item( ui->listeners->row( twi ), 1)->setIcon( confUnmute );
                            }
                            twi->setData( Qt::UserRole+4, QVariant( true ) );
                        }
                        else
                        {
                            int r = ui->listeners->rowCount();
                            ui->listeners->insertRow( r );

                            if( confMuted.compare("Yes")==0 )
                            {
                                twi= new QTableWidgetItem( QString("%1 (%2)").arg(confCallerIDNum).arg(confCallerIDName) );
                                twi->setData( Qt::UserRole, QVariant( false ) );
                                ui->listeners->setItem( r,1, new QTableWidgetItem( confMute, QString("")));
                            }
                            else
                            {
                                twi= new QTableWidgetItem( QString("%1 (%2)").arg(confCallerIDNum).arg(confCallerIDName) );
                                twi->setData( Qt::UserRole, QVariant( true ) );
                                ui->listeners->setItem( r,1, new QTableWidgetItem( confUnmute, QString("")));
                            }
                            twi->setData( Qt::UserRole+1, QVariant( confChannel ) );
                            twi->setData( Qt::UserRole+4, QVariant( true ) );
                            ui->listeners->setItem( r, 0, twi );
                            ui->listeners->item( r,1 )->setTextAlignment(Qt::AlignCenter);
                            QLabel *lbl = new QLabel();
                            lbl->setPixmap( confCall );
                            lbl->setAlignment( Qt::AlignCenter );
                            lbl->setStyleSheet("QLabel{background-color: rgba(0, 0, 0, 0);}");
                            ui->listeners->setCellWidget( r, 2, lbl );
                        }
                    }
                }

                int confCount=0;
                if( pair.at(0).compare("ListItems")==0 )
                {
                    confCount = pair.at(1).toInt() - admin;
                    showListenersCount( confCount );
                    removeUnusedChannels();
                    //Log( QString("Conference users: %1, admins: %2").arg(pair.at(1)).arg(admin) );

                    qDebug() << "Conference has " << confCount << " listeners";
                    state = TRUNK;
                    parse = false;
                    QString rq;
                    if( QString( SIP ).compare( pbxChannel ) == 0 )
                    { // SIP
                        rq = QString( "Action: command\nCommand: sip show registry\n\n" );
                        /*
                         *  Host                                    dnsmgr Username       Refresh State                Reg.Time
                         *  213.170.92.166:5060                     N      000130192         1785 Registered           Tue, 06 Dec 2016 21:02:09
                         *  213.170.92.166:5060                     N      000130200         1785 Registered           Tue, 06 Dec 2016 21:02:10
                         *  2 SIP registrations.
                         */
                    }
                    else
                        { // PJSIP
                        rq = QString( "Action: command\nCommand: pjsip show registrations\n\n" );
                        /*
                         *  <Registration/ServerURI..............................>  <Auth..........>  <Status.......>
                         * ==========================================================================================
                         *
                         *  000130193/sip:213.170.92.166:5060                       000130193         Registered
                         *  000130194/sip:213.170.92.166:5060                       000130194         Registered
                         *  000130195/sip:213.170.92.166:5060                       000130195         Registered
                         *  000130196/sip:213.170.92.166:5060                       000130196         Registered
                         */
                    }
                    pbxSocket->write( rq.toLatin1() );
                }
            }
            else
            {
                if( pair.at(0).compare("Message")==0 && pair.at(1).compare("No active conferences.")==0 )
                {
                    showListenersCount( 0 );
                    qDebug() << "Conference is closed";

                    state = TRUNK;
                    parse = false;
                    QString rq;
                    if( QString( SIP ).compare( pbxChannel ) == 0 )
                    { // SIP
                        rq = QString( "Action: command\nCommand: sip show registry\n\n" );
                        /*
                         *  Host                                    dnsmgr Username       Refresh State                Reg.Time
                         *  213.170.92.166:5060                     N      000130192         1785 Registered           Tue, 06 Dec 2016 21:02:09
                         *  213.170.92.166:5060                     N      000130200         1785 Registered           Tue, 06 Dec 2016 21:02:10
                         *  2 SIP registrations.
                         */
                    }
                    else
                        { // PJSIP
                        rq = QString( "Action: command\nCommand: pjsip show registrations\n\n" );
                        /*
                         *  <Registration/ServerURI..............................>  <Auth..........>  <Status.......>
                         * ==========================================================================================
                         *
                         *  000130193/sip:213.170.92.166:5060                       000130193         Registered
                         *  000130194/sip:213.170.92.166:5060                       000130194         Registered
                         *  000130195/sip:213.170.92.166:5060                       000130195         Registered
                         *  000130196/sip:213.170.92.166:5060                       000130196         Registered
                         */
                    }
                    pbxSocket->write( rq.toLatin1() );
                }
            }
            break;

        case TRUNK:
            pbxOk = true;
            if( pair.at(0).compare("Response")==0 && pair.at(1).compare("Follows")==0 )
                parse = true;
            if( parse )
            {
                QStringList trunk = pair.at(0).simplified().split(" ");
                int trunkSize, trunkNumber, trunkState;
                if( QString( SIP ).compare( pbxChannel ) == 0 )
                { // SIP
                    trunkSize = 10;
                    trunkNumber = 2;
                    trunkState = 4;
                }
                else
                { // PJSIP
                    trunkSize = 3;
                    trunkNumber = 1;
                    trunkState = 2;
                }
                if( trunk.size()==trunkSize && trunk.at(trunkNumber).compare( sipExtTrunk )==0 )
                {
                    if( trunk.at(trunkState).compare("Registered")==0 )
                    {
                        if( trunkOk == false )
                        {
                            trunkOk = true;
                            ui->stTrunk->setPixmap( pxmTrunkOn );
                            Log( QString( tr("SIP trunk %1 connected")).arg( sipExtTrunk ) );
                        }
                    }
                    else
                    {
                        if( trunkOk == true )
                        {
                            trunkOk = false;
                            ui->stTrunk->setPixmap( pxmTrunkOff );
                            Log( QString( tr("SIP trunk %1 disconnected")).arg( sipExtTrunk ) );
                        }
                    }
                }
                if( pair.at(0).simplified().compare("--END COMMAND--")==0 )
                    state = IDLE;
            }
            break;

        case IDLE:
            pbxOk = true;
            if( pair.at(0).compare("Event")==0 && pair.at(1).compare("DTMFEnd")==0 )
            {
                // Log("DTMFEnd Event");
                state=DTMF;
            }
            break;

        case DTMF:
            pbxOk = true;
            if( pair.at(0).compare("Event")==0 && pair.at(1).compare("DTMFEnd")==0 )
            {
                // Log("DTMFEnd Event");
                state=DTMF;
            }
            if( pair.at(0).compare("Channel")==0 )
            {
                dtmfChannel = pair.at(1);
                // Log( QString("Channel=%1").arg( dtmfChannel ) );
            }
            if( pair.at(0).compare("Digit")==0 )
            {
                // Log( data.at(i) );
                // Log( QString("Channel=%1").arg( dtmfChannel ) );
                dtmfDigit = pair.at(1);
                // Log( QString("Digit=%1").arg( dtmfDigit ) );
                if( dtmfDigit.compare( "*" ) == 0 )
                {
                    QTableWidgetItem *twi = findCall( dtmfChannel );
                    if( twi )
                    {
                        // Log( QString("Reset number for %1").arg(twi->text()) );
                        twi->setData( Qt::UserRole+3, QVariant( 0 ) );
                        ui->listeners->item( twi->row(), 1 )->setText( "" );
                    }
                    state = IDLE;
                }
                else if( dtmfDigit.compare("#") == 0)
                {
                    QTableWidgetItem *twi = findCall( dtmfChannel );
                    if( twi )
                    {
                        dtmfNumber = twi->data(Qt::UserRole+3).toInt();
                        if( dtmfNumber>0 && dtmfNumber<100 )
                        {
                            twi->setData( Qt::UserRole+3, QVariant( dtmfNumber ) );
                            ui->listeners->item( twi->row(), 1 )->setText( QString("%1").arg(dtmfNumber) );
                            // Log( QString("Answer to %1 for party %2").arg(dtmfNumber).arg(twi->text()) );
                        }
                        else
                        {
                            twi->setData( Qt::UserRole+3, QVariant( 0 ) );
                            ui->listeners->item( twi->row(), 1 )->setText( "" );
                            // Log( QString("Clear answer for party %1").arg(twi->text()) );
                        }
                    }
                    state = IDLE;
                }
                else
                {
                    QTableWidgetItem *twi = findCall( dtmfChannel );
                    if( twi )
                    {
                        // Log( QString("Calculate number for party %1").arg(twi->text()) );
                        bool Ok;
                        int digit = dtmfDigit.toInt( &Ok, 10 );
                        if( Ok )
                        {
                            dtmfNumber = twi->data(Qt::UserRole+3).toInt()*10+digit;
                            twi->setData( Qt::UserRole+3, QVariant( dtmfNumber ) );
                            // Log( QString("Digit=%1, Number=%2").arg(digit).arg(dtmfNumber) );
                        }
                    }
                    state = IDLE;
                }
            }
            break;

        default:
            pbxOk = true;
            break;
        }
    }
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if( db.isValid() && db.isOpen() )
    {
        ui->stDB->setPixmap( pxmDbOn );
        QString lineStatus( "offline" );
        int confCount = 0;
        QSqlQuery dbq( db );
        if( dbq.exec( "SELECT status FROM gateways WHERE username='"+sipExtTrunk+"';" ))
            if( dbq.first() )
                lineStatus = dbq.value(0).toString();
        if( dbq.exec( "SELECT Count(oid) FROM call_logs WHERE ended=false AND caller!='"+sipUser+"' AND called='"+sipConf+"';" ))
            if( dbq.first() )
                confCount = dbq.value(0).toInt();
        ui->info->clear();
        if( trunkOk == false && lineStatus=="online" )
        {
            trunkOk = true;
            ui->stTrunk->setPixmap( pxmTrunkOn );
            Log( QString( tr("SIP trunk %1 connected")).arg( sipExtTrunk ) );
        }
        if( trunkOk == true && lineStatus!="online" )
        {
            trunkOk = false;
            ui->stTrunk->setPixmap( pxmTrunkOff );
            Log( QString( tr("SIP trunk %1 disconnected")).arg( sipExtTrunk ) );
        }
        showListenersCount( confCount );
    }
    else
    {
        if( QString( YATE ).compare( pbxType )==0 )
        {
            ui->info->clear();
            ui->info->setText( tr("No database connection") );
            // No info :(
            ui->stDB->setPixmap( pxmDbOff );
            ui->stTrunk->setPixmap( pxmTrunkOff );
        }
    }
    if( inetTimeout++ > INET_TIMEOUT )
    {
        inetTimeout = 0;
        if( !inetRq )
            checkInet();
        else
        {  // Rq still waiting
            if( inetOk )
            { // Inet was ok but now is not
                webSocket->disconnectFromHost();
                ui->stInet->setPixmap( pxmInetOff );
                Log( tr("Internet connection failed") );
                inetOk = false;
                inetRq = false;
            }
            else
            { // May be it is restored now
                checkInet();
            }
        }
        if( !pbxRq )
            checkPBX();
        else
        { // Rq still waiting
            if( pbxOk )
            { // PBX was ok but now is not
                pbxSocket->disconnectFromHost();
                ui->stPBX->setPixmap( pxmPbxOff );
                Log( tr("PBX was switched off") );
                pbxOk = false;
                pbxRq = false;
            }
            else
            { // May be it is restarted now
                checkPBX();
            }
        }
    }
    if( dtmfTimeout > 0 )
    {
        if( dtmfTimeout++ > DTMF_TIMEOUT )
        {
            dtmfTimeout = 0;

            pVoIP->sendDtmf( sipConfPin );
            Log( QString( "DTMF [%1] sent").arg(sipConfPin) );
        }
    }
}

void MainWindow::showListenersCount(int count )
{
    if( count == 0 )
    {
        // No listeners
        mute();
        ui->info->setText( QString( tr("No Listeners") ).arg(count) );
        setWindowTitle( QString( tr("Phone Streamer")) );
        // Clear list
        ui->listeners->setRowCount( 0 );
        // Mute our own sound output
        if( sipState.compare("calling")==0 )
            pVoIP->setLineOutVolume( 0.0f );
    }
    else
    {
        // There are some clients
        if( sipShowCount )
        {
            ui->info->setText( QString( tr("Listens: %1") ).arg(count) );
            setWindowTitle( QString( tr("Listens: %1") ).arg(count) );
        }

/*
        int Items = ui->listeners->rowCount();
        for( int row=0; row<Items; row++ )
        {
            // Iterate through all channels
            QTableWidgetItem *lwi = ui->listeners->item( row,0 );
            lwi->setData( Qt::UserRole, QVariant( false ) );
            lwi->setIcon( confMute );
            QString rq = QString("Action: ConfbridgeMute\nConference: %1\nChannel: %2\n\n")
                    .arg(sipConf)
                    .arg(lwi->data( Qt::UserRole+1 ).toString() );
            pbxSocket->write( rq.toLatin1() );
        }
*/
    }
}

void MainWindow::Log( QString text )
{
    /* Get current date/time. */
    pj_time_val now;
    pj_parsed_time pt;
    pj_gettimeofday(&now);
    pj_time_decode(&now, &pt);

    QString Log = QString("%1.%2.%3 %4:%5:%6 - %7")\
            .arg(pt.day,2,10,QChar('0'))\
            .arg(pt.mon,2,10,QChar('0'))\
            .arg(pt.year,4,10,QChar('0'))\
            .arg(pt.hour,2,10,QChar('0'))\
            .arg(pt.min,2,10,QChar('0'))\
            .arg(pt.sec,2,10,QChar('0'))\
            .arg(text);

    ui->log->append( Log );
}

void MainWindow::registerDB()
{
    db = QSqlDatabase::addDatabase("QPSQL", "Freesentral Database");
    if( db.isValid() )
    {
        db.setHostName( psqlServer );
        db.setDatabaseName( psqlBase );
        db.setUserName( psqlUser );
        db.setPassword( psqlPass );
        if( !db.open() )
            Log( tr("Can't open Database") );
        else
            Log( tr("Database opened") );
    }
    else
        Log( tr("No database driver installed") );
}

void MainWindow::registerSip()
{
    pVoIP = new VoIP(sipProto, sipPort.toInt());

    AudioDevInfoVector adv = pVoIP->EnumAudioDevices();
    for( unsigned int s=0; s<adv.size(); s++ )
    {
        if( adv[s]->inputCount > 0 )
        {
            QString dev = QString::fromLocal8Bit(adv[s]->name.data(), adv[s]->name.size()).simplified();
            if( dev.compare( sipInputDev )==0 )
            {
                if( pVoIP->SetAudioInput( s )==s )
                    Log( tr("Select ") + dev + tr(" as input device") );
                break;
            }
        }
    }
    for( unsigned int s=0; s<adv.size(); s++ )
    {
        if( adv[s]->outputCount > 0 )
        {
            QString dev = QString::fromLocal8Bit(adv[s]->name.data(), adv[s]->name.size()).simplified();
            if( dev.compare( sipOutputDev )==0 )
            {
                if( pVoIP->SetAudioOutput( s )==s )
                    Log( tr("Select ") + dev + tr(" as output device") );
                break;
            }
        }
    }

    sipState = "init";
    connect(pVoIP, SIGNAL(callStateChanged(int, int, int, int, QString)), this, SLOT(on_callState_changed(int, int, int, int, QString)));
    connect(pVoIP, SIGNAL(regStateStarted(bool)), this, SLOT(on_regState_changed( bool )));
    connect(pVoIP, SIGNAL(regStateChanged(bool)), this, SLOT(on_regState_changed( bool )));

    if(pVoIP != nullptr && pVoIP->isLoaded())
    {
        pVoIP->createAccount("sip:"+sipUser+"@"+sipServer, "sip:"+sipServer, sipUser, sipPass);
        Log( tr("Registering account sip:")+sipUser+"@"+sipServer);
    }
    else
    {
        Log( tr("Loading SIP library failed!") );
        error( tr("Loading SIP library failed!") );
    }
}

void MainWindow::on_callButton_clicked()
{
    if(sipState=="calling")
    {
//        pVoIP->holdCall(callId);
//        sipState = "calling";
//        Log( tr("Holding call") );
    }
    else if(sipState=="localRing")
    {
//        pVoIP->acceptCall(callId);
//        sipState = "calling";
//        ringtone.stop();
//        Log( tr("Answering call") );
    }
//    else if(sipState!="localRing" && ui->number->text()!="")
    else if(sipState!="localRing")
    {
        pVoIP->makeCall("sip:"+sipConf+"@"+sipServer);
        Log( tr("Calling sip:")+sipConf+"@"+sipServer);
    }
}

void MainWindow::on_hangButton_clicked()
{
    if( sipState != "" )
    {
        pVoIP->hangupCall(callId);
        sipState = "";
        Log( tr("Hangup") );
    }
}

void MainWindow::on_regState_changed( bool state )
{
    if( !state && sipState == "closing" )
    {
        Log( tr("Closing registraton") );
        sipState = "";
        this->close();
        return;
    }

    if( state )
    {
        Log( QString( tr("SIP extension %1 registered succesfully")).arg(sipUser) );
        ui->callButton->setEnabled( true );
        ui->hangButton->setEnabled( true );
        if( sipState == "" )
            ui->stExt->setPixmap( pxmExtOn );
        if( sipAutoConf && first_call==true )
        {
            first_call==false;
            on_callButton_clicked();
        }
    }
    else
    {
        Log( QString( tr("Errof of registration SIP extension %1")).arg(sipUser) );
        on_hangButton_clicked();
        ui->callButton->setEnabled( false );
        ui->hangButton->setEnabled( false );
        ui->stExt->setPixmap( pxmExtOff );
    }
}

char * sip_state[] = {
    "PJSIP_INV_STATE_NULL",
    "PJSIP_INV_STATE_CALLING",
    "PJSIP_INV_STATE_INCOMING",
    "PJSIP_INV_STATE_EARLY",
    "PJSIP_INV_STATE_CONNECTING",
    "PJSIP_INV_STATE_CONFIRMED",
    "PJSIP_INV_STATE_DISCONNECTED"
};

void MainWindow::on_callState_changed(int role, int callId, int state, int status, QString remoteUri)
{
    if(state == PJSIP_INV_STATE_EARLY && status == 180)
    {
        if(role == 1)
        {
            int j = 0;
            QList<int> namePos;
            QList<int> numPos;

            while ((j = remoteUri.indexOf("\"", j)) != -1) {
                namePos.append(j);
                ++j;
            }

            numPos.append(remoteUri.indexOf("sip:"));
            numPos.append(remoteUri.indexOf("@"));

            QString name = remoteUri.mid(namePos[0]+1, namePos[1]-1);
            QString num = remoteUri.mid(numPos[0]+4, numPos[1]-(numPos[0]+4));
            QString caller = name;

            if(name.size()<=1)
                caller = num;

            Log( tr("Call from ")+caller );
            sipState = "localRing";
            ringtone.play();
        }
        else
        {
            sipState = "remoteRing";
            outgoingRing.play();
        }

        this->callId = callId;
    }
    else if(state == PJSIP_INV_STATE_CONFIRMED && status == 200)
    {
        Log( tr("Call to ") + remoteUri + tr(" established") );
        ui->stCast->setPixmap( pxmCastOn );
        if( sipConfPin.length()>0 )
            dtmfTimeout = 1; // Send Conference PIN
        sipState = "calling";
        ringtone.stop();
        outgoingRing.stop();
    }
    else if(state == PJSIP_INV_STATE_DISCONNECTED)
    {
        Log( tr("Call ended") );
        ui->stCast->setPixmap( pxmCastOff );
        if( sipState == "closing" )
        {
            pVoIP->unregisterAccount();
        }
        else
            sipState = "";
        ringtone.stop();
        outgoingRing.stop();
    }
}


void MainWindow::getScreen()
{
    QRect rec = QApplication::desktop()->screenGeometry();
    screenY = rec.height();
    screenX = rec.width();

    ui->hangButton->setIcon(QPixmap(":/images/hangup.png"));
    ui->callButton->setIcon(QPixmap(":/images/call.png"));
    ui->hangButton->setIconSize(QSize(screenX/15,screenY/15));
    ui->callButton->setIconSize(QSize(screenX/15,screenY/15));
    QFont font = ui->info->font();
    font.setPixelSize( screenY/30 );
    ui->info->setFont( font );

    QFont font_list = ui->info->font();
    font_list.setPixelSize( screenY/50 );
    ui->listeners->setFont( font_list );
    QSize icon;
    icon.setHeight(  screenY/50 );
    icon.setWidth(  screenY/50 );
    ui->listeners->setIconSize( icon );

    pxmCall     = QPixmap(":/images/call.png");
    pxmHangup   = QPixmap(":/images/hangup.png");
    pxmInetOn   = QPixmap(":/images/inet_on.png");
    pxmInetOff  = QPixmap(":/images/inet_off.png");
    pxmPbxOn    = QPixmap(":/images/pbx_on.png");
    pxmPbxOff   = QPixmap(":/images/pbx_off.png");
    pxmTrunkOn  = QPixmap(":/images/trunk_on.png");
    pxmTrunkOff = QPixmap(":/images/trunk_off.png");
    pxmDbOn     = QPixmap(":/images/db_on.png");
    pxmDbOff    = QPixmap(":/images/db_off.png");
    pxmExtOn    = QPixmap(":/images/ext_on.png");
    pxmExtOff   = QPixmap(":/images/ext_off.png");
    pxmCastOn   = QPixmap(":/images/cast_on.png");
    pxmCastOff  = QPixmap(":/images/cast_off.png");
    confUnmute  = QIcon( QPixmap(":/images/unmute.png") );
    confUnmute.actualSize( QSize(screenY/30,screenY/30));
    confMute  = QIcon( QPixmap(":/images/mute.png") );
    confMute.actualSize( QSize(screenY/30,screenY/30));
    confHangup = QPixmap(":/images/hangup.png").scaled( screenY/45, screenY/45, Qt::KeepAspectRatio );
    //confHangup.actualSize( QSize(screenY/30,screenY/30));
    confCall = QPixmap(":/images/call.png").scaled( screenY/45, screenY/45, Qt::KeepAspectRatio );
    //confCall.actualSize( QSize(screenY/30,screenY/30));
}

void MainWindow::getSettings()
{
    QSettings settings( QApplication::applicationDirPath()+"/settings.ini", QSettings::IniFormat);
    settings.beginGroup("SIP");
    sipServer = settings.value("server", "").toString() ;
    sipUser =   settings.value("user", "").toString();
    sipPass =   settings.value("password", "").toString();
    sipProto =  settings.value("protocol", "UDP").toString();
    sipPort =   settings.value("port", "5060").toString();
    sipExtTrunk = settings.value("trunk", "").toString();
    sipConf =   settings.value("conference", "").toString();
    sipConfPin = settings.value("pin", "").toString();
    sipAutoConf = settings.value("autoconf", false).toBool();
    sipShowCount = settings.value("listeners", true).toBool();
    sipComments = settings.value("comments", false).toBool();
    sipMultiComments = settings.value("multicomm", false).toBool();
    sipInputDev =  settings.value("inputdev", "").toString();
    sipOutputDev = settings.value("outputdev", "").toString();
    allowVolume = settings.value("allowvolume", false).toBool();
    svolume = settings.value("svolume", 150 ).toInt();
    avolume = settings.value("avolume", 150 ).toInt();
    settings.endGroup();

    settings.beginGroup("PBX");
    pbxType    = settings.value("type", ASTERISK ).toString();
    psqlServer = settings.value("server", "").toString();
    psqlUser   = settings.value("user", "").toString();
    psqlPass   = settings.value("password", "").toString();
    psqlBase   = settings.value("database", "freesentral").toString();
    pbxChannel = settings.value("channel", "SIP").toString();

    settings.endGroup();

    outgoingRing.setSource(QUrl::fromLocalFile(":/sounds/ring.wav"));
    outgoingRing.setLoopCount(QSoundEffect::Infinite);
    ringtone.setSource(QUrl::fromLocalFile(":/sounds/ring.wav"));
    ringtone.setLoopCount(QSoundEffect::Infinite);


    outgoingRing.setSource(QUrl::fromLocalFile(":/sounds/ring.wav"));
    outgoingRing.setLoopCount(QSoundEffect::Infinite);
    ringtone.setSource(QUrl::fromLocalFile(":/sounds/ring.wav"));
    ringtone.setLoopCount(QSoundEffect::Infinite);
}

void MainWindow::error(QString error)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setText(error);
    msgBox.setWindowTitle("Error");
    msgBox.exec();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if( sipState != "" )
    {
        on_hangButton_clicked();
        sipState = "closing";
        event->ignore();
    }
    else
        event->accept();
}

MainWindow::~MainWindow()
{
    if( webSocket )
    {
        webSocket->disconnectFromHost();
        delete webSocket;
    }
    if( pbxSocket )
    {
        pbxSocket->disconnectFromHost();
        delete pbxSocket;
    }
    if( timerId )
        killTimer( timerId );
    delete ui;
}

void MainWindow::on_logButton_clicked(bool checked)
{
    QRect g = this->geometry();
    if( checked )
    {
        ui->log->hide();
        ui->listeners->hide();
        ui->logButton->setChecked( true );
        ui->listenersButton->setChecked( true );
        ui->AVolume->hide();
        ui->lblAVolume->hide();
        ui->lblAVolumeVal->hide();
        this->setGeometry(g.left(),g.top(),width(),0);
        this->resize( width(),0 );
    }
    else
    {
        if( ui->listeners->isHidden() )
            this->setGeometry(g.left(),g.top(),width(),height()+200);
        ui->listeners->hide();
        ui->AVolume->hide();
        ui->lblAVolume->hide();
        ui->lblAVolumeVal->hide();
        ui->listenersButton->setChecked( true );
        ui->log->show();
    }
}

void MainWindow::on_listenersButton_clicked(bool checked)
{
    QRect g = this->geometry();
    if( checked )
    {
        ui->log->hide();
        ui->listeners->hide();
        ui->logButton->setChecked( true );
        ui->listenersButton->setChecked( true );
        ui->AVolume->hide();
        ui->lblAVolume->hide();
        ui->lblAVolumeVal->hide();
        this->setGeometry(g.left(),g.top(),width(),0);
        this->resize( width(),0 );
    }
    else
    {
        if( ui->log->isHidden() )
            this->setGeometry(g.left(),g.top(),width(),height()+200);
        ui->log->hide();
        ui->logButton->setChecked( true );
        ui->listeners->show();
        ui->AVolume->show();
        ui->lblAVolume->show();
        ui->lblAVolumeVal->show();
    }
}

void MainWindow::mute()
{
    setAVolume( 0.0f );
}

void MainWindow::unmute()
{
    setAVolume( (float)avolume/100 );
}

void MainWindow::setSVolume( float value )
{
    if(sipState=="calling")
        pVoIP->setSIPOutVolume( value );
}

void MainWindow::setAVolume( float value )
{
    if(sipState=="calling")
        pVoIP->setLineOutVolume( value );
}

void MainWindow::checkMuting()
{
    unmuteFlag = false;
    // Should we mute all others? Do it.
    int Items = ui->listeners->rowCount();
    for( int r=0; r<Items; r++ )
    {
        // Iterate through all channels
        QTableWidgetItem *twi = ui->listeners->item( r,0 );
        if( twi->data(Qt::UserRole).toBool() == true ) // unmuted
        {
            unmuteFlag=true;
            break;
        }
    }
    if( unmuteFlag )
        unmute();
    else
        mute();
}

void MainWindow::markAllChannels( bool value )
{
    int Items = ui->listeners->rowCount();
    for( int r=0; r<Items; r++ )
        ui->listeners->item( r,0 )->setData(Qt::UserRole+4, QVariant( value ) );
}

void MainWindow::removeUnusedChannels()
{
    int Items = ui->listeners->rowCount();
    for( int r=0; r<Items; r++ )
    {
        if( ui->listeners->item( r,0 )->data(Qt::UserRole+4 ).toBool() == false )
        {
            ui->listeners->removeRow( r );
            Items--;
        }
    }
}

QTableWidgetItem * MainWindow::findCall( QString SIPChannel )
{
    QTableWidgetItem *twi = NULL;

    int Items = ui->listeners->rowCount();
    for( int r=0; r<Items; r++ )
    {
        // Iterate through all channels
        QTableWidgetItem *twr = ui->listeners->item( r,0 );
        if( twr->data(Qt::UserRole+1).toString().compare( SIPChannel ) == 0 ) // found
        {
            twi = twr;
            break;
        }
    }
    return twi;
}

void MainWindow::on_listeners_cellClicked(int row, int column)
{
    // item( row, 0 )
    // Qt::UserRole (bool): false=mute, true=unmute
    // Qt::UserRole+1 (QString): SIP channel
    // Qt::UserRole+2 (bool): false=call, true=hangup
    // Qt::UserRole+3 (int): Paragraph number

    QTableWidgetItem *twi0 = ui->listeners->item( row,0 );
    QTableWidgetItem *twi1 = ui->listeners->item( row,1 );
    //QTableWidgetItem *twi2 = ui->listeners->item( row,2 );
    QLabel *lbl = (QLabel *)( ui->listeners->cellWidget( row, 2));

    if( sipComments && column==1 && twi0->data(Qt::UserRole+2).toBool()==false )
    {
        // Follow this logic if comments are enabled only
        if( twi0->data(Qt::UserRole).toBool() == true )
        {
            // Clicked item is unmuted now, let's mute it
            twi0->setData( Qt::UserRole, QVariant( false ) );
            twi1->setIcon( confMute );
            QString rq = QString("Action: ConfbridgeMute\nConference: %1\nChannel: %2\n\n")
                    .arg(sipConf)
                    .arg(twi0->data( Qt::UserRole+1 ).toString() );
            pbxSocket->write( rq.toLatin1() );
            ui->listeners->item( row,1 )->setText( QString("") );
        }
        else
        {
            // Clicked item is muted - we need to allow it
            if( !sipMultiComments )
            {
                // Should we mute all others? Do it.
                int Items = ui->listeners->rowCount();
                for( int r=0; r<Items; r++ )
                {
                    // Iterate through all channels
                    QTableWidgetItem *lwi = ui->listeners->item( r,0 );
                    lwi->setData( Qt::UserRole, QVariant( false ) );
                    ui->listeners->item( r,1 )->setIcon( confMute );
                    QString rq = QString("Action: ConfbridgeMute\nConference: %1\nChannel: %2\n\n")
                            .arg(sipConf)
                            .arg(lwi->data( Qt::UserRole+1 ).toString() );
                    pbxSocket->write( rq.toLatin1() );
                }
            }
            // Unmute clicked item
            twi0->setData( Qt::UserRole, QVariant( true ) );
            twi1->setIcon( confUnmute );
            QString rq = QString("Action: ConfbridgeUnmute\nConference: %1\nChannel: %2\n\n")
                    .arg(sipConf)
                    .arg(twi0->data( Qt::UserRole+1 ).toString() );
            pbxSocket->write( rq.toLatin1() );
        }
    }
    if( column==2 )
    {
        twi0->setData( Qt::UserRole, QVariant( false ) );  // Mute
        twi0->setData( Qt::UserRole+2, QVariant( true ) ); // Hangup
        twi1->setIcon( confMute );
        twi1->setText( "" );

        // twi2->setIcon( confHangup );
        lbl->setPixmap( confHangup );

        QString rq = QString("Action: ConfbridgeKick\nConference: %1\nChannel: %2\n\n")
                .arg(sipConf)
                .arg(twi0->data( Qt::UserRole+1 ).toString() );
        pbxSocket->write( rq.toLatin1() );
    }
    checkMuting();
}
