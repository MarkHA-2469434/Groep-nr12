#include "Tile.h"
#include "player.h"
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QBrush>

Tile::Tile(int index, const QString& name, QColor color, QPointF position)
    : index(index), name(name)
{
    graphicsItem  = new QGraphicsRectItem(0, 0, 80, 80);
    graphicsItem ->setPos(position);
    graphicsItem ->setBrush(color);

    labelItem  = new QGraphicsTextItem(name);
    labelItem ->setPos(position.x() + 5, position.y() + 5);
}


QGraphicsRectItem* Tile::getGraphics() {
    return graphicsItem;
}

QGraphicsTextItem* Tile::getLabel() {
    return labelItem;
}
