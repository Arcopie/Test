#include "../include/EntitateJoc.h"
#include <utility>

int EntitateJoc::contor = 0;

EntitateJoc::EntitateJoc(int id, const Pozitie& poz, const char* tip)
    : entitate(id, poz, tip) {
    contor++;
}

EntitateJoc::EntitateJoc(const EntitateJoc& e) : entitate(e.entitate) {
    contor++;
}

EntitateJoc::~EntitateJoc() { contor--; }

// swap protejat pentru derivate - schimba entitatea interna
void swap(EntitateJoc& a, EntitateJoc& b) noexcept {
    using std::swap;
    swap(a.entitate, b.entitate);
}

const Pozitie& EntitateJoc::getPoz() const { return entitate.getPoz(); }

int EntitateJoc::getContor() { return contor; }

// NVI: apeleaza afiseazaIntern virtual
std::ostream& operator<<(std::ostream& os, const EntitateJoc& e) {
    e.afiseazaIntern(os);
    return os;
}
