#pragma once
#include "Celula.h"
#include "Pozitie.h"
#include "Exceptii.h"
#include <vector>
#include <memory>
#include <ostream>

class Jucator;
class EntitateJoc;

// compune vector<vector<Celula>>, genereaza grila de taste
class Matrice {
    int linii, coloane;
    std::vector<std::vector<Celula>> grila;

    void construiesteGrila();
public:
    // arunca ExceptieConfigurareInvalida daca linii/coloane <= 0
    explicit Matrice(int linii = 4, int coloane = 10);

    [[nodiscard]] int getLinii() const;
    [[nodiscard]] int getColoane() const;

    void afiseaza(const Jucator& jucator,
                  const std::vector<std::shared_ptr<EntitateJoc>>& entitati) const;
    [[nodiscard]] Pozitie pozitieRandom() const;
    bool gasestePozitie(char ch, Pozitie& rezultat) const;

    friend std::ostream& operator<<(std::ostream& os, const Matrice& m);
};
