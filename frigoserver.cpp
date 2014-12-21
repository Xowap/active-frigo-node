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

FrigoServer::FrigoServer(QObject *parent) : QObject(parent)
{
    QHostAddress bindAddress("225.42.42.42");
    socket.bind(QHostAddress::AnyIPv4, 42000);
    socket.joinMulticastGroup(bindAddress);

    connect(&socket, SIGNAL(readyRead()), this, SLOT(receivedDatagram()));
}

FrigoServer::~FrigoServer()
{

}

void FrigoServer::receivedDatagram()
{
    while (socket.hasPendingDatagrams()) {
        QByteArray datagram;
        QHostAddress sender;
        quint16 senderPort;

        datagram.resize(socket.pendingDatagramSize());
        socket.readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        QJsonParseError error;
        QJsonDocument message = QJsonDocument::fromJson(datagram, &error);

        if (error.error == QJsonParseError::NoError) {
            dispatchMessage(&message);
        }
    }
}

void FrigoServer::dispatchMessage(QJsonDocument *message)
{
    if (message->isObject()) {
        QJsonObject data = message->object();
        QString type = data["type"].toString();

        if (type == "play-sound") {
            QString name = data["name"].toString();
            int volume;

            if (data["volume"].isDouble()) {
                volume = data["volume"].toDouble();
            } else {
                volume = 100;
            }

            playSound(name, volume);
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
    player.setVolume(volume);
    player.play();

    qDebug() << "Playing sound " << key;
}

