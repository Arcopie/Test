#pragma once
#include <exception>
#include <string>

// baza ierarhiei de exceptii proprii
class ExceptieJoc : public std::exception {
    std::string mesaj;
public:
    explicit ExceptieJoc(const std::string& mesaj);
    [[nodiscard]] const char* what() const noexcept override;
};

// pozitie in afara limitelor matricei
class ExceptiePozitieInvalida : public ExceptieJoc {
public:
    ExceptiePozitieInvalida(int lin, int col);
};

// parametri de configurare invalizi (ex: dimensiuni negative)
class ExceptieConfigurareInvalida : public ExceptieJoc {
public:
    explicit ExceptieConfigurareInvalida(const std::string& detaliu);
};

// entitate creata cu parametri invalizi
class ExceptieEntitateInvalida : public ExceptieJoc {
public:
    explicit ExceptieEntitateInvalida(const std::string& detaliu);
};
