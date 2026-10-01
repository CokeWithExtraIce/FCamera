#pragma once

#include <QImage>
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

    void setImage(const QImage& image);

private:
    QImage m_image;
};