#include "frigoserver.h"
#include "config.h"

#include <QHostAddress>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>
#include <QBuffer>

FrigoServer::FrigoServer(QObject *parent) : QObject(parent)
{
    socket.bind(QHostAddress::LocalHostIPv6, 42000);
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
            playSound(data["name"].toString());
        }
    }
}

void FrigoServer::playSound(QString key)
{
    QString sound = Config::getInstance().getSound(key);

    if (sound.isEmpty()) {
        return;
    }

    player.setMedia(QUrl::fromLocalFile(sound));
    player.play();

    qDebug() << "Playing sound " << key;
}

