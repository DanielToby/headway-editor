#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QtCharts/QChartGlobal>
#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLineSeries>

QT_CHARTS_USE_NAMESPACE

class ChartView : public QChartView
{
    Q_OBJECT

public:
    ChartView(QWidget *parent = 0);
    ~ChartView();

    QLineSeries *selectedPoint() const;
    void setSelectedPoint(const QPointF &p);

    QLineSeries *primaryCurve() const;

private Q_SLOTS:

private:
    QLineSeries *_selectedPoint;
    QLineSeries *_primaryCurve;
};

#endif // CHARTVIEW_H
