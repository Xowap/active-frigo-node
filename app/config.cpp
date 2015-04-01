#include "config.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QFile>
#include <QFileInfo>
#include <QDir>

Config::Config(QObject *parent) :
    QObject(parent),
    valid(false)
{
}

Config::~Config()
{
}

bool Config::isValid()
{
    return valid;
}

QString Config::getSound(const QString &key) const
{
    return sounds[key];
}

QMap<QString, QString> Config::getSounds() const
{
    return sounds;
}

QString Config::getName() const
{
    return document.object()["name"].toString();
}

void Config::load(QString filename)
{
    QFile f(filename);

    if (!f.open(QFile::ReadOnly)) {
        return;
    }

    QByteArray data = f.readAll();
    document = QJsonDocument::fromJson(data);

    if (!document.isObject()) {
        return;
    }

    if (!document.object()["name"].isString() || !document.object()["sounds"].isObject()) {
        return;
    }

    QJsonObject soundsConf = document.object()["sounds"].toObject();
    QFileInfo info(filename);
    QDir baseDir = info.absoluteDir();

    valid = true;

    for (QJsonObject::iterator i = soundsConf.begin(); i != soundsConf.end(); i++) {
        if (!i.value().isString()) {
            continue;
        }

        sounds[i.key()] = baseDir.filePath(i.value().toString());
    }
}

