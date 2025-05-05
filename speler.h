#ifndef SPELER_H
#define SPELER_H
#include <string>
#include <vector>

class Vakje;

class Speler
{
public:
    // Initialize all members in constructor
    Speler(const std::string& naam, int startGeld);

    // Getters that can't fail
    const std::string& getNaam() const { return naam; }
    int getGeld() const { return geld; }
    int getPositie() const { return huidigePositie; }

    // More complex getters
    const std::vector<Vakje*>& getEigendommen() const { return eigendommen; }

    // Other methods
    void beweeg(int aantalStappen);
    void koopVakje(Vakje* vakje);

private:
    std::string naam;
    int geld;
    int huidigePositie;
    std::vector<Vakje*> eigendommen;
};


#endif // SPELER_H
