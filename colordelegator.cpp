#include "colordelegator.h"

ColorDelegator::ColorDelegator(QStringList colors, QObject *parent)
    : QObject(parent), _colors(colors), _index(0)
{

}

QString ColorDelegator::getNextColor()
{
    if (_index > _colors.size() - 1) {
        _index = 0;
    }
    return _colors[_index++];
}
