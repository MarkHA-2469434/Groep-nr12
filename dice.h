#ifndef DICE_H
#define DICE_H
#include <QRandomGenerator>
#include <tuple>

class Dice {
public:
    static std::tuple<int, int> roll();
};

#endif // DICE_H
