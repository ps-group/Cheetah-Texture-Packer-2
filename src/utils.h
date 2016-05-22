#ifndef METADATAEXPORTER_H
#define METADATAEXPORTER_H

#include <QString>
#include <QSize>
#include <memory>
#include "imetadatawriter.h"

class ImagePacker;

enum class OutFormat
{
    CHEETAH = 0,
    COCOS2DX = 1,
    CSS_SPRITE = 2
};

class Utils
{
public:
    Utils() = delete;

    static QStringList GetReadableImageFormats();
    static QStringList GetWritableImageFormats();
    static std::unique_ptr<IMetadataWriter> makeMetadataWriter(OutFormat outFormat);
    static QString getFormatExtension(OutFormat outFormat);
    static bool exportMetadata(const QString &outputFile, const QString &imgFile, const QSize &imgSize, int textureId,
                               const QStringList &frameNames, const ImagePacker &packer, IMetadataWriter &writer);
};

#endif // METADATAEXPORTER_H
