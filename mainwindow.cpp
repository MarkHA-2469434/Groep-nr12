#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QPushButton>
#include <QRandomGenerator>
#include <QTimer>
#include <QTextBlock>
#include <QMessageBox>

#include "dice.h"
#include "player_setup.h"
#include "bot.h"
#include "board.h"
#include "Kans_AlgFonds.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qDebug() << "MainWindow constructor started";
    ui->setupUi(this);
    qDebug() << "UI setup complete";

    //initialisatie
    qDebug() << "Creating QGraphicsScene";
    scene = new QGraphicsScene(this);
    qDebug() << "Scene created at" << scene;

    scene->setSceneRect(0,0,BOARD_SIZE, BOARD_SIZE);

    ui->boardView->setScene(scene);
    ui->boardView->setRenderHint(QPainter::Antialiasing);

    ui->boardView->setFixedSize(BOARD_SIZE + 2, BOARD_SIZE + 2);
    ui->boardView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->boardView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //ui->boardView->scale(0.70, 0.70);

    connect(ui->EndTurnButton, &QPushButton::clicked, this, &MainWindow::on_EndTurnButton_released);
    connect(ui->rollButton, &QPushButton::released, this, &MainWindow::on_rollButton_released);
    qDebug() << "Roll button connection established";

    qDebug() << "Creating Board";
    board = new Board(scene);
    qDebug() << "Board created at" << board;
    board->create();
    qDebug() << "Board creation successful";

    // Add center image/logo
    //QGraphicsPixmapItem *logo = new QGraphicsPixmapItem(QPixmap(":/images/monopoly.png"));
    //logo->setPos(300, 300); // Center position
    //scene->addItem(logo);

    ui->EndTurnButton->setEnabled(false);

    //create players
    qDebug() << "Initializing player setup dialog";
    PlayerSetup setupDialog(this);
    qDebug() << "PlayerSetup dialog created";

    if (setupDialog.exec() == QDialog::Accepted) {
        qDebug() << "PlayerSetup accepted";

        int playerCount = setupDialog.getPlayerCount();
        QVector<QString> playerNames = setupDialog.getPlayerNames();
        QVector<bool> isBotFlags = setupDialog.getIsBotFlags();

        qDebug() << "Creating" << playerCount << "players";
        qDebug() << "Player names:" << playerNames;
        qDebug() << "Bot flags:" << isBotFlags;

        // Create players
        QVector<QColor> colors = {
            Qt::red, Qt::blue, Qt::green, Qt::yellow,
            Qt::cyan, Qt::magenta, Qt::darkYellow, Qt::gray
        };

        for (int i = 0; i < playerCount; ++i) {
            qDebug() << "Creating player" << i;

            Player* player = isBotFlags[i] ? new Bot(scene, colors[i]) : new Player(scene, colors[i]);
            qDebug() << "Player" << i << "created at" << player;

            player->setName(playerNames[i]);
            players.append(player);
            qDebug() << "Moving player" << i << "to start position";
            movePlayer(0, player);
        }

        if (!players.isEmpty()) {
            currentPlayer = players.first();
            currentPlayerIndex = 0;

            qDebug() << "Current player set to:" << currentPlayer->getName() << "Is bot:" << isBot(currentPlayer) << "Pointer:" << currentPlayer;
        }

        if (isBot(currentPlayer)) {
            botTurn();
        } else {
            ui->rollButton->setEnabled(true);
        }
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
    qDebug() << "Roll button pressed";

    if (isMoving){
        qDebug() << "Movement already in progress";
        return;
    }

    ui->rollButton->setEnabled(false);
    ui->EndTurnButton->setEnabled(false);

    ui->rollButton->setEnabled(false);
    ui->diceLabel->setText("Rolling...");

    QTimer::singleShot(800, [this]() {
        auto [dice1, dice2] = Dice::roll();
        int total = dice1 + dice2;
        currentPlayer->worp = total;

        ui->diceLabel->setText(QString("%1 + %2 = %3").arg(dice1).arg(dice2).arg(total));
        animatePlayerMovement(total);

        if (currentPlayer->inJail) {
            if (dice1 == dice2) {
                ui->KansAlg->setText("You rolled doubles! You're free.");
                currentPlayer->inJail = false;
                currentPlayer->jailTurns = 0;

                QTimer::singleShot(2000, this, [this, total]() {
                    ui->KansAlg->clear();
                    animatePlayerMovement(total);
                });
            } else {
                currentPlayer->jailTurns++;
                ui->KansAlg->setText("Still in jail. Turn " + QString::number(currentPlayer->jailTurns));

                if (currentPlayer->jailTurns >= 3) {
                    ui->KansAlg->setText("You served 3 turns. You're free!");
                    currentPlayer->inJail = false;
                    currentPlayer->jailTurns = 0;

                    QTimer::singleShot(2000, this, [this, total]() {
                        ui->KansAlg->clear();
                        animatePlayerMovement(total);
                    });
                } else {
                    QTimer::singleShot(3000, this, [this]() {
                        ui->KansAlg->clear();
                        hasRolled = false;
                        ui->rollButton->setEnabled(true);
                        ui->EndTurnButton->setEnabled(true);
                    });
                }
            }
        } else {
            animatePlayerMovement(total);
        }
    });
}


