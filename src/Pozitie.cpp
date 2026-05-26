#include "Pozitie.h"

Pozitie::Pozitie(int lin, int col) : lin(lin), col(col) {}

int Pozitie::getLin() const { return lin; }
int Pozitie::getCol() const { return col; }

bool Pozitie::operator==(const Pozitie& p) const {
    return lin == p.lin && col == p.col;
}

std::ostream& operator<<(std::ostream& os, const Pozitie& p) {
    os << "(" << p.lin << ", " << p.col << ")";
    return os;
}
