#pragma once
#include "EntitateJoc.h"

// inamic care se misca diagonal in loc de orizontal/vertical
class InamicRapid : public EntitateJoc {
    char simbol;
public:
    InamicRapid(int id, const Pozitie& poz, char simbol);
    InamicRapid(const InamicRapid& e);
    InamicRapid& operator=(InamicRapid e);

    bool aplicaLovitura() override;
    void muta(int nrLinii, int nrColoane) override; // miscare diagonala
    [[nodiscard]] bool esteActiv() const override;
    [[nodiscard]] bool esteLA(const Pozitie& p) const override;
    [[nodiscard]] char getSimbol() const override;
    [[nodiscard]] std::unique_ptr<EntitateJoc> clone() const override;

    friend void swap(InamicRapid& a, InamicRapid& b) noexcept;
protected:
    void afiseazaIntern(std::ostream& os) const override;
};
