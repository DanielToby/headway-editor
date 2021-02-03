#include "curves.h"

Curves::Curves(QObject *parent) : QObject(parent)
{

}

Curves::Entries Curves::entries() const
{
    return _entries;
}


CurveEntry *Curves::createEntry()
{
    auto result = new CurveEntry( this );
    if ( result != nullptr ) {
        _entries.append( result );
        emit entryAdded( result );
    }
    return result;
}

bool Curves::deleteEntry(CurveEntry *entry)
{
    if ( _entries.contains( entry )) {
        emit entryRemoved( entry );
        _entries.removeOne( entry );
        delete entry;
        return true;
    }
    return false;
}
