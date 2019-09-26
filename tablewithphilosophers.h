#ifndef TABLEWITHPHILOSOPHERS_H
#define TABLEWITHPHILOSOPHERS_H

#include <QObject>
#include <QVector>
#include <vector>
#include <QThread>
#include "philosopherthread.h"
#include "shadow.h"

extern  QString philosopherName;
/*!
 * \brief The TableWithPhilosophers class - contains list of philosophers sitting around the table.
 */
class TableWithPhilosophers: public QObject
{
    Q_OBJECT
public:
    explicit TableWithPhilosophers(QObject *parent = nullptr);
    QVector<PhilosopherThread *> items() const;

signals:
    /*!
     * \brief prePhilAppend - signal helped to manage the data model
     * \param index- philosopher after whom will be added new philosopher
     */
    void prePhilAppend(int index);
    /*!
     * \brief postPhilAppend - signal helped to manage the data model
     */
    void postPhilAppend();
    /*!
     * \brief prePhilRemoved - signal helped to manage the data model
     * \param index - philosopher who will be removed
     */
    void prePhilRemoved(int index);
    /*!
     * \brief postPhilRemoved - signal helped to manage the data model
     */
    void postPhilRemoved();
    /*!
     * \brief updateState - signal to update the GUI
     * \param id - ID of philosphers which should be updated
     */
    void updateState(const uint id);
public slots:
    /*!
     * \brief changedState - slot using to communicate betwen Philosopher and GUI
     * \param id - Philosopher ID
     */
    void changedState(uint id);

    void appendPhilosopher(const QString& name = philosopherName );
    /*!
     * \brief addPhil - method using to communicate with QUI
     * \param name - name of the philosopher being added
     */
    void addPhil(const QString& name = philosopherName);

    void removePhilosopher(const uint id);

    /*!
     * \brief removePhil - method using to communicate with QUI
     * \param index - index of the philosopher being removed
     */
    void removePhil(const int index);
    /*!
     * \brief getSize - method that returns the current number of philosophers at the table
     * \return
     */
    int getSize() const;

    int getIndex(const uint id) const;
    uint getId(const int index) const;
    int getRemovedIndex() const;

private:
    QVector<PhilosopherThread*> listOfPhilosophers_;
    QSemaphore* waiter_;
    Shadow* shadow_;
    QMutex* addRemove_;
    uint mainIndex_;

    
};

#endif // TABLEWITHPHILOSOPHERS_H
