#pragma once

#include <QObject>
#include <QImage>

class QMLImageProvider;

class VideoFrameUpdater : public QObject
{
    Q_OBJECT

    Q_PROPERTY(
        int frameVersion
            READ frameVersion
                NOTIFY frameVersionChanged
        )

public:
    explicit VideoFrameUpdater(
        QMLImageProvider* imageProvider,
        QObject* parent = nullptr
        );

    int frameVersion() const;

public slots:
    void setFrame(const QImage& image);

signals:
    void frameVersionChanged();

private:
    QMLImageProvider* m_imageProvider = nullptr;
    int m_frameVersion = 0;
};