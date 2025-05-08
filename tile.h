#ifndef TILE_H
#define TILE_H
#include <QString>
#include <QGraphicsItem>

class Tile {
public:
    Tile(int index, const QString& name, QColor color, QPointF position);
    QGraphicsRectItem* getGraphics();
    QGraphicsTextItem* getLabel();

private:
    int index;
    QString name;
    QColor color;
    QPointF position;
    QGraphicsRectItem* graphicsItem;
    QGraphicsTextItem* labelItem;
};

#endif // TILE_H
