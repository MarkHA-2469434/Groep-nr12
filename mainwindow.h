#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>

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

private slots:
    void on_buyButton_clicked();

    void on_rollButton_released();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;

    //board properties
    void createBoard();
    void createProperty(int index, const QString& name, QColor color);
    void animatePlayerMovement(int steps);
    void handleLanding(int position);
    void movePlayer(int newPosition);
    QPointF calculateTilePosition(int index) const;

    QGraphicsEllipseItem* playerToken = nullptr;

    int currentPlayerPosition = 0; //start at GO
    bool isMoving = false; // add this flag

    QList<QTimer*> moveTimers; //track active animations
    QTimer* moveTimer = nullptr; //track active animations

    const int TILE_SIZE = 80;
    const int TILES_PER_SIDE = 11;
    const int BOARD_SIZE = TILE_SIZE * TILES_PER_SIDE;
};
#endif // MAINWINDOW_H
