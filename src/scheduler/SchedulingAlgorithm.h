#pragma once

#include "../data/FixedDeviceSelection.h"

#include <QObject>
#include <QHash>
#include <QList>

class SchedulingAlgorithm : public QObject
{
    Q_OBJECT

public:
    explicit SchedulingAlgorithm(
        const FixedDeviceSelection& fixedSelection,
        QObject* parent = nullptr);

public slots:
    void onScoresReady(const QHash<int, double>& scores);

signals:
    void devicesSelected(const QList<int>& deviceIds);

private:
    FixedDeviceSelection m_fixedSelection;
};