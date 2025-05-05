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
    ui->boardView->setScene(scene);
    //basis
    ui->boardView->setRenderHint(QPainter::Antialiasing);

    createBoard();

    connect(ui->rollButton, &QPushButton::clicked, this, &MainWindow::on_rollButton_clicked);
    connect(ui->buyButton, &QPushButton::clicked, this, &MainWindow::on_buyButton_clicked);

    //create player
    playerToken = new QGraphicsEllipseItem(0,0,30,30);
    playerToken->setBrush(Qt::black);
    scene->addItem(playerToken);
    movePlayer(0); //go to go
}

void MainWindow::createBoard()
{
    scene->clear();

    const int tileSize= 80;
    const int boardSize = 10;

    createProperty(0, "Start", Qt::green); // GO
    createProperty(1, "Leuven Dieststraat", QColor(165,93,68)); // Brown
    createProperty(2, "Algemeen Fonds", Qt::white);
    createProperty(3, "Mechelen Bruul", QColor(165,93,68)); // Brown
    createProperty(4, "Inkomsten Belasting", Qt::gray);
    createProperty(5, "Station Noord", Qt::black);
    createProperty(6, "Leuven Bondgenotenlaan", QColor(23, 173, 207)); // Light Blue
    createProperty(7, "Kans", Qt::white);
    createProperty(8, "Turnhout Gasthuisstr.", QColor(23, 173, 207)); // Light Blue
    createProperty(9, "Turnhout Leopoldstr.", QColor(23, 173, 207)); // Light Blue
    createProperty(10, "Jail/Just Visiting", Qt::darkGray);
    createProperty(11, "Aalst lange Zoutstraat", QColor(216, 27, 171)); // Pink
    createProperty(12, "Electriciteit Centraal", Qt::yellow);
    createProperty(13, "Brugge Vlammingenstr.", QColor(216, 27, 171)); // Pink
    createProperty(14, "Knokke Kustlaan", QColor(216, 27, 171)); // Pink
    createProperty(15, "Station Centraal", Qt::black);
    createProperty(16, "Brugge Steenstraat", QColor(237, 124, 45)); // Orange
    createProperty(17, "Algemeen Fonds", Qt::white);
    createProperty(18, "Oostende Kapelstraat", QColor(237, 124, 45)); // Orange
    createProperty(19, "Kortrijk Rijselstraat", QColor(237, 124, 45)); // Orange
    createProperty(20, "Vrije Parking", Qt::lightGray);
    createProperty(21, "Tienen Leuvensestr.", QColor(237, 28, 36)); // Red
    createProperty(22, "Kans", Qt::white);
    createProperty(23, "Gent Kortrijksestr.", QColor(237, 28, 36)); // Red
    createProperty(24, "Hasselt Grote Markt", QColor(237, 28, 36)); // Red
    createProperty(25, "Buurt-Spoorwegen", Qt::black);
    createProperty(26, "Antwerpen Groenplaats", QColor(251, 242, 54)); // Yellow
    createProperty(27, "Brussel Lakenstraat", QColor(251, 242, 54)); // Yellow
    createProperty(28, "Waterdienst", Qt::cyan);
    createProperty(29, "Gent Veldstraat", QColor(251, 242, 54)); // Yellow
    createProperty(30, "Go To Jail", Qt::darkRed);
    createProperty(31, "Antwerpen Huidevettersstraat", QColor(29, 178, 74)); // Green
    createProperty(32, "Gent Vlaanderenstraat", QColor(29, 178, 74)); // Green
    createProperty(33, "Algemeen Fonds", Qt::white);
    createProperty(34, "Hasselt Demerstraat", QColor(29, 178, 74)); // Green
    createProperty(35, "Station Zuid", Qt::black);
    createProperty(36, "Kans", Qt::white);
    createProperty(37, "Antwerpen Meir", QColor(13, 46, 146)); // Dark Blue
    createProperty(38, "Extra Belasting", Qt::gray);
    createProperty(39, "Brussel Nieuwstraat", QColor(13, 46, 146)); // Dark Blue

    // Add center image/logo
    //QGraphicsPixmapItem *logo = new QGraphicsPixmapItem(QPixmap(":/images/monopoly_logo.png"));
    //logo->setPos(300, 300); // Center position
    //scene->addItem(logo);
}


void MainWindow::createProperty(int index, const QString& name, QColor color)
{
    const int tileSize = 80;
    const int boardSize = 10;

    int side = index / boardSize;
    int pos = index % boardSize;

    QPointF position;
    switch(side) {
    case 0: position = QPointF(pos * tileSize, 0); break; //top
    case 1: position = QPointF(9 * tileSize, pos * tileSize); break; //right
    case 2: position = QPointF((9-pos)* tileSize, 9 * tileSize); break; //bottom
    case 3: position = QPointF(0, (9-pos)*tileSize); break; //left
    }

    //tile making
    QGraphicsRectItem *tile = new QGraphicsRectItem(0,0, tileSize, tileSize);
    tile->setPos(position);
    tile->setBrush(color);
    tile->setPen(QPen(Qt::black, 1));
    scene->addItem(tile);

    //add property name
    QGraphicsTextItem *text = new QGraphicsTextItem(name);
    text->setPos(position.x()+ 5, position.y() + 5);
    text->setTextWidth(tileSize - 10);
    scene->addItem(text);
}

void MainWindow::on_rollButton_clicked() {
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


QPointF MainWindow::calculateTilePosition(int index) const
{
    const int tileSize = 80;
    const int tilesPerSide = 10;

    int side = index / tilesPerSide;
    int pos = index % tilesPerSide;

    switch(side){
    case 0: return QPointF((9-pos) * tileSize, 9 * tileSize); //bottom, right to left
    case 1: return QPointF(0, (9-pos) * tileSize);            // Left (bottom to top)
    case 2: return QPointF(pos * tileSize, 0);                // Top (left to right)
    case 3: return QPointF(9 * tileSize, pos * tileSize);     // Right (top to bottom)
    default: return QPointF(0,0);
    }
}


void MainWindow::movePlayer(int newPosition){
    currentPlayerPosition = newPosition;
    QPointF pos = calculateTilePosition(newPosition);

    //center token on tile
    playerToken->setPos(pos.x() + 15, pos.y() + 15);
}

void MainWindow::handleLanding(int position){
    qDebug() << "Landed on:" << position;
}


void MainWindow::animatePlayerMovement(int steps){
    int targetPosition = (currentPlayerPosition + steps) % 40;

    //smooth animation (1 step every 100ms)
    for (int i = 1; i <= steps; i++){
        QTimer::singleShot(i * 100, [this, i, steps, targetPosition](){
            movePlayer((currentPlayerPosition + 1) % 40);

            //when complete
            if (i == steps) {
                handleLanding(targetPosition);
            }
        });
    }
}


// In mainwindow.cpp


MainWindow::~MainWindow()
{
    delete ui;
}
