#ifndef COCOSMETADATAWRITER_H
#define COCOSMETADATAWRITER_H

#include "imetadatawriter.h"
#include <QList>
QT_BEGIN_NAMESPACE
class QXmlStreamWriter;
QT_END_NAMESPACE

class CocosMetadataWriter : public IMetadataWriter
{
public:
    CocosMetadataWriter();

	void WriteTexture(const QString& path, const QSize& size) override;
	void WriteFrame(const QString& name, const QPoint& pos, const QRect& crop, const QSize& origSize, bool rotated) override;
    QByteArray ToBytes() const override;

private:
    struct FrameData
    {
        QString name;
        QPoint pos;
        QRect crop;
        QSize origSize;
        bool rotated = false;
    };

    struct Metadata
    {
        QString texturePath;
        QSize textureSize;
    };

	void writeFrames(QXmlStreamWriter& xml) const;
	void writeMetadata(QXmlStreamWriter& xml) const;
	void writeTexture(QXmlStreamWriter& xml) const;

    QList<FrameData> m_frames;
    Metadata m_meta;
};

#endif // COCOSMETADATAWRITER_H
