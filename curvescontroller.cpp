#include "curvescontroller.h"

CurvesController::CurvesController(
        Curves *curves, QObject *parent) :
    QObject(parent),
    _curves(curves),
    _count(0)
{
    Q_ASSERT( curves != nullptr );
}

CurveEntry *CurvesController::createEntry()
{
    auto result = _curves->createEntry();
    if ( result != nullptr ) {
        result->setShortName( tr("New Curve ") + QString::number(++_count) );
    }
    return result;
}

bool CurvesController::deleteEntry(CurveEntry *entry)
{
    return _curves->deleteEntry( entry );
}



