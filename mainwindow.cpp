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
#include "player_setup.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //initialisatie
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0,0,BOARD_SIZE, BOARD_SIZE);

    ui->boardView->setScene(scene);
    ui->boardView->setRenderHint(QPainter::Antialiasing);

    ui->boardView->setFixedSize(BOARD_SIZE + 2, BOARD_SIZE + 2);
    ui->boardView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->boardView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->boardView->scale(0.70, 0.70);

    board = new Board(scene);
    board->create();

    // Add center image/logo
    //QGraphicsPixmapItem *logo = new QGraphicsPixmapItem(QPixmap(":/images/monopoly.png"));
    //logo->setPos(300, 300); // Center position
    //scene->addItem(logo);

    ui->EndTurnButton->setEnabled(false);

    //create players
    PlayerSetup setupDialog(this);
    if (setupDialog.exec() == QDialog::Accepted) {
        QVector<QString> playerNames = setupDialog.getPlayerNames();

        // Create players
        for (int i = 0; i < playerNames.size(); ++i) {
            QColor color;
            switch(i) {
            case 0: color = Qt::red; break;
            case 1: color = Qt::blue; break;
            case 2: color = Qt::green; break;
            case 3: color = Qt::yellow; break;
            case 4: color = Qt::cyan; break;
            case 5: color = Qt::magenta; break;
            case 6: color = Qt::darkYellow; break;
            case 7: color = Qt::gray; break;
            default: color = Qt::white;
            }

            Player* player = new Player(scene, color);
            player->setName(playerNames[i]);
            players.append(player);
            movePlayer(0, player); // Move to start
        }

        currentPlayer = players[0];
        currentPlayerIndex = 0;
        updatePlayerUI();
    }
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
    QTimer::singleShot(800, [this] (){
        auto [dice1,dice2] = Dice::roll();
        int total = dice1 + dice2;
        currentPlayer->worp = total;

        ui->diceLabel->setText(QString("%1 + %2 = %3").arg(dice1).arg(dice2).arg(total));
        animatePlayerMovement(total);
        ui->EndTurnButton->setEnabled(true);
    });
}

void MainWindow::on_buyButton_clicked()
{
    int position = currentPlayer->getPosition();
    Tile* tile = board->tiles[position];

    if (tile->prijs == 0 || tile->Owner != nullptr) {
        qDebug() << "owner: "<< tile->Owner << "geld: " << currentPlayer->Balance << "position: "<< currentPlayer->position << "tileprice: " << tile->prijs;
        ui->statusLabel->setText("This property cannot be purchased.");
        return;
    }

    if (currentPlayer->Balance >= tile->prijs) {
        // Deduct money from player
        currentPlayer->trekGeldAf(tile->prijs);

        // Set ownership
        tile->Owner = currentPlayer;

        // Update UI
        ui->statusLabel->setText(QString("%1 bought %2 for $%3")
                                     .arg(currentPlayer->getName())
                                     .arg(tile->name)
                                     .arg(tile->prijs));

        ui->propertyOwnerLabel->setText(currentPlayer->getName());
        ui->propertyPrice->setText("Rent: $");
        if (position == 5 || position == 15 || position == 25 || position == 35){
            ui->propertyPriceLabel->setText(QString::number(calcStationRent(position)));
        }
        else{
            ui->propertyPriceLabel->setText(QString::number(calcRent(position)));
        }
        ui->propertyOwner->setText("Owner:");

        // Update balance label
        ui->BalanceLabel->setText(QString::number(currentPlayer->Balance));
    } else {
        ui->statusLabel->setText("Not enough money to buy this property.");
    }
}


void MainWindow::movePlayer(int index, Player* player){
    if (!player) player = currentPlayer;
    if (!player) return;

    player->setPosition(index);
    QPointF tilePos = board->calculateTilePosition(index);
    player->getToken()->setPos(tilePos.x() + 25, tilePos.y() + 25);
}

void MainWindow::updatePlayerUI() {
    if (!currentPlayer) return;

    ui->playerNameLabel->setText(currentPlayer->getName());
    ui->BalanceLabel->setText(QString::number(currentPlayer->Balance));
    ui->statusLabel->setText(QString("%1's turn").arg(currentPlayer->getName()));
}

