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
    void on_rollButton_clicked();

    void on_addTileButton_clicked();

    void on_buyButton_clicked();

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
};
#endif // MAINWINDOW_H
