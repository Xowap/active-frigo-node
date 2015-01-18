#ifndef FRIGOSERVER_H
#define FRIGOSERVER_H

#include <QObject>
#include <QUdpSocket>
#include <QVariant>
#include <QMediaPlayer>

class FrigoServer : public QObject
{
    Q_OBJECT
public:
    explicit FrigoServer(QObject *parent = 0);
    ~FrigoServer();

signals:

public slots:
    void receivedDatagram();
    void dispatchMessage(QJsonDocument *message);
    void playSound(QString key, int volume);

private:
    QUdpSocket socket;
    QMediaPlayer player;
};

#endif // FRIGOSERVER_H
