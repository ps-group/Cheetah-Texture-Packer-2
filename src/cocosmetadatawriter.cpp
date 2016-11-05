#include "cocosmetadatawriter.h"
#include <QXmlStreamWriter>

namespace
{
const char PLIST_DTD[] = R"***(<!DOCTYPE plist PUBLIC "-//Apple Computer//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">)***";

void writeKey(QXmlStreamWriter &xml, const QString &key)
{
    xml.writeTextElement("key", key);
}

void writeInteger(QXmlStreamWriter &xml, int value)
{
    xml.writeTextElement("integer", QString::number(value));
}

void writeReal(QXmlStreamWriter &xml, double value)
{
    xml.writeTextElement("real", QString::number(value));
}

void writeString(QXmlStreamWriter &xml, const QString &value)
{
    xml.writeTextElement("string", value);
}

void writeBool(QXmlStreamWriter &xml, bool value)
{
    xml.writeEmptyElement(value ? "true" : "false");
}

void writeSize(QXmlStreamWriter &xml, const QSize &value)
{
    char buffer[80];
    sprintf(buffer, "{%d,%d}", value.width(), value.height());
    writeString(xml, buffer);
}
}

CocosMetadataWriter::CocosMetadataWriter()
{
}

void CocosMetadataWriter::WriteTexture(const QString &path, const QSize &size)
{
    m_meta.texturePath = path;
    m_meta.textureSize = size;
}

void CocosMetadataWriter::WriteFrame(const QString &name, const QPoint &pos, const QRect &crop, const QSize &origSize, bool rotated)
{
    FrameData data;
    data.name = name;
    data.pos = pos;
    data.crop = crop;
    data.origSize = origSize;
    data.rotated = rotated;
    m_frames << data;
}

QByteArray CocosMetadataWriter::ToBytes() const
{
    QByteArray content;
    QXmlStreamWriter xml(&content);
    xml.setAutoFormatting(true);
    xml.setAutoFormattingIndent(-1);

    xml.writeStartDocument("1.0");
    xml.writeDTD(PLIST_DTD);
    xml.writeStartElement("plist");
    xml.writeAttribute("version", "1.0");
    xml.writeStartElement("dict");
    writeFrames(xml);
    writeMetadata(xml);
    writeTexture(xml);
    xml.writeEndElement(); // dict
    xml.writeEndElement(); // plist
    xml.writeEndDocument();

    return content;
}

void CocosMetadataWriter::writeFrames(QXmlStreamWriter &xml) const
{
    writeKey(xml, "frames");
    xml.writeStartElement("dict");
    for (const FrameData &frame : m_frames)
    {
        writeKey(xml, frame.name);
        xml.writeStartElement("dict");
        writeKey(xml, "width");
        writeInteger(xml, frame.crop.width());
        writeKey(xml, "height");
        writeInteger(xml, frame.crop.height());
        writeKey(xml, "originalWidth");
        writeInteger(xml, frame.origSize.width());
        writeKey(xml, "originalHeight");
        writeInteger(xml, frame.origSize.height());
        writeKey(xml, "x");
        writeInteger(xml, frame.pos.x());
        writeKey(xml, "y");
        writeInteger(xml, frame.pos.y());
        writeKey(xml, "offsetX");
        writeReal(xml, frame.crop.x());
        writeKey(xml, "offsetY");
        writeReal(xml, frame.crop.y());
        writeKey(xml, "rotated");
        writeBool(xml, frame.rotated);
        xml.writeEndElement(); // dict
    }
    xml.writeEndElement(); // dict
}

void CocosMetadataWriter::writeMetadata(QXmlStreamWriter &xml) const
{
    writeKey(xml, "metadata");
    xml.writeStartElement("dict");
    writeKey(xml, "format");
    writeInteger(xml, 0);
    writeKey(xml, "textureFileName");
    writeString(xml, m_meta.texturePath);
    writeKey(xml, "realTextureFileName");
    writeString(xml, m_meta.texturePath);
    writeKey(xml, "size");
    writeSize(xml, m_meta.textureSize);

    xml.writeEndElement(); // dict
}

void CocosMetadataWriter::writeTexture(QXmlStreamWriter &xml) const
{
    writeKey(xml, "texture");
    xml.writeStartElement("dict");
    writeKey(xml, "width");
    writeInteger(xml, m_meta.textureSize.width());
    writeKey(xml, "height");
    writeInteger(xml, m_meta.textureSize.height());
    xml.writeEndElement(); // dict
}
