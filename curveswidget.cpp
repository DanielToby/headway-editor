#include "curveswidget.h"

CurvesWidget::CurvesWidget(CurvesController *controller,
                       CurveType curveType,
                       QWidget* parent)
    : QWidget(parent),
      _controller(controller)
{
    switch (curveType) {
    case (CurveType::Wait):
        _xTableHeaderText = "X = Headway";
        _yTableHeaderText = "Y = Wait Time";
        _xAxisTitle = "Headway (minutes)";
        _yAxisTitle = "Wait Time (minutes)";
        _defaultPoint = {1.0, 0.01};
        _xRange = 160;
        _yRange = 20;
        break;
    case (CurveType::Crowding):
        _xTableHeaderText = "X = Utilization";
        _yTableHeaderText = "Y = Factor";
        _xAxisTitle = "Utilization (%)";
        _yAxisTitle = "Factor";
        _defaultPoint = {50.0, 5.0};
        _xRange = 100;
        _yRange = 10;
        break;
    }

    createGUI();
}

void CurvesWidget::createGUI()
{
    // Curve add / remove buttons
    _addCurve = new QPushButton("Add Curve", this);
    _removeCurve = new QPushButton("Remove Curve", this);
    _removeCurve->setEnabled(false);

    // Curves list
    _listWidget = new QListWidget(this);
    _listWidget->setMaximumHeight(75);

    // Short Name / Long Name edit
    QLabel *shortNameLabel = new QLabel(tr("Short Name"), this);
    _shortNameEdit = new QLineEdit(this);
    _shortNameEdit->setEnabled(false);
    QLabel *longNameLabel = new QLabel(tr("Long Name"), this);
    _longNameEdit = new QLineEdit(this);
    _longNameEdit->setEnabled(false);


    // Point data model: Model represents a QList of QPointF's.
    _pointModel = new PointModel(_xTableHeaderText, _yTableHeaderText, this);

    // Proxy model allows sorting by column.
    _proxyModel = new QSortFilterProxyModel(this);
    _proxyModel->setSourceModel( _pointModel );

    // Table is a view for the proxyModel.

    // Table
    QLabel *tableLabel = new QLabel(tr("Points"), this);
    _tableView = new QTableView(this);
    _tableView->setModel( _proxyModel );
    _tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    _tableView->verticalHeader()->hide();
    _tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    _tableView->setSortingEnabled(true);
    _tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Plot is also be a view for the proxyModel, but it must be manually updated.
    // QCustomPlot
    _plotView = new PlotView(this);
    _plotView->xAxis->setLabel(_xAxisTitle);
    _plotView->yAxis->setLabel(_yAxisTitle);
    _plotView->xAxis->setRange(0, _xRange);
    _plotView->yAxis->setRange(0, _yRange);
    _plotView->addGraph();
    _plotView->setEnabled(false);
    _plotView->setMinimumWidth(500);
    _plotView->setMinimumHeight(350);

    // Color Delegator provides colors for each new curve.
    QStringList colorList = { "#1f78b4",  "#33a02c", "#e31a1c", "#ff7f00", "#6a3d9a", "#b15928" };
    _colorDelegator = new ColorDelegator(colorList, this);

    // Point add / remove buttons
    _addPoint = new QPushButton("Add Point", this);
    _addPoint->setEnabled(false);
    _removePoint = new QPushButton("Remove Point", this);
    _removePoint->setEnabled(false);

    // Save / Reset / Close buttons
    _buttonBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Reset, this);
    _buttonBox->button( QDialogButtonBox::Save )->setEnabled(false);
    _buttonBox->button( QDialogButtonBox::Reset )->setEnabled(false);
    _changesSaved = true;

    // Save changes message box
    _saveWindow = new QMessageBox(this);
    _saveWindow->setWindowTitle("Save Changes");
    _saveWindow->setStandardButtons(QMessageBox::Save | QMessageBox::Discard);
    _saveWindow->setDefaultButton(QMessageBox::Save);
    _saveWindow->setMinimumSize(200, 100);

    QGridLayout *_mainLayout = new QGridLayout(this);
    _mainLayout->addWidget(_addCurve, 0, 0, 1, 1);
    _mainLayout->addWidget(_removeCurve, 0, 1, 1, 1);
    _mainLayout->addWidget(_listWidget, 1, 0, 1, -1);
    _mainLayout->addWidget(shortNameLabel, 2, 0, 1, 1);
    _mainLayout->addWidget(_shortNameEdit, 2, 1, 1, 1);
    _mainLayout->addWidget(longNameLabel, 2, 2, 1, 1);
    _mainLayout->addWidget(_longNameEdit, 2, 3, 1, -1);
    _mainLayout->addWidget(tableLabel, 3, 0, 1, 1);
    _mainLayout->addWidget(_tableView, 4, 0, 1, 2);
    _mainLayout->addWidget(_plotView, 4, 2, 1, -1);
    _mainLayout->addWidget(_addPoint, 5, 0, 1, 1);
    _mainLayout->addWidget(_removePoint, 5, 1, 1, 1);
    _mainLayout->addWidget(_buttonBox, 5, 5, 1, -1);

    _mainLayout->setColumnStretch(0, 0);
    _mainLayout->setColumnStretch(1, 0);
    _mainLayout->setColumnStretch(2, 0);
    _mainLayout->setColumnStretch(3, 1);

    setupConnections();
    setLayout(_mainLayout);
    setWindowTitle(tr("Curves Editor"));
}

