#ifndef IMETADATAWRITER_H
#define IMETADATAWRITER_H

#include <QString>
#include <QRect>

class IMetadataWriter
{
public:
    virtual ~IMetadataWriter() = default;
    virtual void WriteTexture(const QString &path, const QSize &size) = 0;
    virtual void WriteFrame(const QString &name, const QPoint &pos, const QRect &crop,
                            const QSize &origSize, bool rotated) = 0;
    virtual QByteArray ToBytes()const = 0;
};

#endif // IMETADATAWRITER_H
