#include "atlasmetadatawriter.h"
#include "commandlinehandler.h"
#include "imagepacker.h"
#include "utils.h"
#include <QDir>
#include <QDebug>
#include <QPainter>
#include <QApplication>

namespace
{

const char C_USAGE_TEXT[] =
R"***(Usage: cheetah-texture-packer [-s size] [-o OUTFILE] [options] [file|dir ...]
Avaiable options:
  -s, --size W[xH]           atlas maximum size (if it is not enough - create
                             more than 1 atlas)
  -o, --out-file OUTFILE     output atlas name
      --disable-merge        do not merge similar images
      --disable-crop         do not crop images
      --crop-threshold N     crop threshold (0-255)
      --disable-border       do not make 1px border
      --border-size          set border size in pixels
      --extrude-size         set extrude size in pixels
      --enable-rotate        enable sprites rotation
      --disable-recursion    disable recursive scan (pack only given directory)
      --square               force to make square textures
      --autosize-threshold N auto-optimize atlas size (0-100, 0 - disabled)
      --min-texture-size WxH auto-optimize minimum size
      --sort-order N         select sorting order algorithm (0-4)
  -h, -?, --help             show this help and exit)***";

struct packerData
{
    QString path, file;
};

class CommandLineHandler
{
public:
    CommandLineHandler()
        : m_imageExtensions({"bmp", "png", "jpg", "jpeg"})
    {
    }

