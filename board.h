#ifndef BOARD_H
#define BOARD_H
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <vector>
#include "Tile.h"

class Board {
public:
    static const int TILE_SIZE = 80;
    static const int TILES_PER_SIDE = 10;
    static const int BOARD_SIZE = TILE_SIZE * TILES_PER_SIDE;

    Board(QGraphicsScene* scene);
    void create();
    QPointF calculateTilePosition(int index) const;
    Tile* getTile(int index);

private:
    void createProperty(int index, const QString& name, QColor color);
    QGraphicsScene* m_scene;
    std::vector<Tile*> tiles;

};


#endif // BOARD_H
