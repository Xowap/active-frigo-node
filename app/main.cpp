#include <QCoreApplication>
#include <QTimer>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>

#include "maintask.h"
#include "config.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCoreApplication::setApplicationName("active-frigo-node");
    QCoreApplication::setApplicationVersion("0.1");

    QCommandLineParser parser;
    parser.setApplicationDescription("Provides a server that plays sounds triggered remotely");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption configFile(QStringList() << "c" << "config",
                                  QCoreApplication::translate("main", "Read this configuration <file>"),
                                  QCoreApplication::translate("main", "file"));
    parser.addOption(configFile);

    parser.process(a);

    if (!parser.isSet(configFile)) {
        qDebug() << "No configuration file specified";
        return 1;
    }

    Config::getInstance().load(parser.value(configFile));

    if (!Config::getInstance().isValid()) {
        qDebug() << "Config file is invalid";
        return 1;
    }

    MainTask *t = new MainTask(&a);
    QTimer::singleShot(0, t, SLOT(run()));

    return a.exec();
}
