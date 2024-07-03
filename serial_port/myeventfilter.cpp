#include "myeventfilter.h"
#include <windows.h>
#include <dbt.h>
#include <QDebug>

MyNativeFilter::MyNativeFilter()
{

}

bool MyNativeFilter::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType);
    Q_UNUSED(result);
    MSG *msg = static_cast<MSG*>(message);
    if(msg->message == WM_DEVICECHANGE)
    {
        if(msg->wParam == DBT_DEVICEARRIVAL ||
                msg->wParam == DBT_DEVICEREMOVECOMPLETE) //加入了一个设备
        {
//            qDebug() << "加入了一个设备";
            emit DeviceChanged();
        }

    }

    return false;
}
