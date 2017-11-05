#ifndef VIEW_H
#define VIEW_H

#include <QComboBox>
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QWidget>

class View : public QWidget
{
	Q_OBJECT
public:
	View(QWidget* parent = 0);
	QComboBox* scaleBox;

protected:
	void paintEvent(QPaintEvent* event);

private:
	QList<QPixmap> textures;
	float scale;
	QSize size;
private slots:
	void updatePixmap(const QList<QImage>& image);
	void rescale(QString);
};

#endif // VIEW_H
