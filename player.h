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

    int getPosition() const { return position; };
    int getPrevPosition()const{return prevPosition;}
    void setPosition(int pos) {position = pos;};

    void voegGeldToe(int bedrag) {Balance += bedrag;}
    void trekGeldAf(int bedrag) {Balance -= bedrag;}

    void setName(const QString& newName) {name = newName;}
    QString getName() const {return name;}

private:
    QGraphicsEllipseItem* token;
    int position;
    int prevPosition;
    QColor playerColor;
    QString name;

    int balance;
};


#endif // PLAYER_H
