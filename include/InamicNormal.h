#pragma once
#include "EntitateJoc.h"

class InamicNormal : public EntitateJoc {
    char simbol;
public:
    InamicNormal(int id, const Pozitie& poz, char simbol);
    InamicNormal(const InamicNormal& e);
    InamicNormal& operator=(InamicNormal e); // copy-and-swap

    bool aplicaLovitura() override;
    void muta(int nrLinii, int nrColoane) override;
    [[nodiscard]] bool esteActiv() const override;
    [[nodiscard]] bool esteLA(const Pozitie& p) const override;
    [[nodiscard]] char getSimbol() const override;
    [[nodiscard]] std::unique_ptr<EntitateJoc> clone() const override;

    friend void swap(InamicNormal& a, InamicNormal& b) noexcept;
protected:
    void afiseazaIntern(std::ostream& os) const override;
};
