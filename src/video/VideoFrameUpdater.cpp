#include "VideoFrameUpdater.h"

#include "QMLImageProvider.h"

#include <QDebug>

VideoFrameUpdater::VideoFrameUpdater(
    QMLImageProvider* imageProvider,
    QObject* parent)
    : QObject(parent),
    m_imageProvider(imageProvider)
{
}

int VideoFrameUpdater::frameVersion() const
{
    return m_frameVersion;
}

void VideoFrameUpdater::setFrame(const QImage& image)
{
    // qDebug() << "VideoFrameUpdater::setFrame()";
    if (m_imageProvider == nullptr) {
        return;
    }

    // Image Provider에 새로운 Frame 저장
    m_imageProvider->setImage(image);

    // QML이 새로운 Image를 요청하도록 버전 증가
    ++m_frameVersion;

    // qDebug() << "frameVersion =" << m_frameVersion;

    emit frameVersionChanged();
}