void MainWindow::on_buyButton_clicked()
{
    int position = currentPlayer->getPosition();
    Tile* tile = board->getTiles()[position];

    if (tile->prijs == 0 || tile->Owner != nullptr) {
        qDebug() << "owner: "<< tile->Owner << "geld: " << currentPlayer->Balance << "position: "<< currentPlayer->position << "tileprice: " << tile->prijs;
        ui->statusLabel->setText("This property cannot be purchased.");
        return;
    }

    if (currentPlayer->Balance >= tile->prijs) {
        currentPlayer->trekGeldAf(tile->prijs);
        tile->Owner = currentPlayer;
        currentPlayer->addProperty(tile);

        ui->statusLabel->setText(QString("%1 bought %2").arg(currentPlayer->getName()).arg(tile->name));
        ui->buyButton->setEnabled(false);
    } else {
        ui->statusLabel->setText("Not enough money!");
    }
    ui->BalanceLabel->setText(QString::number(currentPlayer->Balance));
}


void MainWindow::movePlayer(int index, Player* player){
    if (!player || !board || !scene) {
        qCritical() << "Invalid movePlayer parameters!";
        return;
    }
    player->setPosition(index);
    qDebug() << "Player position set to:" << index;

    QPointF tilePos = board->calculateTilePosition(index);
    qDebug() << "Tile position calculated:" << tilePos;

    if (auto token = dynamic_cast<QGraphicsEllipseItem*>(player->getToken())) {
        token->setPos(tilePos.x() + 25, tilePos.y() + 25);
        qDebug() << player->getName() << "moved to" << index << "at" << tilePos;
    } else {
        qWarning() << "Invalid player token!";
    }
}

void MainWindow::updatePlayerUI() {
    if (!currentPlayer) {
        qCritical() << "updatePlayerUI: currentPlayer is null!";
        return;
    }
    ui->playerNameLabel->setText(currentPlayer->getName());
    ui->BalanceLabel->setText(QString::number(currentPlayer->Balance));
    ui->statusLabel->setText(QString("%1's turn").arg(currentPlayer->getName()));

    if (isBot(currentPlayer)) {
        ui->statusLabel->setText(QString("%1 (Bot) is thinking...").arg(currentPlayer->getName()));
    } else {
        ui->statusLabel->setText(QString("%1's turn - Roll the dice!").arg(currentPlayer->getName()));
    }
}

