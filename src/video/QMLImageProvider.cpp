#include "QMLImageProvider.h"

#include <QDebug>

QMLImageProvider::QMLImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Image)
{
}

QImage QMLImageProvider::requestImage(
    const QString& id,
    QSize* size,
    const QSize& requestedSize)
{

    Q_UNUSED(requestedSize)

    // qDebug() << "QMLImageProvider::requestImage()"
    //          << "id =" << id
    //          << "size =" << m_image.size()
    //          << "isNull =" << m_image.isNull();

    if (size != nullptr) {
        *size = m_image.size();
    }

    return m_image;
}

void QMLImageProvider::setImage(const QImage& image)
{
    // qDebug() << "QMLImageProvider::setImage()";

    m_image = image;
}