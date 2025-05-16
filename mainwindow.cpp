#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QPushButton>
#include <QRandomGenerator>
#include <QTimer>
#include <QTextBlock>
#include "dice.h"
#include "Kans_AlgFonds.cpp"


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
    //ui->boardView->scale(0.70, 0.70);

    board = new Board(scene);
    board->create();

    // Add center image/logo
    //QGraphicsPixmapItem *logo = new QGraphicsPixmapItem(QPixmap(":/images/monopoly.png"));
    //logo->setPos(300, 300); // Center position
    //scene->addItem(logo);


    //create players
    player = new Player(scene, Qt::cyan);
    scene->addItem(player->getToken());
    movePlayer(0); //go to go
    ui->BalanceLabel->setText(QString::number(player->Balance));

    connect(ui->buyButton, &QPushButton::clicked, this, &MainWindow::on_buyButton_clicked);
}

void MainWindow::createProperty(int index, const QString& name, QColor color)
{
    QPointF position = board->calculateTilePosition(index);

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
    text->setTextWidth(TILE_SIZE - 10);
    text->setDefaultTextColor(Qt::black);

    QFont font = text->font();
    font.setPointSize(6);
    font.setBold(true);
    text->setFont(font);

    QTextBlockFormat format;
    QTextCursor cursor = QTextCursor(text->document());
    cursor.select(QTextCursor::Document);
    format.setAlignment(Qt::AlignCenter);
    cursor.mergeBlockFormat(format);

    text->setPos(position.x() + 5, position.y() + 5);
    text->setZValue(2);
    scene->addItem(text);

    qDebug() << "Tile" << index << "position:" << position;
}


void MainWindow::on_rollButton_released()
{
    if (isMoving) return;

    ui->rollButton->setEnabled(false);
    ui->diceLabel->setText("Rolling...");
    player->prevPosition = player->getPosition();
    QTimer::singleShot(800, [this] (){
        auto [dice1,dice2] = Dice::roll();
        int total = dice1 + dice2;

        ui->diceLabel->setText(QString("%1 + %2 = %3").arg(dice1).arg(dice2).arg(total));
        animatePlayerMovement(total);
        ui->rollButton->setEnabled(true);
    });
}

void MainWindow::on_buyButton_clicked()
{
    ui->statusLabel->setText("Property purchased");
}

void MainWindow::movePlayer(int index){
    player->setPosition(index);
    QPointF tilePos = board->calculateTilePosition(index);
    player->getToken()->setPos(tilePos.x() + 25, tilePos.y() + 25);

}

void MainWindow::handleLanding(int position){
    qDebug() << "Landed on property index: " << player->getPosition();
    ui->KansAlg->setWordWrap(true);
    KansStapelp kans;
    AlgemeenFondsStapelp AlgFond;
    if (position < player->getPrevPosition()){ //Ontvang startloon
        ui->LoonLabel->setText("Je ontvangt 200 euro loon!");
        player->voegGeldToe(200);
        QTimer::singleShot(3000, this, [this]() {
            ui->LoonLabel->clear();
        });
    }
    if( (position == 7) || (position== 22 ) || (position == 36) ){//Kans kaart
        KansKaart kaart = kans.trekKansKaart(player, this);
        ui->KansAlg->setText(kaart.beschrijving);

        QTimer::singleShot(5000, this, [this]() {
            ui->KansAlg->clear();
        });
    }
    if( (position == 2) || (position== 17) || (position== 33) ){//Algemeen Fonds
        FondsKaart kaart = AlgFond.trekAlgKaart(player, this);
        ui->KansAlg->setText(kaart.beschrijving);

        QTimer::singleShot(5000, this, [this]() {
            ui->KansAlg->clear();
        });
    }
    if((player-> getPosition() == 30)){//Go To jail
        player->inJail = true;
        ui->KansAlg->setText("Go to Jail!");

        QTimer::singleShot(500, this, [this]() {
            this->movePlayer(10);
            ui->KansAlg->clear();
        });
    }
    if((player-> getPosition() == 38)){ //extra belasting
        player->trekGeldAf(100);
        ui->KansAlg->setText("Je moet Extra Belasting betalen aan de gemeente.");

        // Clear the label after 2 seconds
        QTimer::singleShot(5000, this, [this]() {
            ui->KansAlg->clear();
        });
    }
    if((player-> getPosition() == 4)){ //Inkomsten Belasting
        player->trekGeldAf(200);
        ui->KansAlg->setText("Je Moet inkomsten belasting betalen aan de gemeente.");

        QTimer::singleShot(3000, this, [this]() {
            ui->KansAlg->clear();
        });
    }
    ui->BalanceLabel->setText(QString::number(player->Balance));
}


void MainWindow::animatePlayerMovement(int steps){
    if(isMoving) return;

    isMoving = true;
    int targetPosition = (player->getPosition() + steps) % 40;


    // Animate each step
    for (int i = 1; i <= steps; i++) {
        QTimer* timer = new QTimer(this);
        timer->setSingleShot(true);
        timer->setInterval(i * 150);  // 150ms between steps

        connect(timer, &QTimer::timeout, [=]() {
            int newPos = (player->getPosition() + 1) % 40;
            movePlayer(newPos);

            if (i == steps) {
                isMoving = false;
                handleLanding(newPos);
            }
        });

        moveTimers.append(timer);
        timer->start();
    }
}


MainWindow::~MainWindow()
{
    qDeleteAll(moveTimers);
    delete player;
    delete board;
    delete ui;
}

