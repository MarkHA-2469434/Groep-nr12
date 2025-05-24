#include "dice.h"

std::tuple<int, int> Dice::roll() {
    // Generate random numbers between 1-6
    int dice1 = QRandomGenerator::global()->bounded(1, 7);
    int dice2 = QRandomGenerator::global()->bounded(1, 7);
    return std::make_tuple(dice1, dice2);
}
