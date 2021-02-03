#ifndef CURVESEDITOR_H
#define CURVESEDITOR_H

#include "curvesview.h"

#include <QWidget>

class CurvesEditor : public QWidget
{
    Q_OBJECT
public:
    explicit CurvesEditor(QWidget *parent = nullptr);

signals:

private:
    Curves *_waitCurves;
    CurvesController *_waitController;
    CurvesView *_waitView;

};

#endif // CURVESEDITOR_H
