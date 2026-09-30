#include "config/local_config.h"
#include "src/video/FFmpegVideoDecoder.h"

#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // "FFmpegVideoDecoder.h"
    FFmpegVideoDecoder decoder;

    // 임시 mp4파일 open
    if (!decoder.open(SAMPLE_VIDEO_PATH)) {
        return -1;
    }

    // 현재 단계에서는 첫번째 Frame까지만 디코딩하여
    // FFmpeg → Decoder 파이프라인이 정상적으로 동작하는지 확인
    QImage firstFrame = decoder.decodeFirstFrame();

    if (firstFrame.isNull()) {
        qDebug() << "Failed to decode first frame.";
        return -1;
    }
    // 디코딩된 Frame이 QImage로 정상적으로 변환되었는지 확인
    qDebug() << "First frame converted to QImage:"
             << "width =" << firstFrame.width()
             << "height =" << firstFrame.height()
             << "format =" << firstFrame.format();

    // QML 애플리케이션 실행
    QQmlApplicationEngine engine;

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() {
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection
        );
    // CMake에서 설정한 URI, QML 타입 입력
    engine.loadFromModule("FCamera", "Main");

    return app.exec();
}