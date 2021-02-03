#include "pointmodel.h"

PointModel::PointModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

PointModel::PointModel(const QString &xTitle, const QString &yTitle, QObject *parent)
    : QAbstractTableModel(parent),
      _xTitle(xTitle),
      _yTitle(yTitle)
{

}

int PointModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : _points.size();
}

int PointModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : 2;
}

QVariant PointModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= _points.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {
        const auto &point = _points.at(index.row());

        switch (index.column()) {
        case 0:
            return point.x();
        case 1:
            return point.y();
        default:
            break;
        }
    }
    return QVariant();
}

bool PointModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        const int row = index.row();
        auto point = _points.value(row);

        switch (index.column()) {
        case 0:
            point.setX(value.toFloat());
            break;
        case 1:
            point.setY(value.toFloat());
            break;
        default:
            return false;
        }
        _points.replace(row, point);
        emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});

        return true;
    }

    return false;
}

QVariant PointModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr(_xTitle.toLocal8Bit());
        case 1:
            return tr(_yTitle.toLocal8Bit());
        default:
            break;
        }
    }
    return QVariant();
}

bool PointModel::insertRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row)
        _points.insert(position, QPointF());

    endInsertRows();
    emit pointsChanged();
    return true;
}

bool PointModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row)
        _points.removeAt(position);

    endRemoveRows();
    emit pointsChanged();
    return true;
}


Qt::ItemFlags PointModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

const QList<QPointF> &PointModel::points() const
{
    return _points;
}

void PointModel::setPoints(const QList<QPointF> &points)
{
    beginResetModel();
    if (points != _points)
        _points = points;
    sortPoints();
    endResetModel();
    emit pointsChanged();
}

bool PointModel::addPoint(const QPointF &point)
{
    beginResetModel();
    _points.append(point);
    sortPoints();
    endResetModel();
    emit pointsChanged();
    return true;
}

bool PointModel::removePoint(const QPointF &point)
{
    if (_points.contains(point)) {
        beginResetModel();
        _points.removeOne(point);
        endResetModel();
        emit pointsChanged();
        return true;
    } else {
        return false;
    }
}

int PointModel::indexOfPoint(const QPointF &point)
{
    for(int i = 0; i < _points.size(); i++) {
        if (round(_points[i].x()) == round(point.x())
                && round(_points[i].y()) == round(point.y()))
            return i;
    }
    return -1;
}

QPointF PointModel::pointAtIndex(int index)
{
    Q_ASSERT(index < _points.size());
    return _points[index];
}

void PointModel::sortPoints()
{
    std::sort(_points.begin(), _points.end(),
              [](const QPointF &a, const QPointF &b)
    {
        return a.x() < b.x();
    });
}

int PointModel::size()
{
    return _points.size();
}
