#include "config/local_config.h"

#include "src/device/Device.h"
#include "src/device/DeviceManager.h"
#include "src/device/input/FileInput.h"

#include "src/video/QMLImageProvider.h"
#include "src/video/VideoFrameUpdater.h"
#include "src/video/VideoController.h"

#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <memory>

// #include<QFileInfo>

//샘플 영상등록(임시 함수)
bool registerFileDevice(DeviceManager& deviceManager, int deviceId, const QString& filePath);

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);


    DeviceManager deviceManager;
    QQmlApplicationEngine engine;
    QMLImageProvider* imageProvider = new QMLImageProvider;

    engine.addImageProvider(
        "videoFrame",
        imageProvider
        );


    // QML에서 Frame 변경을 전달받기 위한 Updater 생성
    // Updater는 UI Thread에서 동작.
    VideoFrameUpdater* frameUpdater = new VideoFrameUpdater(
        imageProvider,
        &engine
        );

    // QML에서 Updater의 Property를 사용할 수 있도록 등록
    engine.rootContext()->setContextProperty(
        "videoFrameUpdater",
        frameUpdater
        );


    //샘플 영상 등록

    for (int i = 0; i < SAMPLE_VIDEO_COUNT; ++i) {
        if (!registerFileDevice(
                deviceManager,
                i + 1,
                SAMPLE_VIDEO_PATHS[i])) {

            qDebug() << "Failed to register device:"
                     << i + 1;

            return -1;
        }
    }



    // Controller -> Updater (Frame)
    VideoController* videoController =
        new VideoController(
            &deviceManager,
            &engine
            );    QObject::connect(
        videoController,
        &VideoController::frameReady,
        frameUpdater,
        &VideoFrameUpdater::setFrame
        );

    // QML 객체 생성 실패
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

    // Device ID 1번 영상 처리 시작
    videoController->start(1);

    // CMake에서 설정한 URI, QML 타입 입력
    engine.loadFromModule("FCamera", "Main");

    return app.exec();
}

bool registerFileDevice(DeviceManager& deviceManager, int deviceId, const QString& filePath){

    std::unique_ptr<FileInput> fileInput =
        std::make_unique<FileInput>(
            filePath
            );

    std::unique_ptr<Device> fileDevice =
        std::make_unique<Device>(
            deviceId,
            std::move(fileInput)
            );

    return deviceManager.addDevice(
        std::move(fileDevice)
        );
}