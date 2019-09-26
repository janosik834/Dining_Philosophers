#ifndef PHILOSOPHERTHREAD_H
#define PHILOSOPHERTHREAD_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QSemaphore>
extern  QString philosopherName ;

enum TypeState{THINKING, EATING};

enum TypeFork{ LEFT, RIGHT};


/*!
 * \brief The PhilosopherThread class is behaving as real philosopher sitting around the table. This class inherits from QThread and it is a thread.
 */
class PhilosopherThread : public QThread
{
    Q_OBJECT
public:
    explicit PhilosopherThread(QThread *parent = nullptr);
    PhilosopherThread(QString name, QMutex* fork_left, QMutex* fork_right, QSemaphore *waiter, uint id, QMutex *oneThink);
    void run() override;
    /*!
     * \brief requestForFork it is a function that returns the indicator of the selected fork
     * \param fork - the selected fork
     * \return
     */
    QMutex* requestForFork(const TypeFork fork) const;
    void oneThingLock(){oneThing_->lock();}
    void oneThingUnlock() {oneThing_->unlock();}
    void stop();
signals:
    void updateState(uint id);


public slots:
    void setFork(QMutex *fork, TypeFork type);
    private:
    /*!
     * \brief oneThing_  - an attribute that ensures that the class cannot simultaneously add or remove a philosopher.
     */
    QMutex* oneThing_;
    uint id_;
    QMutex* fork_left_;
    QMutex* fork_right_;
    QSemaphore* waiter_;
    QString name_;
    TypeState state_;
    bool live_;
public:

    void eat();
    void changeState(TypeState newState);

    uint id() const;
    void setId(const uint &id);
    QString name() const;
    void setName(const QString &name);
    TypeState state() const;
    void setState(const TypeState &state);
};

#endif // PHILOSOPHERTHREAD_H
