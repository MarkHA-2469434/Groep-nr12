#include <ctime>
#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <queue>
#include <algorithm>
#include <random>
#include <QCoreApplication>
#include <QApplication>
#include <QString>
#include <QVector>
#include <QSharedPointer>
#include <QRandomGenerator>
#include <QTextStream>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>


class Speler {
public:
    // Attributen
    QString Pion;                  // Naam van de pion/speler
    QVector<QString> Property;     // Eigendommen in bezit
    int Positie;                   // Huidige positie op het bord
    int Geld;                      // Hoeveel geld de speler bezit
    bool inJail;
    bool Bankrupt;
    int aantalHuizen;
    int aantalHotels;

    // Constructor
    Speler(const QString& pionNaam, int startGeld = 1500, bool injail = false, bool bankrupt = false, int aantalHuizen = 0, int aantahotels = 0)
        : Pion(pionNaam), Positie(0), Geld(startGeld), inJail(injail), Bankrupt(bankrupt), aantalHuizen(aantalHuizen), aantalHotels(aantahotels) {
    }

    // Methode om geld toe te voegen
    void voegGeldToe(int bedrag) {
        Geld += bedrag;
    }

    void trekGeldAf(int bedrag) {
        Geld -= bedrag;
    }


    void beweeg() {
        int worp1 = QRandomGenerator::global()->bounded(1, 7);
        int worp2 = QRandomGenerator::global()->bounded(1, 7);
        QTextStream(stdout) << Pion << " heeft " << worp1 << " en " << worp2 << " gegooid\n";
        int totaal = worp1 + worp2;
        Positie = (Positie + totaal) % 40;
    }
};

class Game {
private:
    QVector<QSharedPointer<Speler>> spelers;
    int huidigeSpelerIndex = 0;

public:
    void voegSpelerToe(const QString& pionNaam, int startGeld) {
        spelers.push_back(QSharedPointer<Speler>::create(pionNaam, startGeld));
    }

    void start() {
        QTextStream(stdout) << "Spel gestart met " << spelers.size() << " spelers.\n";

        while (aantalActieveSpelers() > 1) {
            speelBeurt();
        }

        QTextStream(stdout) << "\nSpel afgelopen!\n";
        toonEindstanden();
    }

    int aantalActieveSpelers() {
        int count = 0;
        for (const auto& speler : spelers) {
            if (!speler->Bankrupt) count++;
        }
        return count;
    }

private:
    void speelBeurt() {
        if (spelers.isEmpty()) return;

        auto& Speler = spelers[huidigeSpelerIndex];
        QTextStream(stdout) << "Speler " << Speler->Pion << " is aan de beurt.\n";

        Speler->beweeg();
        QTextStream(stdout) << Speler->Pion << " staat nu op positie " << Speler->Positie << ".\n";

        // Volgende speler
        huidigeSpelerIndex = (huidigeSpelerIndex + 1) % spelers.size();
    }

    void toonEindstanden() {
        QTextStream(stdout) << "\nEindstanden:\n";
        for (const auto& Speler : spelers) {
            QTextStream(stdout) << Speler->Pion
                                << " | Positie: " << Speler->Positie
                                << " | Geld: " << Speler->Geld
                                << "\n";
        }
    }
};

class Bord {
public:
    class Property {
    public:
        QString naam;
        int prijs;
        QVector<int> huurLijst; // index 0 = geen huis, 1-4 huizen, 5 = hotel
        Speler* eigenaar = nullptr;
        int huizen = 0;
        bool hotel = false;
        int positie;

        Property(const QString& naam, int prijs, const QVector<int>& huurLijst, int positie)
            : naam(naam), prijs(prijs), huurLijst(huurLijst), positie(positie) {
        }

        bool isBeschikbaar() const {
            return eigenaar == nullptr;
        }

        void koop(Speler* koper) {
            if (isBeschikbaar() && koper->Geld >= prijs) {
                eigenaar = koper;
                koper->trekGeldAf(prijs);
                QTextStream(stdout) << koper->Pion << " kocht " << naam << " voor " << prijs << " euro.\n";
            }
        }

        void betaalHuur(Speler* huurder) {
            if (eigenaar && eigenaar != huurder) {
                int index = hotel ? 5 : huizen;
                int teBetalen = huurLijst.at(index);
                huurder->trekGeldAf(teBetalen);
                eigenaar->voegGeldToe(teBetalen);
                QTextStream(stdout) << huurder->Pion << " betaalt " << teBetalen << " euro huur aan " << eigenaar->Pion << ".\n";
            }
        }
    };
};

struct KansKaart {
    QString beschrijving;
    int Kaartnr;
};

