#ifndef POINTMODEL_H
#define POINTMODEL_H

#include <QObject>
#include <QAbstractItemModel>

class PointModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit PointModel(QObject *parent = nullptr);
    PointModel(const QString &xTitle, const QString &yTitle, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;

    void setPoints(const QList<QPointF> &points);
    const QList<QPointF> &points() const;

    bool addPoint(const QPointF &point);
    bool removePoint(const QPointF &point);
    int indexOfPoint(const QPointF &point);
    QPointF pointAtIndex(int index);
    void sortPoints();
    int size();

signals:
    void pointsChanged();

private:
    QList<QPointF> _points;
    QString _xTitle, _yTitle;
};

#endif // POINTMODEL_H
