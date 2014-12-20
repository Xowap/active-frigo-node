#ifndef MAINTASK_H
#define MAINTASK_H

#include <QObject>

class MainTask : public QObject
{
    Q_OBJECT
public:
    explicit MainTask(QObject *parent = 0);
    ~MainTask();

signals:

public slots:
    void run();
};

#endif // MAINTASK_H
