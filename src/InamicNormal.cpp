#include "../include/InamicNormal.h"
#include <cstdlib>
#include <utility>

InamicNormal::InamicNormal(int id, const Pozitie& poz, char simbol)
    : EntitateJoc(id, poz, "InamicNormal"), simbol(simbol) {}

InamicNormal::InamicNormal(const InamicNormal& e)
    : EntitateJoc(e), simbol(e.simbol) {}

void swap(InamicNormal& a, InamicNormal& b) noexcept {
    using std::swap;
    swap(static_cast<EntitateJoc&>(a), static_cast<EntitateJoc&>(b));
    swap(a.simbol, b.simbol);
}

InamicNormal& InamicNormal::operator=(InamicNormal e) {
    swap(*this, e);
    return *this;
}

bool InamicNormal::aplicaLovitura() {
    if (!entitate.esteInViata()) return false;
    entitate.setInViata(false);
    return true;
}

void InamicNormal::muta(int nrLinii, int nrColoane) {
    if (!entitate.esteInViata()) return;
    const int directii[][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    int d = rand() % 4;
    int linNou = entitate.getPoz().getLin() + directii[d][0];
    int colNou = entitate.getPoz().getCol() + directii[d][1];
    if (linNou < 0) linNou = 0;
    if (linNou >= nrLinii) linNou = nrLinii - 1;
    if (colNou < 0) colNou = 0;
    if (colNou >= nrColoane) colNou = nrColoane - 1;
    entitate.setPoz(Pozitie(linNou, colNou));
}

bool InamicNormal::esteActiv() const { return entitate.esteInViata(); }

bool InamicNormal::esteLA(const Pozitie& p) const {
    return entitate.esteInViata() && entitate.getPoz() == p;
}

char InamicNormal::getSimbol() const { return simbol; }

std::unique_ptr<EntitateJoc> InamicNormal::clone() const {
    return std::make_unique<InamicNormal>(*this);
}

void InamicNormal::afiseazaIntern(std::ostream& os) const {
    os << "[" << simbol << "] " << entitate;
}
