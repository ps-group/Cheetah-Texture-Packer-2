#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGlobal>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#else
#include <QListWidget>
#include <QMainWindow>
#endif

#include "imagepacker.h"
#include <QDrag>
#include <QDropEvent>
#include <QUrl>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

public slots:
    void addTiles();
    void deleteSelectedTiles();
    void packerUpdate();
    void updateAuto();
    void setTextureSize2048();
    void setTextureSize256();
    void setTextureSize512();
    void setTextureSize1024();
    void updateAplhaThreshold();
    void getFolder();
    void exportImage();
    void swapSizes();
    void clearTiles();

signals:
	void renderedImage(const QList<QImage>& image);

protected:
	void dropEvent(QDropEvent* event);
	void dragEnterEvent(QDragEnterEvent* event);

private:
	void recurseDirectory(const QString& dir);
    void customizeUI();
	void writeMetadataFile(const QList<QImage>& images, int imageIndex);

	Ui::MainWindow* ui;
    QStringList imageExtensions;
    QString topImageDir;
    ImagePacker packer;
    QList<PackedImage> packedImageList;
    bool exporting;
    int recursiveLoaderCounter;
    bool recursiveLoaderDone;
    QPixmap pattern;
    void addDir(QString dir);
    struct packerData
    {
		QListWidgetItem* listItem;
        QString path;
    };
};

#endif // MAINWINDOW_H
