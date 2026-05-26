#include "Exceptii.h"

ExceptieJoc::ExceptieJoc(const std::string& mesaj) : mesaj(mesaj) {}

const char* ExceptieJoc::what() const noexcept { return mesaj.c_str(); }

ExceptiePozitieInvalida::ExceptiePozitieInvalida(int lin, int col)
    : ExceptieJoc("Pozitie invalida: (" + std::to_string(lin) +
                  ", " + std::to_string(col) + ")") {}

ExceptieConfigurareInvalida::ExceptieConfigurareInvalida(const std::string& detaliu)
    : ExceptieJoc("Configurare invalida: " + detaliu) {}

ExceptieEntitateInvalida::ExceptieEntitateInvalida(const std::string& detaliu)
    : ExceptieJoc("Entitate invalida: " + detaliu) {}
