#include "WebcamInput.h"

#include <QDebug>

WebcamInput::WebcamInput(int deviceIndex)
    : m_deviceIndex(deviceIndex)
{
}

bool WebcamInput::open()
{
    qDebug() << "WebcamInput::open()"
             << "deviceIndex =" << m_deviceIndex;

    qDebug() << "WebcamInput is not implemented yet.";

    return false;
}

QImage WebcamInput::readFrame()
{
    qDebug() << "WebcamInput::readFrame()"
             << "Webcam input is not implemented yet.";

    return {};
}