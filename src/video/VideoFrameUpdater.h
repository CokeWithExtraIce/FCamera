#pragma once

#include <QObject>
#include "../data/VideoFrame.h"

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
        bool streamingInit
        READ streamingInit
        NOTIFY streamingInitChanged
    )

    Q_PROPERTY(
        int cellId
        READ cellId
        CONSTANT
    )
public:
    explicit VideoFrameUpdater(
        int cellId,
        QMLImageProvider* imageProvider,
        QObject* parent = nullptr
        );
    int cellId() const;
    int frameVersion() const;
    bool streamingInit() const;

public slots:
    void setFrame(int cellId, const VideoFrame& frame);

signals:
    void frameVersionChanged();
    void streamingInitChanged();

private:
    int m_cellId = -1;
    QMLImageProvider* m_imageProvider = nullptr;
    int m_frameVersion = 0;
    bool m_streamingInit = false;
};