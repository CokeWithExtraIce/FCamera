#include "VideoWorker.h"

#include "../device/Device.h"

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
        QImage frame = m_device->readFrame();

        if (frame.isNull()) {
            break;
        }

        emit frameReady(frame);
    }

    emit finished();
}