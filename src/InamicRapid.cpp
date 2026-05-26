#include "InamicRapid.h"
#include <cstdlib>
#include <utility>

InamicRapid::InamicRapid(int id, const Pozitie& poz, char simbol)
    : EntitateJoc(id, poz, "InamicRapid"), simbol(simbol) {}

InamicRapid::InamicRapid(const InamicRapid& e)
    : EntitateJoc(e), simbol(e.simbol) {}

void swap(InamicRapid& a, InamicRapid& b) noexcept {
    using std::swap;
    swap(static_cast<EntitateJoc&>(a), static_cast<EntitateJoc&>(b));
    swap(a.simbol, b.simbol);
}

InamicRapid& InamicRapid::operator=(InamicRapid e) {
    swap(*this, e);
    return *this;
}

bool InamicRapid::aplicaLovitura() {
    if (!entitate.esteInViata()) return false;
    entitate.setInViata(false);
    return true;
}

// se misca pe diagonale, mai greu de evitat
void InamicRapid::muta(int nrLinii, int nrColoane) {
    if (!entitate.esteInViata()) return;
    const int directii[][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    int d = rand() % 4;
    int linNou = entitate.getPoz().getLin() + directii[d][0];
    int colNou = entitate.getPoz().getCol() + directii[d][1];
    if (linNou < 0) linNou = 0;
    if (linNou >= nrLinii) linNou = nrLinii - 1;
    if (colNou < 0) colNou = 0;
    if (colNou >= nrColoane) colNou = nrColoane - 1;
    entitate.setPoz(Pozitie(linNou, colNou));
}

bool InamicRapid::esteActiv() const { return entitate.esteInViata(); }

bool InamicRapid::esteLA(const Pozitie& p) const {
    return entitate.esteInViata() && entitate.getPoz() == p;
}

char InamicRapid::getSimbol() const { return simbol; }

std::unique_ptr<EntitateJoc> InamicRapid::clone() const {
    return std::make_unique<InamicRapid>(*this);
}

void InamicRapid::afiseazaIntern(std::ostream& os) const {
    os << "[" << simbol << "*] " << entitate;
}