void CurvesWidget::createCurve()
{
    auto entry = _controller->createEntry();
    if ( entry ) {
        // Set initial values
        entry->setXRange( _xRange );
        entry->setYRange( _yRange );
        entry->setPoints( { _defaultPoint } );

        // Each curve gets a color from _colorList
        entry->setColor(_colorDelegator->getNextColor());

        // Add Curve to listWidget
        _listWidget->addItem( entry->shortName() );

        QListWidgetItem *listItem = _listWidget->item(_listWidget->count() - 1);

        // Set background color
        QColor color;
        color.setNamedColor(entry->color());
        listItem->setData(Qt::DecorationRole, color);

        // Add (listItem, entry) to map
        _entryMap.insert( listItem, entry );

        // Set as selected listWidgetItem
        _listWidget->setCurrentItem( listItem );

        // Allow remove curve
        _removeCurve->setEnabled(true);
    }
}

void CurvesWidget::deleteCurve()
{
    // Find selected item
    auto listItem = _listWidget->currentItem();
    if ( listItem ) {
        // Find associated entry
        auto entry = _entryMap.value( listItem );
        if ( entry ) {
            if ( _controller->deleteEntry( entry ) ) {
                // Remove from map and delete
                _entryMap.remove( listItem );
                delete listItem;

                // if there are no more entries disable editing
                if (_entryMap.isEmpty()) {
                    disableEditing();
                    _removeCurve->setEnabled(false);
                } else {
                    // edit next curve
                    editCurve( _listWidget->currentItem() );
                }
            }
        }
    }
}

// Called when a curve entry is selected from the listWidget.
void CurvesWidget::editCurve( QListWidgetItem *listItem )
{
    // Find associated entry
    auto entry = _entryMap.value( listItem );
    if ( entry ) {
        resetCurve();
    }
}

// Called when reset is pressed or when a curve is first selected.
void CurvesWidget::resetCurve()
{
    // Find selected item
    auto listItem = _listWidget->currentItem();
    if ( listItem ) {
        // Find associated entry
        auto entry = _entryMap.value( listItem );
        if ( entry ) {
            // Reset values to those of selected curve
            _shortNameEdit->setText(entry->shortName());
            _longNameEdit->setText(entry->longName());
            _xRange = entry->xRange();
            _yRange = entry->yRange();

            // Update Points data
            _pointModel->setPoints(entry->points());

            // Set primary curve color to that of entry
            QColor c;
            c.setNamedColor(entry->color());
            _plotView->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, c, 10));
            _plotView->graph(0)->setPen(QPen(c, 3, Qt::SolidLine));

            enableEditing();
        }
        // Replot everything
        plotPrimaryCurve();
        plotBackgroundCurves();

        changesSavedOrDiscarded( listItem );
    }
}

