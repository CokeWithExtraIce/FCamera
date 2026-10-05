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

    Q_PROPERTY(
        bool hasFrame
        READ hasFrame
        NOTIFY hasFrameChanged
    )


public:
    explicit VideoFrameUpdater(
        QMLImageProvider* imageProvider,
        QObject* parent = nullptr
        );

    int frameVersion() const;
    bool hasFrame() const;

public slots:
    void setFrame(const QImage& image);

signals:
    void frameVersionChanged();
    void hasFrameChanged();

private:
    QMLImageProvider* m_imageProvider = nullptr;
    int m_frameVersion = 0;
    bool m_hasFrame = false;
};