void MainWindow::nextPlayerTurn() {
    qDebug() << "Starting nextPlayerTurn()";

    if (players.empty()){
        qDebug() << "No players exist!";
        return;
    }

    isMoving = false;
    qDeleteAll(moveTimers);
    moveTimers.clear();

    if (currentPlayer) {
        if (QGraphicsEllipseItem* token = dynamic_cast<QGraphicsEllipseItem*>(currentPlayer->getToken())) {
            token->setBrush(QBrush(currentPlayer->getPlayerColor()));
            token->setPen(QPen(Qt::black, 1));
            token->setScale(1.0);
        }
    }

    ui->propertyNameLabel->clear();
    ui->propertyPriceLabel->clear();

    currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
    currentPlayer = players[currentPlayerIndex];

    qDebug() << "New current player:" << currentPlayer->getName() << (isBot(currentPlayer) ? "(Bot)" : "(Human)");

    highlightCurrentPlayer();
    updatePlayerUI();
    if (!currentPlayer) {
        qCritical() << "Invalid current player!";
        currentPlayer = players.first();
    }

    if (currentPlayer->inJail) {
        handleJailTurn();
    }
    if (isBot(currentPlayer)) {
        qDebug() << "Starting bot turn sequence";
        QTimer::singleShot(500, this, &MainWindow::botTurn);
    }
    else {
        //human turn
        ui->rollButton->setEnabled(true);
        ui->EndTurnButton->setEnabled(false);
        ui->statusLabel->setText(QString("%1's turn - Roll the dice!").arg(currentPlayer->getName()));

    updatePlayerUI();

    ui->rollButton->setEnabled(true);
    ui->EndTurnButton->setEnabled(false);
    ui->diceLabel->clear();
}
}

int MainWindow::calcRent(int position) {
    Tile* tile = board->getTiles()[position];
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
    Tile* tile = board->getTiles()[position];
    if ((tile->Owner != currentPlayer) && (tile->Owner != nullptr)){
        int rent = calcRent(position);
        currentPlayer->trekGeldAf(rent);
        tile->Owner->voegGeldToe(rent);
        ui->statusLabel->setText(QString("%1 betaalde %2 huur aan %3 op %4").arg(currentPlayer->getName()).arg(rent).arg(tile->Owner->getName()).arg(tile->name));
    }
}
int MainWindow::calcStationRent(int position){
    Tile* tile = board->getTiles()[position];
    Player* Eigenaar = tile->Owner;
    int stationcount = 0;
    int stationrent = 0;

    //Tel het aantal stations dat de persoon heeft
    if(board->getTiles()[5]->Owner == Eigenaar){ stationcount += 1;}
    if(board->getTiles()[15]->Owner == Eigenaar){ stationcount += 1;}
    if(board->getTiles()[25]->Owner == Eigenaar){ stationcount += 1;}
    if(board->getTiles()[35]->Owner == Eigenaar){ stationcount += 1;}
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
    Tile* tile = board->getTiles()[position];
    Player* Eigenaar = tile->Owner;
    if ((Eigenaar == nullptr) or (Eigenaar == currentPlayer)){ return; }
    currentPlayer->trekGeldAf(calcStationRent(position));
    Eigenaar->voegGeldToe(calcStationRent(position));
    ui->statusLabel->setText(QString("%1 betaalde %2 huur aan %3 op %4").arg(currentPlayer->getName()).arg(calcStationRent(position)).arg(tile->Owner->getName()).arg(tile->name));
}

int MainWindow::calcNutsbedrijvenRent(int worp,int position){
    Tile* tile = board->getTiles()[position];
    int rent = 0;
    if (tile->Owner == board->getTiles()[12]->Owner && tile->Owner == board->getTiles()[28]->Owner){
        rent = worp * 10;
    }
    else{
        rent = worp * 4;
    }
    return rent;
}

