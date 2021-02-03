#include "curveseditor.h"

CurvesEditor::CurvesEditor(QWidget *parent) : QWidget(parent)
{
    _waitCurves = new Curves(this);
    _waitController = new CurvesController( _waitCurves, this );

    _waitView = new CurvesView( _waitController, this );
    _waitView->show();
}
