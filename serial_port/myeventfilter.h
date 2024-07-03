#ifndef MYEVENTFILTER_H
#define MYEVENTFILTER_H

#include <QAbstractNativeEventFilter>
#include <QWidget>

class MyNativeFilter : public QWidget, public QAbstractNativeEventFilter
{
    Q_OBJECT
public:
    MyNativeFilter();
    virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);
signals:
    void DeviceChanged();

};

#endif // MYEVENTFILTER_H