void MainWindow::PayNutsBedrijvenRent(int worp,int position){
    Tile* tile = board->getTiles()[position];
    if (tile->Owner != currentPlayer && tile->Owner != nullptr){
        currentPlayer->trekGeldAf(calcNutsbedrijvenRent(worp,position));
        tile->Owner->voegGeldToe(calcNutsbedrijvenRent(worp,position));
        ui->statusLabel->setText(QString("%1 betaalde %2 huur aan %3 op %4").arg(currentPlayer->getName()).arg(calcNutsbedrijvenRent(worp,position)).arg(tile->Owner->getName()).arg(tile->name));
    }
}

void MainWindow::highlightCurrentPlayer() {
    if (!currentPlayer) return;

    for (Player* p : players) {
        if (p != currentPlayer) {
            QGraphicsEllipseItem* token = static_cast<QGraphicsEllipseItem*>(p->getToken());
            token->setBrush(QBrush(p->getPlayerColor()));
            token->setPen(QPen(Qt::black, 1));
            token->setScale(1.0);
            token->setZValue(500);
        }
    }

    if (QGraphicsEllipseItem* currentToken = dynamic_cast<QGraphicsEllipseItem*>(currentPlayer->getToken())) {
        currentToken->setBrush(QBrush(currentPlayer->getPlayerColor().lighter(150)));
        currentToken->setPen(QPen(Qt::white, 3));
        currentToken->setScale(1.3);
        currentToken->setZValue(1000);
        currentToken->update();
    }

    scene->update();
}

void MainWindow::botTurn() {
    qDebug() << "Starting bot turn for" << currentPlayer->getName();

    if (!currentPlayer || isMoving){
        qDebug() << "Invalid bot turn state - aborting";
        return;
    }

    ui->rollButton->setEnabled(false);
    ui->EndTurnButton->setEnabled(false);
    ui->buyButton->setEnabled(false);

    QTimer::singleShot(1500, this, [this]() {
        qDebug() << "Processing bot move for" << currentPlayer->getName();

        if (!currentPlayer){
            qDebug() << "Current player is null!";
            return;
        }

        try {

            auto [dice1, dice2] = Dice::roll();
            int steps = dice1 + dice2;
            currentPlayer->worp = steps;

            ui->diceLabel->setText(QString("%1 + %2 = %3").arg(dice1).arg(dice2).arg(steps));
            qDebug() << "Bot rolling:" << steps << "steps";

            int newPos = (currentPlayer->getPosition() + steps) % 40;
            qDebug() << "Bot moving from" << currentPlayer->getPosition() << "to" << newPos << "(" << steps << "steps)";

            movePlayer(newPos, currentPlayer);
            handleLanding(newPos);

            Tile* tile = board->getTile(newPos);
            if (!tile->Owner && tile->prijs > 0 && currentPlayer->Balance >= tile->prijs) {
                on_buyButton_clicked();
            }

            QTimer::singleShot(1000, this, &MainWindow::nextPlayerTurn);

        } catch (...) {
            qCritical() << "Exception in botTurn!";
            nextPlayerTurn();
        }
    });
}

void MainWindow::handleJailTurn(){
    if (!currentPlayer->inJail){
        ui->rollButton->setEnabled(true);
        return;
    }

    ui->rollButton->setEnabled(false);
    currentPlayer->turnsInJail++;
    qDebug() << currentPlayer->getName() << "in jail (turn" << currentPlayer->turnsInJail << "/3)";


    //check if player has get out of jail free card
    bool hasJailCard = currentPlayer->jailCards > 0;

    QMessageBox jailOptions(this);
    jailOptions.setWindowTitle("Jail Options");
    jailOptions.setText(QString("%1 is in jail (Turn %2/3)").arg(currentPlayer->getName()).arg(currentPlayer->turnsInJail));

    QPushButton *payButton = jailOptions.addButton("Pay €50", QMessageBox::ActionRole);
    QPushButton *rollButton = jailOptions.addButton("Roll for Doubles", QMessageBox::ActionRole);
    QPushButton *cardButton = nullptr;

    if (hasJailCard) {
        cardButton = jailOptions.addButton("Use Get Out of Jail Card", QMessageBox::ActionRole);
    }

    jailOptions.exec();

    if (jailOptions.clickedButton() == payButton){
        payJailFine();
    }
    else if (jailOptions.clickedButton() == rollButton){
        attemptJailEscape();
    }
    else if (hasJailCard && jailOptions.clickedButton() == cardButton){
        useJailCard();
    }

    if (!currentPlayer->inJail){
        ui->rollButton->setEnabled(true);
    }
}

