#ifndef BOARD_H
#define BOARD_H
#include <QGraphicsScene>
#include <QVector>
#include "Tile.h"

class Tile;

class Board {
public:
    Board(QGraphicsScene* scene);
    void create();
    QPointF calculateTilePosition(int index) const;
    Tile* getTile(int index);

    QVector<Tile*> tiles;

    QVector<Tile*>& getTiles() { return tiles; }
    const QVector<Tile*>& getTiles() const { return tiles; }

private:
    QGraphicsScene* scene;
};

#endif
