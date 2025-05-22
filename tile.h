#ifndef TILE_H
#define TILE_H
#include <QString>
#include <QGraphicsItem>
class Player;

class Tile {
public:
<<<<<<< HEAD
    Tile(int index, Player* Owner, int prijs, int hypo, int prijsPerHuis, int r1,int r2, int r3, int r4, int r5, int r6, const QString& name, QColor color, QPointF position);
=======
    Tile(int index,Player *Owner,int prijs, int hypo, int prijsPerHuis, int r1,int r2, int r3, int r4, int r5, int r6, const QString& name, QColor color, QPointF position);
>>>>>>> 70850b30bb33f7ae62e41648c148dd27abe58b6a
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

    int huizencount = 0;
    int prijs;
    int hypo;
    int prijsPerHuis;
    bool gehypotykeert = false;
    int r1, r2, r3, r4, r5, r6;

    QString name;
    Player* Owner;

private:
<<<<<<< HEAD
    int index;
=======
    int* index;
>>>>>>> 70850b30bb33f7ae62e41648c148dd27abe58b6a
    QColor color;
    QPointF position;
    QGraphicsRectItem* graphicsItem;
    QGraphicsTextItem* labelItem;
};

#endif // TILE_H
