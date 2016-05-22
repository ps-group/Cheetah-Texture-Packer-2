#include <QtGlobal>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#   include <QtWidgets/QApplication>
#else
#   include <QtGui/QApplication>
#endif

#include "mainwindow.h"
#include <QTranslator>
#include <QLocale>
#include "commandlinehandler.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if(argc > 1)
    {
        // run command-line version
        return doCommandLineJobs(a.arguments());
    }

    QTranslator myTranslator;
    myTranslator.load("tile_" + QLocale::system().name(), "qm");
    a.installTranslator(&myTranslator);
    MainWindow w;
    w.show();


    return a.exec();
}
