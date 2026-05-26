#pragma once
#include "Entitate.h"
#include "Pozitie.h"
#include <memory>
#include <ostream>

// clasa de baza abstracta pentru entitatile din joc
class EntitateJoc {
    static int contor; // nr total de entitati active
protected:
    Entitate entitate;

    EntitateJoc(int id, const Pozitie& poz, const char* tip);
    EntitateJoc(const EntitateJoc& e); // protejat - folosit de derivate
    friend void swap(EntitateJoc& a, EntitateJoc& b) noexcept;

public:
    virtual ~EntitateJoc();
    EntitateJoc& operator=(const EntitateJoc&) = delete;

    // functie virtuala pura specifica temei - incearca sa omoare entitatea
    virtual bool aplicaLovitura() = 0;
    virtual void muta(int nrLinii, int nrColoane) = 0;
    [[nodiscard]] virtual bool esteActiv() const = 0;
    [[nodiscard]] virtual bool esteLA(const Pozitie& p) const = 0;
    [[nodiscard]] virtual char getSimbol() const = 0;
    // constructor virtual (clone)
    [[nodiscard]] virtual std::unique_ptr<EntitateJoc> clone() const = 0;

    [[nodiscard]] const Pozitie& getPoz() const;
    static int getContor();

    // NVI: operator<< apeleaza afiseazaIntern virtual
    friend std::ostream& operator<<(std::ostream& os, const EntitateJoc& e);

protected:
    virtual void afiseazaIntern(std::ostream& os) const = 0;
};

void swap(EntitateJoc& a, EntitateJoc& b) noexcept;
