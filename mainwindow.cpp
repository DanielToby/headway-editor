#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    _tabWidget = new QTabWidget(this);

    // Wait Curve tab
    _waitCurves = new Curves(this);
    _waitCurvesController = new CurvesController( _waitCurves, this );
    _waitCurvesWidget = new CurvesWidget( _waitCurvesController, CurvesWidget::CurveType::Wait, this );
    _tabWidget->addTab(_waitCurvesWidget, tr("Wait Curve"));

    // Multiplicative Crowding Curve tab
    _multCurves = new Curves(this);
    _multCurvesController = new CurvesController( _multCurves, this );
    _multCurvesWidget = new CurvesWidget( _multCurvesController, CurvesWidget::CurveType::Crowding, this );
    _tabWidget->addTab(_multCurvesWidget, tr("Multiplicative Crowding Curve"));

    _buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel, this);

    connect(_buttonBox, &QDialogButtonBox::accepted, this, &MainWindow::saveAndClose);
    connect(_buttonBox, &QDialogButtonBox::rejected, this, &MainWindow::close);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(_tabWidget);
    mainLayout->addWidget(_buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("Curves Editor"));
}

void MainWindow::saveAndClose()
{
    // Save changes to curves
    close();
}

