#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QJsonDocument>
#include <QMap>
#include <QByteArray>

class Config : public QObject
{
    Q_OBJECT
public:
    ~Config();

    static Config& getInstance() {
        static Config instance;
        return instance;
    }

    bool isValid();

    const QString &getSound(const QString &key);

signals:

public slots:
    void load(QString filename);

private:
    explicit Config(QObject *parent = 0);
    explicit Config(Config const&); // Don't Implement
    void operator=(Config const&); // Don't implement

    bool valid;
    QJsonDocument document;
    QMap<QString, QString> sounds;
};

#endif // CONFIG_H
