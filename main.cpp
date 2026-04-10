#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>
#include <cstring>
#include <cstdlib>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#endif

using namespace std;

// functii cross-platform pentru input non-blocking si sleep
#ifdef _WIN32

static void sleepMs(int ms) { Sleep(ms); }
static void clearScreen() { system("cls"); }
static bool tastaDisponibila() { return _kbhit(); }
static int citesteTasta() {
    int t = _getch();
    if (t == 0 || t == 224) t = _getch();
    return t;
}

#else

static void sleepMs(int ms) { usleep(ms * 1000); }
static void clearScreen() { system("clear"); }

static bool tastaDisponibila() {
    struct timeval tv = {0, 0};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv) > 0;
}

static int citesteTasta() {
    struct termios oldt, newt;
    tcgetattr(0, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &newt);
    int ch = getchar();
    tcsetattr(0, TCSANOW, &oldt);
    return ch;
}

#endif

// --- Clasa Pozitie ---

class Pozitie {
    int lin, col;
public:
    explicit Pozitie(int lin = 0, int col = 0) : lin(lin), col(col) {}
    [[nodiscard]] int getLin() const { return lin; }
    [[nodiscard]] int getCol() const { return col; }

    bool operator==(const Pozitie& p) const {
        return lin == p.lin && col == p.col;
    }

    friend ostream& operator<<(ostream& os, const Pozitie& p) {
        os << "(" << p.lin << ", " << p.col << ")";
        return os;
    }
};

// --- Clasa Celula (compune Pozitie) ---

class Celula {
    Pozitie poz;
    char simbol;
public:
    Celula(const Pozitie& poz, char simbol) : poz(poz), simbol(simbol) {}
    [[nodiscard]] char getSimbol() const { return simbol; }
    // getPozitie se poate folosi in extinderi viitoare
    // [[nodiscard]] const Pozitie& getPozitie() const { return poz; }


    friend ostream& operator<<(ostream& os, const Celula& c) {
        os << c.simbol;
        return os;
    }
};

// --- Clasa Entitate (Big Three: constructor copiere, operator=, destructor) ---
// Folosim un char* alocat dinamic pentru nume ca sa justificam Big Three

class Entitate {
    int id;
    Pozitie poz;
    bool inViata;
    char* nume; // alocat dinamic -> avem nevoie de Big Three
public:
    Entitate(int id, const Pozitie& poz, const char* nume)
        : id(id), poz(poz), inViata(true) {
        this->nume = new char[strlen(nume) + 1];
        strcpy(this->nume, nume);
    }

    // constructor de copiere
    Entitate(const Entitate& e) : id(e.id), poz(e.poz), inViata(e.inViata) {
        nume = new char[strlen(e.nume) + 1];
        strcpy(nume, e.nume);
    }

    // operator= de copiere
    Entitate& operator=(const Entitate& e) {
        if (this != &e) {
            delete[] nume;
            id = e.id;
            poz = e.poz;
            inViata = e.inViata;
            nume = new char[strlen(e.nume) + 1];
            strcpy(nume, e.nume);
        }
        return *this;
    }

    // destructor
    ~Entitate() {
        delete[] nume;
    }

    [[nodiscard]] const Pozitie& getPoz() const { return poz; }
    [[nodiscard]] bool esteInViata() const { return inViata; }
    void setPoz(const Pozitie& p) { poz = p; }
    void setInViata(bool v) { inViata = v; }

    friend ostream& operator<<(ostream& os, const Entitate& e) {
        os << e.nume << " #" << e.id << " la " << e.poz;
        if (!e.inViata) os << " [mort]";
        return os;
    }
};

// --- Clasa Inamic (compune Entitate) ---

class Inamic {
    Entitate entitate;
    char simbol;
public:
    Inamic(int id, const Pozitie& poz, char simbol)
        : entitate(id, poz, "Inamic"), simbol(simbol) {}

    // muta inamicul random intr-o directie
    void muta(int nrLinii, int nrColoane) {
        if (!entitate.esteInViata()) return;
        const int directii[][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};
        int d;
        d = rand() % 4;
        int linNou = entitate.getPoz().getLin() + directii[d][0];
        int colNou = entitate.getPoz().getCol() + directii[d][1];
        // verific limitele
        if (linNou < 0) linNou = 0;
        if (linNou >= nrLinii) linNou = nrLinii - 1;
        if (colNou < 0) colNou = 0;
        if (colNou >= nrColoane) colNou = nrColoane - 1;
        entitate.setPoz(Pozitie(linNou, colNou));
    }

    // verifica daca inamicul e viu si la pozitia data

