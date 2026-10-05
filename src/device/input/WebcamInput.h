#pragma once

#include "DeviceInput.h"

class WebcamInput : public DeviceInput
{
public:
    explicit WebcamInput(int deviceIndex = 0);

    bool open() override;
    QImage readFrame() override;

private:
    int m_deviceIndex = 0;
};