#include "Utils.h"
#include "rlutil.h"

void sleepMs(int ms) { rlutil::msleep(ms); }
void clearScreen() { rlutil::cls(); }

bool tastaDisponibila() {
    if (runs_on_ci()) return true;
    return kbhit();
}

int citesteTasta() {
    if (runs_on_ci()) return getchar();
    return getch();
}
