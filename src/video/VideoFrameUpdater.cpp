#include "VideoFrameUpdater.h"

#include "QMLImageProvider.h"

#include <QDebug>

VideoFrameUpdater::VideoFrameUpdater(
    int cellId,
    QMLImageProvider* imageProvider,
    QObject* parent)
    : QObject(parent),
    m_cellId(cellId),
    m_imageProvider(imageProvider)
{
}

// Qt Property에서는 일반적으로 Property이름과 get 함수 이름 똑같이 사용
// videoFrameUpdater.frameVersion이 아래 함수 호출
int VideoFrameUpdater::cellId() const
{
    return m_cellId;
}
int VideoFrameUpdater::frameVersion() const
{
    return m_frameVersion;
}

bool VideoFrameUpdater::streamingInit() const
{
    return m_streamingInit;
}

void VideoFrameUpdater::setFrame(
    int cellId,
    const VideoFrame& frame)
{
    // qDebug() << "VideoFrameUpdater::setFrame()";

    if (cellId != m_cellId) {
        return;
    }

    if (m_imageProvider == nullptr ||
        frame.deviceId < 0 ||
        frame.image.isNull()) {
        return;
    }

    // Image Provider에 새로운 Frame 저장
    m_imageProvider->setImage(m_cellId, frame.image);

    // 최초 프레임 수신 시 스트리밍 초기화 완료 상태로 변경
    if (!m_streamingInit) {
        m_streamingInit = true;
        emit streamingInitChanged();
    }
    // QML이 새로운 Image를 요청하도록 버전 증가
    ++m_frameVersion;
    emit frameVersionChanged();
}