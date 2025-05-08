#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QPushButton>
#include <QRandomGenerator>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //initialisatie
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0,0,BOARD_SIZE, BOARD_SIZE);

    ui->boardView->setScene(scene);
    //basis
    ui->boardView->setRenderHint(QPainter::Antialiasing);

    ui->boardView->setFixedSize(BOARD_SIZE + 2, BOARD_SIZE + 2);
    ui->boardView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->boardView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    createBoard();

    //create player
    playerToken = new QGraphicsEllipseItem(0,0,30,30);
    playerToken->setBrush(Qt::cyan);
    scene->addItem(playerToken);
    movePlayer(0); //go to go

    connect(ui->buyButton, &QPushButton::clicked, this, &MainWindow::on_buyButton_clicked);
}

QPointF MainWindow::calculateTilePosition(int index) const {
    // Normalize index first (0-39)
    index = index % 40;

    if (index < 10) { // Bottom side (0–9): right to left
        return QPointF((TILES_PER_SIDE - 1 - index) * TILE_SIZE, BOARD_SIZE - TILE_SIZE);
    } else if (index < 20) { // Left side (10–19): bottom to top
        return QPointF(0, (TILES_PER_SIDE - 1 - (index - 10)) * TILE_SIZE);
    } else if (index < 30) { // Top side (20–29): left to right
        return QPointF((index - 20) * TILE_SIZE, 0);
    } else { // Right side (30–39): top to bottom
        return QPointF(BOARD_SIZE - TILE_SIZE, (index - 30) * TILE_SIZE);
    }
}


void MainWindow::createProperty(int index, const QString& name, QColor color)
{
    const int tileSize = 80;
    //const int boardSize = 10;

    //int side = index / boardSize;
    //int pos = index % boardSize;

    QPointF position = calculateTilePosition(index);

    // Manual positioning for corners
    switch(index) {
    case 0: position = QPointF((TILES_PER_SIDE - 1) * TILE_SIZE, BOARD_SIZE - TILE_SIZE); break; // Tile 0
    case 10: position = QPointF(0, (TILES_PER_SIDE - 1) * TILE_SIZE); break; // Tile 10
    case 20: position = QPointF(0, 0); break; // Tile 20
    case 30: position = QPointF((TILES_PER_SIDE - 1) * TILE_SIZE, 0); break; // Tile 30
    }

    //tile making
    QGraphicsRectItem *tile = new QGraphicsRectItem(0,0, TILE_SIZE, TILE_SIZE);
    tile->setPos(position);
    tile->setBrush(color);
    tile->setPen(QPen(Qt::black, 1));
    scene->addItem(tile);

    //add property name
    QGraphicsTextItem *text = new QGraphicsTextItem(name);
    text->setPos(position.x()+ 5, position.y() + 5);
    text->setTextWidth(tileSize - 10);
    text->setZValue(2);
    scene->addItem(text);

    qDebug() << "Tile" << index << "position:" << position;
}



