#include "VideoController.h"

#include "VideoWorker.h"

VideoController::VideoController(QObject* parent)
    : QObject(parent)
{
    // 영상 처리를 담당할 Worker 생성
    m_worker = new VideoWorker;

    // Worker를 별도 Thread로 이동
    m_worker->moveToThread(&m_workerThread);

    // Controller → Worker
    // startRequested가 발생하면 Worker의 start() 실행
    connect(
        this,
        &VideoController::startRequested,
        m_worker,
        &VideoWorker::start
        );

    // Worker → Controller -> FrameUpdater
    // Worker의 캡슐화(은닉)와 Controller 의존성 분리 with FrameUpdate
    connect(
        m_worker,
        &VideoWorker::frameReady,
        this,
        &VideoController::frameReady
        );

    // Worker의 영상 처리가 끝나면 Thread 종료
    connect(
        m_worker,
        &VideoWorker::finished,
        &m_workerThread,
        &QThread::quit
        );

    // Thread가 종료되면 Worker 삭제
    connect(
        &m_workerThread,
        &QThread::finished,
        m_worker,
        &QObject::deleteLater
        );

    // Worker Thread 시작
    m_workerThread.start();
}

VideoController::~VideoController()
{
    // Worker Thread의 Event Loop 종료
    m_workerThread.quit();

    // Thread가 완전히 종료될 때까지 대기
    m_workerThread.wait();
}

void VideoController::start(const QString& filename)
{
    emit startRequested(filename);
}