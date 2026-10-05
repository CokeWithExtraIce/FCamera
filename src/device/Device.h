#pragma once

#include "input/DeviceInput.h"

#include <QImage>

#include <memory>

class Device
{
public:
    Device(
        int id,
        std::unique_ptr<DeviceInput> input
        );

    ~Device() = default;

    Device(const Device&) = delete;
    Device& operator=(const Device&) = delete;

    Device(Device&&) = default;
    Device& operator=(Device&&) = default;

    int id() const;

    bool open();
    QImage readFrame();

private:
    int m_id = -1;

    std::unique_ptr<DeviceInput> m_input;
};