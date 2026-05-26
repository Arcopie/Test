#include "../include/Matrice.h"
#include "../include/Jucator.h"
#include "../include/EntitateJoc.h"
#include <iostream>
#include <cstdlib>
#include <cstring>

Matrice::Matrice(int linii, int coloane) : linii(linii), coloane(coloane) {
    if (linii <= 0 || coloane <= 0)
        throw ExceptieConfigurareInvalida("dimensiuni invalide pentru matrice");
    construiesteGrila();
}

void Matrice::construiesteGrila() {
    // rand 0: qwerty, rand 1: asdf, rand 2: zxcv, rand 3: 1234...0
    const char taste[] = "qwertyuiopasdfghjkl;zxcvbnm,./1234567890";
    int nrTaste = (int)(sizeof(taste) - 1);
    for (int i = 0; i < linii; i++) {
        grila.emplace_back();
        for (int j = 0; j < coloane; j++) {
            int idx = i * coloane + j;
            char ch = (idx < nrTaste) ? taste[idx] : '.';
            grila[i].emplace_back(Pozitie(i, j), ch);
        }
    }
}

int Matrice::getLinii() const { return linii; }
int Matrice::getColoane() const { return coloane; }

void Matrice::afiseaza(const Jucator& jucator,
    const std::vector<std::shared_ptr<EntitateJoc>>& entitati) const {
    std::cout << "  +-";
    for (int j = 0; j < coloane; j++) std::cout << "--";
    std::cout << "+" << std::endl;

    for (int i = 0; i < linii; i++) {
        std::cout << "  | ";
        for (int j = 0; j < coloane; j++) {
            Pozitie p(i, j);
            if (jucator.getPoz() == p) {
                std::cout << "@ ";
                continue;
            }
            bool gasit = false;
            for (const auto& e : entitati) {
                if (e->esteActiv() && e->esteLA(p)) {
                    std::cout << e->getSimbol() << " ";
                    gasit = true;
                    break;
                }
            }
            if (!gasit) std::cout << grila[i][j].getSimbol() << " ";
        }
        std::cout << "|" << std::endl;
    }

    std::cout << "  +-";
    for (int j = 0; j < coloane; j++) std::cout << "--";
    std::cout << "+" << std::endl;
}

Pozitie Matrice::pozitieRandom() const {
    return Pozitie(rand() % linii, rand() % coloane);
}

bool Matrice::gasestePozitie(char ch, Pozitie& rezultat) const {
    for (int i = 0; i < linii; i++)
        for (int j = 0; j < coloane; j++)
            if (grila[i][j].getSimbol() == ch) {
                rezultat = Pozitie(i, j);
                return true;
            }
    return false;
}

std::ostream& operator<<(std::ostream& os, const Matrice& m) {
    os << "Matrice " << m.linii << "x" << m.coloane << ":" << std::endl;
    for (int i = 0; i < m.linii; i++) {
        for (int j = 0; j < m.coloane; j++)
            os << m.grila[i][j].getSimbol() << " ";
        os << std::endl;
    }
    return os;
}
