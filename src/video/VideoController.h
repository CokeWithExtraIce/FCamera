#pragma once

#include <QObject>
#include <QThread>
#include <QHash>
#include "../data/VideoFrame.h"

class DeviceManager;
class VideoWorker;

class VideoController : public QObject
{
    Q_OBJECT

public:
    explicit VideoController(
        DeviceManager* deviceManager,
        QObject* parent = nullptr
        );

    ~VideoController();

public slots:
    void start(int deviceId);

signals:
    // Worker에게 영상 처리를 시작하도록 요청
    // void startRequested();

    // Worker가 생성한 Frame을 VideoFrameUpdater로 전달
    void frameReady(const VideoFrame& frame);

private:
    DeviceManager* m_deviceManager = nullptr;

    // QThread m_workerThread;
    QHash<int, QThread*> m_workerThreads;
    QHash<int, VideoWorker*> m_workers;
    // VideoWorker* m_worker = nullptr;
};