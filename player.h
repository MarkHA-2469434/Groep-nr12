#ifndef PLAYER_H
#define PLAYER_H
#include <QGraphicsItem>
#include <QGraphicsScene>

class Player {
public:
    int Balance = 1500;
    bool inJail = false;
    bool Bankrupt = false;
    int aantalHuizen = 0;
    int aantalHotels = 0;
    Player(QGraphicsScene* scene, QColor color);
    void moveTo(int index, const QPointF& pos);
    QGraphicsItem* getToken() const;
    int getPosition() const;
    int getPrevPosition()const;
    void setPosition(int position);
    int prevPosition;


    void voegGeldToe(int bedrag) {
        Balance += bedrag;
    }

    void trekGeldAf(int bedrag) {
        Balance -= bedrag;
    }

private:
    QGraphicsEllipseItem* token;
    int position;
    QColor playerColor;
};


#endif // PLAYER_H
