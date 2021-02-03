#include "plotview.h"


PlotView::PlotView(QWidget* parent) : QCustomPlot(parent)
{

}

void PlotView::mousePressEvent(QMouseEvent *event)
{
    // Select point for mouse move event
    if( (_primaryCurveNumber >= 0) && ( _primaryCurveNumber < this->graphCount()) )
    {
        // Identifies index of point closest to clickEvent.
        QPoint clickPoint = event->pos();
        QCPGraph *graph = this->graph(_primaryCurveNumber);
        QCPGraphDataContainer::const_iterator it = graph->data()->constEnd();
        QVariant details;

        if (graph->selectTest(clickPoint, false, &details)) {
            QCPDataSelection dataPoints = details.value<QCPDataSelection>();
            if (dataPoints.dataPointCount() > 0) {
                int index = dataPoints.dataRange().begin();
                it = graph->data()->at(index);

                // Translate click position to coordinates, determine if close enough to register
                double x = this->xAxis->pixelToCoord(clickPoint.x());
                double y = this->yAxis->pixelToCoord(clickPoint.y());
                int distance = fabs( x - it->key) + fabs( y - it->value);

                if ( distance < _pointRegisterRadius ) {
                    _selectedPointIndex = index;
                    emit SelectedPointChanged( QPointF(it->key, it->value) );
                }
            }
        }
    }
    QCustomPlot::mousePressEvent(event);
}

void PlotView::mouseMoveEvent(QMouseEvent *event)
{
    if ( _selectedPointIndex != -1 ) {
        // move selected point
        if( (_primaryCurveNumber >= 0) && ( _primaryCurveNumber < this->graphCount()) ) {
            QCPGraph* primary = this->graph(_primaryCurveNumber);
            QPoint clickPoint = event->pos();
            double x = this->xAxis->pixelToCoord(clickPoint.x());
            double y = this->yAxis->pixelToCoord(clickPoint.y());

            x = round( x * 4 ) / 4; //snap to grid
            y = round( y * 4 ) / 4; //snap to grid

            double xMin = this->xAxis->range().lower;
            double xMax = this->xAxis->range().upper;
            double yMin = this->yAxis->range().lower;
            double yMax = this->yAxis->range().upper;

            // Point cannot move below previous points values.
            if (_selectedPointIndex > 0) {
                xMin = (primary->data()->begin() + _selectedPointIndex - 1)->key;
                yMin = (primary->data()->begin() + _selectedPointIndex - 1)->value;
            }

            // Point cannot move above next points values.
            if (_selectedPointIndex < primary->dataCount() - 1) {
                xMax = (primary->data()->begin()+_selectedPointIndex + 1)->key;
                yMax = (primary->data()->begin()+_selectedPointIndex + 1)->value;
            }

            if (x < xMin) x = xMin;
            if (x > xMax) x = xMax;
            if (y < yMin) y = yMin;
            if (y > yMax) y = yMax;

            (primary->data()->begin()+_selectedPointIndex)->key = x;
            (primary->data()->begin()+_selectedPointIndex)->value = y;
            emit( DataChanged() );
            this->replot();
        }
    }
}

void PlotView::mouseReleaseEvent(QMouseEvent *event)
{
    if ( _selectedPointIndex != -1 ) {
        _selectedPointIndex = -1;
        if( (_primaryCurveNumber >= 0) && ( _primaryCurveNumber < this->graphCount()) ) {
            this->graph(_primaryCurveNumber)->data().data()->sort();
        }
        this->replot();
        emit( EditingFinished() );
        QCustomPlot::mouseReleaseEvent(event);
    }
}

void PlotView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QCPGraph *primary = this->graph(_primaryCurveNumber);
    QPoint clickPoint = event->pos();
    double x = this->xAxis->pixelToCoord(clickPoint.x());
    double y = this->yAxis->pixelToCoord(clickPoint.y());

    x = round( x * 8 ) / 8; //snap to grid
    y = round( y * 8 ) / 8; //snap to grid

    // Only add points within legal bounds.
    if (this->xAxis->range().contains(x) && this->yAxis->range().contains(y)) {

        const QVector<double> xData = { x };
        const QVector<double> yData = { y };

        primary->addData( xData, yData, true );

        // Snaps point to a legal position.
        PlotView::mousePressEvent(event);
        PlotView::mouseMoveEvent(event);
        PlotView::mouseReleaseEvent(event);

        emit( DataChanged() );
    }
}

