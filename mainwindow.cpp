#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QPushButton>
#include <QRandomGenerator>
#include <QTimer>
#include <QTextBlock>
#include <QLabel>
#include <QString>
#include "dice.h"
#include "board.h"


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

    board = new Board(scene);
    board->create();

    //create player
    player = new Player(scene, Qt::cyan, "Player 1");
    currentPlayer = player;
    scene->addItem(player->getToken());

    propertyInfoDisplay = new QGraphicsTextItem();
    propertyInfoDisplay->setZValue(100);
    propertyInfoDisplay->setPos(350, 350);
    scene->addItem(propertyInfoDisplay);

    ui->buyButton->setEnabled(false);
    ui->propertyNameLabel->setAlignment(Qt::AlignCenter);
    ui->propertyPriceLabel->setAlignment(Qt::AlignCenter);

    movePlayer(0); //go to go


    connect(ui->buyButton, &QPushButton::clicked, this, &MainWindow::on_buyButton_clicked);
}

void MainWindow::on_rollButton_released()
{
    if (isMoving) return;

    ui->rollButton->setEnabled(false);
    ui->diceLabel->setText("Rolling...");

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
    if (!currentTile || currentTile->isOwned()) return;

    if (currentPlayer->getMoney() >= currentTile->getPrice()) {
        currentPlayer->deductMoney(currentTile->getPrice());
        currentTile->setOwner(currentPlayer);

        ui->statusLabel->setText(QString("%1 bought %2 for $%3!")
                                     .arg(currentPlayer->getName())
                                     .arg(currentTile->getName())
                                     .arg(currentTile->getPrice()));
        handleLanding(currentTile->getIndex());
    }
    else {
        ui->statusLabel->setText("Not enough money to buy this property");
    }
}

void MainWindow::movePlayer(int index){
    QPointF tilePos = board->calculateTilePosition(index);
    player->moveTo(index, tilePos);
    handleLanding(index);
}

void MainWindow::handleLanding(int position){
    currentTile = board->getTile(position);
    if (!currentTile) return;

    // Update property name label
    ui->propertyNameLabel->setText(currentTile->getName());

    // Update property price label (if purchasable)
    if (currentTile->getPrice() > 0) {
        QString priceText = (QString("Price: $%1\nRent: $%2")
                                            .arg(currentTile->getPrice())
                                            .arg(currentTile->getRent()));
        if (currentTile->isOwned()) {
            priceText += QString("\n(Owned by %1)").arg(currentTile->getOwner()->getName());
        }
        ui->propertyPriceLabel->setText(priceText);
    }
    else {
        QString specialText;
        switch(currentTile->getIndex()) {
        case 0: specialText = "Collect $200"; break;
        case 10: specialText = "Just Visiting"; break;
        case 20: specialText = "Free Parking"; break;
        case 30: specialText = "Go to Jail"; break;
        default: specialText = "Not for sale";
        }
        ui->propertyPriceLabel->setText(specialText);
    }
    bool canBuy = !currentTile->isOwned() &&
                  currentTile->getPrice() > 0 &&
                  currentPlayer->getMoney() >= currentTile->getPrice();
    ui->buyButton->setEnabled(canBuy);

    // Highlight current tile
    if (lastHighlightedTile) {
        lastHighlightedTile->highlight(false);
    }
    currentTile->highlight(true);
    lastHighlightedTile = currentTile;

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

