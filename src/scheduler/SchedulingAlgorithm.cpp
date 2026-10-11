#include "SchedulingAlgorithm.h"

#include <QDebug>
#include <QPair>

#include <algorithm>

SchedulingAlgorithm::SchedulingAlgorithm(
    const FixedDeviceSelection& fixedSelection,
    QObject* parent)
    : QObject(parent),
    m_fixedSelection(fixedSelection)
{
}

void SchedulingAlgorithm::onScoresReady(
    const QHash<int, double>& scores)
{
    QList<QPair<int, double>> candidates;

    for (auto it = scores.constBegin();
         it != scores.constEnd();
         ++it) {
        const int deviceId = it.key();

        if (deviceId == m_fixedSelection.firstDeviceId ||
            deviceId == m_fixedSelection.secondDeviceId) {
            continue;
        }

        candidates.append(qMakePair(deviceId, it.value()));
    }

    std::sort(
        candidates.begin(),
        candidates.end(),
        [](const QPair<int, double>& a,
           const QPair<int, double>& b) {
            if (a.second != b.second) {
                return a.second > b.second;
            }

            return a.first < b.first;
        });

    QList<int> selectedDeviceIds;

    for (int i = 0;
         i < candidates.size() && selectedDeviceIds.size() < 2;
         ++i) {
        selectedDeviceIds.append(candidates[i].first);
    }

    qDebug() << "Scheduled device IDs:" << selectedDeviceIds;

    emit devicesSelected(selectedDeviceIds);
}