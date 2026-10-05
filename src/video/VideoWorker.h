#pragma once

#include <QObject>
#include <QImage>

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
    void frameReady(const QImage& image);
    void finished();

private:
    Device* m_device = nullptr;
};