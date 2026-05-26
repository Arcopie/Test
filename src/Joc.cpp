#include "Joc.h"
#include "InamicNormal.h"
#include "InamicPericulos.h"
#include "MiniBoss.h"
#include "InamicRapid.h"
#include "Utils.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>

Joc::Joc()
    : matrice(4, 10), jucator(Pozitie(1, 5)), timer(5.0, 6.0),
      ultimSpawnPericulos(std::chrono::steady_clock::now()),
      ruleaza(false), gameOver(false), nextId(1),
      totalOmorati(0), urmatorPragMiniBoss(100) {}

// gaseste o pozitie random care nu se suprapune cu jucatorul
Pozitie Joc::spawneazaPozitie() {
    Pozitie p = matrice.pozitieRandom();
    int inc = 0;
    while (p == jucator.getPoz() && inc < 20) {
        p = matrice.pozitieRandom();
        inc++;
    }
    return p;
}

// sterge entitatile inactive cu remove_if (STL)
void Joc::curataMortii() {
    entitati.erase(
        std::remove_if(entitati.begin(), entitati.end(),
            [](const std::shared_ptr<EntitateJoc>& e) {
                return !e->esteActiv();
            }),
        entitati.end());
}

void Joc::adaugaInamicPericulos() {
    entitati.push_back(
        std::make_shared<InamicPericulos>(nextId++, spawneazaPozitie(), 2.0));
}

void Joc::adaugaMiniBoss() {
    // scor 100 -> 4 vieti, 200 -> 5, ..., max 8
    int vieti = std::min(3 + urmatorPragMiniBoss / 100, 8);
    entitati.push_back(
        std::make_shared<MiniBoss>(nextId++, spawneazaPozitie(), vieti));
    urmatorPragMiniBoss += 100;
}

void Joc::adaugaInamicRapid() {
    // inamicul rapid apare dupa scor 50 si se misca diagonal
    if (jucator.getScor() < 50) return;
    const char simboluri[] = "RSTUVWXY";
    char sim = simboluri[(nextId - 1) % 8];
    entitati.push_back(
        std::make_shared<InamicRapid>(nextId++, spawneazaPozitie(), sim));
}

void Joc::adaugaInamic() {
    // dynamic_cast pentru a numara doar inamicii normali
    int nrNormali = 0;
    for (const auto& e : entitati)
        if (dynamic_cast<const InamicNormal*>(e.get()) && e->esteActiv())
            nrNormali++;
    if (nrNormali >= 8) return;

    const char simboluri[] = "EFGHIJKL";
    char sim = simboluri[(nextId - 1) % 8];
    // la fiecare al 3-lea inamic normal, spawnez unul rapid in loc
    if (nextId % 3 == 0 && jucator.getScor() >= 50)
        entitati.push_back(
            std::make_shared<InamicRapid>(nextId++, spawneazaPozitie(), sim));
    else
        entitati.push_back(
            std::make_shared<InamicNormal>(nextId++, spawneazaPozitie(), sim));
}

bool Joc::proceseazaTasta(int tasta) {
    if (tasta == 27 || tasta == EOF) {
        ruleaza = false;
        return false;
    }
    char ch = static_cast<char>(tasta);
    if (ch >= 'A' && ch <= 'Z') ch = ch - 'A' + 'a';

    Pozitie dest(0, 0);
    if (!matrice.gasestePozitie(ch, dest)) return false;

    Pozitie pozVeche = jucator.getPoz();
    jucator.teleport(dest);

    // aterizat fix pe o entitate -> game over
    if (jucator.atingeEntitate(entitati)) {
        gameOver = true;
        ruleaza = false;
        return true;
    }

    int omorati = jucator.slice(entitati, pozVeche);
    if (omorati > 0) {
        totalOmorati += omorati;
        curataMortii();
        while (totalOmorati >= 2) {
            adaugaInamicPericulos();
            totalOmorati -= 2;
        }
    }

    // spawn mini-boss la fiecare prag de 100 puncte
    while (jucator.getScor() >= urmatorPragMiniBoss)
        adaugaMiniBoss();

    return true;
}

void Joc::mutaEntitati() {
    for (auto& e : entitati)
        e->muta(matrice.getLinii(), matrice.getColoane());
}

void Joc::afiseazaEcran() const {
    clearScreen();
    std::cout << "SLICE GAME" << std::endl;
    std::cout << "Scor: " << jucator.getScor()
              << " | Sliceuri: " << jucator.getSliceuri()
              << " | Entitati: " << EntitateJoc::getContor() << std::endl;
    std::cout << "Apasa litera/cifra = teleportare + slice" << std::endl;
    std::cout << "ESC = iesire | ! = periculos | B = MiniBoss" << std::endl;

    // dynamic_cast pentru info specifice MiniBoss
    for (const auto& e : entitati) {
        if (const auto* boss = dynamic_cast<const MiniBoss*>(e.get())) {
            if (boss->esteActiv()) {
                std::cout << ">>> MINI-BOSS activ! Vieti: "
                          << boss->getVietiRamase() << "/"
                          << boss->getVietiMaxime() << " <<<" << std::endl;
            }
        }
    }
    std::cout << std::endl;
    matrice.afiseaza(jucator, entitati);
    std::cout << std::endl;
}

void Joc::ruleazaJocul() {
    srand((unsigned)time(nullptr));
    ruleaza = true;
    gameOver = false;
    ultimSpawnPericulos = std::chrono::steady_clock::now();
    adaugaInamic();
    afiseazaEcran();

    while (ruleaza) {
        bool trebuieRedesnat = false;

        if (timer.trebuieSpawn()) {
            adaugaInamic();
            timer.resetSpawn();
            trebuieRedesnat = true;
        }

        double secDePericulos = std::chrono::duration<double>(
            std::chrono::steady_clock::now() - ultimSpawnPericulos).count();
        if (secDePericulos >= 2.0) {
            adaugaInamicPericulos();
            ultimSpawnPericulos = std::chrono::steady_clock::now();
            trebuieRedesnat = true;
        }

        int inainte = (int)entitati.size();
        curataMortii();
        if ((int)entitati.size() != inainte) trebuieRedesnat = true;

        if (timer.trebuieMiscare()) {
            mutaEntitati();
            timer.resetMiscare();
            trebuieRedesnat = true;
            if (jucator.atingeEntitate(entitati)) {
                gameOver = true;
                ruleaza = false;
            }
        }

        if (jucator.atingeEntitate(entitati)) {
            gameOver = true;
            ruleaza = false;
        }

        if (tastaDisponibila()) {
            int tasta = citesteTasta();
            if (proceseazaTasta(tasta)) trebuieRedesnat = true;
        }

        if (trebuieRedesnat) afiseazaEcran();
        sleepMs(100);
    }

    afiseazaEcran();
    if (gameOver) {
        std::cout << "GAME OVER! Un inamic te-a atins!" << std::endl;
        std::cout << "Scor final: " << jucator.getScor() << std::endl;
    }
    sleepMs(2000);
}

bool Joc::esteGameOver() const { return gameOver; }

std::ostream& operator<<(std::ostream& os, const Joc& j) {
    os << "Joc: " << j.jucator << " | " << j.timer;
    return os;
}
