#include "board.h"
#include "Tile.h"
#include "mainwindow.h"

Board::Board(QGraphicsScene* scene) : scene(scene) {}

void Board::create() {
    tiles.resize(40);

    // Create all tiles with their properties
    tiles[0]  = new Tile(0, nullptr, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Start", Qt::red, calculateTilePosition(0));
    tiles[20] = new Tile(20,nullptr, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Vrije Parking", Qt::blue, calculateTilePosition(20));

    //Algemeen fonds
    tiles[2]  = new Tile(2,nullptr, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Algemeen Fonds", Qt::lightGray, calculateTilePosition(2));
    tiles[33] = new Tile(33,nullptr, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Algemeen Fonds", Qt::lightGray, calculateTilePosition(33));
    tiles[17] = new Tile(17,nullptr, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Algemeen Fonds", Qt::lightGray, calculateTilePosition(17));

    //Kans
    tiles[7]  = new Tile(7,nullptr, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Kans", Qt::lightGray, calculateTilePosition(7));
    tiles[22] = new Tile(22,nullptr, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Kans", Qt::lightGray, calculateTilePosition(22));
    tiles[36] = new Tile(36,nullptr, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Kans", Qt::lightGray, calculateTilePosition(36));

    //Belastingen
    tiles[4]  = new Tile(4,nullptr, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Inkomsten Belasting", Qt::gray, calculateTilePosition(4));
    tiles[38] = new Tile(38,nullptr, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Extra Belasting", Qt::gray, calculateTilePosition(38));

    //Jail
    tiles[10] = new Tile(10,nullptr, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Jail/Just Visiting", Qt::blue, calculateTilePosition(10));
    tiles[30] = new Tile(30,nullptr, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Go To Jail", Qt::blue, calculateTilePosition(30));

    //Special Propertys
    tiles[12] = new Tile(12,nullptr, 150, 75, 0, 0, 0, 0, 0, 0, 0, "Electriciteit Centraal", Qt::lightGray, calculateTilePosition(12));
    tiles[28] = new Tile(28,nullptr, 150, 75, 0, 0, 0, 0, 0, 0, 0, "Waterdienst", Qt::cyan, calculateTilePosition(28));

    //Stations
    tiles[5]  = new Tile(5,nullptr, 200, 100, 0, 25, 50, 100, 200, 0, 0, "Station Noord", Qt::black, calculateTilePosition(5));
    tiles[15] = new Tile(15,nullptr, 200, 100, 0, 25, 50, 100, 200, 0, 0, "Station Centraal", Qt::black, calculateTilePosition(15));
    tiles[25] = new Tile(25,nullptr, 200, 100, 0, 25, 50, 100, 200, 0, 0, "Buurt-Spoorwegen", Qt::black, calculateTilePosition(25));
    tiles[35] = new Tile(35, nullptr,200, 100, 0, 25, 50, 100, 200, 0, 0, "Station Zuid", Qt::black, calculateTilePosition(35));



    // ALLE STRATEN (22 straten)
    tiles[1]  = new Tile(1,nullptr, 60, 30, 50, 2, 10, 30, 90, 180, 250, "Leuven Dieststraat", QColor(165,93,68), calculateTilePosition(1));
    tiles[3]  = new Tile(3, nullptr,60, 30, 50, 4, 20, 60, 180, 360, 450, "Mechelen Bruul", QColor(165,93,68), calculateTilePosition(3));
    tiles[6]  = new Tile(6,nullptr, 100, 50, 50, 6, 30, 90, 270, 400,550, "Leuven Bondgenotenlaan", QColor(23, 173, 207), calculateTilePosition(6));
    tiles[8]  = new Tile(8, nullptr,100, 50, 50, 6, 30, 90, 270, 400, 550,"Turnhout Gasthuisstraat", QColor(23, 173, 207), calculateTilePosition(8));
    tiles[9]  = new Tile(9, nullptr,120, 60, 50, 8, 40, 100, 300, 450, 600, "Turnhout Leopoldstraat", QColor(23, 173, 207), calculateTilePosition(9));
    tiles[11] = new Tile(11, nullptr,140, 70, 100, 10, 50, 150, 450, 625, 750, "Aalst Lange Zoutstraat", QColor(216, 27, 171), calculateTilePosition(11));
    tiles[13] = new Tile(13, nullptr,140, 70, 100, 10, 50, 150, 450, 625, 750, "Brugge Vlamingenstraat", QColor(216, 27, 171), calculateTilePosition(13));
    tiles[14] = new Tile(14, nullptr,160, 80, 100, 12, 60, 180, 500, 700, 900, "Knokke Kustlaan", QColor(216, 27, 171), calculateTilePosition(14));
    tiles[16] = new Tile(16, nullptr,180, 90, 100, 14, 70, 200, 550, 750, 950, "Brugge Steenstraat", QColor(237, 124, 45), calculateTilePosition(16));
    tiles[18] = new Tile(18, nullptr,180, 90, 100, 14, 70, 200, 550, 750, 950, "Oostende Kapelstraat", QColor(237, 124, 45), calculateTilePosition(18));
    tiles[19] = new Tile(19, nullptr,200, 100, 100, 16, 80, 220, 600, 800, 1000, "Kortrijk Rijselstraat", QColor(237, 124, 45), calculateTilePosition(19));
    tiles[21] = new Tile(21, nullptr,220, 110, 150, 18, 90, 250, 700, 875, 1050,"Tienen Leuvensestraat", QColor(237, 28, 36), calculateTilePosition(21));
    tiles[23] = new Tile(23, nullptr,220, 110, 150, 18, 90, 250, 700, 875, 1050, "Gent Kortrijksestraat", QColor(237, 28, 36), calculateTilePosition(23));
    tiles[24] = new Tile(24, nullptr,240, 120, 150, 20, 100, 300, 750, 925, 1100,"Hasselt Grote Markt", QColor(237, 28, 36), calculateTilePosition(24));
    tiles[26] = new Tile(26, nullptr,260, 130, 150, 22, 110, 330, 800, 975, 1150,"Antwerpen Groenplaats", QColor(251, 242, 54), calculateTilePosition(26));
    tiles[27] = new Tile(27, nullptr,260, 130, 150, 22, 110, 330, 800, 975, 1150,"Brussel Lakenstraat", QColor(251, 242, 54), calculateTilePosition(27));
    tiles[29] = new Tile(29, nullptr,280, 140, 150, 24, 120, 360, 850, 1025, 1200,"Gent Veldstraat", QColor(251, 242, 54), calculateTilePosition(29));
    tiles[31] = new Tile(31, nullptr,300, 150, 200, 26, 130, 390, 900, 1100, 1250, "Antwerpen Huidevettersstraat", QColor(29, 178, 74), calculateTilePosition(31));
    tiles[32] = new Tile(32, nullptr,300, 150, 200, 26, 130, 390, 900, 1100, 1250,"Gent Vlaanderenstraat", QColor(29, 178, 74), calculateTilePosition(32));
    tiles[34] = new Tile(34, nullptr,320, 160, 200, 28, 150, 450, 1000, 1200, 1400,"Hasselt Demerstraat", QColor(29, 178, 74), calculateTilePosition(34));
    tiles[37] = new Tile(37, nullptr,350, 175, 200, 35, 175, 500, 1100, 1300, 1500, "Antwerpen Meir", QColor(13, 46, 146), calculateTilePosition(37));
    tiles[39] = new Tile(39, nullptr,400, 200, 200, 50, 200, 600, 1400, 1700, 2000,"Brussel Nieuwstraat", QColor(13, 46, 146), calculateTilePosition(39));

    // Add all tiles to the scene
    for (Tile* tile : tiles) {
        if (tile) {
            scene->addItem(tile->getGraphics());
            scene->addItem(tile->getLabel());
        }
    }
}


QPointF Board::calculateTilePosition(int index) const {

    const int TILE_SIZE = 80;
    const int TILES_PER_SIDE = 11;
    const int BOARD_SIZE = TILE_SIZE * TILES_PER_SIDE;

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

Tile* Board::getTile(int index) {
    if (index >= 0 && index < tiles.size()) {
        return tiles[index];
    }
    return nullptr;
}
