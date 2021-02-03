#ifndef CURVESCONTROLLER_H
#define CURVESCONTROLLER_H

#include "curves.h"

#include <QObject>
#include <QVector>

class CurvesController : public QObject
{
    Q_OBJECT
public:
    explicit CurvesController(Curves *curves, QObject *parent = nullptr);

    CurveEntry *createEntry();
    bool deleteEntry(CurveEntry *entry);

private:
    Curves *_curves;
    int _count;
};

#endif // CURVESCONTROLLER_H
