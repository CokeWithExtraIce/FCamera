#pragma once

#include <QObject>
#include <QImage>
#include <QString>

#include "FFmpegVideoDecoder.h"

//https://doc.qt.io/qt-6/qthread.html
class VideoWorker : public QObject
{
    Q_OBJECT

public:
    explicit VideoWorker(QObject* parent = nullptr);

public slots:
    void start(const QString& filename);

signals:
    void frameReady(const QImage& image);
    void finished();

private:
    FFmpegVideoDecoder m_decoder;
};