void CurvesWidget::saveCurve( QListWidgetItem *listItem )
{
    auto entry = _entryMap.value( listItem );
    if ( entry ) {
        // Set values to those in memory
        entry->setShortName( _shortNameEdit->text() );
        entry->setLongName( _longNameEdit->text() );
        entry->setPoints( _pointModel->points() );
        listItem->setText( entry->shortName() );
    }

    // Replot background curves
    plotBackgroundCurves();
    changesSavedOrDiscarded( listItem );
}

void CurvesWidget::saveCurveButtonHandler()
{
    saveCurve( _listWidget->currentItem() );
}

void CurvesWidget::addPoint(const QPointF &p)
{
    _pointModel->addPoint(p);
    changesMade();
}

void CurvesWidget::addPointButtonHandler() {

    addPoint(_defaultPoint);
}

void CurvesWidget::removePoint()
{
    // If there is a row selected, delete that. Otherwise, delete the last row.
    int rowToDelete = _pointModel->size() - 1;
    auto i = _tableView->selectionModel()->selectedRows();
    if ( i.size() > 0 ) rowToDelete = i[0].row();

    if (_pointModel->size() > 0) {
        _pointModel->removeRows(rowToDelete, 1, QModelIndex());
        changesMade();
    }

    if (_pointModel->size() == 0) {
        _removePoint->setEnabled(false);
    }
}


void CurvesWidget::plotPrimaryCurve()
{
    QVector<double> xValues, yValues;
    for(const QPointF &p : _pointModel->points()) {
        xValues.append(p.x());
        yValues.append(p.y());
    }

    // graph(0) is reserved for the primary curve.
    QCPGraph *primaryCurve = _plotView->graph(0);
    primaryCurve->setData(xValues, yValues);

    _plotView->replot();
    _plotView->update();
}

void CurvesWidget::plotBackgroundCurves()
{
    for (int i = 1; i < _plotView->graphCount(); i++) {
        _plotView->removeGraph(i);
    }

    int index = 1;
    // Plot all curve entries.
    for (const CurveEntry *e : _entryMap.values()) {
        QCPGraph *currentGraph;
        if (index < _plotView->graphCount()) {
            currentGraph = _plotView->graph(index);
        } else {
            currentGraph = _plotView->addGraph();
        }

        QVector<double> xValues, yValues;
        for(const QPointF &p : e->points()) {
            xValues.append(p.x());
            yValues.append(p.y());
        }

        currentGraph->setData(xValues, yValues);
        currentGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone));

        QColor color;
        color.setNamedColor(e->color());
        currentGraph->setPen(QPen(color, 2));

        // If this is the current curve, use a dashed line
        if (e == _entryMap.value( _listWidget->currentItem() )) {
            color.setNamedColor(e->color());
            currentGraph->setPen(QPen(color, 2, Qt::DashLine));
        }

        index++;
    }
    _plotView->replot();
    _plotView->update();
}

void CurvesWidget::setCurrentPointInTable( const QPointF &p )
{
    int row = _pointModel->indexOfPoint(p);
    QModelIndex index = _pointModel->index(row, 0);
    _tableView->setCurrentIndex(index);
}

void CurvesWidget::updateModelFromPlot()
{
    QList<QPointF> graphPoints;
    auto begin = _plotView->graph(0)->data()->begin();
    auto end = _plotView->graph(0)->data()->end();

    for (QCPGraphDataContainer::const_iterator it=begin; it!=end; ++it)
    {
        // iterates through data, appending (key, value) to graphPoints
        graphPoints.append(QPointF(it->key, it->value));
    }
    _pointModel->setPoints(graphPoints);
}

