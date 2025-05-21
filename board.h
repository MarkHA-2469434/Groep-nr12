#ifndef BOARD_H
#define BOARD_H
#include <QGraphicsScene>
#include <vector>
#include "Tile.h"

class Tile;

class Board {
public:
    Board(QGraphicsScene* scene);
    void create();
    QPointF calculateTilePosition(int index) const;
    Tile* getTile(int index);
    std::vector<Tile*> tiles;
    QGraphicsScene* scene;


private:
};


#endif // BOARD_H
