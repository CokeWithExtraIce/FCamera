#include "QMLImageProvider.h"

QMLImageProvider::QMLImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Image)
{
}

QImage QMLImageProvider::requestImage(
    const QString& id,
    QSize* size,
    const QSize& requestedSize)
{
    Q_UNUSED(id)
    Q_UNUSED(requestedSize)

    if (size != nullptr) {
        *size = m_image.size();
    }

    return m_image;
}

void QMLImageProvider::setImage(const QImage& image)
{
    m_image = image;
}