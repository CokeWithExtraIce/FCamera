#include "VideoController.h"
#include "../device/Device.h"
#include "../device/DeviceManager.h"
#include "VideoWorker.h"

#include <QDebug>
// #include <QMetaType>

VideoController::VideoController(
    DeviceManager* deviceManager,
    QObject* parent)
    : QObject(parent),
    m_deviceManager(deviceManager)
{
    // qRegisterMetaType<VideoFrame>("VideoFrame");
}

VideoController::~VideoController()
{
    auto it = m_workerThreads.constBegin();

    while (it != m_workerThreads.constEnd()) {
        QThread* thread = it.value();

        if (thread != nullptr) {
            thread->requestInterruption();
            thread->quit();
            // thread->wait();
        }

        ++it;
    }

    // 모든 워커 스레드가 종료될 때까지 대기
    it = m_workerThreads.constBegin();

    while (it != m_workerThreads.constEnd()) {
        QThread* thread = it.value();

        if (thread != nullptr) {
            thread->wait();
        }

        ++it;
    }
    qDebug() << "All worker threads stopped.";
}

void VideoController::start(int deviceId)
{
    if (m_deviceManager == nullptr) {
        qDebug() << "VideoController has no DeviceManager.";

        return;
    }

    if(m_workerThreads.contains(deviceId)){
        qDebug() << "VideoWorker already started:" << deviceId;
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
    // m_worker = new VideoWorker(device);

    // m_worker->moveToThread(&m_workerThread);
    QThread* workerThread = new QThread(this);
    VideoWorker* worker =
        new VideoWorker(device);

    worker->moveToThread(workerThread);

    connect(
        workerThread,
        &QThread::started,
        worker,
        &VideoWorker::start
        );

    connect(
        worker,
        &VideoWorker::frameReady,
        this,
        &VideoController::frameReady
        );

    connect(
        worker,
        &VideoWorker::finished,
        workerThread,
        &QThread::quit
        );

    connect(
        workerThread,
        &QThread::finished,
        worker,
        &QObject::deleteLater
        );

    m_workerThreads.insert(
        deviceId,
        workerThread
        );

    m_workers.insert(
        deviceId,
        worker
        );

    workerThread->start();
}