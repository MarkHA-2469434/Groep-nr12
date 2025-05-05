#pragma once
#ifndef BORD_H
#define BORD_H
#include "Vakje.h"
#include <vector>

class Bord
{
private:
    std::vector<Vakje> vakjes;
    void initializeBord(); // Hardcoded vakjes toevoegen
;
public:
    Bord();
    const std::vector<Vakje>& getVakjes() const { return vakjes; }
    Vakje& getVakje(int index) { return vakjes.at(index); }
};

#endif // BORD_H
