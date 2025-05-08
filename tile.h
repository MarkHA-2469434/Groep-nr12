#ifndef TILE_H
#define TILE_H
#include <QString>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include "Player.h"

class Player;

class Tile {
public:
    Tile(int index, const QString& name, QColor color, int price, int rent, QPointF position, QGraphicsScene* scene);

    void setOwner(Player* player);
    Player* getOwner() const {return owner;}
    bool isOwned() const {return owner != nullptr;}

    int getIndex() const {return price;}
    QString getName() const {return name;}

    int getPrice() const {return price;}
    int getRent() const {return rent;}
    QString getInfo() const;

    void highlight(bool on);

    QGraphicsRectItem* getGraphics() const {return background; }
    QGraphicsTextItem* getLabel() const {return label;}

private:
    int index;
    int price;
    Player* owner = nullptr;
    void updateColor();
    int rent;
    QString name;
    QColor color;
    QGraphicsRectItem* background = nullptr;
    QGraphicsTextItem* label = nullptr;

    void updateAppearance();
};

#endif // TILE_H