    [[nodiscard]] bool esteLA(const Pozitie& p) const {
        return entitate.esteInViata() && entitate.getPoz() == p;
    }

    // omoara inamicul
    void omoara() {
        entitate.setInViata(false);
    }

    [[nodiscard]] bool esteViu() const { return entitate.esteInViata(); }
    [[nodiscard]] const Pozitie& getPoz() const { return entitate.getPoz(); }
    [[nodiscard]] char getSimbol() const { return simbol; }

    friend ostream& operator<<(ostream& os, const Inamic& e) {
        os << "[" << e.simbol << "] " << e.entitate;
        return os;
    }
};

// --- Clasa Jucator (compune Entitate) ---

class Jucator {
    Entitate entitate;
    int scor;
    int nrSliceuri;
public:
    explicit Jucator(const Pozitie& start)
        : entitate(0, start, "Jucator"), scor(0), nrSliceuri(0) {}

    // teleporteaza jucatorul la o pozitie data
    void teleport(const Pozitie& p) {
        entitate.setPoz(p);
    }

    // face slice pe inamicii de pe pozitia curenta
    int slice(vector<Inamic>& inamici) {
        int omorati = 0;
        for (auto & i : inamici) {
            if (i.esteLA(entitate.getPoz())) {
                i.omoara();
                scor += 10;
                nrSliceuri++;
                omorati++;
            }
        }
        return omorati;
    }

    // verifica daca un inamic e pe aceeasi pozitie cu jucatorul
    [[nodiscard]] bool atingeInamic(const vector<Inamic>& inamici) const {
        for (const auto & i : inamici)
            if (i.esteLA(entitate.getPoz()))
                return true;
        return false;
    }

    [[nodiscard]] int getScor() const { return scor; }
    [[nodiscard]] int getSliceuri() const { return nrSliceuri; }
    [[nodiscard]] const Pozitie& getPoz() const { return entitate.getPoz(); }

    friend ostream& operator<<(ostream& os, const Jucator& j) {
        os << j.entitate << " | scor: " << j.scor << " | sliceuri: " << j.nrSliceuri;
        return os;
    }
};

// --- Clasa Matrice (compune vector<vector<Celula>>) ---

class Matrice {
    int linii, coloane;
    vector<vector<Celula>> grila;

    void construiesteGrila() {
        const char taste[] = "qwertyuiopasdfghjkl;zxcvbnm,./";
        for (int i = 0; i < linii; i++) {
            grila.emplace_back();
            for (int j = 0; j < coloane; j++) {
                int idx = i * coloane + j;
                char ch = (idx < 30) ? taste[idx] : '.';
                grila[i].emplace_back(Pozitie(i, j), ch);
            }
        }
    }
public:
    explicit Matrice(int linii = 3, int coloane = 10) : linii(linii), coloane(coloane) {
        construiesteGrila();
    }

    [[nodiscard]] int getLinii() const { return linii; }

    [[nodiscard]] int getColoane() const { return coloane; }

    // afiseaza tabla cu jucatorul si inamicii
    void afiseaza(const Jucator& jucator, const vector<Inamic>& inamici) const {
        // linia de sus
        cout << "  +";
        for (int j = 0; j < coloane; j++) cout << "---";
        cout << "-+" << endl;

        for (int i = 0; i < linii; i++) {
            cout << "  | ";
            for (int j = 0; j < coloane; j++) {
                Pozitie p(i, j);

                // verificam daca e jucatorul
                if (jucator.getPoz() == p) {
                    cout << "@ ";
                    continue;
                }

                // verificam daca e un inamic viu
                bool gasit = false;
                for (const auto & k : inamici) {
                    if (k.esteViu() && k.esteLA(p)) {
                        cout << k.getSimbol() << " ";
                        gasit = true;
                        break;
                    }
                }
                if (gasit) continue;

                // altfel afisam litera din grila
                cout << grila[i][j].getSimbol() << " ";
            }
            cout << " |" << endl;
        }

        // linia de jos
        cout << "  +";
        for (int j = 0; j < coloane; j++) cout << "---";
        cout << "-+" << endl;
    }

    // returneaza simbolul de la o pozitie
    [[nodiscard]] char simbolLa(const Pozitie& p) const {
        if (p.getLin() < 0 || p.getLin() >= linii) return '\0';
        if (p.getCol() < 0 || p.getCol() >= coloane) return '\0';
        return grila[p.getLin()][p.getCol()].getSimbol();
    }

    // genereaza o pozitie random pe matrice
    [[nodiscard]] Pozitie pozitieRandom() const {
        return Pozitie(rand() % linii, rand() % coloane);
    }

