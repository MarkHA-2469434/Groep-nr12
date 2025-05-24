#include "Kans_AlgFonds.h"
#include "player.h"
#include "mainwindow.h"
#include <QTextStream>
#include <QTimer>
#include <QRandomGenerator>
#include <iostream>
#include <ctime>


KansStapelp::KansStapelp() {
    kaarten = {
        {"Repareer uw huizen: betaal voor elk huis 25 euro, voor elk hotel 100 euro.", 1, false},
        {"De bank betaalt u 100 euro dividend.", 2, false},
        {"Ga verder naar Lange Zoutstraat, indien u langs start komt ontvangt u 200 euro.", 3, false},
        {"Ga verder naar Nieuwstraat", 4, false},
        {"Betaal schoolgeld: 150 euro.", 5, false},
        {"Boete voor te snel rijden: betaal 15 euro.", 6, false},
        {"Boete wegens dronkenschap: betaal 20 euro.", 7, false},
        {"Uw bouwverzekering vervalt, u ontvangt 150 euro.", 8, false},
        {"Ga 3 plaatsen terug.", 9, false},
        {"U wint een kruiswoordraadselwedstrijd: ontvang 100 euro.", 10, false},
        {"Straatgeld: betaal voor elk huis 40 euro, voor elk hotel 115 euro.", 11, false},
        {"Ga verder naar Start.", 12, false},
        {"Ga naar Grote Markt, ontvang 200 euro indien voorbij Start.", 13, false},
        {"Ga naar Station Centraal, ontvang 200 euro indien voorbij Start.", 14, false},
        {"Get Out of Jail Free", 15, true}  // Example jail card
    };
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

KansKaart KansStapelp::trekKansKaart(Player* speler, MainWindow* window) {
    if (!speler || !window) {
        qWarning() << "Invalid player or window pointer";
        return KansKaart();
    }

    int index = QRandomGenerator::global()->bounded(kaarten.size());
    KansKaart kaart = kaarten[index];
    qDebug() << "Drew Chance card:" << kaart.beschrijving;

    switch (kaart.Kaartnr) {
    case 1:
        speler->trekGeldAf(speler->aantalHuizen * 25 + speler->aantalHotels * 100);
        break;
    case 2:
        speler->voegGeldToe(100);
        break;
    case 3:
        QTimer::singleShot(1000, window, [window, speler]() {
            window->movePlayer(11, speler);
            window->handleLanding(speler->getPosition());
        });
        break;
    case 4:
        window->movePlayer(39, speler);
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
            window->movePlayer((speler->getPosition() + 37) % 40, speler);
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
        window->movePlayer(0, speler);
        window->handleLanding(speler->getPosition());
        break;
    case 13:
        QTimer::singleShot(1000, window, [window, speler]() {
            window->movePlayer(24, speler);
            window->handleLanding(speler->getPosition());
        });
        break;
    case 14:
        QTimer::singleShot(1000, window, [window, speler]() {
            window->movePlayer(15, speler);
            window->handleLanding(speler->getPosition());
        });
        break;
    case 15: // Jail card
        speler->jailCards++;
        break;
    }

    return kaart;
}


AlgemeenFondsStapelp::AlgemeenFondsStapelp() {
    kaarten = {
        {"Lijfrente vervalt, u ontvangt 100 euro.", 1, false},
        {"Betaal het hospitaal: 100 euro.", 2, false},
        {"Betaal uw verzekeringspremie: 50 euro.", 3, false},
        {"U hebt de tweede prijs in een schoonheidswedstrijd gewonnen en ontvangt 10 euro.", 4, false},
        {"Restitutie inkomstenbelasting: u ontvangt 10 euro.", 5, false},
        {"U bent jarig en ontvangt van elke speler 10 euro.", 6, false},
        {"U erft 100 euro.", 7, false},
        {"Betaal 10 euro boete of neem een Kans kaart.", 8, false},
        {"Ga verder naar start.", 9, false},
        {"U ontvangt rente van 7% preferente aandelen en ontvangt 25 euro.", 10, false},
        {"Een vergissing van de bank in uw voordeel: u ontvangt 200 euro.", 11, false},
        {"Ga terug naar Diestsestraat.", 12, false},
        {"Door verkoop van effecten ontvangt u 50 euro.", 13, false},
        {"Betaal uw doktersrekening: 50 euro.", 14, false},
        {"Get Out of Jail Free", 15, true}  // Example jail card
    };
}

FondsKaart AlgemeenFondsStapelp::trekAlgKaart(Player* speler, MainWindow* window) {
    if (!speler || !window) {
        qWarning() << "Invalid player or window pointer";
        return FondsKaart();
    }

    int index = QRandomGenerator::global()->bounded(kaarten.size());
    FondsKaart kaart = kaarten[index];
    qDebug() << "Drew Community Chest card:" << kaart.beschrijving;

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
        // Birthday - collect from all players would need additional logic
        speler->voegGeldToe(10); // Simplified for now
        break;
    case 7:
        speler->voegGeldToe(100);
        break;
    case 8: {
        // In a real game, you'd show a dialog for this choice
        bool payFine = true; // Default to paying fine
        if (payFine) {
            speler->trekGeldAf(10);
        } else {
            KansStapelp kans;
            kans.trekKansKaart(speler, window);
        }
        break;
    }
    case 9:
        window->movePlayer(0, speler);
        speler->voegGeldToe(200);
        break;
    case 10:
        speler->voegGeldToe(25);
        break;
    case 11:
        speler->voegGeldToe(200);
        break;
    case 12:
        window->movePlayer(1, speler);
        window->payRent(speler->getPosition());
        break;
    case 13:
        speler->voegGeldToe(50);
        break;
    case 14:
        speler->trekGeldAf(50);
        break;
    case 15: // Jail card
        speler->jailCards++;
        break;
    }

    return kaart;
}
