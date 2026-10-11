#pragma once

#include <QObject>
#include <array>

#include "../data/FixedDeviceSelection.h"
#include "../data/VideoFrame.h"
#include "DisplayCell.h"

class DisplayManager : public QObject
{
    Q_OBJECT

public:
    static constexpr int CELL_COUNT = 4;

    explicit DisplayManager(
        const FixedDeviceSelection& selection,
        QObject* parent = nullptr
        );

    const DisplayCell* cell(int cellId) const;

public slots:
    void processFrame(const VideoFrame& frame);

signals:
    void cellFrameReady(int cellId, const VideoFrame& frame);

private:
    std::array<DisplayCell, CELL_COUNT> m_cells;
};