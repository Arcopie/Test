#include "../include/MeniuRestart.h"
#include "../include/Utils.h"
#include <iostream>

MeniuRestart::MeniuRestart() : nrRestartari(0) {}

void MeniuRestart::afiseazaOptiuni() {
    std::cout << std::endl;
    std::cout << "Apasa [R] pentru a reincepe jocul" << std::endl;
    std::cout << "Apasa [ESC] pentru a iesi" << std::endl;
}

bool MeniuRestart::asteaptaDecizie() {
    while (true) {
        if (tastaDisponibila()) {
            int tasta = citesteTasta();
            if (tasta == 'r' || tasta == 'R') {
                nrRestartari++;
                return true;
            }
            if (tasta == 27 || tasta == EOF) return false;
        }
        sleepMs(100);
    }
}

bool MeniuRestart::gestioneazaJoc(Joc& joc) {
    joc.ruleazaJocul();
    if (joc.esteGameOver()) {
        afiseazaOptiuni();
        return asteaptaDecizie();
    }
    return false;
}

std::ostream& operator<<(std::ostream& os, const MeniuRestart& m) {
    os << "MeniuRestart[restartari: " << m.nrRestartari << "]";
    return os;
}