void MainWindow::nextPlayerTurn() {
    if (players.isEmpty()) return;

    currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
    currentPlayer = players[currentPlayerIndex];
    updatePlayerUI();

    ui->rollButton->setEnabled(true);
}

int MainWindow::calcRent(int position) {
    Tile* tile = board->tiles[position];
    int rent = 0;

    switch(tile->huizencount) {
    case 0:
        rent = tile->r1;
        break;
    case 1:
        rent = tile->r2;
        break;
    case 2:
        rent = tile->r3;
        break;
    case 3:
        rent = tile->r4;
        break;
    case 4:
        rent = tile->r5;
        break;
    case 5:
        rent = tile->r6;
        break;
    }

    return rent;
}
void MainWindow::payRent(int position) {
    Tile* tile = board->tiles[position];
    if ((tile->Owner != currentPlayer) && (tile->Owner != nullptr)){
        int rent = calcRent(position);
        currentPlayer->trekGeldAf(rent);
        tile->Owner->voegGeldToe(rent);
        ui->statusLabel->setText(QString("%1 betaalde %2 huur aan %3 op %4")
                                     .arg(currentPlayer->getName())
                                     .arg(rent)
                                     .arg(tile->Owner->getName())
                                     .arg(tile->name));
    }
}
int MainWindow::calcStationRent(int position){
    Tile* tile = board->tiles[position];
    Player* Eigenaar = tile->Owner;
    int stationcount = 0;
    int stationrent = 0;

    //Tel het aantal stations dat de persoon heeft
    if(board->tiles[5]->Owner == Eigenaar){ stationcount += 1;}
    if(board->tiles[15]->Owner == Eigenaar){ stationcount += 1;}
    if(board->tiles[25]->Owner == Eigenaar){ stationcount += 1;}
    if(board->tiles[35]->Owner == Eigenaar){ stationcount += 1;}
    switch(stationcount){
    case 1:
        stationrent = tile->r1;
        break;
    case 2:
        stationrent = tile->r2;
        break;
    case 3:
        stationrent = tile->r3;
        break;
    case 4:
        stationrent = tile->r4;
        break;
    }
    return stationrent;
}
void MainWindow::payStationRent(int position){
    Tile* tile = board->tiles[position];
    Player* Eigenaar = tile->Owner;
    if ((Eigenaar == nullptr) or (Eigenaar == currentPlayer)){ return; }
    currentPlayer->trekGeldAf(calcStationRent(position));
    Eigenaar->voegGeldToe(calcStationRent(position));
    ui->statusLabel->setText(QString("%1 betaalde %2 huur aan %3 op %4")
                                 .arg(currentPlayer->getName())
                                 .arg(calcStationRent(position))
                                 .arg(tile->Owner->getName())
                                 .arg(tile->name));
}

int MainWindow::calcNutsbedrijvenRent(int worp,int position){
    Tile* tile = board->tiles[position];
    int rent = 0;
    if (tile->Owner == board->tiles[12]->Owner && tile->Owner == board->tiles[28]->Owner){
        rent = worp * 10;
    }
    else{
        rent = worp * 4;
    }
    return rent;
}

void MainWindow::PayNutsBedrijvenRent(int worp,int position){
    Tile* tile = board->tiles[position];
    if (tile->Owner != currentPlayer && tile->Owner != nullptr){
        currentPlayer->trekGeldAf(calcNutsbedrijvenRent(worp,position));
        tile->Owner->voegGeldToe(calcNutsbedrijvenRent(worp,position));
        ui->statusLabel->setText(QString("%1 betaalde %2 huur aan %3 op %4")
                                     .arg(currentPlayer->getName())
                                     .arg(calcNutsbedrijvenRent(worp,position))
                                     .arg(tile->Owner->getName())
                                     .arg(tile->name));
    }
}


