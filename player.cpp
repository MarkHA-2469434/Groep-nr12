#include "Player.h"
#include <QGraphicsEllipseItem>

Player::Player(QGraphicsScene* scene, QColor color) : position(0), playerColor(color) {
    token = new QGraphicsEllipseItem(0, 0, 30, 30);
    token->setBrush(playerColor);
    scene->addItem(token);
}

void Player::moveTo(int index, const QPointF& pos) {
    position = index;
    token->setPos(pos.x() + 25, pos.y() + 25);
}
<<<<<<< HEAD
=======

QGraphicsItem* Player::getToken() const {
    return token;
}

void Player::sendToJail(int jailPosition, const QPointF& pos) {
    inJail = true;
    jailTurns = 0;
    setPosition(jailPosition);
    moveTo(jailPosition, pos);
}
>>>>>>> 70850b30bb33f7ae62e41648c148dd27abe58b6a
