#include "Player.h"
#include <QGraphicsEllipseItem>

Player::Player(QGraphicsScene* scene, QColor color, const QString& name) : position(0), playerColor(color), name(name), money(1500) {
    token = new QGraphicsEllipseItem(0, 0, 20, 20);
    token->setBrush(playerColor);
    scene->addItem(token);
}

void Player::moveTo(int index, const QPointF& pos) {
    position = index;
    token->setPos(pos.x() + 30, pos.y() + 30);
}

QGraphicsItem* Player::getToken() const {
    return token;
}

QColor Player::getColor() const {
    return playerColor;
}

void Player::setPosition(int position) {
    this->position = position;
}

int Player::getPosition() const {
    return position;
}
