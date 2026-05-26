#include "Entitate.h"
#include "Exceptii.h"
#include <cstring>
#include <utility>

Entitate::Entitate(int id, const Pozitie& poz, const char* nume)
    : id(id), poz(poz), inViata(true) {
    if (id < 0) throw ExceptieEntitateInvalida("id negativ");
    this->nume = new char[strlen(nume) + 1];
    strcpy(this->nume, nume);
}

Entitate::Entitate(const Entitate& e)
    : id(e.id), poz(e.poz), inViata(e.inViata) {
    nume = new char[strlen(e.nume) + 1];
    strcpy(nume, e.nume);
}

// copy-and-swap: parametrul e creat prin copiere, apoi swap
void swap(Entitate& a, Entitate& b) noexcept {
    using std::swap;
    swap(a.id, b.id);
    swap(a.poz, b.poz);
    swap(a.inViata, b.inViata);
    swap(a.nume, b.nume);
}

Entitate& Entitate::operator=(Entitate e) {
    swap(*this, e);
    return *this;
}

Entitate::~Entitate() { delete[] nume; }

const Pozitie& Entitate::getPoz() const { return poz; }
bool Entitate::esteInViata() const { return inViata; }
void Entitate::setPoz(const Pozitie& p) { poz = p; }
void Entitate::setInViata(bool v) { inViata = v; }

std::ostream& operator<<(std::ostream& os, const Entitate& e) {
    os << e.nume << " #" << e.id << " la " << e.poz;
    if (!e.inViata) os << " [mort]";
    return os;
}
