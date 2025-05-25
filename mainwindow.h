#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QVBoxLayout>
#include <QList>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include <QVector>

#include "Board.h"
#include "Player.h"
#include "bot.h"
#include "Kans_AlgFonds.h"

class Board;
class Player;
class Tile;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    Board* getBoard() const { return board; }

    void movePlayer(int index, Player* player);
    void handleLanding(int position);
    void nextPlayerTurn();
    void updatePlayerUI();

    void payRent(int position);
    int calcRent(int position);

    int calcStationRent(int position);
    void payStationRent(int position);

    int calcNutsbedrijvenRent(int worp, int position);
    void PayNutsBedrijvenRent(int worp, int position);

    bool checkMonopoly(const QString colorgroup);
    void updateTileHouses(int position);
    void updateTileOwnerLabel(Tile* tile);

    bool isBot(Player* player) const;
    void botTurn();
    void rollDice();

    void highlightCurrentPlayer();
    void purchaseCurrentProperty();

    void createPlayerInfoWidgets();
    void updatePlayerInfo();

    //jail members
    void handleJailTurn();
    void payJailFine();
    void useJailCard();
    void attemptJailEscape();

private slots:
    void on_buyButton_clicked();
    void on_rollButton_released();
    void on_EndTurnButton_released();

    void on_OwnerPropsButton_released();
    void on_tradeButton_released();

    void on_FaillietButton_released();

private:
    Ui::MainWindow* ui;
    QGraphicsScene* scene;

    KansStapelp kans;
    AlgemeenFondsStapelp algFond;

    Player* player;
    Board* board;
    QList<QTimer*> moveTimers;

    bool isMoving = false;
    bool hasRolled = false;

    Player* currentPlayer;
    int currentPlayerIndex;

    static const int TILE_SIZE {80};
    static const int TILES_PER_SIDE {11};
    static const int BOARD_SIZE {TILE_SIZE * TILES_PER_SIDE};

    void resetTurnAfterJail();
    void resetTurnForRollAgain();
    void handleThreeDoubles();
    void handleJailRoll(int dice1, int dice2);

    void createBoard();
    void createProperty(int index, const QString& name, QColor color);
    void animatePlayerMovement(int steps, bool wasDoubles);

    bool checkHouseBuy(Tile* tile);
    bool checkHouseSell(Tile* tile);

    QGraphicsEllipseItem* playerToken = nullptr;

    QVector<Player*> players;

    int consecutiveDoubles = 0;
};
#endif
