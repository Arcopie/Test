#pragma once
#include "Pozitie.h"
#include <ostream>

class Celula {
    Pozitie poz;
    char simbol;
public:
    Celula(const Pozitie& poz, char simbol);
    [[nodiscard]] char getSimbol() const;
    friend std::ostream& operator<<(std::ostream& os, const Celula& c);
};
