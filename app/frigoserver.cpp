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
#include <QFile>

FrigoServer::FrigoServer(QObject *parent) :
    QObject(parent),
    tunnel(new FrigoTunnel(Config::getInstance().getName(), this)),
    globalVolume(100),
    trackVolume(100)
{
    connect(tunnel, &FrigoTunnel::gotMessage, this, &FrigoServer::handleMessage);
    preloadSounds();
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
    } else if (type == "set-volume") {
        if (message["volume"].isDouble()) {
            globalVolume = message["volume"].toDouble();
            updateVolume();
        }
    }
}

void FrigoServer::playSound(QString key, int volume)
{
    QString sound = Config::getInstance().getSound(key);

    if (sound.isEmpty()) {
        return;
    }

    player.setMedia(QUrl::fromLocalFile(sound));

    trackVolume = volume;
    updateVolume();

    player.play();

    qDebug() << "Playing sound " << key;
}

void FrigoServer::updateVolume()
{
    int newVolume = (trackVolume * globalVolume) / 100;
    qDebug() << "Setting volume" << newVolume;
    player.setVolume(newVolume);
}

void FrigoServer::preloadSounds()
{
    QMap<QString, QString> sounds = Config::getInstance().getSounds();

    for (auto key : sounds.keys()) {
        QFile f(sounds[key]);

        if (f.open(QFile::ReadOnly)) {
            f.readAll();
            qDebug() << "Preloaded" << sounds[key];
        }
    }
}

