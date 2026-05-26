#include "MiniBoss.h"
#include <cstdlib>
#include <utility>

MiniBoss::MiniBoss(int id, const Pozitie& poz, int vietiMaxime)
    : EntitateJoc(id, poz, "MiniBoss"),
      vietiRamase(vietiMaxime), vietiMaxime(vietiMaxime) {
    if (vietiMaxime < 1)
        throw ExceptieEntitateInvalida("vietiMaxime trebuie sa fie >= 1");
}

MiniBoss::MiniBoss(const MiniBoss& e)
    : EntitateJoc(e), vietiRamase(e.vietiRamase), vietiMaxime(e.vietiMaxime) {}

void swap(MiniBoss& a, MiniBoss& b) noexcept {
    using std::swap;
    swap(static_cast<EntitateJoc&>(a), static_cast<EntitateJoc&>(b));
    swap(a.vietiRamase, b.vietiRamase);
    swap(a.vietiMaxime, b.vietiMaxime);
}

MiniBoss& MiniBoss::operator=(MiniBoss e) {
    swap(*this, e);
    return *this;
}

// decrementeaza vietiRamase; returneaza true cand moare
bool MiniBoss::aplicaLovitura() {
    if (vietiRamase <= 0) return false;
    --vietiRamase;
    return vietiRamase == 0;
}

void MiniBoss::muta(int nrLinii, int nrColoane) {
    if (vietiRamase <= 0) return;
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

bool MiniBoss::esteActiv() const { return vietiRamase > 0; }

bool MiniBoss::esteLA(const Pozitie& p) const {
    return vietiRamase > 0 && entitate.getPoz() == p;
}

char MiniBoss::getSimbol() const { return 'B'; }

std::unique_ptr<EntitateJoc> MiniBoss::clone() const {
    return std::make_unique<MiniBoss>(*this);
}

int MiniBoss::getVietiRamase() const { return vietiRamase; }
int MiniBoss::getVietiMaxime() const { return vietiMaxime; }

void MiniBoss::afiseazaIntern(std::ostream& os) const {
    os << "[B] " << entitate
       << " vieti: " << vietiRamase << "/" << vietiMaxime;
}
