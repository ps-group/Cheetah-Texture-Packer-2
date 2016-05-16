#ifndef METADATAEXPORTER_H
#define METADATAEXPORTER_H

#include <QString>
#include <QSize>

class ImagePacker;
class IMetadataWriter;

class Utils
{
public:
    Utils() = delete;

    static bool exportMetadata(const QString &outputFile, const QString &imgFile, const QSize &imgSize, int textureId,
                               const QStringList &frameNames, const ImagePacker &packer, IMetadataWriter &writer);
};

#endif // METADATAEXPORTER_H
