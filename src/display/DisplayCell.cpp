#include "DisplayCell.h"

DisplayCell::DisplayCell(int cellId)
    : m_cellId(cellId)
{
}

int DisplayCell::cellId() const
{
    return m_cellId;
}

int DisplayCell::deviceId() const
{
    return m_deviceId;
}

bool DisplayCell::hasFrame() const
{
    return m_hasFrame;
}

const VideoFrame& DisplayCell::latestFrame() const
{
    return m_latestFrame;
}

void DisplayCell::setDeviceId(int deviceId)
{
    // 표시 장치가 변경되면 이전 장치의 프레임은 초기화
    if (m_deviceId == deviceId) {
        return;
    }

    m_deviceId = deviceId;
    m_hasFrame = false;
    m_latestFrame = VideoFrame{};
}

bool DisplayCell::updateFrame(const VideoFrame& frame)
{
    // 현재 셀에 할당된 장치의 프레임만 갱신
    if (frame.deviceId != m_deviceId ||
        frame.image.isNull()) {
        return false;
    }

    m_latestFrame = frame;
    m_hasFrame = true;

    return true;
}