void MainWindow::handleLanding(int position) {
    qDebug() << "Landed on property index: " << currentPlayer->getPosition();
    Tile* tile = board->tiles[position];

    // === Display Property Info ===
    ui->propertyNameLabel->setText(tile->name);

    // === Reactivate buyproperty ===
    ui->buyButton->setEnabled(true);

    // === Special Tiles Logic ===
    ui->KansAlg->setWordWrap(true);

    if (tile->prijs == 0) {
        ui->propertyPrice->clear();
        ui->propertyPriceLabel->clear();
        ui->propertyOwner->clear();
        ui->propertyOwnerLabel->clear();
    } else if (tile->Owner == nullptr) {
        ui->propertyPrice->setText("Price: $");
        ui->propertyPriceLabel->setText(QString::number(tile->prijs));
        ui->propertyOwner->clear();
        ui->propertyOwnerLabel->clear();
    } else {
        ui->propertyPrice->setText("Rent: $");
        if (position == 5 || position == 15 || position == 25 || position == 35){
            ui->propertyPriceLabel->setText(QString::number(calcStationRent(position)));}
        else{
            ui->propertyPriceLabel->setText(QString::number(calcRent(position)));}
        ui->propertyOwner->setText("Owner:");
        ui->propertyOwnerLabel->setText(tile->Owner->name);
    }


    // Start salary if passed "Start"
    if (position < currentPlayer->getPrevPosition()) {
        ui->LoonLabel->setText("Je ontvangt 200 euro loon!");
        currentPlayer->voegGeldToe(200);
        QTimer::singleShot(3000, this, [this]() {
            ui->LoonLabel->clear();
        });
    }
    //Train Stations
    if((position == 5) || (position == 15) || (position == 25) || (position == 35)){
        payStationRent(currentPlayer->getPosition());
    }
    else if (position == 12 || position == 28){
        PayNutsBedrijvenRent(currentPlayer->worp,position);
    }
    // Kans
    else if (position == 7 || position == 22 || position == 36) {
        KansStapelp kans;
        KansKaart kaart = kans.trekKansKaart(currentPlayer, this);
        ui->KansAlg->setText(kaart.beschrijving);
        QTimer::singleShot(5000, this, [this]() { ui->KansAlg->clear(); });
    }

    // Algemeen Fonds
    else if (position == 2 || position == 17 || position == 33) {
        AlgemeenFondsStapelp AlgFond;
        FondsKaart kaart = AlgFond.trekAlgKaart(currentPlayer, this);
        ui->KansAlg->setText(kaart.beschrijving);
        QTimer::singleShot(5000, this, [this]() { ui->KansAlg->clear(); });
    }

    // Go to jail
    else if (position == 30) {
        currentPlayer->inJail = true;
        ui->KansAlg->setText("Go to Jail!");
        QTimer::singleShot(500, this, [this]() {
            this->movePlayer(10, currentPlayer);
            ui->KansAlg->clear();
        });
    }

    // Extra belasting
    else if (position == 38) {
        currentPlayer->trekGeldAf(100);
        ui->KansAlg->setText("Je moet Extra Belasting betalen aan de gemeente.");
        QTimer::singleShot(5000, this, [this]() { ui->KansAlg->clear(); });
    }

    // Inkomsten belasting
    else if (position == 4) {
        currentPlayer->trekGeldAf(200);
        ui->KansAlg->setText("Je Moet inkomsten belasting betalen aan de gemeente.");
        QTimer::singleShot(3000, this, [this]() { ui->KansAlg->clear(); });
    }
    else {
        payRent(currentPlayer->getPosition());
    }

    // Update player balance
    currentPlayer->prevPosition = currentPlayer->getPosition();
    ui->BalanceLabel->setText(QString::number(currentPlayer->Balance));
}

void MainWindow::animatePlayerMovement(int steps){
    if(isMoving) return;

    isMoving = true;
    int targetPosition = (currentPlayer->getPosition() + steps) % 40;

    // Animate each step
    for (int i = 1; i <= steps; i++) {
        QTimer* timer = new QTimer(this);
        timer->setSingleShot(true);
        timer->setInterval(i * 150);  // 150ms between steps

        connect(timer, &QTimer::timeout, [=]() {
            int newPos = (currentPlayer->getPosition() + 1) % 40;
            movePlayer(newPos, currentPlayer);

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
    moveTimers.clear();
    qDeleteAll(players);
    delete board;
    delete ui;
}


void MainWindow::on_EndTurnButton_released()
{
    ui->EndTurnButton->setEnabled(false);
    ui->buyButton->setEnabled(false);
    ui->propertyNameLabel->clear();
    ui->propertyPrice->clear();
    ui->propertyPriceLabel->clear();
    ui->propertyOwner->clear();
    ui->propertyOwnerLabel->clear();
    ui->KansAlg->clear();
    nextPlayerTurn();
    ui->rollButton->setEnabled(true);

}
