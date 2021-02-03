#include "curveentry.h"

CurveEntry::CurveEntry(QObject *parent) : QObject(parent)
{

}

QString CurveEntry::shortName() const
{
    return _shortName;
}

void CurveEntry::setShortName(const QString &shortName)
{
    if ( _shortName != shortName ) {
        _shortName = shortName;
        emit shortNameChanged();
    }
}

QString CurveEntry::longName() const
{
    return _longName;
}

void CurveEntry::setLongName(const QString &longName)
{
    if ( _longName != longName ) {
        _longName = longName;
        emit longNameChanged();
    }
}

double CurveEntry::xRange() const
{
    return _xRange;
}

void CurveEntry::setXRange(double xRange)
{
    _xRange = xRange;
}

double CurveEntry::yRange() const
{
    return _yRange;
}

void CurveEntry::setYRange(double yRange)
{
    _yRange = yRange;
}


QList<QPointF> CurveEntry::points() const
{
    return _points;
}

void CurveEntry::setPoints(const QList<QPointF> &points)
{
    _points = points;
}

QString CurveEntry::color() const
{
    return _color;
}

void CurveEntry::setColor(const QString &color)
{
    _color = color;
}
