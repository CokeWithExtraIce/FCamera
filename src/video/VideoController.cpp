#include "VideoController.h"
#include "../device/Device.h"
#include "../device/DeviceManager.h"
#include "VideoWorker.h"

#include <QDebug>
#include <QMetaType>

VideoController::VideoController(
    DeviceManager* deviceManager,
    QObject* parent)
    : QObject(parent),
    m_deviceManager(deviceManager)
{
    qRegisterMetaType<VideoFrame>("VideoFrame");
}

VideoController::~VideoController()
{
    m_workerThread.quit();
    m_workerThread.wait();
}

void VideoController::start(int deviceId)
{
    if (m_deviceManager == nullptr) {
        qDebug() << "VideoController has no DeviceManager.";

        return;
    }

    Device* device =
        m_deviceManager->findDevice(deviceId);

    if (device == nullptr) {
        qDebug() << "Device not found:"
                 << deviceId;

        return;
    }

    qDebug() << "VideoController::start()"
             << "deviceId =" << deviceId;

    // worker
    m_worker = new VideoWorker(device);

    m_worker->moveToThread(&m_workerThread);

    connect(
        this,
        &VideoController::startRequested,
        m_worker,
        &VideoWorker::start
        );

    connect(
        m_worker,
        &VideoWorker::frameReady,
        this,
        &VideoController::frameReady
        );

    connect(
        m_worker,
        &VideoWorker::finished,
        &m_workerThread,
        &QThread::quit
        );

    connect(
        &m_workerThread,
        &QThread::finished,
        m_worker,
        &QObject::deleteLater
        );

    m_workerThread.start();

    emit startRequested();
}