#pragma once
#include "Entitate.h"
#include <vector>
#include <memory>
#include <ostream>

class EntitateJoc; // forward declaration

class Jucator {
    Entitate entitate;
    int scor;
    int nrSliceuri;
public:
    explicit Jucator(const Pozitie& start);

    void teleport(const Pozitie& p);

    // slice pe traseul intre pozVeche si pozitia curenta; returneaza nr omorati
    int slice(std::vector<std::shared_ptr<EntitateJoc>>& entitati,
              const Pozitie& pozVeche);

    // verifica daca vreo entitate activa se afla pe pozitia jucatorului
    [[nodiscard]] bool atingeEntitate(
        const std::vector<std::shared_ptr<EntitateJoc>>& entitati) const;

    [[nodiscard]] int getScor() const;
    [[nodiscard]] int getSliceuri() const;
    [[nodiscard]] const Pozitie& getPoz() const;

    friend std::ostream& operator<<(std::ostream& os, const Jucator& j);
};