    // cauta pozitia unei litere pe matrice (pentru teleportare)
    bool gasestePozitie(char ch, Pozitie& rezultat) const {
        for (int i = 0; i < linii; i++)
            for (int j = 0; j < coloane; j++)
                if (grila[i][j].getSimbol() == ch) {
                    rezultat = Pozitie(i, j);
                    return true;
                }
        return false;
    }

    friend ostream& operator<<(ostream& os, const Matrice& m) {
        os << "Matrice " << m.linii << "x" << m.coloane << ":" << endl;
        for (int i = 0; i < m.linii; i++) {
            for (int j = 0; j < m.coloane; j++)
                os << m.grila[i][j].getSimbol() << " ";
            os << endl;
        }
        return os;
    }
};

// --- Clasa Timer ---

class Timer {
    chrono::steady_clock::time_point ultimSpawn;
    chrono::steady_clock::time_point ultimaMiscare;
    double intervalSpawn;   // secunde
    double intervalMiscare; // secunde

    [[nodiscard]] static double secDe(const chrono::steady_clock::time_point& t) {
        return chrono::duration<double>(chrono::steady_clock::now() - t).count();
    }
public:
    explicit Timer(double spawn = 5.0, double miscare = 3.0)
        : ultimSpawn(chrono::steady_clock::now()),
          ultimaMiscare(chrono::steady_clock::now()),
          intervalSpawn(spawn), intervalMiscare(miscare) {}

    [[nodiscard]] bool trebuieSpawn() const { return secDe(ultimSpawn) >= intervalSpawn; }
    [[nodiscard]] bool trebuieMiscare() const { return secDe(ultimaMiscare) >= intervalMiscare; }
    void resetSpawn() { ultimSpawn = chrono::steady_clock::now(); }
    void resetMiscare() { ultimaMiscare = chrono::steady_clock::now(); }

    friend ostream& operator<<(ostream& os, const Timer& t) {
        os << "Timer[spawn la " << t.intervalSpawn << "s, miscare la " << t.intervalMiscare << "s]";
        return os;
    }
};

// --- Clasa Joc (compune Matrice, Jucator, vector<Inamic>, Timer) ---

class Joc {
    Matrice matrice;
    Jucator jucator;
    vector<Inamic> inamici;
    Timer timer;
    bool ruleaza;
    int nextId;

    void curataMortii() {
        // stergem inamicii morti din vector
        vector<Inamic> vii;
        for (const auto & i : inamici)
            if (i.esteViu())
                vii.push_back(i);
        inamici = vii;
    }
public:
    Joc() : matrice(3, 10), jucator(Pozitie(1, 5)), timer(5.0, 3.0),
            ruleaza(false), nextId(1) {}

    // adauga un inamic nou
    void adaugaInamic() {
        if ((int)inamici.size() >= 8) return; // max 8 inamici pe ecran
        const char simboluri[] = "EFGHIJKL";
        Pozitie p = matrice.pozitieRandom();
        // sa nu apara fix pe jucator
        int incercari = 0;
        while (p == jucator.getPoz() && incercari < 20) {
            p = matrice.pozitieRandom();
            incercari++;
        }
        char sim = simboluri[(nextId - 1) % 8];
        inamici.emplace_back(nextId, p, sim);
        nextId++;
    }

    // proceseaza o tasta - teleportare pe litera apasata
    bool proceseazaTasta(int tasta) {
        if (tasta == 27) { // ESC = iesire
            ruleaza = false;
            return false;
        }
        // convertim la lowercase
        char ch = static_cast<char>(tasta);
        if (ch >= 'A' && ch <= 'Z') ch = ch - 'A' + 'a';

        Pozitie dest(0, 0);
        if (matrice.gasestePozitie(ch, dest)) {
            jucator.teleport(dest);
            if (jucator.slice(inamici) > 0)
                curataMortii();
            return true;
        }
        return false;
    }

    // muta toti inamicii
    void mutaInamici() {
        for (auto & i : inamici)

            i.muta(matrice.getLinii(), matrice.getColoane());
    }

    // afiseaza ecranul
    void afiseazaEcran() {
        clearScreen();
        cout << "=== SLICE GAME ===" << endl;
        cout << "Scor: " << jucator.getScor()
             << " | Sliceuri: " << jucator.getSliceuri()
             << " | Inamici: " << inamici.size() << endl;
        cout << "Apasa o litera de pe matrice = teleportare + slice" << endl;
        cout << "ESC = iesire" << endl;
        cout << endl;
        matrice.afiseaza(jucator, inamici);
        cout << endl;
    }

