#include "DisplayManager.h"

#include <QDebug>

DisplayManager::DisplayManager(
    const FixedDeviceSelection& selection,
    QObject* parent)
    : QObject(parent),
    m_cells{
        DisplayCell(0),
        DisplayCell(1),
        DisplayCell(2),
        DisplayCell(3)
    }
{
    m_cells[0].setDeviceId(selection.firstDeviceId);
    m_cells[1].setDeviceId(selection.secondDeviceId);

    qDebug() << "DisplayManager initialized"
             << "Cell 0:" << m_cells[0].deviceId()
             << "Cell 1:" << m_cells[1].deviceId();
}

const DisplayCell* DisplayManager::cell(int cellId) const
{
    if (cellId < 0 || cellId >= CELL_COUNT) {
        return nullptr;
    }

    return &m_cells[static_cast<std::size_t>(cellId)];
}

void DisplayManager::setScheduledDevices(const QList<int>& deviceIds)
{
    for (int cellId = 2; cellId < CELL_COUNT; ++cellId) {
        const int deviceIndex = cellId - 2;

        if (deviceIndex < deviceIds.size()) {
            m_cells[cellId].setDeviceId(deviceIds[deviceIndex]);
        } else {
            m_cells[cellId].setDeviceId(-1);
        }
    }

    qDebug() << "Scheduled device assignments:"
             << "Cell 2:" << m_cells[2].deviceId()
             << "Cell 3:" << m_cells[3].deviceId();
}

void DisplayManager::processFrame(const VideoFrame& frame)
{
    for (DisplayCell& DisplayCell : m_cells) {
        if (!DisplayCell.updateFrame(frame)) {
            continue;
        }

        emit cellFrameReady(
            DisplayCell.cellId(),
            frame
            );

        return;
    }
}