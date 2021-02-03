#ifndef CURVEENTRY_H
#define CURVEENTRY_H

#include <QObject>
#include <QString>
#include <QSet>

class CurveEntry : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString shortName READ shortName WRITE setShortName NOTIFY shortNameChanged)
    Q_PROPERTY(QString longName READ longName WRITE setLongName NOTIFY longNameChanged)
    Q_PROPERTY(double xRange READ xRange WRITE setXRange NOTIFY xRangeChanged)
    Q_PROPERTY(double yRange READ yRange WRITE setYRange NOTIFY yRangeChanged)
    Q_PROPERTY(PointList points READ points WRITE setPoints NOTIFY pointsChanged)
    Q_PROPERTY(QString color READ color WRITE setColor NOTIFY colorChanged)

public:
    typedef QList<QPointF> PointList;
    explicit CurveEntry(QObject *parent = nullptr);

    QString shortName() const;
    void setShortName(const QString &shortName);

    QString longName() const;
    void setLongName(const QString &longName);

    double xRange() const;
    void setXRange(double xRange);

    double yRange() const;
    void setYRange(double yRange);

    PointList points() const;
    void setPoints(const PointList &points);

    QString color() const;
    void setColor(const QString &color);
signals:

    void shortNameChanged();
    void longNameChanged();
    void xRangeChanged();
    void yRangeChanged();
    void pointsChanged();
    void colorChanged();

public slots:

private:
    QString _shortName;
    QString _longName;
    double _xRange;
    double _yRange;
    PointList _points;
    QString _color;
};

#endif // CURVEENTRY_H
