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
    tunnel(new FrigoTunnel(Config::getInstance().getName(), QStringList() << "/dev/ttyUSB0" << "/dev/ttyUSB1", this)),
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
    if (!sounds.contains(key)) {
        return;
    }

    stopAll();

    trackVolume = volume;
    updateVolume();

    sounds[key]->play();

    qDebug() << "Playing sound " << key;
}

void FrigoServer::stopAll()
{
    for (SoundMap::iterator i = sounds.begin(); i != sounds.end(); i++) {
        i.value()->stop();
    }
}

void FrigoServer::updateVolume()
{
    qreal newVolume = volumef();
    qDebug() << "Setting volume" << newVolume;

    for (SoundMap::iterator i = sounds.begin(); i != sounds.end(); i++) {
        i.value()->setVolume(newVolume);
    }
}

void FrigoServer::preloadSounds()
{
    QMap<QString, QString> sounds = Config::getInstance().getSounds();

    for (auto key : sounds.keys()) {
        auto sound = new QSoundEffect(this);
        sound->setSource(QUrl::fromLocalFile(sounds[key]));
        sound->setLoopCount(1);
        sound->setVolume(volumef());

        this->sounds[key] = sound;
    }
}

qreal FrigoServer::volumef()
{
    return (qreal) (trackVolume * globalVolume) / 10000.0f;
}

