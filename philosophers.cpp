#include "philosophers.h"
#include "tablewithphilosophers.h"
PhilosophersModel::PhilosophersModel(QObject *parent)
    : QAbstractListModel(parent), table_(nullptr)
{
}



int PhilosophersModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return  table_->items().size();

}

QVariant PhilosophersModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant(QStringLiteral(""));
    PhilosopherThread* phil = table_->items().at(index.row());
    switch (role) {
    case philosopherName :
        return QVariant(phil->name());
    case philosopherState:
        switch (phil->state()) {
        case TypeState::THINKING:
            return QVariant(QStringLiteral("Thinking"));
        case TypeState::EATING:
            return  QVariant(QStringLiteral("Eating"));
        }
    }
    return QVariant(QStringLiteral(""));
}

bool PhilosophersModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!table_)
        return  false;
    PhilosopherThread* phil = table_->items().at(index.row());
    switch (role) {
    case philosopherName :
        phil->setName(value.toString());
        break;
    case philosopherState:
        phil->setState(static_cast<TypeState>(value.toInt()));
        break;
    }
    if (data(index, role) != value) {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags PhilosophersModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> PhilosophersModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[philosopherName] = "name";
    names[philosopherState] = "current_state";
    return names;

}

TableWithPhilosophers *PhilosophersModel::getTable() const
{
    return table_;
}

void PhilosophersModel::setTable(TableWithPhilosophers *value)
{   beginResetModel();
    if (table_)
        table_->disconnect(this);
    table_ = value;
    if (table_){
        connect(table_, &TableWithPhilosophers::prePhilAppend, this, [this](int index){
            beginInsertRows(QModelIndex(), index, index);}
        );
        connect(table_, &TableWithPhilosophers::postPhilAppend, this,[this](){endInsertRows();});
        connect(table_, &TableWithPhilosophers::prePhilRemoved, this,[this](int index){ beginRemoveRows(QModelIndex(), index, index);});
        connect(table_, &TableWithPhilosophers::postPhilRemoved, this, [this](){ endRemoveRows();});
        connect(table_, &TableWithPhilosophers::updateState,this, &PhilosophersModel::changeState);

    }
    endResetModel();
}


void PhilosophersModel::changeState(const uint id)
{
    int size = table_->items().size();
    int index;

    for(index = 0;index<size;index++){
    QVector<PhilosopherThread*> philosophers = table_->items();
        if (philosophers[index]->id() == id)
            break;
    }
    if (index != size)
        // signal to model that He should update the GUI
        emit dataChanged(this->index(index), this->index(index), QVector<int>() << philosopherState);

}