void CurvesWidget::enableEditing()
{
    // Editing available only when a curve is selected.
    _shortNameEdit->setEnabled(true);
    _longNameEdit->setEnabled(true);
    _addPoint->setEnabled(true);
    if (_pointModel->size() > 0)
        _removePoint->setEnabled(true);
    _tableView->setEnabled(true);
    _plotView->setEnabled(true);
}

void CurvesWidget::disableEditing()
{
    // Disabled when window first shows and after deleting the last curve.
    _shortNameEdit->setEnabled(false);
    _longNameEdit->setEnabled(false);
    _addPoint->setEnabled(false);
    _removePoint->setEnabled(false);
    _tableView->setEnabled(false);
    _plotView->setEnabled(false);
}


void CurvesWidget::changesMade()
{
    if (_changesSaved == true) {
        auto listItem = _listWidget->currentItem();
        if ( listItem ) {
            // Find associated entry
            auto entry = _entryMap.value( listItem );
            if ( entry ) {
                // Set values to those in memory
                listItem->setText( entry->shortName() + " *" );
            }
        }
        _buttonBox->button( QDialogButtonBox::Save )->setEnabled(true);
        _buttonBox->button( QDialogButtonBox::Reset )->setEnabled(true);
        _changesSaved = false;
    }
}

void CurvesWidget::changesSavedOrDiscarded( QListWidgetItem *listItem )
{
    if (_changesSaved == false) {
        auto entry = _entryMap.value( listItem );
        if ( entry ) {
            // Set values to those in memory
            listItem->setText(entry->shortName() );
        }
        _buttonBox->button( QDialogButtonBox::Save )->setEnabled(false);
        _buttonBox->button( QDialogButtonBox::Reset )->setEnabled(false);
        _changesSaved = true;
    }
}

void CurvesWidget::handleCurveSelectionChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (_changesSaved == false) {
        QString previousEntryTitle = _entryMap.value( previous )->shortName();
        _saveWindow->setText(previousEntryTitle + " has unsaved changes.");
        int ret = _saveWindow->exec();
        switch (ret) {
        case QMessageBox::Save:
            saveCurve( previous );
            changesSavedOrDiscarded( previous );
            editCurve( current );
            break;
        case QMessageBox::Discard:
            changesSavedOrDiscarded( previous );
            editCurve( current );
            break;
        }

    } else {
        editCurve( current );
    }
}




void CurvesWidget::setupConnections()
{
    connect( _addCurve, &QPushButton::clicked,
             this, &CurvesWidget::createCurve );
    connect( _removeCurve, &QPushButton::clicked,
             this, &CurvesWidget::deleteCurve );

    connect( _listWidget, &QListWidget::currentItemChanged,
             this, &CurvesWidget::handleCurveSelectionChanged);

    connect( _addPoint, &QPushButton::clicked,
             this, &CurvesWidget::addPointButtonHandler);
    connect( _removePoint, &QPushButton::clicked,
             this, &CurvesWidget::removePoint);

    connect( _shortNameEdit, &QLineEdit::editingFinished,
             this, &CurvesWidget::changesMade);
    connect( _longNameEdit, &QLineEdit::editingFinished,
             this, &CurvesWidget::changesMade);

    connect( _pointModel, &PointModel::dataChanged,
             this, &CurvesWidget::changesMade);
    connect( _pointModel, &PointModel::pointsChanged,
             this, &CurvesWidget::plotPrimaryCurve);
    connect( _pointModel, &PointModel::dataChanged,
             this, &CurvesWidget::plotPrimaryCurve);

    connect( _plotView, &PlotView::DataChanged,
             this, &CurvesWidget::updateModelFromPlot);
    connect( _plotView, &PlotView::DataChanged,
             this, &CurvesWidget::changesMade);
    connect( _plotView, &PlotView::SelectedPointChanged,
             this, &CurvesWidget::setCurrentPointInTable);

    connect( _buttonBox->button( QDialogButtonBox::Save ), &QPushButton::clicked,
             this, &CurvesWidget::saveCurveButtonHandler );
    connect( _buttonBox->button( QDialogButtonBox::Reset ), &QPushButton::clicked,
             this, &CurvesWidget::resetCurve);
}

