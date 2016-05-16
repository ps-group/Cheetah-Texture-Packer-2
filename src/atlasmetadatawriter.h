#ifndef ATLASMETADATAWRITER_H
#define ATLASMETADATAWRITER_H

#include "imetadatawriter.h"
#include <QTextStream>

class AtlasMetadataWriter : public IMetadataWriter
{
public:
    AtlasMetadataWriter();

    void WriteTexture(const QString &path, const QSize &size) override;
    void WriteFrame(const QString &name, const QPoint &pos, const QRect &crop, const QSize &origSize, bool rotated) override;
    QByteArray ToBytes()const override;

private:
    QByteArray m_bytes;
    mutable QTextStream m_stream;
};

#endif // ATLASMETADATAWRITER_H
