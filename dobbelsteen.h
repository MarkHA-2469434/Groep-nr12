#pragma once
#ifndef DOBBELSTEEN_H
#define DOBBELSTEEN_H
#include <random>

class Dobbelsteen
{
private:
    std::random_device rd;
    std::mt19937 gen;
public:
    Dobbelsteen();
    int gooi(); // 1-6
};

#endif // DOBBELSTEEN_H
