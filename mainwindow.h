#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "curveswidget.h"

#include <QWidget>
#include <QTabWidget>
#include <QDialogButtonBox>

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void saveAndClose();

private:
    QTabWidget *_tabWidget;
    QDialogButtonBox *_buttonBox;

    Curves *_waitCurves;
    CurvesController *_waitCurvesController;
    CurvesWidget *_waitCurvesWidget;

    Curves *_multCurves;
    CurvesController *_multCurvesController;
    CurvesWidget *_multCurvesWidget;
};

#endif // MAINWINDOW_H
