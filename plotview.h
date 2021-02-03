#ifndef PLOTVIEW_H
#define PLOTVIEW_H

#include "qcustomplot.h"

class PlotView : public QCustomPlot
{
    Q_OBJECT
public:
    PlotView(QWidget* parent);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

signals:
    void DataChanged();
    void EditingFinished();
    void SelectedPointChanged( QPointF );

private:
    int _selectedPointIndex = -1;
    int _primaryCurveNumber = 0;
    int _pointRegisterRadius = 4;
};

#endif // PLOTVIEW_H
