#include "bord.h"

Bord::Bord() {
    initializeBord();
}

void Bord::initializeBord() {
    vakjes = {
        // Eerste rij (bovenkant)
        {VakjeType::START, "Start"},
        {VakjeType::STRAAT, "Oudestraat"},
        {VakjeType::ALGEMEEN_FONDS, "Algemeen Fonds"},
        {VakjeType::STRAAT, "Nieuwstraat"},
        {VakjeType::STATION, "Station Noord"},
        {VakjeType::STRAAT, "Binnenweg"},
        {VakjeType::KANS, "Kans"},
        {VakjeType::STRAAT, "Coolsingel"},
        {VakjeType::STRAAT, "Hofplein"},
        {VakjeType::STRAAT, "Blaak"},

        // Rechterkant
        {VakjeType::GEVANGENIS_BEZOEK, "Gevangenis (Bezoek)"},
        {VakjeType::STRAAT, "Leidschestraat"},
        {VakjeType::ELEKTRICITEIT, "Elektriciteitsbedrijf"},
        {VakjeType::STRAAT, "Kalverstraat"},
        {VakjeType::STRAAT, "Vijzelstraat"},
        {VakjeType::STATION, "Station Zuid"},
        {VakjeType::STRAAT, "PC Hooftstraat"},
        {VakjeType::ALGEMEEN_FONDS, "Algemeen Fonds"},
        {VakjeType::STRAAT, "Rokin"},
        {VakjeType::STRAAT, "Dam"},

        // Onderkant
        {VakjeType::VRIJ_PARKEREN, "Vrij Parkeren"},
        {VakjeType::STRAAT, "Muntplein"},
        {VakjeType::KANS, "Kans"},
        {VakjeType::STRAAT, "Rembrandtplein"},
        {VakjeType::STRAAT, "Waterlooplein"},
        {VakjeType::STATION, "Station West"},
        {VakjeType::STRAAT, "Weesperstraat"},
        {VakjeType::STRAAT, "Sarphatistraat"},
        {VakjeType::WATERLEIDING, "Waterleiding"},
        {VakjeType::STRAAT, "Frederiksplein"},

        // Linkerkant
        {VakjeType::GA_NAAR_GEVANGENIS, "Ga naar Gevangenis"},
        {VakjeType::STRAAT, "Spuistraat"},
        {VakjeType::STRAAT, "Plein"},
        {VakjeType::ALGEMEEN_FONDS, "Algemeen Fonds"},
        {VakjeType::STRAAT, "Lange Poten"},
        {VakjeType::STATION, "Station Oost"},
        {VakjeType::KANS, "Kans"},
        {VakjeType::STRAAT, "Korte Poten"},
        {VakjeType::EXTRA_BELASTING, "Extra Belasting"},
        {VakjeType::STRAAT, "Paleisstraat"}
    };
}
