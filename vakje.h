#ifndef VAKJE_H
#define VAKJE_H
#include <string>

class Speler;

enum class VakjeType {START,
                       STRAAT,
                       STATION,
                       ALGEMEEN_FONDS,
                       KANS,
                       GEVANGENIS_BEZOEK,
                       GEVANGENIS,
                       GA_NAAR_GEVANGENIS,
                       ELEKTRICITEIT,
                       WATERLEIDING,
                       VRIJ_PARKEREN,
                       EXTRA_BELASTING
};

class Vakje
{
private:
    VakjeType type;
    std::string naam;
    Speler* eigenaar;
public:
    Vakje(VakjeType type, const std::string& naam);

    //getters
    VakjeType getType() const;
    std::string getNaam() const;
    Speler* getEigenaar() const;

    //setters
    void setEigenaar(Speler* nieuweEigenaar);
};

#endif // VAKJE_H
