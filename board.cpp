#include "board.h"
#include "Tile.h"
#include <QTextDocument>
#include <QTextOption>

Board::Board(QGraphicsScene* scene) : m_scene(scene) {}

void Board::create() {
    tiles.resize(40);

    // Create all tiles with their properties
    tiles[0] = new Tile(0, "Start", Qt::red,0,0, calculateTilePosition(0), m_scene);
    tiles[1] = new Tile(1, "Leuven\n Dieststraat", QColor(165,93,68),60, 4, calculateTilePosition(1), m_scene);
    tiles[2] = new Tile(2, "Algemeen\n Fonds", Qt::lightGray, 30, 2, calculateTilePosition(2), m_scene);
    tiles[3] = new Tile(3, "Mechelen\n Bruul", QColor(165,93,68), 25, 9, calculateTilePosition(3), m_scene);
    tiles[4] = new Tile(4, "Inkomsten\n Belasting", Qt::gray, 23, 5, calculateTilePosition(4), m_scene);
    tiles[5] = new Tile(5, "Station\n Noord", Qt::black, 87, 10, calculateTilePosition(5), m_scene);
    tiles[6] = new Tile(6, "Leuven\n Bondgenoten\nlaan", QColor(23, 173, 207), 10, 10, calculateTilePosition(6), m_scene);
    tiles[7] = new Tile(7, "Kans", Qt::lightGray, 8, 4, calculateTilePosition(7), m_scene);
    tiles[8] = new Tile(8, "Turnhout\n Gasthuisstr.", QColor(23, 173, 207), 1,1, calculateTilePosition(8), m_scene);
    tiles[9] = new Tile(9, "Turnhout\n Leopoldstr.", QColor(23, 173, 207),100, 80, calculateTilePosition(9), m_scene);
    tiles[10] = new Tile(10, "Jail/\nJust Visiting", Qt::blue,0, 0, calculateTilePosition(10), m_scene);
    tiles[11] = new Tile(11, "Aalst lange\n Zoutstraat", QColor(216, 27, 171), 200,20, calculateTilePosition(11), m_scene);
    tiles[12] = new Tile(12, "Electriciteit\n Centraal", Qt::lightGray,80, 100,  calculateTilePosition(12), m_scene);
    tiles[13] = new Tile(13, "Brugge\n Vlammingenstr.", QColor(216, 27, 171),90, 20, calculateTilePosition(13), m_scene);
    tiles[14] = new Tile(14, "Knokke\n Kustlaan", QColor(216, 27, 171),10, 10, calculateTilePosition(14),m_scene);
    tiles[15] = new Tile(15, "Station\n Centraal", Qt::black,60, 15, calculateTilePosition(15), m_scene);
    tiles[16] = new Tile(16, "Brugge\n Steenstraat", QColor(237, 124, 45), 30, 15, calculateTilePosition(16), m_scene);
    tiles[17] = new Tile(17, "Algemeen\n Fonds", Qt::lightGray, 10, 15, calculateTilePosition(17), m_scene);
    tiles[18] = new Tile(18, "Oostende\n Kapelstraat", QColor(237, 124, 45),9, 4, calculateTilePosition(18), m_scene);
    tiles[19] = new Tile(19, "Kortrijk\n Rijselstraat", QColor(237, 124, 45),10, 5, calculateTilePosition(19), m_scene);
    tiles[20] = new Tile(20, "Vrije\n Parking", Qt::blue, 0,0, calculateTilePosition(20), m_scene);
    tiles[21] = new Tile(21, "Tienen\n Leuvensestr.", QColor(237, 28, 36), 20, 10, calculateTilePosition(21), m_scene);
    tiles[22] = new Tile(22, "Kans", Qt::lightGray, 10, 5, calculateTilePosition(22),m_scene);
    tiles[23] = new Tile(23, "Gent\n Kortrijksestr.", QColor(237, 28, 36), 5, 10, calculateTilePosition(23), m_scene);
    tiles[24] = new Tile(24, "Hasselt\n Grote Markt", QColor(237, 28, 36), 10, 8, calculateTilePosition(24), m_scene);
    tiles[25] = new Tile(25, "Buurt-\nSpoorwegen", Qt::black, 10, 10, calculateTilePosition(25),m_scene);
    tiles[26] = new Tile(26, "Antwerpen\n Groenplaats", QColor(251, 242, 54), 8, 4, calculateTilePosition(26), m_scene);
    tiles[27] = new Tile(27, "Brussel\n Lakenstraat", QColor(251, 242, 54), 9, 3, calculateTilePosition(27), m_scene);
    tiles[28] = new Tile(28, "Waterdienst", Qt::cyan, 6, 3, calculateTilePosition(28), m_scene);
    tiles[29] = new Tile(29, "Gent\n Veldstraat", QColor(251, 242, 54), 4, 2, calculateTilePosition(29), m_scene);
    tiles[30] = new Tile(30, "Go To Jail", Qt::blue,0, 0, calculateTilePosition(30), m_scene);
    tiles[31] = new Tile(31, "Antwerpen\n Huidevettersstraat", QColor(29, 178, 74),40, 30, calculateTilePosition(31), m_scene);
    tiles[32] = new Tile(32, "Gent\n Vlaanderenstraat", QColor(29, 178, 74),20,30, calculateTilePosition(32), m_scene);
    tiles[33] = new Tile(33, "Algemeen\n Fonds", Qt::lightGray,50,20, calculateTilePosition(33),m_scene);
    tiles[34] = new Tile(34, "Hasselt\n Demerstraat", QColor(29, 178, 74), 80, 20, calculateTilePosition(34), m_scene);
    tiles[35] = new Tile(35, "Station Zuid", Qt::black, 140, 60, calculateTilePosition(35),m_scene);
    tiles[36] = new Tile(36, "Kans", Qt::lightGray,60, 120, calculateTilePosition(36), m_scene);
    tiles[37] = new Tile(37, "Antwerpen\n Meir", QColor(13, 46, 146),200, 80, calculateTilePosition(37), m_scene);
    tiles[38] = new Tile(38, "Extra\n Belasting", Qt::gray, 300, 200, calculateTilePosition(38),m_scene);
    tiles[39] = new Tile(39, "Brussel\n Nieuwstraat", QColor(13, 46, 146), 100, 90, calculateTilePosition(39),m_scene);

    // Add all tiles to the scene
    for (Tile* tile : tiles) {
        if (tile) {
            m_scene->addItem(tile->getGraphics());
            m_scene->addItem(tile->getLabel());
        }
    }
    // Add center image/logo
    QGraphicsPixmapItem *logo = new QGraphicsPixmapItem(QPixmap(":/monopoly/markopoly.png"));
    logo->setPos(300, 300); // Center position
    m_scene->addItem(logo);
}


QPointF Board::calculateTilePosition(int index) const {
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

