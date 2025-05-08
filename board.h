#ifndef BOARD_H
#define BOARD_H
#include <QGraphicsScene>
#include <vector>
#include "Tile.h"

class Board {
public:
    Board(QGraphicsScene* scene);
    void create();
    QPointF calculateTilePosition(int index) const;
    Tile* getTile(int index);

private:
    QGraphicsScene* scene;
    std::vector<Tile*> tiles;
};


#endif // BOARD_H