void MainWindow::payJailFine() {
    if (currentPlayer->Balance >= 50) {
        currentPlayer->trekGeldAf(50);
        currentPlayer->inJail = false;
        currentPlayer->turnsInJail = 0;
        ui->statusLabel->setText("Paid €50 to get out of jail");
        ui->BalanceLabel->setText(QString::number(currentPlayer->Balance));
        ui->rollButton->setEnabled(true);
    } else {
        ui->statusLabel->setText("Not enough money to pay jail fine!");
    }
}

void MainWindow::useJailCard() {
    currentPlayer->jailCards--;
    currentPlayer->inJail = false;
    currentPlayer->turnsInJail = 0;
    ui->statusLabel->setText("Used Get Out of Jail Free card");
}

void MainWindow::attemptJailEscape() {
    auto [dice1, dice2] = Dice::roll();
    int total = dice1 + dice2;

    ui->diceLabel->setText(QString("%1 + %2 = %3").arg(dice1).arg(dice2).arg(total));

    if (dice1 == dice2) { //doubles rolled
        currentPlayer->inJail = false;
        currentPlayer->turnsInJail = 0;
        ui->statusLabel->setText("Rolled doubles! You're free!");
        animatePlayerMovement(total);
    } else {
        if (currentPlayer->turnsInJail >= 3) {
            payJailFine(); //pay after 3 fails
            if (currentPlayer->Balance < 50) {
                // Handle bankruptcy if can't pay
                ui->statusLabel->setText("Couldn't pay jail fine! Bankrupt!");
                // handle bankruptcy
            }
        } else {
            ui->statusLabel->setText("Failed to roll doubles. Still in jail.");
            QTimer::singleShot(1500, this, &MainWindow::nextPlayerTurn);
        }
    }
}

void MainWindow::rollDice() {
    on_rollButton_released();
}

void MainWindow::purchaseCurrentProperty() {
    on_buyButton_clicked();
}


