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
    std::vector<Tile*> tiles;
    QGraphicsScene* scene;


    QVector<Tile*>& getTiles() { return tiles; }
    const QVector<Tile*>& getTiles() const { return tiles; }


private:
<<<<<<< HEAD
    QGraphicsScene* scene;
    QVector<Tile*> tiles;

=======
>>>>>>> 70850b30bb33f7ae62e41648c148dd27abe58b6a
};


#endif // BOARD_H
