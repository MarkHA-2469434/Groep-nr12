#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QPushButton>
#include <QRandomGenerator>
#include <QTimer>
#include <QTextBlock>
#include <QMessageBox>
#include <QComboBox>
#include <QSpinBox>
#include <QListWidget>

#include "dice.h"
#include "player_setup.h"
#include "bot.h"
#include "board.h"
#include "Kans_AlgFonds.h"
#include "playerinfobox.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    showMaximized();

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

    board = new Board(scene);
    board->create();

    connect(ui->rollButton, &QPushButton::released, this, &MainWindow::on_rollButton_released);
    connect(ui->EndTurnButton, &QPushButton::released, this, &MainWindow::on_EndTurnButton_released);
    connect(ui->buyButton, &QPushButton::clicked, this, &MainWindow::on_buyButton_clicked);
    connect(ui->OwnerPropsButton, &QPushButton::released, this, &MainWindow::on_OwnerPropsButton_released);
    connect(ui->tradeButton, &QPushButton::clicked, this, &MainWindow::on_tradeButton_released);
    connect(ui->forceDoublesCheck, &QCheckBox::toggled, ui->specificRollSpin, &QSpinBox::setDisabled);
    connect(ui->FaillietButton, &QPushButton::released, this, &MainWindow::on_FaillietButton_released);

    QPixmap logo(":/image/markopoly.png");
    logo = logo.scaled(720, 720, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QGraphicsPixmapItem *logoItem = new QGraphicsPixmapItem(logo);
    logoItem->setPos(
        (BOARD_SIZE - logo.width()) / 2,  // X
        (BOARD_SIZE - logo.height()) / 2  // Y
        );
    logoItem->setOpacity(0.85);
    scene->addItem(logoItem);
    logoItem->setZValue(1);

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
        createPlayerInfoWidgets();

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

void MainWindow::createPlayerInfoWidgets() {
    // Clear existing widgets
    QLayoutItem* item;
    while ((item = ui->playerInfoLayout->takeAt(0))) {
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    // Create a widget for each player
    for (Player* player : players) {
        PlayerInfoBox* playerBox = new PlayerInfoBox();
        playerBox->setTitle(player->getName());
        playerBox->setFixedWidth(320);

        QVBoxLayout* boxLayout = new QVBoxLayout();

        // Player color indicator
        QLabel* colorLabel = new QLabel();
        colorLabel->setFixedSize(20, 20);
        QPalette colorPal = colorLabel->palette();
        colorPal.setColor(QPalette::Window, player->getPlayerColor());
        colorLabel->setAutoFillBackground(true);
        colorLabel->setPalette(colorPal);
        colorLabel->setFrameShape(QFrame::Panel);
        colorLabel->setFrameShadow(QFrame::Sunken);


        // Money label
        QLabel* moneyLabel = new QLabel();
        moneyLabel->setObjectName("money_" + player->getName());
        moneyLabel->setText(QString("€%1").arg(player->Balance));

        // Properties owned
        QLabel* propsLabel = new QLabel();
        propsLabel->setObjectName("props_" + player->getName());
        propsLabel->setText(QString("%1 properties").arg(player->ownedProperties.size()));

        // Jail status
        QLabel* jailLabel = new QLabel();
        jailLabel->setObjectName("jail_" + player->getName());
        jailLabel->setText(player->inJail ? "In Jail" : "Free");

        // Assemble widget
        boxLayout->addWidget(colorLabel);
        boxLayout->addWidget(moneyLabel);
        boxLayout->addWidget(propsLabel);
        boxLayout->addWidget(jailLabel);
        playerBox->setLayout(boxLayout);

        ui->playerInfoLayout->addWidget(playerBox);
        ui->playerInfoLayout->addSpacing(5);
    }
    ui->playerInfoLayout->addStretch();

}

void MainWindow::updatePlayerInfo() {
    for (Player* player : players) {
        PlayerInfoBox* box = findChild<PlayerInfoBox*>("playerBox_" + player->getName());
        if (box) {
            box->setHighlighted(false);
        }
    }

    if (currentPlayer) {
        PlayerInfoBox* currentBox = findChild<PlayerInfoBox*>("playerBox_" + currentPlayer->getName());

        if (currentBox) {
            PlayerInfoBox* currentBox = findChild<PlayerInfoBox*>("playerBox_" + currentPlayer->getName());
            if (currentBox) {
                currentBox->setHighlighted(true);
        }
    }

    for (Player* player : players) {
        // Find widgets by object name
        QLabel* moneyLabel = findChild<QLabel*>("money_" + player->getName());
        QLabel* propsLabel = findChild<QLabel*>("props_" + player->getName());
        QLabel* jailLabel = findChild<QLabel*>("jail_" + player->getName());

        if (moneyLabel) moneyLabel->setText(QString("€%1").arg(player->Balance));
        if (propsLabel) propsLabel->setText(QString("%1 properties").arg(player->ownedProperties.size()));
        if (jailLabel) jailLabel->setText(player->inJail ? "In Jail" : "Free");
    }
}
}
void MainWindow::on_rollButton_released()
{
    bool forceDoubles = ui->forceDoublesCheck->isChecked();
    bool useForcedRoll = ui->specificRollSpin->isEnabled();
    int forcedRoll = ui->specificRollSpin->value();

    qDebug() << "Roll button pressed";
    qDebug() << "Force doubles:" << forceDoubles;
    qDebug() << "Forced roll:" << (useForcedRoll ? QString::number(forcedRoll) : "disabled");

    if (isMoving){
        qDebug() << "Movement already in progress";
        return;
    }

    ui->rollButton->setEnabled(false);
    ui->EndTurnButton->setEnabled(false);
    ui->diceLabel->setText("Rolling...");

    QTimer::singleShot(800, [this, forceDoubles, useForcedRoll, forcedRoll]() {
        int dice1, dice2;

        if (forceDoubles) {
            //pick a random number 1-6 for both dice
            dice1 = QRandomGenerator::global()->bounded(1, 7);
            dice2 = dice1;
        }
        else if (useForcedRoll) {
            // Distribute total as evenly as possible between two dice
            int half = forcedRoll / 2;
            dice1 = qMax(1, qMin(6, half));
            dice2 = forcedRoll - dice1;

            if (dice2 > 6) {
                dice1 = 6;
                dice2 = forcedRoll - 6;
            } else if (dice2 < 1) {
                dice1 = forcedRoll - 1;
                dice2 = 1;
            }
        }
        else {
            // Normal roll
            auto roll = Dice::roll();
            dice1 = std::get<0>(roll);
            dice2 = std::get<1>(roll);
        }

        int total = dice1 + dice2;
        currentPlayer->worp = total;

        ui->diceLabel->setText(QString("%1 + %2 = %3").arg(dice1).arg(dice2).arg(total));

        bool isDoubles = (dice1 == dice2);

        if (isDoubles) {
            consecutiveDoubles++;
            qDebug() << "Doubles rolled! Consecutive count:" << consecutiveDoubles;
        }

        if (consecutiveDoubles >= 3) {
            handleThreeDoubles();
            return;
        }

        animatePlayerMovement(total, isDoubles);
    });
}


void MainWindow::on_buyButton_clicked()
{
    int position = currentPlayer->getPosition();
    Tile* tile = board->tiles[position];

    if (tile->prijs == 0 || tile->Owner != nullptr) {
        ui->statusLabel->setText("This property cannot be purchased.");
        return;
    }

    if (currentPlayer->Balance >= tile->prijs) {
        // Deduct money from player
        currentPlayer->trekGeldAf(tile->prijs);
        // Set ownership
        tile->Owner = currentPlayer;
        updateTileOwnerLabel(tile);

        currentPlayer->ownedProperties.append(tile);

        // Update UI
        ui->statusLabel->setText(QString("%1 bought %2 for $%3")
                                     .arg(currentPlayer->getName())
                                     .arg(tile->name)
                                     .arg(tile->prijs));

        ui->propertyOwnerLabel->setText(currentPlayer->getName());
        ui->propertyPrice->setText("Rent: $");
        if (position == 5 || position == 15 || position == 25 || position == 35){
            ui->propertyPriceLabel->setText(QString::number(calcStationRent(position)));}
        else if (position == 12 || position == 28){
            ui->propertyPriceLabel->setText(QString::number(calcNutsbedrijvenRent(currentPlayer->worp,position)));}

        else{
            ui->propertyPriceLabel->setText(QString::number(calcRent(position)));}
        ui->propertyOwner->setText("Owner:");

        // Update balance label
        ui->BalanceLabel->setText(QString::number(currentPlayer->Balance));
    } else {
        ui->statusLabel->setText("Not enough money to buy this property.");
    }
}

void MainWindow::handleJailRoll(int dice1, int dice2)
{
    //show what was rolled
    int total = dice1 + dice2;
    ui->diceLabel->setText(QString("%1 + %2 = %3").arg(dice1).arg(dice2).arg(total));

    if (dice1 == dice2) {
        ui->KansAlg->setText("You rolled doubles! You're free.");
        currentPlayer->inJail = false;
        currentPlayer->jailTurns = 0;
        consecutiveDoubles = 1;

        QTimer::singleShot(2000, this, [this, dice1, dice2]() {
            ui->KansAlg->clear();
            animatePlayerMovement(dice1 + dice2, true);
        });
    } else {
        currentPlayer->jailTurns++;
        ui->KansAlg->setText("Still in jail. Turn " + QString::number(currentPlayer->jailTurns));

        if (currentPlayer->jailTurns >= 3) {
            if (currentPlayer->Balance >= 50) {
                currentPlayer->trekGeldAf(50); // 50 fine
                ui->BalanceLabel->setText(QString::number(currentPlayer->Balance));
                ui->KansAlg->setText("Paid €50 fine. You're free!");
            } else {
                ui->KansAlg->setText("Couldn't pay €50 fine! Bankrupt!");
            }

            currentPlayer->inJail = false;
            currentPlayer->jailTurns = 0;

            QTimer::singleShot(2000, this, [this, dice1, dice2]() {
                ui->KansAlg->clear();
                animatePlayerMovement(dice1 + dice2, false);
            });
        } else {
            QTimer::singleShot(3000, this, [this]() {
                ui->KansAlg->clear();
                resetTurnForRollAgain();
            });
        }
    }
}

void MainWindow::handleThreeDoubles()
{
    ui->KansAlg->setText("Three doubles in a row! Go to Jail!");
    currentPlayer->inJail = true;
    currentPlayer->turnsInJail = 0;
    consecutiveDoubles = 0;

    QTimer::singleShot(2000, this, [this]() {
        movePlayer(10, currentPlayer); // Move to jail
        ui->KansAlg->clear();
        resetTurnAfterJail();
    });
}

void MainWindow::movePlayer(int index, Player* player){
    player->setPosition(index);
    QPointF tilePos = board->calculateTilePosition(index);

    qDebug() << "Token position is being set";
    auto token = dynamic_cast<QGraphicsEllipseItem*>(player->getToken());
    token->setPos(tilePos.x() + 25, tilePos.y() + 25);
    qDebug() << player->getName() << "moved to" << index << "at" << tilePos;

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

    updatePlayerUI();

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
        ui->OwnerPropsButton->setEnabled(false);
        ui->statusLabel->setText(QString("%1's turn - Roll the dice!").arg(currentPlayer->getName()));

        updatePlayerUI();
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
        if (!currentPlayer->inJail){
            ui->rollButton->setEnabled(true);
        }
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
        animatePlayerMovement(total, true);
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
    ui->OwnerPropsButton->setEnabled(true);
    ui->FaillietButton->setEnabled(true);
    ui->tradeButton->setEnabled(true);

    // Special Tiles Logic
    ui->KansAlg->setWordWrap(true);

    if (tile->prijs == 0) {
        ui->propertyPrice->clear();
        ui->propertyPriceLabel->clear();
        ui->propertyOwner->clear();
        ui->propertyOwnerLabel->clear();
        ui->buyButton->setEnabled(false);
    }
    else if (tile->Owner == nullptr) {
        ui->propertyPrice->setText("Price: €");
        ui->propertyPriceLabel->setText(QString::number(tile->prijs));
        ui->propertyOwner->clear();
        ui->propertyOwnerLabel->clear();
    }
    else {
        ui->propertyPrice->setText("Rent: €");
        if (position == 5 || position == 15 || position == 25 || position == 35){
            ui->propertyPriceLabel->setText(QString::number(calcStationRent(position)));
        }
        else if (position == 12 || position == 28){
            ui->propertyPriceLabel->setText(QString::number(calcNutsbedrijvenRent(currentPlayer->worp,position)));
        }
        else
        {
            ui->propertyPriceLabel->setText(QString::number(calcRent(position)));}
            ui->propertyOwner->setText("Owner:");
            ui->propertyOwnerLabel->setText(tile->Owner->name);
            ui->GehypothykeerdLabel->setText("Gehypothykeerd:");
            ui->HypoJaNeeLabel->setText(tile->gehypotykeert ? "Ja" : "Nee");
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
    if(((position == 5) || (position == 15) || (position == 25) || (position == 35)) && tile->gehypotykeert == false){
        payStationRent(currentPlayer->getPosition());
    }
    else if ((position == 12 || position == 28) && tile->gehypotykeert == false){
        PayNutsBedrijvenRent(currentPlayer->worp,position);
    }

    // Kans
    else if (position == 7 || position == 22 || position == 36) {
        KansStapelp kans;
        KansKaart kaart = kans.trekKansKaart(currentPlayer, this);
        ui->KansAlg->setText(kaart.beschrijving);
        if (kaart.isJailCard) {
            currentPlayer->jailCards++;
        }
        QTimer::singleShot(5000, this, [this]() { ui->KansAlg->clear(); });
    }

    // Algemeen Fonds
    else if (position == 2 || position == 17 || position == 33) {
        AlgemeenFondsStapelp AlgFond;
        FondsKaart kaart = AlgFond.trekAlgKaart(currentPlayer, this);
        ui->KansAlg->setText(kaart.beschrijving);
        if (kaart.isJailCard) {
            currentPlayer->jailCards++;
        }
        QTimer::singleShot(5000, this, [this]() { ui->KansAlg->clear(); });
    }

    // Go to jail
    else if (position == 30) { // Go to jail
        currentPlayer->inJail = true;
        currentPlayer->turnsInJail = 0;
        ui->KansAlg->setText("Go to Jail!");
        QTimer::singleShot(500, this, [this]() {
            this->movePlayer(10, currentPlayer);
            ui->KansAlg->clear();
            handleJailTurn();
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
        if (tile->gehypotykeert == false){
            payRent(currentPlayer->getPosition());
        }
    }

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

    currentPlayer->prevPosition = currentPlayer->getPosition();
    ui->BalanceLabel->setText(QString::number(currentPlayer->Balance));
    ui->EndTurnButton->setEnabled(true);
}

void MainWindow::animatePlayerMovement(int steps, bool wasDoubles) {
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

        connect(timer, &QTimer::timeout, [this, startPos, i, steps, wasDoubles]() {
            if (!currentPlayer) return;

            int newPos = (startPos + i) % 40;
            movePlayer(newPos, currentPlayer);
            qDebug() << currentPlayer->getName() << "moved to position" << newPos;

            // Handle final position
            if (i == steps) {
                isMoving = false;
                currentPlayer->setPosition(newPos);
                qDebug() << "Movement complete at position" << newPos;
                QTimer::singleShot(100, [this, newPos, wasDoubles]() {
                    handleLanding(newPos);

                    if (!currentPlayer->inJail) {
                        if (wasDoubles && consecutiveDoubles < 3) {
                            // Allow another roll after doubles
                            ui->statusLabel->setText("Doubles! Roll again.");
                            ui->rollButton->setEnabled(true);
                        } else {
                            // Normal turn end
                            consecutiveDoubles = 0;
                            ui->EndTurnButton->setEnabled(true);
                        }
                    }
                });
            }
        });

        timer->setSingleShot(true);
        timer->start(i * 200); // 200ms inbetween steps
    }
}


void MainWindow::resetTurnForRollAgain()
{
    consecutiveDoubles = 0;
    ui->rollButton->setEnabled(true);
    ui->EndTurnButton->setEnabled(false);
}

void MainWindow::resetTurnAfterJail()
{
    consecutiveDoubles = 0;
    ui->rollButton->setEnabled(false);
    ui->EndTurnButton->setEnabled(true);
}

void MainWindow::on_FaillietButton_released() {
    if (!currentPlayer) return;
    qDebug() << "in failliet";

    // Bevestiging vragen
    auto reply = QMessageBox::question(
        this,
        "Failliet",
        QString("%1, wil je je echt failliet verklaren?").arg(currentPlayer->getName()),
        QMessageBox::Yes | QMessageBox::No
        );
    if (reply != QMessageBox::Yes) return;

    //Ontneem alle eigendommen
    for (Tile* tile : currentPlayer->ownedProperties) {
        tile->Owner = nullptr;
        updateTileOwnerLabel(tile);  // update ownerlabels
    }
    currentPlayer->ownedProperties.clear();

    //Verwijder token
    if (auto token = currentPlayer->getToken()) {
        scene->removeItem(token);
        delete token;
    }

    //Verwijder speler uit lijst
    int removedIndex = currentPlayerIndex;
    delete players[removedIndex];
    players.remove(removedIndex);

    //Check of er nog maar 1 speler over is
    if (players.size() <= 1) {
        QString winner = players.isEmpty() ? QString("Geen") : players.first()->getName();
        QMessageBox::information(this, "Spel Voorbij",
                                 QString("Het spel is voorbij! Winnaar: %1").arg(winner));
        // disable alle knoppen
        ui->rollButton->setEnabled(false);
        ui->buyButton->setEnabled(false);
        ui->OwnerPropsButton->setEnabled(false);
        ui->EndTurnButton->setEnabled(false);
        this->close();
        return;
    }

    //Stel currentPlayerIndex opnieuw in
    currentPlayerIndex = removedIndex % players.size();
    currentPlayer = players[currentPlayerIndex];

    //UI bijwerken en door naar de volgende speler
    updatePlayerUI();
    ui->BalanceLabel->setText(QString::number(currentPlayer->Balance));
    ui->statusLabel->setText(QString("%1 is failliet gegaan. Volgende: %2")
                                 .arg(currentPlayer->getName())
                                 .arg(currentPlayer->getName()));

    nextPlayerTurn();
}

void MainWindow::on_EndTurnButton_released()
{
    if (currentPlayer -> Balance < 0){
        ui->statusLabel->setText("Los eerst je schulden af!");
        return;
    }
    ui->EndTurnButton->setEnabled(false);
    ui->buyButton->setEnabled(false);
    ui->OwnerPropsButton->setEnabled(false);
    ui->tradeButton->setEnabled(false);
    ui->FaillietButton->setEnabled(false);

    ui->LoonLabel->clear();
    ui->propertyNameLabel->clear();
    ui->propertyPrice->clear();
    ui->propertyPriceLabel->clear();
    ui->propertyOwner->clear();
    ui->propertyOwnerLabel->clear();
    ui->GehypothykeerdLabel->clear();
    ui->HypoJaNeeLabel->clear();
    ui->KansAlg->clear();
    updatePlayerInfo();
    nextPlayerTurn();
    consecutiveDoubles = 0;
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

void MainWindow::on_OwnerPropsButton_released() {
    if (!currentPlayer) return;

    QDialog* propDialog = new QDialog(this);
    propDialog->setWindowTitle(currentPlayer->getName() + "'s Properties");

    QVBoxLayout* mainLayout = new QVBoxLayout(propDialog);

    // Scroll area setup
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    QWidget* scrollWidget = new QWidget();
    QVBoxLayout* scrollLayout = new QVBoxLayout(scrollWidget);

    for (Tile* tile : currentPlayer->ownedProperties) {
        QGroupBox* group = new QGroupBox(tile->name);
        QVBoxLayout* groupLayout = new QVBoxLayout(group);
        QLabel* kleurLabel = new QLabel();
        kleurLabel->setFixedSize(40, 20); // Klein gekleurd vakje zelfde kleur als property op bord
        kleurLabel->setStyleSheet(QString("background-color: rgb(%1, %2, %3); border: 1px solid black;")
                                      .arg(tile->color.red())
                                      .arg(tile->color.green())
                                      .arg(tile->color.blue()));
        groupLayout->addWidget(kleurLabel);

        QLabel* label = nullptr;
        bool isStation = tile->name.contains("Station", Qt::CaseInsensitive);
        bool isUtility = tile->name.contains("Water", Qt::CaseInsensitive) || tile->name.contains("Elektriciteit", Qt::CaseInsensitive);

        QString info;
        if (isStation) {
            info = QString(
                       "Hypotheek: €%1\n"
                       "Huur:\n"
                       " - 1 station: €25\n"
                       " - 2 stations: €50\n"
                       " - 3 stations: €100\n"
                       " - 4 stations: €200\n"
                       "Gehypothekeerd: %2")
                       .arg(tile->hypo)
                       .arg(tile->gehypotykeert ? "Ja" : "Nee");
        } else if (isUtility) {
            info = QString(
                       "Hypotheek: €%1\n"
                       "Huur:\n"
                       " - 1 nutsbedrijf: 4× dobbelstenen\n"
                       " - 2 nutsbedrijven: 10× dobbelstenen\n"
                       "Gehypothekeerd: %2")
                       .arg(tile->hypo)
                       .arg(tile->gehypotykeert ? "Ja" : "Nee");
        } else {
            info = QString(
                       "Hypotheek: €%1\n"
                       "Prijs per huis: €%2\n"
                       "Huur:\n"
                       " - Onbebouwd: €%3\n"
                       " - 1 huis: €%4\n"
                       " - 2 huizen: €%5\n"
                       " - 3 huizen: €%6\n"
                       " - 4 huizen: €%7\n"
                       " - Hotel: €%8\n"
                       "Gehypothekeerd: %9")
                       .arg(tile->hypo)
                       .arg(tile->prijsPerHuis)
                       .arg(tile->r1)
                       .arg(tile->r2)
                       .arg(tile->r3)
                       .arg(tile->r4)
                       .arg(tile->r5)
                       .arg(tile->r6)
                       .arg(tile->gehypotykeert ? "Ja" : "Nee");
        }

        label = new QLabel(info);
        groupLayout->addWidget(label);

        // For street properties, add buy/sell house buttons
        if (!isStation && !isUtility) {
            QPushButton* buyHouseButton = new QPushButton("Koop Huis");
            QPushButton* sellHouseButton = new QPushButton("Verkoop Huis");
            QPushButton* actionButton = new QPushButton(tile->gehypotykeert ? "Terugkopen" : "Hypotheek");

            buyHouseButton->setEnabled(false);
            sellHouseButton->setEnabled(tile->huizencount > 0);

            groupLayout->addWidget(buyHouseButton);
            groupLayout->addWidget(sellHouseButton);
            groupLayout->addWidget(actionButton);

            // Enable buy button if monopoly is owned
            if (checkMonopoly(tile->colorgroup) && checkHouseBuy(tile)) {
                buyHouseButton->setEnabled(true);
            } else {
                buyHouseButton->setEnabled(false);
            }

            if (tile->huizencount > 0 && checkHouseSell(tile)) {
                sellHouseButton->setEnabled(true);
            } else {
                sellHouseButton->setEnabled(false);
            }

            bool colorHasHouses = false;
            QString group = tile->colorgroup;
            for (Tile* t : board->tiles) {
                if (t->colorgroup == group && t->huizencount > 0) {
                    colorHasHouses = true;
                    break;
                }
            }
            actionButton->setEnabled(!colorHasHouses);


            connect(buyHouseButton, &QPushButton::clicked, [=]() {
                int housePrice = tile->prijsPerHuis;

                if (tile->huizencount == 5) {
                    ui->statusLabel->setText("Maximaal aantal huizen bereikt op dit eigendom.");
                    return;
                }
                if (currentPlayer->Balance < housePrice) {
                    ui->statusLabel->setText("Niet genoeg geld om een huis te kopen.");
                    return;
                }

                tile->huizencount++;
                if (tile->huizencount == 5) {
                    currentPlayer->aantalHuizen -= 4;  // Remove 4 houses
                    currentPlayer->aantalHotels += 1;  // Add 1 hotel
                } else {
                    currentPlayer->aantalHuizen++;
                }
                currentPlayer->trekGeldAf(housePrice);
                ui->BalanceLabel->setText(QString::number(currentPlayer->Balance));
                ui->statusLabel->setText(QString("Huis gekocht op %1.").arg(tile->name));

                if (tile->huizencount == 5) {
                    buyHouseButton->setEnabled(false);
                }

                // Update label text
                QString updatedInfo = QString(
                                          "Hypotheek: €%1\n"
                                          "Prijs per huis: €%2\n"
                                          "Huur:\n"
                                          " - Onbebouwd: €%3\n"
                                          " - 1 huis: €%4\n"
                                          " - 2 huizen: €%5\n"
                                          " - 3 huizen: €%6\n"
                                          " - 4 huizen: €%7\n"
                                          " - Hotel: €%8\n"
                                          "Gehypothekeerd: %9\n"
                                          "Aantal huizen: %10")
                                          .arg(tile->hypo)
                                          .arg(tile->prijsPerHuis)
                                          .arg(tile->r1)
                                          .arg(tile->r2)
                                          .arg(tile->r3)
                                          .arg(tile->r4)
                                          .arg(tile->r5)
                                          .arg(tile->r6)
                                          .arg(tile->gehypotykeert ? "Ja" : "Nee")
                                          .arg(tile->huizencount);

                label->setText(updatedInfo);
                updateTileHouses(tile->index);
                propDialog->accept();
                on_OwnerPropsButton_released();
            });

            connect(sellHouseButton, &QPushButton::clicked, [=]() {
                if (tile->huizencount == 0) {
                    ui->statusLabel->setText("Geen huizen om te verkopen op dit eigendom.");
                    return;
                }

                int refund = tile->prijsPerHuis / 2;
                tile->huizencount--;
                if (tile->huizencount == 4) {
                    currentPlayer->aantalHotels -= 1;  // Remove hotel
                    currentPlayer->aantalHuizen += 4;  // Add 4 houses back
                } else {
                    currentPlayer->aantalHuizen--;
                }
                currentPlayer->voegGeldToe(refund);
                ui->BalanceLabel->setText(QString::number(currentPlayer->Balance));
                ui->statusLabel->setText(QString("Huis verkocht op %1.").arg(tile->name));

                if (tile->huizencount < 5) {
                    buyHouseButton->setEnabled(true);
                }

                if (tile->huizencount == 0) {
                    actionButton->setEnabled(true);
                    sellHouseButton->setEnabled(false);
                }

                // Update label text
                QString updatedInfo = QString(
                                          "Hypotheek: €%1\n"
                                          "Prijs per huis: €%2\n"
                                          "Huur:\n"
                                          " - Onbebouwd: €%3\n"
                                          " - 1 huis: €%4\n"
                                          " - 2 huizen: €%5\n"
                                          " - 3 huizen: €%6\n"
                                          " - 4 huizen: €%7\n"
                                          " - Hotel: €%8\n"
                                          "Gehypothekeerd: %9\n"
                                          "Aantal huizen: %10")
                                          .arg(tile->hypo)
                                          .arg(tile->prijsPerHuis)
                                          .arg(tile->r1)
                                          .arg(tile->r2)
                                          .arg(tile->r3)
                                          .arg(tile->r4)
                                          .arg(tile->r5)
                                          .arg(tile->r6)
                                          .arg(tile->gehypotykeert ? "Ja" : "Nee")
                                          .arg(tile->huizencount);

                label->setText(updatedInfo);
                updateTileHouses(tile->index);
                propDialog->accept();
                on_OwnerPropsButton_released();

            });

            connect(actionButton, &QPushButton::clicked, [=]() {
                actionButton->setText(tile->gehypotykeert ? "Hypotheek" : "Terugkopen");

                if (!tile->gehypotykeert) {
                    tile->gehypotykeert = true;
                    currentPlayer->voegGeldToe(tile->hypo);
                    ui->BalanceLabel->setText(QString::number(currentPlayer->Balance));
                    ui->statusLabel->setText(QString("%1 is gehypothekeerd voor €%2.")
                                                 .arg(tile->name)
                                                 .arg(tile->hypo));
                } else {
                    int terugKoopPrijs = tile->hypo * 1.1;
                    if (currentPlayer->Balance < terugKoopPrijs) {
                        ui->statusLabel->setText("Niet genoeg geld om terug te kopen!");
                        return;
                    }

                    tile->gehypotykeert = false;
                    currentPlayer->trekGeldAf(terugKoopPrijs);
                    ui->BalanceLabel->setText(QString::number(currentPlayer->Balance));
                    ui->statusLabel->setText(QString("%1 is teruggekocht voor €%2.")
                                                 .arg(tile->name)
                                                 .arg(terugKoopPrijs));
                }

                if (checkMonopoly(tile->colorgroup)) {
                    buyHouseButton->setEnabled(true);
                }
                else{
                    buyHouseButton->setEnabled(false);
                }
                // Update label
                QString updatedInfo;
                if (isStation) {
                    updatedInfo = QString(
                                      "Hypotheek: €%1\n"
                                      "Huur:\n"
                                      " - 1 station: €25\n"
                                      " - 2 stations: €50\n"
                                      " - 3 stations: €100\n"
                                      " - 4 stations: €200\n"
                                      "Gehypothekeerd: %2")
                                      .arg(tile->hypo)
                                      .arg(tile->gehypotykeert ? "Ja" : "Nee");
                } else if (isUtility) {
                    updatedInfo = QString(
                                      "Hypotheek: €%1\n"
                                      "Huur:\n"
                                      " - 1 nutsbedrijf: 4× dobbelstenen\n"
                                      " - 2 nutsbedrijven: 10× dobbelstenen\n"
                                      "Gehypothekeerd: %2")
                                      .arg(tile->hypo)
                                      .arg(tile->gehypotykeert ? "Ja" : "Nee");
                } else {
                    updatedInfo = QString(
                                      "Hypotheek: €%1\n"
                                      "Prijs per huis: €%2\n"
                                      "Huur:\n"
                                      " - Onbebouwd: €%3\n"
                                      " - 1 huis: €%4\n"
                                      " - 2 huizen: €%5\n"
                                      " - 3 huizen: €%6\n"
                                      " - 4 huizen: €%7\n"
                                      " - Hotel: €%8\n"
                                      "Gehypothekeerd: %9")
                                      .arg(tile->hypo)
                                      .arg(tile->prijsPerHuis)
                                      .arg(tile->r1)
                                      .arg(tile->r2)
                                      .arg(tile->r3)
                                      .arg(tile->r4)
                                      .arg(tile->r5)
                                      .arg(tile->r6)
                                      .arg(tile->gehypotykeert ? "Ja" : "Nee");
                }

                label->setText(updatedInfo);
                propDialog->accept();
                on_OwnerPropsButton_released();
            });
        }
        scrollLayout->addWidget(group);
    }

    scrollWidget->setLayout(scrollLayout);
    scrollArea->setWidget(scrollWidget);
    mainLayout->addWidget(scrollArea);

    QPushButton* closeButton = new QPushButton("Sluiten");
    mainLayout->addWidget(closeButton);
    connect(closeButton, &QPushButton::clicked, propDialog, &QDialog::accept);

    propDialog->setLayout(mainLayout);
    propDialog->exec();
}
bool MainWindow::checkHouseBuy(Tile* tile){
    QString color = tile->colorgroup;
    int minHouses = INT_MAX;

    for (Tile* looptile : board->tiles) {
        if (looptile->colorgroup == color) {
            minHouses = std::min(minHouses, looptile->huizencount);
        }
    }

    return tile->huizencount == minHouses;
}

bool MainWindow::checkHouseSell(Tile* tile) {
    QString color = tile->colorgroup;
    for (Tile* loopTile : board->tiles) {
        if (loopTile->colorgroup == color) {
            if (tile->huizencount < loopTile->huizencount)
                return false;
        }
    }
    return true;
}

void MainWindow::on_tradeButton_released() {
    Player* currentPlayer = players[currentPlayerIndex];

    // Trade Dialog
    QDialog dialog(this);
    dialog.setWindowTitle("Trade");

    QVBoxLayout* layout = new QVBoxLayout(&dialog);

    // Player Selection
    QLabel* playerLabel = new QLabel("Select a player to trade with:");
    QComboBox* playerCombo = new QComboBox();
    QVector<Player*> tradeablePlayers;

    for (int i = 0; i < players.size(); ++i) {
        if (i != currentPlayerIndex) {
            playerCombo->addItem(players[i]->name);
            tradeablePlayers.append(players[i]);
        }
    }

    // Money Offer Inputs
    QSpinBox* offerMoneySpin = new QSpinBox();
    offerMoneySpin->setMaximum(currentPlayer->Balance);
    offerMoneySpin->setPrefix("You give: €");

    QSpinBox* requestMoneySpin = new QSpinBox();
    requestMoneySpin->setMaximum(10000); // placeholder
    requestMoneySpin->setPrefix("You get: €");

    // Property Selection
    QLabel* yourPropsLabel = new QLabel("Your properties:");
    QListWidget* yourPropsList = new QListWidget();
    for (Tile* tile : currentPlayer->ownedProperties) {
        yourPropsList->addItem(tile->name);
    }
    yourPropsList->setSelectionMode(QAbstractItemView::MultiSelection);

    QLabel* theirPropsLabel = new QLabel("Their properties:");
    QListWidget* theirPropsList = new QListWidget();
    theirPropsList->setSelectionMode(QAbstractItemView::MultiSelection);

    // When other player is selected, populate their properties
    QObject::connect(playerCombo, &QComboBox::currentTextChanged, [&]() {
        theirPropsList->clear();
        Player* selectedPlayer = tradeablePlayers[playerCombo->currentIndex()];
        requestMoneySpin->setMaximum(selectedPlayer->Balance);
        for (Tile* t : selectedPlayer->ownedProperties) {
            theirPropsList->addItem(t->name);
        }
    });

    // Confirm Button
    QPushButton* confirmButton = new QPushButton("Confirm Trade");
    QObject::connect(confirmButton, &QPushButton::clicked, [&]() {
        Player* otherPlayer = tradeablePlayers[playerCombo->currentIndex()];
        int offerMoney = offerMoneySpin->value();
        int requestMoney = requestMoneySpin->value();

        // Validate
        if (currentPlayer->Balance < offerMoney || otherPlayer->Balance < requestMoney) {
            QMessageBox::warning(&dialog, "Invalid Trade", "One of the players doesn't have enough money.");
            return;
        }

        // Perform trade
        currentPlayer->Balance -= offerMoney;
        currentPlayer->Balance += requestMoney;
        otherPlayer->Balance -= requestMoney;
        otherPlayer->Balance += offerMoney;

        // Property transfers
        for (QListWidgetItem* item : yourPropsList->selectedItems()) {
            QString propName = item->text();
            for (Tile* tile : currentPlayer->ownedProperties) {
                if (tile->name == propName) {
                    tile->Owner = otherPlayer;
                    updateTileOwnerLabel(tile);
                    currentPlayer->ownedProperties.removeOne(tile);
                    otherPlayer->ownedProperties.append(tile);
                    break;
                }
            }
        }

        for (QListWidgetItem* item : theirPropsList->selectedItems()) {
            QString propName = item->text();
            for (Tile* tile : otherPlayer->ownedProperties) {
                if (tile->name == propName) {
                    tile->Owner = currentPlayer;
                    updateTileOwnerLabel(tile);
                    otherPlayer->ownedProperties.removeOne(tile);
                    currentPlayer->ownedProperties.append(tile);
                    break;
                }
            }
        }
        ui->BalanceLabel->setText(QString::number(currentPlayer->Balance));
        dialog.accept();
    });

    // Add widgets to layout
    layout->addWidget(playerLabel);
    layout->addWidget(playerCombo);
    layout->addWidget(offerMoneySpin);
    layout->addWidget(requestMoneySpin);
    layout->addWidget(yourPropsLabel);
    layout->addWidget(yourPropsList);
    layout->addWidget(theirPropsLabel);
    layout->addWidget(theirPropsList);
    layout->addWidget(confirmButton);

    // Initialize first selection
    if (playerCombo->count() > 0) {
        emit playerCombo->currentTextChanged(playerCombo->currentText());
        dialog.exec();
    } else {
        QMessageBox::information(this, "No Trade", "No available players to trade with.");
    }
}

bool MainWindow::checkMonopoly(const QString colorGroup) {
    for (Tile* tile : board->tiles) {
        if (tile->colorgroup == colorGroup) {
            if (tile->Owner != currentPlayer) {
                return false;
            }
            if (tile->gehypotykeert == true){
                return false;
            }

        }
    }
    return true;
}

void MainWindow::updateTileHouses(int position) {
    Tile* tile = board->tiles[position];
    if (!tile || (tile->huizencount < 0) || (tile->huizencount) > 5) return;

    // Remove previous graphics
    for (QGraphicsItem* item : tile->houseGraphics) {
        scene->removeItem(item);
        delete item;
    }
    tile->houseGraphics.clear();

    // Base tile position
    QPointF basePos = board->calculateTilePosition(position);

    const int houseWidth = 10;
    const int houseHeight = 10;

    if (tile->huizencount == 5) {
        // Draw 1 red hotel
        QGraphicsRectItem* hotel = new QGraphicsRectItem(0, 0, houseWidth * 2, houseHeight);
        hotel->setBrush(Qt::red);
        hotel->setPen(QPen(Qt::black));
        hotel->setZValue(3);
        hotel->setPos(basePos.x() + 15, basePos.y() + 2); // Centered
        tile->houseGraphics.append(hotel);
        scene->addItem(hotel);
    } else {
        // Draw 1–4 green houses
        for (int i = 0; i < tile->huizencount; ++i) {
            QGraphicsRectItem* house = new QGraphicsRectItem(0, 0, houseWidth, houseHeight);
            house->setBrush(Qt::green);
            house->setPen(QPen(Qt::black));
            house->setZValue(3);
            house->setPos(basePos.x() + 5 + (i * (houseWidth + 2)), basePos.y() + 2);
            tile->houseGraphics.append(house);
            scene->addItem(house);
        }
    }
}

void MainWindow::updateTileOwnerLabel(Tile* tile) {
    // Remove old label if it exists
    if (tile->ownerLabel != nullptr) {
        scene->removeItem(tile->ownerLabel);
        delete tile->ownerLabel;
        tile->ownerLabel = nullptr;
    }

    // If no owner, don't draw label
    if (tile->Owner == nullptr) {
        return;
    }

    QGraphicsRectItem* tileGraphics = tile->getGraphics();
    if (!tileGraphics) return;

    QPointF pos = tileGraphics->pos();
    QRectF rect = tileGraphics->rect();

    qreal labelWidth = 15;
    qreal labelHeight = 15;
    QPointF labelPos(
        pos.x() + 2,
        pos.y() + rect.height() - labelHeight - 2
        );

    tile->ownerLabel = new QGraphicsRectItem(0, 0, labelWidth, labelHeight);
    tile->ownerLabel->setBrush(tile->Owner->playerColor);
    tile->ownerLabel->setPen(QPen(Qt::black)); // Black border
    tile->ownerLabel->setPos(labelPos);

    scene->addItem(tile->ownerLabel);
}