class KansStapelp {
private:
    QVector<KansKaart> kaarten;

public:
    KansStapelp() {
        kaarten = {
            {"Repareer uw huizen: betaal voor elk huis 25 euro, voor elk hotel 100 euro.", 1},
            {"De bank betaalt u 100 euro dividend.", 2},
            {"Ga verder naar ‘Lange Zoutstraat’, indien u langs start komt ontvangt u 200 euro.", 3},
            {"Ga verder naar ‘Nieuwstraat’", 4},
            {"Betaal schoolgeld: 150 euro.", 5},
            {"Boete voor te snel rijden: betaal 15 euro.", 6},
            {"Boete wegens dronkenschap: betaal 20 euro.", 7},
            {"Uw bouwverzekering vervalt, u ontvangt 150 euro.", 8},
            {"Ga 3 plaatsen terug.", 9},
            {"U wint een kruiswoordraadselwedstrijd: ontvang 100 euro.", 10},
            {"Straatgeld: betaal voor elk huis 40 euro, voor elk hotel 115 euro.", 11},
            {"Ga verder naar Start.", 12},
            {"Ga naar ‘Grote Markt’, ontvang 200 euro indien voorbij Start.", 13},
            {"Ga naar ‘Station Centraal’, ontvang 200 euro indien voorbij Start.", 14}
        };
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
    }

    void trekKansKaart(Speler* speler) {
        int index = std::rand() % kaarten.size();
        KansKaart kaart = kaarten[index];

        QTextStream(stdout) << "\nKanskaart: " << kaart.beschrijving << "\n";

        switch (kaart.Kaartnr) {
        case 1:
            speler->trekGeldAf(speler->aantalHuizen * 25 + speler->aantalHotels * 100);
            break;
        case 2:
            speler->voegGeldToe(100);
            break;
        case 3:
            if (speler->Positie > 11) speler->voegGeldToe(200);
            speler->Positie = 11;
            break;
        case 4:
            speler->Positie = 24;
            break;
        case 5:
            speler->trekGeldAf(150);
            break;
        case 6:
            speler->trekGeldAf(15);
            break;
        case 7:
            speler->trekGeldAf(20);
            break;
        case 8:
            speler->voegGeldToe(150);
            break;
        case 9:
            speler->Positie = (speler->Positie + 37) % 40;
            break;
        case 10:
            speler->voegGeldToe(100);
            break;
        case 11:
            speler->trekGeldAf(speler->aantalHuizen * 40 + speler->aantalHotels * 115);
            break;
        case 12:
            speler->Positie = 0;
            speler->voegGeldToe(200);
            break;
        case 13:
            if (speler->Positie > 5) speler->voegGeldToe(200);
            speler->Positie = 5;
            break;
        case 14:
            if (speler->Positie > 15) speler->voegGeldToe(200);
            speler->Positie = 15;
            break;
        default:
            break;
        }
    }
};
struct FondsKaart {
    QString beschrijving;
    int Kaartnr;
};

class AlgemeenFondsStapelp {
private:
    QVector<FondsKaart> kaarten;

public:
    AlgemeenFondsStapelp() {
        kaarten = {
            {"Lijfrente vervalt, u ontvangt 100 euro.", 1},
            {"Betaal het hospitaal: 100 euro.", 2},
            {"Betaal uw verzekeringspremie: 50 euro.", 3},
            {"U hebt de tweede prijs in een schoonheidswedstrijd gewonnen en ontvangt 10 euro.", 4},
            {"Restitutie inkomstenbelasting: u ontvangt 10 euro.", 5},
            {"U bent jarig en ontvangt van elke speler 10 euro.", 6},
            {"U erft 100 euro.", 7},
            {"Betaal 10 euro boete of neem een Kans kaart.", 8},
            {"Ga verder naar start.", 9},
            {"U ontvangt rente van 7% preferente aandelen en ontvangt 25 euro.", 10},
            {"Een vergissing van de bank in uw voordeel: u ontvangt 200 euro.", 11},
            {"Ga terug naar ‘Diestsestraat’.", 12},
            {"Door verkoop van effecten ontvangt u 50 euro.", 13},
            {"Betaal uw doktersrekening: 50 euro.", 14}
        };

        QRandomGenerator(static_cast<unsigned int>(std::time(nullptr)));
    };

    void trekAlgKaart(Speler* speler) {
        int index = std::rand() % kaarten.size();
        FondsKaart kaart = kaarten[index];

        QTextStream(stdout) << "\nAlgemeen Fonds kaart: " << kaart.beschrijving << "\n";

        switch (kaart.Kaartnr) {
        case 1:
            speler->voegGeldToe(100);
            break;
        case 2:
            speler->trekGeldAf(100);
            break;
        case 3:
            speler->trekGeldAf(50);
            break;
        case 4:
            speler->voegGeldToe(10);
            break;
        case 5:
            speler->voegGeldToe(10);
            break;
        case 6:
            speler->voegGeldToe(10);
            break;
        case 7:
            speler->voegGeldToe(100);
            break;
        case 8: {
            std::cout << "Geef b in voor boete en k voor kanskaart: ";
            char keuze;
            std::cin >> keuze;
            if (keuze == 'b') {
                speler->trekGeldAf(10);
            } else if (keuze == 'k') {
                KansStapelp kans;
                kans.trekKansKaart(speler);
            }
            break;
        }
        case 9:
            speler-> Positie =0;
            speler->voegGeldToe(200);
            break;
        case 10:
            speler->voegGeldToe(25);
            break;
        case 11:
            speler->voegGeldToe(200);
            break;
        case 12:
            speler->Positie = 3;
            break;
        case 13:
            speler->voegGeldToe(50);
            break;
        case 14:
            speler->trekGeldAf(50);
            break;
        default:
            break;
        }
    }
};
