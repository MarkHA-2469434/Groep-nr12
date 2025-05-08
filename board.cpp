#include "board.h"
#include "Tile.h"
#include "mainwindow.h"

Board::Board(QGraphicsScene* scene) : scene(scene) {}

void Board::create() {
    tiles.resize(40);

    // Create all tiles with their properties
    tiles[0] = new Tile(0, "Start", Qt::red, calculateTilePosition(0));
    tiles[1] = new Tile(1, "Leuven Dieststraat", QColor(165,93,68), calculateTilePosition(1));
    tiles[2] = new Tile(2, "Algemeen Fonds", Qt::lightGray, calculateTilePosition(2));
    tiles[3] = new Tile(3, "Mechelen Bruul", QColor(165,93,68), calculateTilePosition(3));
    tiles[4] = new Tile(4, "Inkomsten Belasting", Qt::gray, calculateTilePosition(4));
    tiles[5] = new Tile(5, "Station Noord", Qt::black, calculateTilePosition(5));
    tiles[6] = new Tile(6, "Leuven Bondgenotenlaan", QColor(23, 173, 207), calculateTilePosition(6));
    tiles[7] = new Tile(7, "Kans", Qt::lightGray, calculateTilePosition(7));
    tiles[8] = new Tile(8, "Turnhout Gasthuisstr.", QColor(23, 173, 207), calculateTilePosition(8));
    tiles[9] = new Tile(9, "Turnhout Leopoldstr.", QColor(23, 173, 207), calculateTilePosition(9));
    tiles[10] = new Tile(10, "Jail/Just Visiting", Qt::blue, calculateTilePosition(10));
    tiles[11] = new Tile(11, "Aalst lange Zoutstraat", QColor(216, 27, 171), calculateTilePosition(11));
    tiles[12] = new Tile(12, "Electriciteit Centraal", Qt::lightGray, calculateTilePosition(12));
    tiles[13] = new Tile(13, "Brugge Vlammingenstr.", QColor(216, 27, 171), calculateTilePosition(13));
    tiles[14] = new Tile(14, "Knokke Kustlaan", QColor(216, 27, 171), calculateTilePosition(14));
    tiles[15] = new Tile(15, "Station Centraal", Qt::black, calculateTilePosition(15));
    tiles[16] = new Tile(16, "Brugge Steenstraat", QColor(237, 124, 45), calculateTilePosition(16));
    tiles[17] = new Tile(17, "Algemeen Fonds", Qt::lightGray, calculateTilePosition(17));
    tiles[18] = new Tile(18, "Oostende Kapelstraat", QColor(237, 124, 45), calculateTilePosition(18));
    tiles[19] = new Tile(19, "Kortrijk Rijselstraat", QColor(237, 124, 45), calculateTilePosition(19));
    tiles[20] = new Tile(20, "Vrije Parking", Qt::blue, calculateTilePosition(20));
    tiles[21] = new Tile(21, "Tienen Leuvensestr.", QColor(237, 28, 36), calculateTilePosition(21));
    tiles[22] = new Tile(22, "Kans", Qt::lightGray, calculateTilePosition(22));
    tiles[23] = new Tile(23, "Gent Kortrijksestr.", QColor(237, 28, 36), calculateTilePosition(23));
    tiles[24] = new Tile(24, "Hasselt Grote Markt", QColor(237, 28, 36), calculateTilePosition(24));
    tiles[25] = new Tile(25, "Buurt-Spoorwegen", Qt::black, calculateTilePosition(25));
    tiles[26] = new Tile(26, "Antwerpen Groenplaats", QColor(251, 242, 54), calculateTilePosition(26));
    tiles[27] = new Tile(27, "Brussel Lakenstraat", QColor(251, 242, 54), calculateTilePosition(27));
    tiles[28] = new Tile(28, "Waterdienst", Qt::cyan, calculateTilePosition(28));
    tiles[29] = new Tile(29, "Gent Veldstraat", QColor(251, 242, 54), calculateTilePosition(29));
    tiles[30] = new Tile(30, "Go To Jail", Qt::blue, calculateTilePosition(30));
    tiles[31] = new Tile(31, "Antwerpen Huidevettersstraat", QColor(29, 178, 74), calculateTilePosition(31));
    tiles[32] = new Tile(32, "Gent Vlaanderenstraat", QColor(29, 178, 74), calculateTilePosition(32));
    tiles[33] = new Tile(33, "Algemeen Fonds", Qt::lightGray, calculateTilePosition(33));
    tiles[34] = new Tile(34, "Hasselt Demerstraat", QColor(29, 178, 74), calculateTilePosition(34));
    tiles[35] = new Tile(35, "Station Zuid", Qt::black, calculateTilePosition(35));
    tiles[36] = new Tile(36, "Kans", Qt::lightGray, calculateTilePosition(36));
    tiles[37] = new Tile(37, "Antwerpen Meir", QColor(13, 46, 146), calculateTilePosition(37));
    tiles[38] = new Tile(38, "Extra Belasting", Qt::gray, calculateTilePosition(38));
    tiles[39] = new Tile(39, "Brussel Nieuwstraat", QColor(13, 46, 146), calculateTilePosition(39));

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

