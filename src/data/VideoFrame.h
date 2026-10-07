#pragma once

#include <QImage>
#include <QMetaType>

struct VideoFrame
{
    int deviceId = -1;
    QImage image;
    qint64 timestamp = 0;
};

Q_DECLARE_METATYPE(VideoFrame)