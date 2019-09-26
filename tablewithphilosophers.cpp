#include "tablewithphilosophers.h"
#include <iostream>


TableWithPhilosophers::TableWithPhilosophers(QObject *parent): QObject(parent)
{
    shadow_ = new Shadow();
    addRemove_ = new QMutex();
    waiter_ = new QSemaphore(4);
    mainIndex_ =1;
    QString name = philosopherName;
    QMutex* fLeft ;
    QMutex* fright= new QMutex();
    uint numberOfPhil =5;
    for (uint i{0};i<numberOfPhil;i++) {
        fLeft = fright;
        if (i == (numberOfPhil-1))
            fright = listOfPhilosophers_[0]->requestForFork(TypeFork::LEFT);
        else
            fright = new QMutex();
        QMutex* oneThink = new QMutex();
        PhilosopherThread*  phil= new PhilosopherThread(name, fLeft, fright,waiter_,mainIndex_++, oneThink);
        connect(phil,&PhilosopherThread::updateState,this, &TableWithPhilosophers::changedState, Qt::DirectConnection);
        listOfPhilosophers_.append(phil);
        phil->start();

    }
    connect(shadow_,&Shadow::sAppendPhilosopher, this, &TableWithPhilosophers::appendPhilosopher, Qt::QueuedConnection);
    connect(shadow_, &Shadow::sRemovePhilosopher, this, &TableWithPhilosophers::removePhilosopher,Qt::QueuedConnection);
    shadow_->start();
}


QVector<PhilosopherThread *> TableWithPhilosophers::items() const
{
    return  listOfPhilosophers_;
}

void TableWithPhilosophers::changedState(const uint id)
{
    emit updateState(id);
}

void TableWithPhilosophers::appendPhilosopher(const QString &name)
{   QMutex* forkRight;
    QMutex* forkLeft;
    addRemove_->lock();
    if (listOfPhilosophers_.size() == 0) {
        forkRight = new QMutex();
        forkLeft = new QMutex();
    }
    else if (listOfPhilosophers_.size() == 1){
        forkRight = listOfPhilosophers_.last()->requestForFork(TypeFork::LEFT);
        forkLeft = listOfPhilosophers_.last()->requestForFork(TypeFork::RIGHT);
    }
    else {
        forkRight = listOfPhilosophers_.last()->requestForFork(TypeFork::RIGHT);
        forkLeft = new QMutex();
        listOfPhilosophers_.last()->setFork(forkLeft, TypeFork::RIGHT);
    }
    QMutex* oneThink = new QMutex();
    PhilosopherThread* phil= new PhilosopherThread(name,forkLeft,forkRight,   waiter_, mainIndex_++, oneThink);
    int index = items().size();
    if (index==-1) index =0;
    emit prePhilAppend(index);
    listOfPhilosophers_.append(phil);
    emit postPhilAppend();
    connect(listOfPhilosophers_.last(),&PhilosopherThread::updateState,this, &TableWithPhilosophers::changedState, Qt::DirectConnection);
    waiter_->release();
    phil->start();
    addRemove_->unlock();
}

void TableWithPhilosophers::addPhil(const QString &name)
{
    shadow_->addPhilosopher(name);
}

void TableWithPhilosophers::removePhilosopher(const uint id)
{
    addRemove_->lock();
    int index = getIndex(id);
    if (index != -1){
        PhilosopherThread* temp =listOfPhilosophers_.at(index);
        temp->stop();
        disconnect(listOfPhilosophers_.at(index),&PhilosopherThread::updateState,this, &TableWithPhilosophers::changedState);
        if (listOfPhilosophers_.size() > 2) {
            int nextindex = (index +1) % listOfPhilosophers_.size();
            listOfPhilosophers_.at(index)->oneThingLock();
            listOfPhilosophers_.at(nextindex)->setFork(listOfPhilosophers_.at(index)->requestForFork(TypeFork::LEFT),TypeFork::LEFT);
            listOfPhilosophers_.at(index)->oneThingUnlock();
            waiter_->acquire();
        }

        emit prePhilRemoved(index);
        listOfPhilosophers_.removeAt(index);
        emit postPhilRemoved();
    }
    addRemove_->unlock();
}

void TableWithPhilosophers::removePhil(const int index)
{

    if ((listOfPhilosophers_.size()!=0) || (index !=-1)) {
        uint id = listOfPhilosophers_[index]->id();
        shadow_->removePhilosopher(id);
    }
}

int TableWithPhilosophers::getRemovedIndex() const
{
    QVector<PhilosopherThread*> temp = listOfPhilosophers_;
    int index = temp.size()-1;
    if (index != -1){
        uint id = temp[index]->id();
        while ((shadow_->isInListHistory(id)) && (index > 0)) {
            index--;
            id = temp[index]->id();
        }
    }
    return index;
}

int TableWithPhilosophers::getSize() const
{
    return  listOfPhilosophers_.size();
}

int TableWithPhilosophers::getIndex(const uint id) const
{
    int i;
    bool found = false;
    QVector<PhilosopherThread*> temp = listOfPhilosophers_;
    for(i=0;i<temp.size();i++)
        if (temp[i]->id()== id){
            found = true;
            break;
        }
    if (found)
        return i;
    else {
        return -1;
    }
}

uint TableWithPhilosophers::getId(const int index) const
{
    PhilosopherThread* temp =listOfPhilosophers_.at(index);
    if ( temp )
        return  temp->id();
    else {
        return static_cast<uint>(-1);
    }

}
