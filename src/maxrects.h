#ifndef MAXRECTS_H
#define MAXRECTS_H
#include "imagepacker.h"
#include <QImage>

struct trbl
{
    QPoint t, r, b, l;
};

struct MaxRectsNode
{
    QRect r; //rect
    //    QSize *i; //image
    trbl b; //border
};
class MaxRects
{
public:
	MaxRects();
	QList<MaxRectsNode> F;
	QList<QRect> R;
	QList<MaxRectsNode*> FR;
	QPoint insertNode(InputImage*);
	int heuristic, w, h, rotation;
	bool leftToRight;
	Border* border;
};

#endif // MAXRECTS_H
