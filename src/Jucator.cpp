#include "../include/Jucator.h"
#include "../include/EntitateJoc.h"
#include <algorithm>

Jucator::Jucator(const Pozitie& start)
    : entitate(0, start, "Jucator"), scor(0), nrSliceuri(0) {}

void Jucator::teleport(const Pozitie& p) { entitate.setPoz(p); }

int Jucator::slice(std::vector<std::shared_ptr<EntitateJoc>>& entitati,
                   const Pozitie& pozVeche) {
    Pozitie pozNoua = entitate.getPoz();
    int omorati = 0;

    if (pozVeche.getLin() == pozNoua.getLin()) {
        int lin = pozNoua.getLin();
        int c1 = std::min(pozVeche.getCol(), pozNoua.getCol());
        int c2 = std::max(pozVeche.getCol(), pozNoua.getCol());
        for (auto& e : entitati) {
            if (e->esteActiv() &&
                e->getPoz().getLin() == lin &&
                e->getPoz().getCol() >= c1 &&
                e->getPoz().getCol() <= c2) {
                if (e->aplicaLovitura()) {
                    scor += 10;
                    nrSliceuri++;
                    omorati++;
                }
            }
        }
    } else if (pozVeche.getCol() == pozNoua.getCol()) {
        int col = pozNoua.getCol();
        int r1 = std::min(pozVeche.getLin(), pozNoua.getLin());
        int r2 = std::max(pozVeche.getLin(), pozNoua.getLin());
        for (auto& e : entitati) {
            if (e->esteActiv() &&
                e->getPoz().getCol() == col &&
                e->getPoz().getLin() >= r1 &&
                e->getPoz().getLin() <= r2) {
                if (e->aplicaLovitura()) {
                    scor += 10;
                    nrSliceuri++;
                    omorati++;
                }
            }
        }
    }

    return omorati;
}

bool Jucator::atingeEntitate(
    const std::vector<std::shared_ptr<EntitateJoc>>& entitati) const {
    for (const auto& e : entitati)
        if (e->esteLA(entitate.getPoz()))
            return true;
    return false;
}

int Jucator::getScor() const { return scor; }
int Jucator::getSliceuri() const { return nrSliceuri; }
const Pozitie& Jucator::getPoz() const { return entitate.getPoz(); }

std::ostream& operator<<(std::ostream& os, const Jucator& j) {
    os << j.entitate << " | scor: " << j.scor
       << " | sliceuri: " << j.nrSliceuri;
    return os;
}
