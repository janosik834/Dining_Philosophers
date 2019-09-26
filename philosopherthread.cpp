#include "philosopherthread.h"

QString philosopherName = QStringLiteral("Phiosopher ");
#define time_eating 500
#define time_thinking 300

PhilosopherThread::PhilosopherThread(QThread *parent) : QThread(parent)
{
    id_ = 0;
    name_ = philosopherName;
    state_ = TypeState::THINKING;
    live_ = true;
}

PhilosopherThread::PhilosopherThread(QString name, QMutex *fork_left, QMutex *fork_right, QSemaphore *waiter, uint id, QMutex* oneThink)
{
    oneThing_ = oneThink;
    id_=id;
    name_ = name+QString( std::to_string(id).c_str());
    state_ = TypeState::THINKING;
    fork_left_ = fork_left;
    fork_right_ = fork_right;
    waiter_ = waiter;
    live_ = true;
}

void PhilosopherThread::run()
{
    while (live_) {
        eat();
        msleep(time_thinking);
    }
}

void PhilosopherThread::setFork(QMutex* fork, TypeFork type)
{   oneThing_->lock();
    if( type == TypeFork::LEFT ){
        QMutex* tempfork = fork_left_;
        waiter_->acquire();
        tempfork->lock();
        fork->lock();
        fork_left_ = fork;
        fork->unlock();
        tempfork->unlock();
        waiter_->release();
    }
    else {
        QMutex* tempfork = fork_right_;
        waiter_->acquire();
        tempfork->lock();
        fork->lock();
        fork_right_ = fork;
        fork->unlock();
        tempfork->unlock();
        waiter_->release();
    }
    oneThing_->unlock();


}

TypeState PhilosopherThread::state() const
{
    return state_;
}

void PhilosopherThread::setState(const TypeState &state)
{
    state_ = state;
}

QString PhilosopherThread::name() const
{
    return name_;
}

void PhilosopherThread::setName(const QString &name)
{
    name_ = name;
}

uint PhilosopherThread::id() const
{
    return id_;
}

void PhilosopherThread::setId(const uint &id)
{
    id_ = id;
}

QMutex* PhilosopherThread::requestForFork(const TypeFork fork) const
{
    if (fork == TypeFork::LEFT){
        return  fork_left_;}
    else {
        return fork_right_;
    }
}

void PhilosopherThread::stop()
{
    live_ = false;
}

void PhilosopherThread::eat()
{
    oneThing_->lock();
    waiter_->acquire();
    fork_left_->lock();
    fork_right_->lock();
    changeState(TypeState::EATING);
    msleep(time_eating);
    changeState(TypeState::THINKING);
    fork_right_->unlock();
    fork_left_->unlock();
    waiter_->release();
    oneThing_->unlock();
}

void PhilosopherThread::changeState(TypeState newState)
{
    state_ = newState;
    emit updateState(id_);
}

