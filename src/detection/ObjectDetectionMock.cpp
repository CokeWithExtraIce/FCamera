#include "ObjectDetectionMock.h"

ObjectDetectionMock::ObjectDetectionMock(QObject* parent)
    : QObject(parent)
{
    m_timer.setInterval(10000);

    connect(
        &m_timer,
        &QTimer::timeout,
        this,
        &ObjectDetectionMock::publishScores);
}

void ObjectDetectionMock::start()
{
    if (m_timer.isActive()) {
        return;
    }

    // 초기값 세팅
    // 10초마다 갱신.
    publishScores();
    m_timer.start();
}

void ObjectDetectionMock::publishScores()
{
    QHash<int, double> scores;

    switch (m_callCount % 3) {
    case 0:
        scores = {
            {1, 1}, {2, 2}, {3, 3},
            {4, 4}, {5, 5}, {6, 6}
        };
        break;

    case 1:
        scores = {
            {1, 80}, {2, 95}, {3, 65},
            {4, 98}, {5, 90}, {6, 70}
        };
        break;

    default:
        scores = {
            {1, 70}, {2, 80}, {3, 88},
            {4, 75}, {5, 96}, {6, 91}
        };
        break;
    }

    ++m_callCount;

    emit scoresReady(scores);
}