#pragma once

#include "Device.h"

#include <memory>
#include <vector>

class DeviceManager
{
public:
    DeviceManager() = default;
    ~DeviceManager() = default;

    DeviceManager(const DeviceManager&) = delete;
    DeviceManager& operator=(const DeviceManager&) = delete;

    bool addDevice(std::unique_ptr<Device> device);

    Device* findDevice(int id);

    std::size_t deviceCount() const;

private:
    std::vector<std::unique_ptr<Device>> m_devices;
};