#ifndef DEVICEGAMEPAD_H
#define DEVICEGAMEPAD_H

#include <QObject>
QT_BEGIN_NAMESPACE
class QGamepad;
QT_END_NAMESPACE

class DeviceGamepad : public QObject
{
public:
    DeviceGamepad();
private:
    QGamepad *m_gamepad;
};

#endif // DEVICEGAMEPAD_H