    int run(const QStringList &argv)
    {
        int textureWidth = 512;
        int textureHeight = 512;
        bool merge = true;
        bool crop = true;
        int border = 1;
        int extrude = 0;
        bool rotate = false;
        bool recursion = true;
        bool square = false;
        bool autosize = false;
        int cropThreshold = 1;
        int autosizeThreshold = 80;
        int minTextureSizeX = 32;
        int minTextureSizeY = 32;
        int sortorder = 4;
        QString outDir = QDir::currentPath();
        QString outFile = "atlas";

        int argc = argv.count();
        for(int i = 1; i < argc; ++i)
        {
            auto isOption = [&](const char *opt) -> bool {
                return argv[i] == QLatin1String(opt);
            };

            if(isOption("--help") || isOption("-h") || isOption("-?"))
            {
                return printHelp();
            }
            else if(isOption("-s") || isOption("--size"))
            {
                ++i;
                if(i >= argc)
                {
                    return printHelp("Argument needed for option -s");
                }
                if (!parseSize(argv[i], textureWidth, textureHeight))
                {
                    if (!parseInt(argv[i], textureWidth))
                    {
                        return printHelp("Wrong texture size format");
                    }
                    else
                    {
                        textureHeight = textureWidth;
                    }
                }
                printf("Setting texture size: %dx%d\n", textureWidth, textureHeight);
            }
            else if(isOption("-o") || isOption("--out-file"))
            {
                ++i;
                if(i >= argc)
                {
                    return printHelp("Argument needed for option -o");
                }
                QFileInfo info(argv[i]);
                outFile = info.baseName();
                outDir = info.absolutePath();
            }
            else if(isOption("--disable-merge"))
            {
                merge = false;
            }
            else if(isOption("--disable-crop"))
            {
                crop = false;
            }
            else if(isOption("--disable-recursion"))
            {
                recursion = false;
            }
            else if(isOption("--square"))
            {
                square = true;
            }
            else if(isOption("--autosize-threshold"))
            {
                autosize = true;
                ++i;
                if(i >= argc)
                {
                    return printHelp("Argument needed for option --autosize-threshold");
                }
                if(!parseInt(argv[i], autosizeThreshold) ||
                        (autosizeThreshold < 0) ||
                        (autosizeThreshold > 100))
                {
                    return printHelp("Wrong autosize threshold");
                }
            }
            else if(isOption("--extrude-size"))
            {
                ++i;
                if(i >= argc)
                {
                    return printHelp("Argument needed for option --extrude-size");
                }
                if(!parseInt(argv[i], extrude) || (extrude < 0))
                {
                    return printHelp("Wrong extrude size");
                }
            }
            else if(isOption("--border-size"))
            {
                ++i;
                if(i >= argc)
                {
                    return printHelp("Argument needed for option --border-size");
                }
                if(!parseInt(argv[i], border) || (border < 0))
                {
                    return printHelp("Wrong border size");
                }
            }
            else if(isOption("--min-texture-size"))
            {
                ++i;
                if(i >= argc)
                {
                    return printHelp("Argument needed for option -min-texture-size");
                }
                if(!parseSize(argv[i], minTextureSizeX, minTextureSizeY))
                {
                    if(!parseInt(argv[i], minTextureSizeX))
                    {
                        return printHelp("Wrong texture size format");
                    }
                    else
                    {
                        minTextureSizeY = minTextureSizeX;
                    }
                }
            }
            else if(isOption("--crop-threshold"))
            {
                ++i;
                if(i >= argc)
                {
                    return printHelp("Argument needed for option --crop-threshold");
                }
                if(!parseInt(argv[i], cropThreshold) ||
                        (cropThreshold < 0) ||
                        (cropThreshold > 255))
                {
                    return printHelp("Wrong crop threshold");
                }
            }
            else if(isOption("--sort-order"))
            {
                ++i;
                if(i >= argc)
                {
                    return printHelp("Argument needed for option --sort-order");
                }
                if(!parseInt(argv[i], sortorder) ||
                        (sortorder < 0) ||
                        (sortorder > 4))
                {
                    return printHelp("Wrong sortorder must be from 0 to 4");
                }
            }
            else if(isOption("--disable-border"))
            {
                border = 0;
            }
            else if(isOption("--enable-rotate"))
            {
                rotate = true;
            }
            //dir or file
            else
            {
                QFileInfo file(argv[i]);
                if(file.isFile())
                {
                    packerData *data = new packerData;
                    data->path = file.absoluteFilePath();
                    data->file = file.fileName();
                    m_packer.addItem(data->path, data);
                }
                else if(file.isDir())
                {
                    m_topImageDir = file.absoluteFilePath();
                    recurseDirectory(file.absoluteFilePath(), recursion);
                }
            }
        }

        qDebug() << "Saving to dir" << outDir << "and file" << outFile;
        m_packer.sortOrder = sortorder;
        m_packer.border.t = 0;
        m_packer.border.l = 0;
        m_packer.border.r = border;
        m_packer.border.b = border;
        m_packer.extrude = extrude;
        m_packer.cropThreshold = crop ? cropThreshold : 0;
        m_packer.minFillRate = autosize ? autosizeThreshold : 0;
        m_packer.minTextureSizeX = minTextureSizeX;
        m_packer.minTextureSizeY = minTextureSizeY;
        m_packer.merge = merge;
        m_packer.mergeBF = false;
        m_packer.rotate = rotate;
        m_packer.square = square;
        m_packer.autosize = autosize;
        int heuristic = 1;

        QString outFormat("PNG");

        if(m_packer.images.size() == 0)
        {
            fprintf(stderr, "No images found, exitting\n");
            return 1;
        }

        m_packer.pack(heuristic, textureWidth, textureHeight);

        QList<QImage> textures;
        for(int i = 0; i < m_packer.bins.size(); i++)
        {
            QImage texture(m_packer.bins.at(i).width(), m_packer.bins.at(i).height(),
                           QImage::Format_ARGB32);
            texture.fill(Qt::transparent);
            textures << texture;
        }
        for(int j = 0; j < textures.count(); j++)
        {
            QString outputFile = outDir;
            outputFile += QDir::separator();
            outputFile += outFile;
            if(textures.count() > 1)
            {
                outputFile += QString("_") + QString::number(j + 1);
            }
            outputFile += ".atlas";
            QString imgFile = outFile;
            if(textures.count() > 1)
            {
                imgFile += QString("_") + QString::number(j + 1);
            }
            imgFile += ".";
            imgFile += outFormat.toLower();

            QStringList frameNames;
            for(int i = 0; i < m_packer.images.size(); i++)
            {
                frameNames << (static_cast<packerData *>(m_packer.images.at(i).id))->file;
            }

            AtlasMetadataWriter writer;
            if (!Utils::exportMetadata(outputFile, imgFile, textures[j].size(), j, frameNames, m_packer, writer))
            {
                fprintf(stderr, "Cannot create file %s", qPrintable(outputFile));
            }
        }

        for(int i = 0; i < m_packer.images.size(); i++)
        {
            qDebug() << "Processing" << (static_cast<packerData *>(m_packer.images.at(
                                             i).id))->file;
            if(m_packer.images.at(i).duplicateId != NULL && m_packer.merge)
            {
                continue;
            }
            QPoint pos(m_packer.images.at(i).pos.x() + m_packer.border.l,
                       m_packer.images.at(i).pos.y() + m_packer.border.t);
            QSize size;
            QRect crop;
            if(!m_packer.cropThreshold)
            {
                size = m_packer.images.at(i).size;
                crop = QRect(0, 0, size.width(), size.height());
            }
            else
            {
                size = m_packer.images.at(i).crop.size();
                crop = m_packer.images.at(i).crop;
            }
            QImage img;
            img = QImage((static_cast<packerData *>(m_packer.images.at(i).id))->path);
            if(m_packer.images.at(i).rotated)
            {
                QTransform myTransform;
                myTransform.rotate(90);
                img = img.transformed(myTransform);
                size.transpose();
                crop = QRect(m_packer.images.at(i).size.height() - crop.y() - crop.height(),
                             crop.x(), crop.height(), crop.width());
            }
            if(m_packer.images.at(i).textureId < m_packer.bins.size())
            {
                QPainter p(&textures.operator [](m_packer.images.at(i).textureId));

                if(m_packer.extrude)
                {
                    QColor color1 = QColor::fromRgba(img.pixel(crop.x(), crop.y()));
                    p.setPen(color1);
                    p.setBrush(color1);
                    if(m_packer.extrude == 1)
                    {
                        p.drawPoint(QPoint(pos.x(), pos.y()));
                    }
                    else
                    {
                        p.drawRect(QRect(pos.x(), pos.y(), m_packer.extrude - 1, m_packer.extrude - 1));
                    }

                    QColor color2 = QColor::fromRgba(img.pixel(crop.x(),
                                                     crop.y() + crop.height() - 1));
                    p.setPen(color2);
                    p.setBrush(color2);
                    if(m_packer.extrude == 1)
                    {
                        p.drawPoint(QPoint(pos.x(), pos.y() + crop.height() + m_packer.extrude));
                    }
                    else
                    {
                        p.drawRect(QRect(pos.x(), pos.y() + crop.height() + m_packer.extrude,
                                         m_packer.extrude - 1, m_packer.extrude - 1));
                    }

                    QColor color3 = QColor::fromRgba(img.pixel(crop.x() + crop.width() - 1,
                                                     crop.y()));
                    p.setPen(color3);
                    p.setBrush(color3);
                    if(m_packer.extrude == 1)
                    {
                        p.drawPoint(QPoint(pos.x() + crop.width() + m_packer.extrude, pos.y()));
                    }
                    else
                    {
                        p.drawRect(QRect(pos.x() + crop.width() + m_packer.extrude, pos.y(),
                                         m_packer.extrude - 1, m_packer.extrude - 1));
                    }

                    QColor color4 = QColor::fromRgba(img.pixel(crop.x() + crop.width() - 1,
                                                     crop.y() + crop.height() - 1));
                    p.setPen(color4);
                    p.setBrush(color4);
                    if(m_packer.extrude == 1)
                    {
                        p.drawPoint(QPoint(pos.x() + crop.width() + m_packer.extrude,
                                           pos.y() + crop.height() + m_packer.extrude));
                    }
                    else
                    {
                        p.drawRect(QRect(pos.x() + crop.width() + m_packer.extrude,
                                         pos.y() + crop.height() + m_packer.extrude, m_packer.extrude - 1,
                                         m_packer.extrude - 1));
                    }

                    p.drawImage(QRect(pos.x(), pos.y() + m_packer.extrude, m_packer.extrude,
                                      crop.height()), img, QRect(crop.x(), crop.y(), 1, crop.height()));
                    p.drawImage(QRect(pos.x() + crop.width() + m_packer.extrude,
                                      pos.y() + m_packer.extrude, m_packer.extrude, crop.height()), img,
                                QRect(crop.x() + crop.width() - 1, crop.y(), 1, crop.height()));

                    p.drawImage(QRect(pos.x() + m_packer.extrude, pos.y(), crop.width(),
                                      m_packer.extrude), img, QRect(crop.x(), crop.y(), crop.width(), 1));
                    p.drawImage(QRect(pos.x() + m_packer.extrude,
                                      pos.y() + crop.height() + m_packer.extrude, crop.width(), m_packer.extrude), img,
                                QRect(crop.x(), crop.y() + crop.height() - 1, crop.width(), 1));

                    p.drawImage(pos.x() + m_packer.extrude, pos.y() + m_packer.extrude, img, crop.x(),
                                crop.y(), crop.width(), crop.height());
                }
                else
                {
                    p.drawImage(pos.x(), pos.y(), img, crop.x(), crop.y(), crop.width(),
                                crop.height());
                }
            }
        }
        qint64 area = 0;
        for(int i = 0; i < textures.count(); i++)
        {
            area += textures.at(i).width() * textures.at(i).height();
        }
        float percent = (((float)m_packer.area / (float)area) * 100.0f);
        //        float percent2 = (float)(((float)packer.neededArea / (float)area) * 100.0f );
        printf("Atlas generated. %f%% filled, %d images missed, %d merged, %d KB\n",
               percent, m_packer.missingImages, m_packer.mergedImages, (int)((area * 4) / 1024));

        //        const char * format = qPrintable(outFormat);
        for(int i = 0; i < textures.count(); i++)
        {
            QString imgdirFile;
            imgdirFile = outDir;
            imgdirFile += QDir::separator();
            imgdirFile += outFile;
            if(textures.count() > 1)
            {
                imgdirFile += QString("_") + QString::number(i + 1);
            }
            imgdirFile += ".";
            imgdirFile += outFormat.toLower();
            textures.at(i).save(imgdirFile);
        }

        return 0;
    }

private:
    bool parseInt(const QString &str, int &value)
    {
        bool ok = false;
        const int readValue = str.toInt(&ok);
        if (ok)
        {
            value = readValue;
        }
        return ok;
    }

