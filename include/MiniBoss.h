#pragma once
#include "EntitateJoc.h"
#include "Exceptii.h"

// boss care necesita mai multe lovituri pentru a fi omorat
class MiniBoss : public EntitateJoc {
    int vietiRamase;
    int vietiMaxime;
public:
    // arunca ExceptieEntitateInvalida daca vietiMaxime < 1
    MiniBoss(int id, const Pozitie& poz, int vietiMaxime);
    MiniBoss(const MiniBoss& e);
    MiniBoss& operator=(MiniBoss e);

    bool aplicaLovitura() override; // decrementeaza vietiRamase
    void muta(int nrLinii, int nrColoane) override;
    [[nodiscard]] bool esteActiv() const override;
    [[nodiscard]] bool esteLA(const Pozitie& p) const override;
    [[nodiscard]] char getSimbol() const override;
    [[nodiscard]] std::unique_ptr<EntitateJoc> clone() const override;

    [[nodiscard]] int getVietiRamase() const;
    [[nodiscard]] int getVietiMaxime() const;

    friend void swap(MiniBoss& a, MiniBoss& b) noexcept;
protected:
    void afiseazaIntern(std::ostream& os) const override;
};
