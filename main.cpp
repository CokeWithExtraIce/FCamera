#include "config/local_config.h"
#include "src/video/FFmpegVideoDecoder.h"

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
    if (!decoder.decodeFirstFrame()) {
        return -1;
    }

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