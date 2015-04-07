#ifndef FRIGOSERVER_H
#define FRIGOSERVER_H

#include <QObject>
#include <QUdpSocket>
#include <QVariant>
#include <QJsonObject>
#include <QSoundEffect>
#include <QString>
#include <QMap>

#include "frigotunnel.h"

typedef QMap<QString, QSoundEffect *> SoundMap;

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
    void stopAll();
    void updateVolume();
    void preloadSounds();

private:
    qreal volumef();

    FrigoTunnel *tunnel;
    SoundMap sounds;

    int globalVolume, trackVolume;
};

#endif // FRIGOSERVER_H
