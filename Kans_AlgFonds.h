#ifndef KANS_ALGFONDS_H
#define KANS_ALGFONDS_H

#include <QString>
#include <QVector>

class Player;  // Forward declaration
class MainWindow;  // Forward declaration

struct KansKaart {
    QString beschrijving;
    int Kaartnr;
    bool isJailCard = false;
};

struct FondsKaart {
    QString beschrijving;
    int Kaartnr;
    bool isJailCard = false;
};

class KansStapelp {
public:
    KansStapelp();
    KansKaart trekKansKaart(Player* speler, MainWindow* window);

private:
    QVector<KansKaart> kaarten;
};

class AlgemeenFondsStapelp {
public:
    AlgemeenFondsStapelp();
    FondsKaart trekAlgKaart(Player* speler, MainWindow* window);

private:
    QVector<FondsKaart> kaarten;
};

#endif // KANS_ALGFONDS_H
