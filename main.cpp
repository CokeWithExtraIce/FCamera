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

#include "src/data/FixedDeviceSelection.h"
#include "src/display/DisplayManager.h"

#include "src/detection/ObjectDetectionMock.h"
#include "src/scheduler/SchedulingAlgorithm.h"

// 샘플 영상등록(임시 함수)
bool registerFileDevice(DeviceManager& deviceManager, int deviceId, const QString& filePath);
bool registerSampleDevice(DeviceManager& deviceManager);

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    DeviceManager deviceManager;

    if (!registerSampleDevice(deviceManager))
        return -1;
    QQmlApplicationEngine engine;
    QMLImageProvider* imageProvider = new QMLImageProvider;

    engine.addImageProvider(
        "videoFrame",
        imageProvider
        );

    // 고정 cell 설정
    FixedDeviceSelection fixedSelection;
    fixedSelection.firstDeviceId = 1;
    fixedSelection.secondDeviceId = 2;

    DisplayManager displayManager(fixedSelection);

    // 스케쥴링
    ObjectDetectionMock detectionMock;
    SchedulingAlgorithm schedulingAlgorithm(fixedSelection);

    QObject::connect(
        &detectionMock,
        &ObjectDetectionMock::scoresReady,
        &schedulingAlgorithm,
        &SchedulingAlgorithm::onScoresReady);

    QObject::connect(
        &schedulingAlgorithm,
        &SchedulingAlgorithm::devicesSelected,
        &displayManager,
        &DisplayManager::setScheduledDevices);

    // QML에서 Frame 변경을 전달받기 위한 Updater 생성
    // Updater는 UI Thread에서 동작.

    VideoFrameUpdater* cell0Updater =
        new VideoFrameUpdater(0, imageProvider, &engine);

    VideoFrameUpdater* cell1Updater =
        new VideoFrameUpdater(1, imageProvider, &engine);

    VideoFrameUpdater* cell2Updater =
        new VideoFrameUpdater(2, imageProvider, &engine);

    VideoFrameUpdater* cell3Updater =
        new VideoFrameUpdater(3, imageProvider, &engine);

    engine.rootContext()->setContextProperty(
        "cell0Updater",
        cell0Updater
        );

    engine.rootContext()->setContextProperty(
        "cell1Updater",
        cell1Updater
        );

    engine.rootContext()->setContextProperty(
        "cell2Updater",
        cell2Updater
        );

    engine.rootContext()->setContextProperty(
        "cell3Updater",
        cell3Updater
        );

    // 영상 처리
    // Controller -> Updater (Frame)
    VideoController videoController(
        &deviceManager
        );

    // 모든 입력 프레임을 DisplayManager로 전달
    QObject::connect(
        &videoController,
        &VideoController::frameReady,
        &displayManager,
        &DisplayManager::processFrame
        );

    // CellId에 해당하는 프레임 갱신
    QObject::connect(
        &displayManager,
        &DisplayManager::cellFrameReady,
        cell0Updater,
        &VideoFrameUpdater::setFrame
        );

    QObject::connect(
        &displayManager,
        &DisplayManager::cellFrameReady,
        cell1Updater,
        &VideoFrameUpdater::setFrame
        );

    QObject::connect(
        &displayManager,
        &DisplayManager::cellFrameReady,
        cell2Updater,
        &VideoFrameUpdater::setFrame
        );

    QObject::connect(
        &displayManager,
        &DisplayManager::cellFrameReady,
        cell3Updater,
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

    qDebug() << "Starting video worker";



    // Device ID 영상 처리 시작
    // videoController->start(1);
    for (int deviceId = 1;
         deviceId <= SAMPLE_VIDEO_COUNT;
         ++deviceId) {

        videoController.start(deviceId);
    }
    // CMake에서 설정한 URI, QML 타입 입력
    engine.loadFromModule("FCamera", "Main");

    detectionMock.start();
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

bool registerSampleDevice(DeviceManager& deviceManager){
    for (int i = 0; i < SAMPLE_VIDEO_COUNT; ++i) {
        if (!registerFileDevice(
                deviceManager,
                i + 1,
                SAMPLE_VIDEO_PATHS[i])) {

            qDebug() << "Failed to register device:"
                     << i + 1;

            return false;
        }
    }
    return true;
}