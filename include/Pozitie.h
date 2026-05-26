#pragma once
#include <ostream>

class Pozitie {
    int lin, col;
public:
    explicit Pozitie(int lin = 0, int col = 0);
    [[nodiscard]] int getLin() const;
    [[nodiscard]] int getCol() const;
    bool operator==(const Pozitie& p) const;
    friend std::ostream& operator<<(std::ostream& os, const Pozitie& p);
};