void MainWindow::handleLanding(int position) {
    qDebug() << "Landed on property index: " << currentPlayer->getPosition();
    Tile* tile = board->getTiles()[position];

    //Display Property Info
    ui->propertyNameLabel->setText(tile->name);

    //Reactivate buyproperty
    ui->buyButton->setEnabled(true);

    // Special Tiles Logic
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

    if( (position == 7) || (position== 22 ) || (position == 36) ){//kans kaart
        KansKaart kaart = kans.trekKansKaart(currentPlayer, this);
        ui->KansAlg->setText(kaart.beschrijving);

        if (kaart.isJailCard) {
            currentPlayer->jailCards++;
        }

        QTimer::singleShot(5000, this, [this]() {
            ui->KansAlg->clear();
        });
    }
    else if( (position == 2) || (position== 17) || (position== 33) ){//Algemeen Fonds
        FondsKaart kaart = algFond.trekAlgKaart(currentPlayer, this);
        ui->KansAlg->setText(kaart.beschrijving);

        if (kaart.isJailCard) {
            currentPlayer->jailCards++;
        }

        QTimer::singleShot(5000, this, [this]() {
            ui->KansAlg->clear();
        });
    }
    else if (position == 30) { // Go to jail
        currentPlayer->inJail = true;
        currentPlayer->turnsInJail = 0;
        ui->KansAlg->setText("Go to Jail!");
        QTimer::singleShot(500, this, [this]() {
            this->movePlayer(10, currentPlayer);
            ui->KansAlg->clear();
            handleJailTurn();
        });
        return;
    }
    else if (position == 38) { // Extra belasting
        currentPlayer->trekGeldAf(100);
        ui->KansAlg->setText("Je moet Extra Belasting betalen aan de gemeente.");
        QTimer::singleShot(5000, this, [this]() {
            ui->KansAlg->clear();
        });
    }
    else if (position == 4) { // Inkomsten Belasting
        currentPlayer->trekGeldAf(200);
        ui->KansAlg->setText("Je Moet inkomsten belasting betalen aan de gemeente.");
        QTimer::singleShot(3000, this, [this]() {
            ui->KansAlg->clear();
        });
    }

    // Handle property payments
    if (position == 5 || position == 15 || position == 25 || position == 35) {
        payStationRent(position);
    }
    else if (position == 12 || position == 28) {
        PayNutsBedrijvenRent(currentPlayer->worp, position);
    }
    else if (tile->Owner && tile->Owner != currentPlayer) {
        payRent(position);
    }

    // Update UI
    currentPlayer->prevPosition = currentPlayer->getPosition();
    ui->BalanceLabel->setText(QString::number(currentPlayer->Balance));

    if (currentPlayer->inJail) {
        handleJailTurn();
    }

    if (!currentPlayer->inJail && !isBot(currentPlayer)) {
        ui->EndTurnButton->setEnabled(true);
    }

    if (isBot(currentPlayer)) {
        Tile* tile = board->getTiles()[position];
        if (!tile->Owner && tile->prijs > 0 && currentPlayer->Balance >= tile->prijs) {
            on_buyButton_clicked();
        }
    }
}

void MainWindow::animatePlayerMovement(int steps) {
    if (isMoving) {
        qDebug() << "Movement already in progress";
        return;
    }

    isMoving = true;
    int startPos = currentPlayer->getPosition();

    // Clear any existing timers
    qDeleteAll(moveTimers);
    moveTimers.clear();

    for (int i = 1; i <= steps; i++) {
        QTimer* timer = new QTimer(this);
        moveTimers.append(timer);

        connect(timer, &QTimer::timeout, [this, startPos, i, steps]() {
            if (!currentPlayer) return;

            int newPos = (startPos + i) % 40;
            movePlayer(newPos, currentPlayer);
            qDebug() << currentPlayer->getName() << "moved to position" << newPos;

            // Handle final position
            if (i == steps) {
                isMoving = false;
                currentPlayer->setPosition(newPos);
                qDebug() << "Movement complete at position" << newPos;

                QTimer::singleShot(300, [this, newPos]() {
                    handleLanding(newPos);

                    if (!isBot(currentPlayer)) {
                        ui->EndTurnButton->setEnabled(!currentPlayer->inJail);
                    }
                });
            }
        });

        timer->setSingleShot(true);
        timer->start(i * 200); // 200ms inbetween steps
    }
}

void MainWindow::on_EndTurnButton_released()
{
    if (isMoving) return;

    ui->EndTurnButton->setEnabled(false);
    ui->rollButton->setEnabled(false);
    ui->buyButton->setEnabled(false);

    ui->propertyNameLabel->clear();
    ui->propertyPriceLabel->clear();
    ui->propertyOwnerLabel->clear();
    ui->KansAlg->clear();

    QTimer::singleShot(500, this, &MainWindow::nextPlayerTurn);
}

bool MainWindow::isBot(Player* player) const {
    return dynamic_cast<Bot*>(player) != nullptr;
}

MainWindow::~MainWindow()
{
    qDeleteAll(moveTimers);
    moveTimers.clear();
    qDeleteAll(players);
    delete board;
    delete ui;
}
