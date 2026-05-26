#include "../include/Celula.h"

Celula::Celula(const Pozitie& poz, char simbol) : poz(poz), simbol(simbol) {}

char Celula::getSimbol() const { return simbol; }

std::ostream& operator<<(std::ostream& os, const Celula& c) {
    os << c.simbol;
    return os;
}
