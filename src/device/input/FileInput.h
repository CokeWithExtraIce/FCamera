#pragma once

#include "DeviceInput.h"

#include "../../video/FFmpegVideoDecoder.h"

#include <QString>

class FileInput : public DeviceInput
{
public:
    explicit FileInput(const QString& filename);

    bool open() override;
    QImage readFrame() override;

private:
    QString m_filename;
    FFmpegVideoDecoder m_decoder;
};