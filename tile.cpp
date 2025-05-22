#include "Tile.h"
#include "player.h"
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QBrush>

<<<<<<< HEAD
Tile::Tile(int index, Player* Owner, int prijs, int hypo, int prijsPerHuis, int r1, int r2, int r3, int r4, int r5, int r6, const QString& name, QColor color, QPointF position)
    : index(index), Owner(Owner), prijs(prijs), hypo(hypo), prijsPerHuis(prijsPerHuis),
    r1(r1), r2(r2), r3(r3), r4(r4), r5(r5), r6(r6),
    name(name), color(color), position(position)
=======

Tile::Tile(int index,Player* owner, int prijs, int hypo, int prijsPerHuis, int r1, int r2, int r3, int r4, int r5, int r6, const QString& name, QColor color, QPointF position)
    : name(name), Owner(owner), prijs(prijs), hypo(hypo), prijsPerHuis(prijsPerHuis), r1(r1),r2(r2),r3(r3),r4(r4),r5(r5),r6(r6)
>>>>>>> 70850b30bb33f7ae62e41648c148dd27abe58b6a
{
    graphicsItem = new QGraphicsRectItem(0, 0, 80, 80);
    graphicsItem->setPos(position);
    graphicsItem->setBrush(color);

    labelItem  = new QGraphicsTextItem(name);
    labelItem ->setPos(position.x() + 5, position.y() + 5);
}

QGraphicsRectItem* Tile::getGraphics() {
    return graphicsItem;
}

QGraphicsTextItem* Tile::getLabel() {
    return labelItem;
}
