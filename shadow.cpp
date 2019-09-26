#include "shadow.h"
#include <QCoreApplication>
Shadow::Shadow(QThread *parent): QThread(parent)
{
}

void Shadow::addPhilosopher(const QString& name)
{
    listOfTasks_.push_back(pair(TypeOfChanged::ADD, name));
    QEvent* event = new QEvent(QEvent::ActionChanged);
    QCoreApplication::sendEvent(this, event);
}

void Shadow::removePhilosopher(const uint id)
{
    std::string name = std::to_string(id);
    listOfTasks_.push_back(pair(TypeOfChanged::REMOVE,QString(name.c_str())));
    QEvent* event = new QEvent(QEvent::ActionChanged);
    QCoreApplication::sendEvent(this, event);
}

bool Shadow::isInListHistory( const uint id) const
{
    for (const pair& item: listOfTasksHistory_) {
        if( item.value_.toUInt() == id){
            return  true;
        }
    }
    return  false;
}

bool Shadow::event(QEvent *ev)
{
    if (ev->type() == QEvent::ActionChanged){
        if(listOfTasks_.size()>0){
            pair task = listOfTasks_.front();
            switch (task.genus_) {
            case TypeOfChanged::ADD : emit sAppendPhilosopher(task.value_);

                break;
            case TypeOfChanged::REMOVE:
                emit sRemovePhilosopher(task.value_.toUInt());
                this->listOfTasksHistory_.push_back(task);
                break;
            }
            listOfTasks_.pop_front();
            return  true;
        }
    }
    return QThread::event(ev);
}
