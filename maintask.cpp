#include "maintask.h"
#include "frigoserver.h"

#include <QDebug>

MainTask::MainTask(QObject *parent) : QObject(parent)
{

}

MainTask::~MainTask()
{

}

void MainTask::run()
{
    new FrigoServer(this);
}

