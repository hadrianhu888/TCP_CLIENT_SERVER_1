#include "mainwindow.h"
//#include "mainwindow.cpp"
#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[]);//function prototype

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "TCP_CLIENT_SERVER_1_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.show();
    return a.exec();
}
