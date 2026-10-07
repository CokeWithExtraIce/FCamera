#pragma once

#include <QObject>
#include "../data/VideoFrame.h"

class Device;

class VideoWorker : public QObject
{
    Q_OBJECT

public:
    explicit VideoWorker(
        Device* device,
        QObject* parent = nullptr
        );

public slots:
    void start();

signals:
    void frameReady(const VideoFrame& frame);
    void finished();

private:
    Device* m_device = nullptr;
};