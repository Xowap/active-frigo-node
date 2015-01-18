#include "frigoserver.h"
#include "config.h"

#include <QHostAddress>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>
#include <QBuffer>
#include <QNetworkInterface>
#include <QList>

FrigoServer::FrigoServer(QObject *parent) :
    QObject(parent),
    tunnel(new FrigoTunnel(Config::getInstance().getName(), this))
{
    connect(tunnel, &FrigoTunnel::gotMessage, this, &FrigoServer::handleMessage);
}

FrigoServer::~FrigoServer()
{
}

void FrigoServer::handleMessage(const QJsonObject &message)
{
    QString type = message["type"].toString();

    if (type == "play-sound") {
        QString name = message["name"].toString();
        int volume;

        if (message["volume"].isDouble()) {
            volume = message["volume"].toDouble();
        } else {
            volume = 100;
        }

        playSound(name, volume);
    }
}

void FrigoServer::playSound(QString key, int volume)
{
    QString sound = Config::getInstance().getSound(key);

    if (sound.isEmpty()) {
        return;
    }

    player.setMedia(QUrl::fromLocalFile(sound));
    player.setVolume(volume);
    player.play();

    qDebug() << "Playing sound " << key;
}

