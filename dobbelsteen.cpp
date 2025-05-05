#include "dobbelsteen.h"

Dobbelsteen::Dobbelsteen() : gen(rd()) {}

int Dobbelsteen::gooi() {
    std::uniform_int_distribution<> dis(1,6);
    return dis(gen);
}
