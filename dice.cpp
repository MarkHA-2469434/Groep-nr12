#include "dice.h"

std::tuple<int, int> Dice::roll() {
    // Generate random numbers between 1-6
    QRandomGenerator* gen = QRandomGenerator::global();
    return {gen->bounded(1, 7), gen->bounded(1, 7)};
}
