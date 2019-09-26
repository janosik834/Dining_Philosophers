#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

#include <QAbstractListModel>
extern  QString philosopherName;
class TableWithPhilosophers;

/*!
 * \brief The PhilosophersModel class - contains the QML model of rendered data
 */
class PhilosophersModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(TableWithPhilosophers* tab READ getTable WRITE setTable )

public:
    explicit PhilosophersModel(QObject *parent = nullptr);

    // enum - help to modify the model changed
    enum {
        philosopherName,
        philosopherState
    };

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    TableWithPhilosophers *getTable() const;
    void setTable(TableWithPhilosophers* value);
public slots:
    /*!
     * \brief changeState - method called to update GUI
     * \param id - ID of phliosopher who should be updated
     */
    void changeState(const uint id);



private:
    /*!
     * \brief table_ - attribute of class containing list of philosophers
     */
    TableWithPhilosophers *table_;
};
#endif // PHILOSOPHERS_H
