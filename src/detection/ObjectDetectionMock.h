#pragma once

#include <QObject>
#include <QHash>
#include <QTimer>

class ObjectDetectionMock : public QObject
{
    Q_OBJECT

public:
    explicit ObjectDetectionMock(QObject* parent = nullptr);

    void start();

signals:
    void scoresReady(const QHash<int, double>& scores);

private slots:
    void publishScores();

private:
    QTimer m_timer;
    int m_callCount = 0;
};