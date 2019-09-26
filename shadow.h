#ifndef SHADOW_H
#define SHADOW_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QEvent>
extern  QString philosopherName;
enum TypeOfChanged {ADD, REMOVE};
/*!
 * \briefThe pair struct - help to manage the add/remove philosophers.
 */
struct pair
{   TypeOfChanged genus_;
    QString value_;
    pair(TypeOfChanged genus, QString value) {
        genus_=genus;
        value_=value;}
};

/*!
 * \brief The Shadow class contains a list of modifications to be made in the listOfPhilosophers and manages them.
 */
class Shadow : public QThread
{
    Q_OBJECT
public:
    explicit  Shadow(QThread *parent = nullptr);

    void addPhilosopher(const QString &name = philosopherName );
    void removePhilosopher(const uint id);

    bool isInListHistory(const uint id) const;
    bool event(QEvent* ev) override;
signals:
    void sAppendPhilosopher(const QString& name = philosopherName );
    void sRemovePhilosopher(const uint id);

private:
    std::list<pair> listOfTasks_;
    std::list<pair> listOfTasksHistory_;
};

#endif // SHADOW_H
