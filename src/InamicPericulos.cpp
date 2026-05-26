#include "../include/InamicPericulos.h"
#include <utility>

InamicPericulos::InamicPericulos(int id, const Pozitie& poz, double durata)
    : EntitateJoc(id, poz, "InamicPericulos"),
      momentSpawn(std::chrono::steady_clock::now()),
      durataViata(durata) {}

InamicPericulos::InamicPericulos(const InamicPericulos& e)
    : EntitateJoc(e), momentSpawn(e.momentSpawn), durataViata(e.durataViata) {}

void swap(InamicPericulos& a, InamicPericulos& b) noexcept {
    using std::swap;
    swap(static_cast<EntitateJoc&>(a), static_cast<EntitateJoc&>(b));
    swap(a.momentSpawn, b.momentSpawn);
    swap(a.durataViata, b.durataViata);
}

InamicPericulos& InamicPericulos::operator=(InamicPericulos e) {
    swap(*this, e);
    return *this;
}

bool InamicPericulos::aExpirat() const {
    return std::chrono::duration<double>(
        std::chrono::steady_clock::now() - momentSpawn).count() >= durataViata;
}

// nu poate fi omorat prin slice
bool InamicPericulos::aplicaLovitura() { return false; }

// stationar - nu se misca
void InamicPericulos::muta(int, int) {}

bool InamicPericulos::esteActiv() const { return !aExpirat(); }

bool InamicPericulos::esteLA(const Pozitie& p) const {
    return !aExpirat() && entitate.getPoz() == p;
}

char InamicPericulos::getSimbol() const { return '!'; }

std::unique_ptr<EntitateJoc> InamicPericulos::clone() const {
    return std::make_unique<InamicPericulos>(*this);
}

void InamicPericulos::afiseazaIntern(std::ostream& os) const {
    os << "[!] " << entitate;
    if (aExpirat()) os << " [expirat]";
}
