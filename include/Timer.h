#pragma once
#include <chrono>
#include <ostream>

class Timer {
    std::chrono::steady_clock::time_point ultimSpawn;
    std::chrono::steady_clock::time_point ultimaMiscare;
    double intervalSpawn;
    double intervalMiscare;

    [[nodiscard]] static double secDe(
        const std::chrono::steady_clock::time_point& t);
public:
    explicit Timer(double spawn = 5.0, double miscare = 3.0);

    [[nodiscard]] bool trebuieSpawn() const;
    [[nodiscard]] bool trebuieMiscare() const;
    void resetSpawn();
    void resetMiscare();

    friend std::ostream& operator<<(std::ostream& os, const Timer& t);
};
