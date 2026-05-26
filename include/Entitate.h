#pragma once
#include "Pozitie.h"
#include <ostream>

// clasa cu alocare dinamica (char*) - demonstreaza cc/op=/dtor + copy-and-swap
class Entitate {
    int id;
    Pozitie poz;
    bool inViata;
    char* nume;
public:
    Entitate(int id, const Pozitie& poz, const char* nume);
    Entitate(const Entitate& e);
    Entitate& operator=(Entitate e); // copy-and-swap
    ~Entitate();

    [[nodiscard]] const Pozitie& getPoz() const;
    [[nodiscard]] bool esteInViata() const;
    void setPoz(const Pozitie& p);
    void setInViata(bool v);

    friend void swap(Entitate& a, Entitate& b) noexcept;
    friend std::ostream& operator<<(std::ostream& os, const Entitate& e);
};
