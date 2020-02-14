#include "devicegamepad.h"
#include <QtGamepad/QGamepad>

DeviceGamepad::DeviceGamepad()
{
    auto gamepads = QGamepadManager::instance()->connectedGamepads();
    if (gamepads.isEmpty()) {
        //qDebug() << "Did not find any connected gamepads";
        //return;
    }

    m_gamepad = new QGamepad(*gamepads.begin(), this);
}
