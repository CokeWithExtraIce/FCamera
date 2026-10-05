#include "Device.h"

#include <QDebug>

Device::Device(
    int id,
    std::unique_ptr<DeviceInput> input)
    : m_id(id),
    m_input(std::move(input))
{
}

int Device::id() const
{
    return m_id;
}

bool Device::open()
{
    if (m_input == nullptr) {
        qDebug() << "Device has no input."
                 << "deviceId =" << m_id;

        return false;
    }

    return m_input->open();
}

QImage Device::readFrame()
{
    if (m_input == nullptr) {
        return {};
    }

    return m_input->readFrame();
}