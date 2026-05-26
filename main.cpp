#include "include/EntitateJoc.h"
#include "include/Exceptii.h"
#include "include/Joc.h"
#include "include/Matrice.h"
#include "include/MeniuRestart.h"
#include "include/Utils.h"
#include <iostream>

// implementari (single-file compilation)
#include "src/Celula.cpp"
#include "src/Entitate.cpp"
#include "src/EntitateJoc.cpp"
#include "src/Exceptii.cpp"
#include "src/InamicNormal.cpp"
#include "src/InamicPericulos.cpp"
#include "src/InamicRapid.cpp"
#include "src/Joc.cpp"
#include "src/Jucator.cpp"
#include "src/Matrice.cpp"
#include "src/MeniuRestart.cpp"
#include "src/MiniBoss.cpp"
#include "src/Pozitie.cpp"
#include "src/Timer.cpp"
#include "src/Utils.cpp"

int main() {
  try {
    std::cout << "Inamicii se spawneaza - elimina-i efectuand un slice "
              << std::endl;
    std::cout << "Apasa o litera sau cifra pentru a te teleporta + slice"
              << std::endl;
    std::cout << "! = inamic periculos (nu il poti omori)" << std::endl;
    std::cout << "B = MiniBoss (necesita mai multe lovituri)" << std::endl;
    std::cout << "@ = jucatorul tau" << std::endl;
    std::cout << "La scor multiplu de 100 apare un MiniBoss" << std::endl;

    Matrice mat(4, 10);
    std::cout << std::endl << mat;

    Joc joc;
    std::cout << joc << std::endl;
    std::cout << "Entitati active total: " << EntitateJoc::getContor()
              << std::endl;

    std::cout << std::endl
              << "Apasa orice tasta pentru a incepe jocul" << std::endl;
    citesteTasta();

    MeniuRestart meniu;
    bool joaca = true;
    while (joaca) {
      Joc jocNou;
      joaca = meniu.gestioneazaJoc(jocNou);
    }

    std::cout << meniu << std::endl;
  } catch (const ExceptieJoc &e) {
    std::cerr << "Eroare joc: " << e.what() << std::endl;
    return 1;
  } catch (const std::exception &e) {
    std::cerr << "Eroare: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
