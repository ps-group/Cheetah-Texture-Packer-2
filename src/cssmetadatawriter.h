#pragma once

#include "imetadatawriter.h"
#include <QTextStream>

class CSSMetadataWriter : public IMetadataWriter
{
public:
    CSSMetadataWriter();

	void WriteTexture(const QString& path, const QSize& size) override;
	void WriteFrame(const QString& name, const QPoint& pos, const QRect& crop, const QSize& origSize, bool rotated) override;
    QByteArray ToBytes() const override;

private:
	static QString positionToString(const QPoint& pos);
    static QString coordinateToString(const int value);

    QString m_textureFilename;
    QByteArray m_bytes;
    mutable QTextStream m_stream;
};
