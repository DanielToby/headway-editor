#ifndef COLORDELEGATOR_H
#define COLORDELEGATOR_H

#include <QObject>

class ColorDelegator : public QObject
{
    Q_OBJECT
public:
    explicit ColorDelegator(QStringList colors, QObject *parent = nullptr);
    QString getNextColor();

signals:

private:
    QStringList _colors;
    int _index;

};

#endif // COLORDELEGATOR_H
