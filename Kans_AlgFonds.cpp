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
#include "player.h"
#include <QMainWindow>
#include <QTimer>
#include "mainwindow.h"
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
            {"Ga verder naar Lange Zoutstraat, indien u langs start komt ontvangt u 200 euro.", 3},
            {"Ga verder naar Nieuwstraat", 4},
            {"Betaal schoolgeld: 150 euro.", 5},
            {"Boete voor te snel rijden: betaal 15 euro.", 6},
            {"Boete wegens dronkenschap: betaal 20 euro.", 7},
            {"Uw bouwverzekering vervalt, u ontvangt 150 euro.", 8},
            {"Ga 3 plaatsen terug.", 9},
            {"U wint een kruiswoordraadselwedstrijd: ontvang 100 euro.", 10},
            {"Straatgeld: betaal voor elk huis 40 euro, voor elk hotel 115 euro.", 11},
            {"Ga verder naar Start.", 12},
            {"Ga naar Grote Markt, ontvang 200 euro indien voorbij Start.", 13},
            {"Ga naar Station Centraal, ontvang 200 euro indien voorbij Start.", 14}
        };
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
    }

    KansKaart trekKansKaart(Player* speler, MainWindow* window) {
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
            QTimer::singleShot(3000, window, [window, speler]() {
                window->movePlayer(11);
                window->handleLanding(speler->getPosition());
            });

            break;
        case 4:
            window->movePlayer(39);
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
            QTimer::singleShot(1000, window, [window, speler]() {
                window->movePlayer( ((speler->getPosition() + 37) % 40));
                window->handleLanding(speler->getPosition());
            });

            break;
        case 10:
            speler->voegGeldToe(100);
            break;
        case 11:
            speler->trekGeldAf(speler->aantalHuizen * 40 + speler->aantalHotels * 115);
            break;
        case 12:
            window->movePlayer(0);
            window->handleLanding(speler->getPosition());
            break;
        case 13:
            QTimer::singleShot(3000, window, [window, speler]() {
                window->movePlayer(24);
                window->handleLanding(speler->getPosition());
            });

            break;
        case 14:
            QTimer::singleShot(3000, window, [window, speler]() {
                window->movePlayer(15);
                window->handleLanding(speler->getPosition());
            });
            break;
        default:
            break;
        }
        return kaart;
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
            {"Ga terug naar Diestsestraat.", 12},
            {"Door verkoop van effecten ontvangt u 50 euro.", 13},
            {"Betaal uw doktersrekening: 50 euro.", 14}
        };

        QRandomGenerator(static_cast<unsigned int>(std::time(nullptr)));
    };

    FondsKaart trekAlgKaart(Player* speler, MainWindow* window) {
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
            char keuze = 'b';
            std::cin >> keuze;
            if (keuze == 'b') {
                speler->trekGeldAf(10);
            } else if (keuze == 'k') {
                KansStapelp kans;
                kans.trekKansKaart(speler, window);
            }
            break;
        }
        case 9:
            window->movePlayer(0);
            speler->voegGeldToe(200);
            break;
        case 10:
            speler->voegGeldToe(25);
            break;
        case 11:
            speler->voegGeldToe(200);
            break;
        case 12:
            window->movePlayer(1);
            window->handleLanding(speler->getPosition());
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
        return kaart;
    }
};
