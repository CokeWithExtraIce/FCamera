#include "QMLImageProvider.h"
#include <QMutexLocker>
#include <QDebug>

// QHash<int, QImage> m_images;
// mutable QMutex m_mutex;

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

    // 이미지 URL의 첫 번째 경로 요소를 cellId로 사용한다.
    // 예: image://videoFrame/0/15 -> display ID 0
    const QString cellIdText = id.section('/', 0, 0);

    bool conversionSucceeded = false;
    const int cellId = cellIdText.toInt(&conversionSucceeded);

    if (!conversionSucceeded) {
        if (size != nullptr) {
            *size = QSize();
        }
        return {};
    }

    QImage image;

    {QMutexLocker locker(&m_mutex);//lock
        image = m_images.value(cellId);
    }//unlock

    if (size != nullptr) {
        *size = image.size();
    }

    return image;

}

void QMLImageProvider::setImage(
    int cellId,
    const QImage& image)
{
    // qDebug() << "QMLImageProvider::setImage()";
    QMutexLocker locker(&m_mutex);
    m_images.insert(cellId, image);
}