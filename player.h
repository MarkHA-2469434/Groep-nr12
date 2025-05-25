#ifndef PLAYER_H
#define PLAYER_H
#include <QGraphicsItem>
#include <QGraphicsScene>
#include "tile.h"

class Tile;
class Player{
public:
    Player(QGraphicsScene* scene, QColor color);
    virtual ~Player() = default;

    int getPosition() const { return position; }
    int getPrevPosition()const{return prevPosition;}
    void setPosition(int pos) {position = pos;};


    int Balance = 1500;

    bool inJail = false;
    int turnsInJail = 0;
    int jailCards = 0;
    bool isJailCard = false;

    bool Bankrupt = false;
    int aantalHuizen = 0;
    int aantalHotels = 0;

    int position = 0;
    int prevPosition = 0;
    int worp;

    QString name;


    QColor getPlayerColor() const { return playerColor; }

    void moveTo(int index, const QPointF& pos);
    QGraphicsItem* getToken() const{return token;}


    void sendToJail(int jailPosition, const QPointF& pos);
    int jailTurns = 0;

    QColor playerColor;

    void voegGeldToe(int bedrag) {Balance += bedrag;}
    void trekGeldAf(int bedrag) {Balance -= bedrag;}

    void setName(const QString& newName) {name = newName;}
    QString getName() const {return name;}

    void addProperty(Tile* property) { ownedProperties.append(property); }
    bool ownsProperty(Tile* property) const { return ownedProperties.contains(property); }

    QGraphicsEllipseItem* token;
    QVector<Tile*> ownedProperties;
};


#endif // PLAYER_H
