#include "Timer.h"

Timer::Timer(double spawn, double miscare)
    : ultimSpawn(std::chrono::steady_clock::now()),
      ultimaMiscare(std::chrono::steady_clock::now()),
      intervalSpawn(spawn), intervalMiscare(miscare) {}

double Timer::secDe(const std::chrono::steady_clock::time_point& t) {
    return std::chrono::duration<double>(
        std::chrono::steady_clock::now() - t).count();
}

bool Timer::trebuieSpawn() const { return secDe(ultimSpawn) >= intervalSpawn; }
bool Timer::trebuieMiscare() const { return secDe(ultimaMiscare) >= intervalMiscare; }
void Timer::resetSpawn() { ultimSpawn = std::chrono::steady_clock::now(); }
void Timer::resetMiscare() { ultimaMiscare = std::chrono::steady_clock::now(); }

std::ostream& operator<<(std::ostream& os, const Timer& t) {
    os << "Timer[spawn la " << t.intervalSpawn
       << "s, miscare la " << t.intervalMiscare << "s]";
    return os;
}
