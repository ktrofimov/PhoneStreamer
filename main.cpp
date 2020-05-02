#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QTranslator>
#include "runguard.h"

int main(int argc, char *argv[])
{
    QFile styleF;
    QApplication a(argc, argv);

    RunGuard guard( "some_random_key" );
    if ( !guard.tryToRun() )
        return 0;

    styleF.setFileName(":/qdarkstyle/style.qss");
    if( styleF.open(QFile::ReadOnly) )
    {
        QString qssStr = styleF.readAll();
        a.setStyleSheet( qssStr );
    }

    QString locale = QLocale::system().name();
    locale.truncate( locale.lastIndexOf('_'));

    QTranslator psTranslator;
    if( psTranslator.load( "ps_"+locale, a.applicationDirPath()+"/translations") )
        a.installTranslator(&psTranslator);

    QTranslator qtTranslator;
    if( qtTranslator.load( "qt_"+locale, a.applicationDirPath()+"/translations") )
        a.installTranslator(&qtTranslator);

    MainWindow w;
    w.show();

    return a.exec();
}
