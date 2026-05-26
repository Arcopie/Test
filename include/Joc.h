#pragma once
#include "Matrice.h"
#include "Jucator.h"
#include "EntitateJoc.h"
#include "Timer.h"
#include <vector>
#include <memory>
#include <chrono>
#include <ostream>

// compune Matrice, Jucator, Timer si un vector de entitati polimorfice
class Joc {
    Matrice matrice;
    Jucator jucator;
    std::vector<std::shared_ptr<EntitateJoc>> entitati;
    Timer timer;
    std::chrono::steady_clock::time_point ultimSpawnPericulos;
    bool ruleaza;
    bool gameOver;
    int nextId;
    int totalOmorati;
    int urmatorPragMiniBoss; // urmatorul scor la care apare mini-boss

    void curataMortii();
    void adaugaInamicPericulos();
    void adaugaMiniBoss();
    void adaugaInamicRapid();
    Pozitie spawneazaPozitie(); // gaseste pozitie random departe de jucator
public:
    Joc();

    void adaugaInamic();
    bool proceseazaTasta(int tasta);
    void mutaEntitati();
    void afiseazaEcran() const;
    void ruleazaJocul();

    [[nodiscard]] bool esteGameOver() const;
    friend std::ostream& operator<<(std::ostream& os, const Joc& j);
};
