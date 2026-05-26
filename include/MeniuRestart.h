#pragma once
#include "Joc.h"
#include <ostream>

class MeniuRestart {
    int nrRestartari;

    static void afiseazaOptiuni();
    bool asteaptaDecizie();
public:
    MeniuRestart();
    bool gestioneazaJoc(Joc& joc);
    friend std::ostream& operator<<(std::ostream& os, const MeniuRestart& m);
};
