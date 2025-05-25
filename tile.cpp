#include "Tile.h"
#include "player.h"
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QBrush>

#include <QTextBlockFormat>
#include <QTextCursor>


Tile::Tile(int index,QString colorgroup, Player *owner, int prijs, int hypo, int prijsPerHuis, int r1, int r2, int r3, int r4, int r5, int r6, const QString& name, QColor color, QPointF position)
    : index(index), colorgroup(colorgroup),name(name), Owner(owner), prijs(prijs), hypo(hypo), prijsPerHuis(prijsPerHuis), r1(r1),r2(r2),r3(r3),r4(r4),r5(r5),r6(r6),color(color)
{
    graphicsItem = new QGraphicsRectItem(0, 0, 80, 80);
    graphicsItem->setPos(position);
    graphicsItem->setBrush(color);
    graphicsItem->setPen(QPen(Qt::black, 1));

    labelItem = new QGraphicsTextItem(name);
    labelItem ->setPos(position.x() + 5, position.y() + 5);

    labelItem->setFont(QFont("Arial", 8, QFont::Bold));

    QRectF textRect = labelItem->boundingRect();
    labelItem->setPos(
        position.x() + (80 - textRect.width()) / 2,
        position.y() + (80 - textRect.height()) / 2
        );

}

QGraphicsRectItem* Tile::getGraphics() {
    return graphicsItem;
}

QGraphicsTextItem* Tile::getLabel() {
    return labelItem;
}
