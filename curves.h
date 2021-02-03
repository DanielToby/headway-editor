#ifndef CURVES_H
#define CURVES_H

#include "curveentry.h"

#include <QObject>
#include <QList>

class Curves : public QObject
{
    Q_OBJECT
public:
    typedef QList<CurveEntry*> Entries;

    explicit Curves(QObject *parent = nullptr);

    Entries entries() const;
    CurveEntry* createEntry();
    bool deleteEntry( CurveEntry *entry );

signals:

    void entryAdded( CurveEntry *entry );
    void entryRemoved( CurveEntry *entry );

private:
    Entries _entries;

};

#endif // CURVES_H
