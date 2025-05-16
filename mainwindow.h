#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QList>
#include <QTimer>
#include <QGraphicsPixmapItem>

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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void movePlayer(int index);
    void handleLanding(int position);

private slots:
    void on_buyButton_clicked();
    void on_rollButton_released();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;

    Player *player;
    Board *board;
    QList<QTimer*> moveTimers;
    bool isMoving = false; // add this flag

    static const int TILE_SIZE {80};
    static const int TILES_PER_SIDE {11};
    static const int BOARD_SIZE {TILE_SIZE * TILES_PER_SIDE};

    void createBoard();
    void createProperty(int index, const QString& name, QColor color);
    void animatePlayerMovement(int steps);

    QGraphicsEllipseItem* playerToken = nullptr;
};
#endif // MAINWINDOW_H
