#ifndef PLAYER_H
#define PLAYER_H
#include <QGraphicsItem>
#include <QGraphicsScene>
#include "tile.h"

class Tile;
class Player{
public:
    int Balance = 1500;

    bool inJail = false;
    int turnsInJail = 0;
    int jailCards = 0;
    bool isJailCard = false;

    bool Bankrupt = false;
    int aantalHuizen = 0;
    int aantalHotels = 0;

<<<<<<< HEAD
    int position = 0;
    int prevPosition = 0;
    int worp;

    QString name;

    Player(QGraphicsScene* scene, QColor color);

    virtual ~Player() = default;
    QColor getPlayerColor() const { return playerColor; }

    void moveTo(int index, const QPointF& pos);
    QGraphicsItem* getToken() const{return token;}

    int getPosition() const { return position; }
    int getPrevPosition()const{return prevPosition;}
    void setPosition(int pos) {prevPosition = position; position = pos;};
=======
    void sendToJail(int jailPosition, const QPointF& pos);
    int jailTurns = 0;

    QColor playerColor;
    int position = 0;
    int prevPosition = 0;

    QString name;

    int worp;
    QVector<Tile> OwnedProperties;

    Player(QGraphicsScene* scene, QColor color);
    void moveTo(int index, const QPointF& pos);
    QGraphicsItem* getToken() const;

    int getPosition() const { return position; }
    int getPrevPosition()const{return prevPosition;}
    void setPosition(int pos) {position = pos;}
>>>>>>> 70850b30bb33f7ae62e41648c148dd27abe58b6a

    void voegGeldToe(int bedrag) {Balance += bedrag;}
    void trekGeldAf(int bedrag) {Balance -= bedrag;}

    void setName(const QString& newName) {name = newName;}
    QString getName() const {return name;}

    void addProperty(Tile* property) { ownedProperties.append(property); }
    bool ownsProperty(Tile* property) const { return ownedProperties.contains(property); }

private:
    QGraphicsEllipseItem* token;
<<<<<<< HEAD
    QColor playerColor;
    QVector<Tile*> ownedProperties;

=======
    int balance;
>>>>>>> 70850b30bb33f7ae62e41648c148dd27abe58b6a
};


#endif // PLAYER_H
