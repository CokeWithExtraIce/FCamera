#pragma once

#include <QObject>
#include <QImage>
#include <QThread>

//https://doc.qt.io/qt-6/qthread.html
class VideoWorker;

class VideoController : public QObject
{
    Q_OBJECT

public:
    explicit VideoController(QObject* parent = nullptr);
    ~VideoController();

public slots:
    void start(const QString& filename);

signals:
    // Worker에게 영상 처리를 시작하도록 요청
    void startRequested(const QString& filename);

    // Worker가 생성한 Frame을 외부로 전달
    void frameReady(const QImage& image);

private:
    QThread m_workerThread;
    VideoWorker* m_worker = nullptr;
};