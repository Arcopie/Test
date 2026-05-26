#pragma once
#include "EntitateJoc.h"
#include <chrono>

// inamic temporar cu simbol '!' - nu poate fi omorat, dispare dupa un timp
class InamicPericulos : public EntitateJoc {
    std::chrono::steady_clock::time_point momentSpawn;
    double durataViata;

    [[nodiscard]] bool aExpirat() const;
public:
    InamicPericulos(int id, const Pozitie& poz, double durata = 2.0);
    InamicPericulos(const InamicPericulos& e);
    InamicPericulos& operator=(InamicPericulos e);

    bool aplicaLovitura() override; // returneaza false - nu poate fi omorat
    void muta(int nrLinii, int nrColoane) override; // stationar
    [[nodiscard]] bool esteActiv() const override;
    [[nodiscard]] bool esteLA(const Pozitie& p) const override;
    [[nodiscard]] char getSimbol() const override;
    [[nodiscard]] std::unique_ptr<EntitateJoc> clone() const override;

    friend void swap(InamicPericulos& a, InamicPericulos& b) noexcept;
protected:
    void afiseazaIntern(std::ostream& os) const override;
};
