#ifndef BOT_H
#define BOT_H

#include "player.h"

class MainWindow;

class Bot : public Player
{
public:
    explicit Bot(QGraphicsScene* scene, QColor color);
    void handleJailMove(MainWindow* gameWindow);
    void makeMove(MainWindow* gameWindow);
};

#endif
