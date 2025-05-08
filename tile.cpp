#include "Tile.h"
#include "Player.h"
#include "Board.h"
#include <QTextDocument>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QTextOption>
#include <QPen>
#include <QBrush>

Tile::Tile(int index, const QString& name, QColor color, int price, int rent, QPointF position, QGraphicsScene* scene)
    : index(index), name(name),color(color), price(price), rent(rent), background(nullptr), label(nullptr)
{
    background  = new QGraphicsRectItem(0, 0, Board::TILE_SIZE, Board::TILE_SIZE);

    background->setPos(position);
    background->setBrush(color);
    background->setPen(QPen(Qt::black, 1));

    label = new QGraphicsTextItem(name);
    label->setDefaultTextColor(Qt::black);

    QTextDocument* doc = label->document();
    doc->setDefaultTextOption(QTextOption(Qt::AlignCenter));
    doc->setTextWidth(Board::TILE_SIZE - 15);

    QFont font;
    font.setPointSize(9);
    font.setBold(true);
    label->setFont(font);

    // Center the text
    label->setPos(
        position.x() + (Board::TILE_SIZE - label->boundingRect().width())/2,
        position.y() + (Board::TILE_SIZE - label->boundingRect().height())/2
        );

    if (scene){
        scene->addItem(background);
        scene->addItem(label);
    }
}

QString Tile::getInfo() const{
    return QString("%1\n").arg(name);
}

void Tile::setOwner(Player* player){
    owner = player;
    updateAppearance();
}

void Tile::updateAppearance() {
    if (owner) {
        // Blend tile color with owner's color
        QColor ownerColor = owner->getColor();
        QColor blended(
            (color.red() + ownerColor.red())/2,
            (color.green() + ownerColor.green())/2,
            (color.blue() + ownerColor.blue())/2
            );
        background->setBrush(blended);
    } else {
        background->setBrush(color);
    }
}

void Tile::highlight(bool on) {
    background->setPen(on ? QPen(Qt::yellow, 3) : QPen(Qt::black, 1));
}