    bool parseSize(const QString &str, int sizeX, int sizeY)
    {
        int readCount = sscanf(str.toLatin1().data(), "%10dx%10d", &sizeX, &sizeY);
        return (readCount == 2);
    }

    int printHelp(const char *error = NULL)
    {
        if(error)
        {
            fputs(error, stderr);
        }
        printf(C_USAGE_TEXT);

        return (error) ? 1 : 0;
    }

    void recurseDirectory(const QString &dir, bool recursion)
    {
        QDir dirEnt(dir);
        QFileInfoList list = dirEnt.entryInfoList();
        for(int i = 0; i < list.count(); i++)
        {
            QFileInfo info = list[i];

            QString filePath = info.filePath();
            QString fileExt = info.suffix().toLower();
            QString name = dir + QDir::separator();
            if(recursion && info.isDir())
            {
                // recursive
                if(info.fileName() != ".." && info.fileName() != ".")
                {
                    recurseDirectory(filePath, recursion);
                }
            }
            else if(m_imageExtensions.contains(fileExt))
            {
                if(!QFile::exists(name + info.completeBaseName() + QString(".atlas")))
                {
                    packerData *data = new packerData;
                    data->path = info.absoluteFilePath();
                    data->file = filePath.replace(m_topImageDir, "");
//                        qDebug() << "Packing " << data->path << "...";
                    m_packer.addItem(data->path, data);
                }
            }
        }
    }

    QStringList m_imageExtensions;
    ImagePacker m_packer;
    QString m_topImageDir;
};
}

int doCommandLineJobs(const QStringList &args)
{
    CommandLineHandler handler;
    return handler.run(args);
}
