#include "utils.h"
#include "imagepacker.h"
#include "imetadatawriter.h"
#include "atlasmetadatawriter.h"
#include "cocosmetadatawriter.h"
#include <QRect>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

std::unique_ptr<IMetadataWriter> Utils::makeMetadataWriter(OutFormat outFormat)
{
    switch (outFormat)
    {
    case OutFormat::CHEETAH:
        return std::make_unique<AtlasMetadataWriter>();
    case OutFormat::COCOS2DX:
        return std::make_unique<CocosMetadataWriter>();
    case OutFormat::CSS_SPRITE:
        // FIXME: not implemented
        break;
    }
    Q_ASSERT(false); // unhandled enum option.
    QMessageBox::critical(0, QObject::tr("Internal Error"),
                          QObject::tr("Exporting to selected format not implemented. Falling back to Cheetah format"));
    return std::make_unique<AtlasMetadataWriter>();
}

QString Utils::getFormatExtension(OutFormat outFormat)
{
    switch (outFormat)
    {
    case OutFormat::CHEETAH:
        return ".atlas";
    case OutFormat::COCOS2DX:
        return ".plist";
    case OutFormat::CSS_SPRITE:
        return ".css";
    }
    Q_ASSERT(false); // unhandled enum option.
    return ".txt";
}

bool Utils::exportMetadata(const QString &outputFile, const QString &imgFile, const QSize &imgSize, int textureId,
                           const QStringList &frameNames, const ImagePacker &packer, IMetadataWriter &writer)
{
    writer.WriteTexture(imgFile, imgSize);
    for(int i = 0; i < packer.images.size(); i++)
    {
        if(packer.images.at(i).textureId != textureId)
        {
            continue;
        }
        QPoint pos(packer.images.at(i).pos.x() + packer.border.l + packer.extrude,
                   packer.images.at(i).pos.y() + packer.border.t + packer.extrude);
        QSize size, origSize;
        QRect crop;
        origSize = packer.images.at(i).size;
        if(!packer.cropThreshold)
        {
            size = packer.images.at(i).size;
            crop = QRect(0, 0, size.width(), size.height());
        }
        else
        {
            size = packer.images.at(i).crop.size();
            crop = packer.images.at(i).crop;
        }
        if(packer.images.at(i).rotated)
        {
            size.transpose();
            crop = QRect(crop.y(), crop.x(), crop.height(), crop.width());
        }
        QString frameName = frameNames[i];
        writer.WriteFrame(frameName, pos, crop, origSize, packer.images.at(i).rotated);
    }

    QFile positionsFile(outputFile);
    if(!positionsFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return false;
    }
    QTextStream out(&positionsFile);
    out << writer.ToBytes();

    return true;
}
