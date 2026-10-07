#include "VideoWorker.h"

#include "../device/Device.h"

#include<QDateTime>
#include <QDebug>

VideoWorker::VideoWorker(
    Device* device,
    QObject* parent)
    : QObject(parent),
    m_device(device)
{
}

void VideoWorker::start()
{
    if (m_device == nullptr) {
        qDebug() << "VideoWorker has no device.";

        emit finished();
        return;
    }

    qDebug() << "VideoWorker::start()"
             << "deviceId =" << m_device->id();

    if (!m_device->open()) {
        qDebug() << "Failed to open device."
                 << "deviceId =" << m_device->id();

        emit finished();
        return;
    }

    while (true) {
        QImage image = m_device->readFrame();

        if (image.isNull()) {
            break;
        }
        VideoFrame frame;
        frame.deviceId = m_device->id();
        frame.image = image;
        frame.timestamp = QDateTime::currentMSecsSinceEpoch();
        emit frameReady(frame);
    }

    emit finished();
}