#ifndef CURVESWIDGET_H
#define CURVESWIDGET_H

#include "curvescontroller.h"
#include "pointmodel.h"
#include "plotview.h"
#include "colordelegator.h"

#include <QWidget>
#include <QHash>
#include <QLineEdit>
#include <QLabel>
#include <QListWidget>
#include <QStringList>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QGridLayout>
#include <QMessageBox>
#include <QHeaderView>
#include <QDebug>
#include <QTableView>

class QListWidgetItem;

class CurvesWidget : public QWidget
{
    Q_OBJECT
public:
    enum class CurveType { Wait, Crowding };
    CurvesWidget(CurvesController *controller,
                        CurveType curveType,
                        QWidget *parent = nullptr);
    CurvesWidget(const CurvesWidget&) = delete;
    CurvesWidget& operator=(const CurvesWidget&) = delete;

private slots:
    void createCurve();
    void deleteCurve();
    void editCurve( QListWidgetItem *listItem );
    void resetCurve();
    void saveCurve( QListWidgetItem *listItem );
    void saveCurveButtonHandler();
    void addPoint(const QPointF &p);
    void addPointButtonHandler();
    void removePoint();
    void setCurrentPointInTable( const QPointF &p );
    void updateModelFromPlot();
    void changesMade();
    void changesSavedOrDiscarded(QListWidgetItem *listItem);
    void handleCurveSelectionChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    CurvesController *_controller;
    QHash< QListWidgetItem*, CurveEntry*> _entryMap;

    double _xRange;
    double _yRange;
    QString _xTableHeaderText;
    QString _yTableHeaderText;
    QString _xAxisTitle;
    QString _yAxisTitle;
    QPointF _defaultPoint;
    ColorDelegator *_colorDelegator;
    bool _changesSaved;

    QPushButton *_addCurve, *_removeCurve, *_addPoint, *_removePoint;
    QListWidget *_listWidget;
    QLineEdit *_shortNameEdit, *_longNameEdit;
    PointModel *_pointModel;
    QSortFilterProxyModel *_proxyModel;
    QTableView *_tableView;
    PlotView *_plotView;
    QDialogButtonBox *_buttonBox;
    QMessageBox *_saveWindow;

    void createGUI();
    void setupConnections();
    void plotPrimaryCurve();
    void plotBackgroundCurves();
    void enableEditing();
    void disableEditing();

};

#endif // CURVESWIDGET_H
