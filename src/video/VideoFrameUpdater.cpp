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

//Qt Property에서는 일반적으로 Property이름과 get 함수 이름 똑같이 사용
// videoFrameUpdater.frameVersion이 아래 함수 호출
int VideoFrameUpdater::frameVersion() const
{
    return m_frameVersion;
}

bool VideoFrameUpdater::hasFrame() const
{
    return m_hasFrame;
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

    m_hasFrame = true;

    // qDebug() << "VideoFrameUpdater:"
    //          << "frameVersion =" << m_frameVersion
    //          << "hasFrame =" << m_hasFrame;

    emit hasFrameChanged();
    // qDebug() << "frameVersion =" << m_frameVersion;

    emit frameVersionChanged();
}