#include "atlasmetadatawriter.h"

AtlasMetadataWriter::AtlasMetadataWriter()
    : m_stream(&m_bytes)
{
}

void AtlasMetadataWriter::WriteTexture(const QString &path, const QSize &size)
{
    (void)size;
    m_stream << "textures: " << path << "\n";
}

void AtlasMetadataWriter::WriteFrame(const QString &name, const QPoint &pos, const QRect &crop,
                                     const QSize &origSize, bool rotated)
{
    m_stream << name << "\t" <<
     pos.x() << "\t" <<
     pos.y() << "\t" <<
     crop.width() << "\t" <<
     crop.height() << "\t" <<
     crop.x() << "\t" <<
     crop.y() << "\t" <<
     origSize.width() << "\t" <<
     origSize.height() << "\t" <<
     (rotated ? "r" : "") << "\n";
}

QByteArray AtlasMetadataWriter::ToBytes() const
{
    m_stream.flush();
    return m_bytes;
}
