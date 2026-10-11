#pragma once

#include <QImage>
#include <QHash>
#include <QMutex>
#include <QQuickImageProvider>

class QMLImageProvider : public QQuickImageProvider
{
public:
    QMLImageProvider();

    QImage requestImage(
        const QString& id,
        QSize* size,
        const QSize& requestedSize
        ) override;

    // 화면 영역 식별 1~4 : cellId
    void setImage(int cellId, const QImage& image);

private:
    QHash<int, QImage> m_images;
    mutable QMutex m_mutex;
};