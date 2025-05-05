#include "speler.h"
#include "vakje.h"
#include <iostream>

Speler::Speler(const std::string& naam, int startGeld)
    : naam(naam), geld(startGeld), huidigePositie(0) {
    std::cout << "Speler created: " << naam << std::endl;
}

void Speler::beweeg(int aantalStappen) {
    if (aantalStappen < 0) return; // Validate input
    huidigePositie = (huidigePositie + aantalStappen) % 40;
    std::cout << naam << " moved to position " << huidigePositie << std::endl;
}

void Speler::koopVakje(Vakje* vakje) {
    if (!vakje) {
        std::cerr << "Attempt to buy null tile!" << std::endl;
        return;
    }

    if (vakje->getEigenaar() == nullptr && geld >= 100) {
        eigendommen.push_back(vakje);
        vakje->setEigenaar(this);
        geld -= 100;
        std::cout << naam << " bought " << vakje->getNaam() << std::endl;
    }
}
