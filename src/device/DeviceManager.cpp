#include "DeviceManager.h"

#include <QDebug>

bool DeviceManager::addDevice(
    std::unique_ptr<Device> device)
{
    if (device == nullptr) {
        qDebug() << "Failed to add device:"
                 << "device is null.";

        return false;
    }

    if (findDevice(device->id()) != nullptr) {
        qDebug() << "Failed to add device:"
                 << "duplicate device ID ="
                 << device->id();

        return false;
    }

    m_devices.push_back(std::move(device));

    return true;
}

Device* DeviceManager::findDevice(int id)
{
    for (const auto& device : m_devices) {
        if (device->id() == id) {
            return device.get();
        }
    }

    return nullptr;
}

std::size_t DeviceManager::deviceCount() const
{
    return m_devices.size();
}