void MainWindow::createBoard()
{
    scene->clear();

    createProperty(0, "Start", Qt::red); // GO
    createProperty(1, "Leuven Dieststraat", QColor(165,93,68)); // Brown
    createProperty(2, "Algemeen Fonds", Qt::lightGray);
    createProperty(3, "Mechelen Bruul", QColor(165,93,68)); // Brown
    createProperty(4, "Inkomsten Belasting", Qt::gray);
    createProperty(5, "Station Noord", Qt::black);
    createProperty(6, "Leuven Bondgenotenlaan", QColor(23, 173, 207)); // Light Blue
    createProperty(7, "Kans", Qt::lightGray);
    createProperty(8, "Turnhout Gasthuisstr.", QColor(23, 173, 207)); // Light Blue
    createProperty(9, "Turnhout Leopoldstr.", QColor(23, 173, 207)); // Light Blue
    createProperty(10, "Jail/Just Visiting", Qt::blue);
    createProperty(11, "Aalst lange Zoutstraat", QColor(216, 27, 171)); // Pink
    createProperty(12, "Electriciteit Centraal", Qt::lightGray);
    createProperty(13, "Brugge Vlammingenstr.", QColor(216, 27, 171)); // Pink
    createProperty(14, "Knokke Kustlaan", QColor(216, 27, 171)); // Pink
    createProperty(15, "Station Centraal", Qt::black);
    createProperty(16, "Brugge Steenstraat", QColor(237, 124, 45)); // Orange
    createProperty(17, "Algemeen Fonds", Qt::lightGray);
    createProperty(18, "Oostende Kapelstraat", QColor(237, 124, 45)); // Orange
    createProperty(19, "Kortrijk Rijselstraat", QColor(237, 124, 45)); // Orange
    createProperty(20, "Vrije Parking", Qt::blue);
    createProperty(21, "Tienen Leuvensestr.", QColor(237, 28, 36)); // Red
    createProperty(22, "Kans", Qt::lightGray);
    createProperty(23, "Gent Kortrijksestr.", QColor(237, 28, 36)); // Red
    createProperty(24, "Hasselt Grote Markt", QColor(237, 28, 36)); // Red
    createProperty(25, "Buurt-Spoorwegen", Qt::black);
    createProperty(26, "Antwerpen Groenplaats", QColor(251, 242, 54)); // Yellow
    createProperty(27, "Brussel Lakenstraat", QColor(251, 242, 54)); // Yellow
    createProperty(28, "Waterdienst", Qt::cyan);
    createProperty(29, "Gent Veldstraat", QColor(251, 242, 54)); // Yellow
    createProperty(30, "Go To Jail", Qt::blue);
    createProperty(31, "Antwerpen Huidevettersstraat", QColor(29, 178, 74)); // Green
    createProperty(32, "Gent Vlaanderenstraat", QColor(29, 178, 74)); // Green
    createProperty(33, "Algemeen Fonds", Qt::lightGray);
    createProperty(34, "Hasselt Demerstraat", QColor(29, 178, 74)); // Green
    createProperty(35, "Station Zuid", Qt::black);
    createProperty(36, "Kans", Qt::lightGray);
    createProperty(37, "Antwerpen Meir", QColor(13, 46, 146)); // Dark Blue
    createProperty(38, "Extra Belasting", Qt::gray);
    createProperty(39, "Brussel Nieuwstraat", QColor(13, 46, 146)); // Dark Blue

    // Add center image/logo
    //QGraphicsPixmapItem *logo = new QGraphicsPixmapItem(QPixmap(":/images/monopoly_logo.png"));
    //logo->setPos(300, 300); // Center position
    //scene->addItem(logo);

}


void MainWindow::on_rollButton_released()
{
    //diable button during animation
    ui->rollButton->setEnabled(false);

    //show rolling animation
    ui->diceLabel->setText("Rolling...");

    QTimer::singleShot(800, [this] (){
    int dice1 = QRandomGenerator::global()->bounded(1,7);
    int dice2 = QRandomGenerator::global()->bounded(1,7);
    int total = dice1 + dice2;

    //updaet UI
    ui->diceLabel->setText(QString("%1 + %2 = %3").arg(dice1).arg(dice2).arg(total));

    qDebug() << "total" << total;
    //move player
    animatePlayerMovement(total);
    //reenable button
    ui->rollButton->setEnabled(true);
    });

}

void MainWindow::on_buyButton_clicked()
{
    ui->statusLabel->setText("Property purchased");
}



void MainWindow::movePlayer(int newPosition){
    currentPlayerPosition = newPosition;

    QPointF pos = calculateTilePosition(newPosition);

    qDebug() << "Moving player to position" << newPosition << " at coordinates " <<pos;

    //center token on tile
    playerToken->setPos(pos.x() + 25, pos.y() + 25);
}

void MainWindow::handleLanding(int position){
    qDebug() << "Landed on property index: " << position;
}


void MainWindow::animatePlayerMovement(int steps){
    qDebug() << "Steps" << steps;

    int stepsRemaining = steps;
    int targetPosition = (currentPlayerPosition + steps) % 40;

    // Animate each step
    for (int i = 1; i <= steps; i++) {
        QTimer* timer = new QTimer(this);
        timer->setSingleShot(true);
        timer->setInterval(i * 150);  // 150ms between steps

        connect(timer, &QTimer::timeout, [this, i, steps, targetPosition]() {
            currentPlayerPosition = (currentPlayerPosition + 1) % 40;
            movePlayer(currentPlayerPosition);

            qDebug() << "Step" << i << "/" << steps << "- Now at" << currentPlayerPosition;

            if (i == steps) {
                qDebug() << "Landed on:" << currentPlayerPosition;
                isMoving = false;
                handleLanding(currentPlayerPosition);
            }
        });

        timer->start();
        moveTimers.append(timer);
    }
}


MainWindow::~MainWindow()
{
    for (auto timer : moveTimers) {
        if (timer) {
            timer->stop();
            timer->deleteLater();
        }
    }
    moveTimers.clear();
    delete ui;
}

