#ifndef PLAYER_H
#define PLAYER_H
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QColor>
#include <QString>

class Player {
public:
    Player(QGraphicsScene* scene, QColor color, const QString& name);
    QGraphicsItem* getToken() const;
    int getPosition() const;
    void setPosition(int position);

    void moveTo(int index, const QPointF& pos);

    QString getName() const {return name;}
    QColor getColor() const;
    int getMoney() const {return money;}
    void deductMoney(int amount) {money -= amount;}

private:
    QGraphicsEllipseItem* token;
    int position;
    QColor playerColor;

    QString name;
    int money = 1500;
};


#endif // PLAYER_H
