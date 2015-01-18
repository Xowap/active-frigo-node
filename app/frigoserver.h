#ifndef FRIGOSERVER_H
#define FRIGOSERVER_H

#include <QObject>
#include <QUdpSocket>
#include <QVariant>
#include <QMediaPlayer>
#include <QJsonObject>

#include "frigotunnel.h"

class FrigoServer : public QObject
{
    Q_OBJECT
public:
    explicit FrigoServer(QObject *parent = 0);
    ~FrigoServer();

signals:

public slots:
    void handleMessage(const QJsonObject &message);
    void playSound(QString key, int volume);

private:
    FrigoTunnel *tunnel;
    QMediaPlayer player;
};

#endif // FRIGOSERVER_H