    // bucla principala
    void ruleazaJocul() {
        srand((unsigned)time(nullptr));
        ruleaza = true;
        adaugaInamic();
        afiseazaEcran();

        while (ruleaza) {
            bool trebuieRedesnat = false;

            // verificam timerele
            if (timer.trebuieSpawn()) {
                adaugaInamic();
                timer.resetSpawn();
                trebuieRedesnat = true;
            }
            if (timer.trebuieMiscare()) {
                mutaInamici();
                timer.resetMiscare();
                trebuieRedesnat = true;

                // game over daca un inamic s-a mutat pe jucator
                if (jucator.atingeInamic(inamici)) {
                    afiseazaEcran();
                    cout << "GAME OVER! Un inamic te-a atins!" << endl;
                    cout << "Scor final: " << jucator.getScor() << endl;
                    sleepMs(2000);
                    ruleaza = false;
                    break;
                }
            }

            // input non-blocking
            if (tastaDisponibila()) {
                int tasta = citesteTasta();
                if (proceseazaTasta(tasta))
                    trebuieRedesnat = true;
            }

            // redesnam doar cand s-a intamplat ceva
            if (trebuieRedesnat)
                afiseazaEcran();

            sleepMs(100);
        }
    }

    friend ostream& operator<<(ostream& os, const Joc& j) {
        os << "Joc: " << j.jucator << " | " << j.timer;
        return os;
    }
};

// --- MAIN ---

int main() {

    // demo clase - apelam toate functiile publice

    cout << "--- Demo Position ---" << endl;
    Pozitie p1(1, 3), p2(2, 7);
    cout << "p1 = " << p1 << ", p2 = " << p2 << endl;
    cout << "p1 == p2? " << (p1 == p2 ? "da" : "nu") << endl;
    cout << endl;

    cout << "--- Demo Celula ---" << endl;
    Celula c1(Pozitie(0, 0), 'q');
    Celula c2(Pozitie(2, 6), 'm');
    cout << "c1 = " << c1 << ", c2 = " << c2 << endl;
    cout << endl;

    cout << "--- Demo Entitate (Big Three) ---" << endl;
    Entitate e1(1, Pozitie(0, 0), "Alfa");
    const Entitate& e2(e1); // copiere
    Entitate e3(2, Pozitie(2, 9), "Beta");
    e3 = e1; // operator=
    cout << "e1: " << e1 << endl;
    cout << "e2 (copie): " << e2 << endl;
    cout << "e3 (dupa e3=e1): " << e3 << endl;
    cout << endl;

    cout << "--- Demo Inamic ---" << endl;
    Inamic in1(10, Pozitie(0, 5), 'E');
    cout << "inainte de mutare: " << in1 << endl;
    in1.muta(3, 10);
    cout << "dupa mutare: " << in1 << endl;
    cout << "e la (0,5)? " << (in1.esteLA(Pozitie(0, 5)) ? "da" : "nu") << endl;
    cout << endl;

    cout << "--- Demo Jucator ---" << endl;
    vector<Inamic> testInamici;
    testInamici.emplace_back(20, Pozitie(1, 4), 'F');
    Jucator jTest(Pozitie(1, 5));
    cout << "initial: " << jTest << endl;
    jTest.teleport(Pozitie(1, 4)); // teleportare pe (1,4)
    cout << "dupa teleportare: " << jTest << endl;
    int k = jTest.slice(testInamici);
    cout << "slice -> " << k << " omorati" << endl;
    cout << "final: " << jTest << endl;
    cout << endl;

    cout << "--- Demo Matrice ---" << endl;
    Matrice mat(3, 10);
    cout << mat;
    cout << "simbol la (0,0): " << mat.simbolLa(Pozitie(0, 0)) << endl;
    cout << "simbol la (2,6): " << mat.simbolLa(Pozitie(2, 6)) << endl;
    cout << endl;

    cout << "--- Demo Matrice::gasestePozitie ---" << endl;
    Pozitie gasit(0,0);
    if (mat.gasestePozitie('g', gasit))
        cout << "litera 'g' e la " << gasit << endl;
    cout << endl;

    cout << "--- Demo Timer ---" << endl;
    Timer t(5.0, 3.0);
    cout << t << endl;
    cout << endl;

    cout << "--- Demo Joc ---" << endl;
    Joc joc;
    cout << joc << endl;
    cout << endl;

    cout << "Apasa orice tasta pentru a incepe jocul..." << endl;
    citesteTasta();

    joc.ruleazaJocul();

    return 0;
}
