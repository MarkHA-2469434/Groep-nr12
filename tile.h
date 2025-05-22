#ifndef TILE_H
#define TILE_H
#include <QString>
#include <QGraphicsItem>
#include "player.h"

class Tile {
public:
    Tile(int index, Player* Owner, int prijs, int hypo, int prijsPerHuis, int r1,int r2, int r3, int r4, int r5, int r6, const QString& name, QColor color, QPointF position);
    QGraphicsRectItem* getGraphics();
    QGraphicsTextItem* getLabel();

    int huizencount = 0;
    int prijs;
    int hypo;
    int prijsPerHuis;
    bool gehypotykeert = false;
    int r1, r2, r3, r4, r5, r6;

    QString name;
    Player* Owner;

private:
    int index;
    QColor color;
    QPointF position;
    QGraphicsRectItem* graphicsItem;
    QGraphicsTextItem* labelItem;
};

#endif // TILE_H
