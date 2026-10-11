#pragma once

#include "../data/VideoFrame.h"

class DisplayCell
{
public:
    explicit DisplayCell(int cellId = -1);

    int cellId() const;
    int deviceId() const;
    bool hasFrame() const;

    const VideoFrame& latestFrame() const;

    void setDeviceId(int deviceId);
    bool updateFrame(const VideoFrame& frame);

private:
    int m_cellId = -1;
    int m_deviceId = -1;
    bool m_hasFrame = false;

    VideoFrame m_latestFrame;
};