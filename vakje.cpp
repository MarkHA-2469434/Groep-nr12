#include "vakje.h"

Vakje::Vakje(VakjeType type, const std::string& naam ) : type(type), naam(naam), eigenaar(nullptr) {}

//implementatie getters
VakjeType Vakje::getType() const {return type; }
std::string Vakje::getNaam() const {return naam; }
Speler* Vakje::getEigenaar() const {return eigenaar; }

//setter
void Vakje::setEigenaar(Speler* nieuweEigenaar) {
    eigenaar = nieuweEigenaar;
}
