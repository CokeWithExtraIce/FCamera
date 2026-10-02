#include "VideoWorker.h"

#include <QDebug>

VideoWorker::VideoWorker(QObject* parent)
    : QObject(parent)
{
}

void VideoWorker::start(const QString& filename)
{
    qDebug() << "VideoWorker::start()"
             << "filename =" << filename;

    // 영상 파일 및 Decoder 준비
    // FFmpegVideoDecoder
    if (!m_decoder.open(filename)) {
        qDebug() << "Failed to open video.";

        emit finished();
        return;
    }

    // 영상을 Frame 단위로 계속 디코딩
    while (true) {

        QImage frame = m_decoder.decodeNextFrame();

        // 더 이상 디코딩할 Frame이 없는 경우 종료
        if (frame.isNull()) {
            break;
        }

        // qDebug() << "VideoWorker: frameReady";

        // 디코딩된 Frame을 다른 객체로 전달
        emit frameReady(frame);
    }

    emit finished();
}