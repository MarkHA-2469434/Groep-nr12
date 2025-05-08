#ifndef PLAYER_H
#define PLAYER_H
#include <QGraphicsItem>
#include <QGraphicsScene>

class Player {
public:
    Player(QGraphicsScene* scene, QColor color);
    void moveTo(int index, const QPointF& pos);
    QGraphicsItem* getToken() const;
    int getPosition() const;
    void setPosition(int position);

private:
    QGraphicsEllipseItem* token;
    int position;
    QColor playerColor;
};


#endif // PLAYER_H
