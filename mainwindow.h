#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QList>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include <QVector>

#include "Board.h"
#include "Player.h"

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
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void movePlayer(int index, Player* player);
    void handleLanding(int position);
    void nextPlayerTurn();
    void updatePlayerUI();
    void payRent(int position);
    int calcRent(int position);
    int calcStationRent(int position);
    void payStationRent(int position);
    int calcNutsbedrijvenRent(int worp,int position);
    void PayNutsBedrijvenRent(int worp,int position);

private slots:
    void on_buyButton_clicked();
    void on_rollButton_released();
    void on_EndTurnButton_released();

    //void on_OwnerPropsButton_released();

private:
    Ui::MainWindow* ui;
    QGraphicsScene* scene;

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

    void createBoard();
    void createProperty(int index, const QString& name, QColor color);
    void animatePlayerMovement(int steps);

    QGraphicsEllipseItem* playerToken = nullptr;

    QVector<Player*> players;

};
#endif // MAINWINDOW_H
