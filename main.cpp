#include "config/local_config.h"

#include "src/video/QMLImageProvider.h"
#include "src/video/VideoFrameUpdater.h"
#include "src/video/VideoController.h"

#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QThread>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // QML 애플리케이션 실행

    //QMLImageProvider imageProvider; //_CrtIsValidHeapPointer(block)
    QQmlApplicationEngine engine;
    //QMLImageProvider imageProvider; //terminated abnormally
    QMLImageProvider* imageProvider = new QMLImageProvider;
    // ref: https://doc.qt.io/qt-6/ko/qqmlengine.html
    // QMLImageProvider 등록
    engine.addImageProvider(
        "videoFrame",
        imageProvider
        );


    // QML에서 Frame 변경을 전달받기 위한 Updater 생성
    // Updater는 UI Thread에서 동작한다.
    VideoFrameUpdater* frameUpdater = new VideoFrameUpdater(
        imageProvider,
        &engine
        );

    // QML에서 Updater의 Property를 사용할 수 있도록 등록
    engine.rootContext()->setContextProperty(
        "videoFrameUpdater",
        frameUpdater
        );

    // Video Worker를 관리하는 Controller
    VideoController* videoController =
        new VideoController(
            &engine
        );

    // Controller가 전달하는 Frame을 Updater로 전달
    QObject::connect(
        videoController,
        &VideoController::frameReady,
        frameUpdater,
        &VideoFrameUpdater::setFrame
        );

    //QML 객체 생성 실패
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() {
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection
        );

    qDebug() << "Starting video thread";

    //videoThread->start(); // controller로 분리
    // 영상 처리 시작
    videoController->start(
        SAMPLE_VIDEO_PATH
        );

    // CMake에서 설정한 URI, QML 타입 입력
    engine.loadFromModule("FCamera", "Main");

    return app.exec();
}