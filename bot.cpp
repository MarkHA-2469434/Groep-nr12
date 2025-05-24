#include "bot.h"
#include "mainwindow.h"
#include "board.h"
#include <QRandomGenerator>

Bot::Bot(QGraphicsScene* scene, QColor color)
    : Player(scene, color)
{
}


void Bot::handleJailMove(MainWindow* gameWindow) {
    if (jailCards > 0) {
        gameWindow->useJailCard();
    } else if (Balance >= 50 && QRandomGenerator::global()->bounded(100) < 70) {
        gameWindow->payJailFine();
    } else {
        gameWindow->attemptJailEscape();
    }
}


void Bot::makeMove(MainWindow* gameWindow) {
    if (inJail) {
        handleJailMove(gameWindow);
    } else {
        // Roll normally
        gameWindow->rollDice();

        // Get current tile after moving
        int pos = getPosition();
        Tile* tile = gameWindow->getBoard()->getTile(pos);

        // Automatically buy if possible
        if (tile->prijs > 0 && !tile->Owner && Balance >= tile->prijs) {
            gameWindow->purchaseCurrentProperty();
        }
    }
}
