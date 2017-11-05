#include "cssmetadatawriter.h"
#include <QFileInfo>

CSSMetadataWriter::CSSMetadataWriter()
    : m_stream(&m_bytes)
{
}

void CSSMetadataWriter::WriteTexture(const QString& path, const QSize& size)
{
    m_textureFilename = QFileInfo(path).fileName();
    (void)size;
}

void CSSMetadataWriter::WriteFrame(const QString& name, const QPoint& pos, const QRect& crop, const QSize& origSize, bool rotated)
{
    (void)crop;
    (void)origSize;
    (void)rotated;
    QString className = QFileInfo(name).baseName();
    const QLatin1Char underscore('_');
    for (QChar ch : className)
    {
        if (!ch.isLetterOrNumber())
        {
            ch = underscore;
        }
    }

    m_stream << "." << className << "\n{\n"
             << "    width: " << coordinateToString(crop.width()) << ";\n"
             << "    height: " << coordinateToString(crop.height()) << ";\n"
             << "    background: url('" << m_textureFilename << "') " << positionToString(pos) << ";\n"
             << "}\n";
}

QByteArray CSSMetadataWriter::ToBytes() const
{
    m_stream.flush();
    return m_bytes;
}

QString CSSMetadataWriter::positionToString(const QPoint& pos)
{
    return coordinateToString(pos.x()) + " " + coordinateToString(pos.y());
}

QString CSSMetadataWriter::coordinateToString(const int value)
{
    if (value == 0)
    {
        return "0";
    }
    return QString::number(value) + "px";
}
