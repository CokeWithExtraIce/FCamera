#include "VideoWorker.h"

#include "../device/Device.h"

#include <QDateTime>
#include <QDebug>
#include <QThread>

#include <QElapsedTimer>

VideoWorker::VideoWorker(
    Device* device,
    QObject* parent)
    : QObject(parent),
    m_device(device)
{
}

void VideoWorker::start()
{
    QElapsedTimer timer;
    timer.start();

    int frameCount = 0;
    bool countFinished = false;

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

    QThread* currentThread = QThread::currentThread();

    while (!currentThread->isInterruptionRequested()) {
        QImage image = m_device->readFrame();

        if (currentThread->isInterruptionRequested()) {
            break;
        }
        if (image.isNull()) {
            break;
        }
        VideoFrame frame;
        frame.deviceId = m_device->id();
        frame.image = image;
        frame.timestamp = QDateTime::currentMSecsSinceEpoch();
        emit frameReady(frame);

        // 프레임체크
        if (!countFinished) {
            if (timer.elapsed() < 10000) {
                ++frameCount;
            } else {
                qDebug() << "Device ID:" << m_device->id()
                << "| Frames in first 10 seconds:" << frameCount;

                countFinished = true;
            }}

        QThread::msleep(30);
        }

    qDebug() << "VideoWorker stopped."
             << "deviceId =" << m_device->id();

    emit finished();
}