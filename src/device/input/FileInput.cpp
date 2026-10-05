#include "FileInput.h"

#include <QDebug>

FileInput::FileInput(const QString& filename)
    : m_filename(filename)
{
}

bool FileInput::open()
{
    qDebug() << "FileInput::open()"
             << "filename =" << m_filename;

    if (!m_decoder.open(m_filename)) {
        qDebug() << "FileInput failed to open file:"
                 << m_filename;

        return false;
    }

    qDebug() << "FileInput opened successfully.";

    return true;
}

QImage FileInput::readFrame()
{
    return m_decoder.decodeNextFrame();
}