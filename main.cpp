
#include <iostream>
#include <vector>

#include <chrono>

#include <cstring>
#include <ctime>


//  Clasa Tasta - citire de la tastatura si utilitati terminal

class Tasta {
public:


  // curata ecranul folosind coduri ANSI
  static void clearScreen() {
    std::cout << "\033[2J\033[H" << std::flush;
  }

  // citeste un caracter de la tastatura (blocant - asteapta input)
  // returneaza codul caracterului, sau 0 daca input-ul s-a terminat (EOF)
  static int citesteTasta() {
    char ch;
    if (std::cin >> ch) {
      return static_cast<int>(ch);
    }
    return 0;
  }

  friend std::ostream &operator<<(std::ostream &os, const Tasta &) {
    os << "Tasta[cin]";
    return os;
  }
};


//  Clasa Pozitie 

class Pozitie {
  int lin, col;

public:
  explicit Pozitie(int lin = 0, int col = 0) : lin(lin), col(col) {}
  [[nodiscard]] int getLin() const { return lin; }
  [[nodiscard]] int getCol() const { return col; }

  bool operator==(const Pozitie &p) const {
    return lin == p.lin && col == p.col;
  }

  friend std::ostream &operator<<(std::ostream &os, const Pozitie &p) {
    os << "(" << p.lin << ", " << p.col << ")";
    return os;
  }
};

//   Celula

class Celula {
  Pozitie poz;
  char simbol;

public:
  void set_poz(const Pozitie &poz) {
    this->poz = poz;
  }

  void set_simbol(const char simbol) {
    this->simbol = simbol;
  }

  Celula(const Pozitie &poz, char simbol) : poz(poz), simbol(simbol) {}
  [[nodiscard]] char getSimbol() const { return simbol; }


  friend std::ostream &operator<<(std::ostream &os, const Celula &c) {
    os << c.simbol;
    return os;
  }
};

//  Clasa Entitate (

class Entitate {
  int id;
  Pozitie poz;
  bool inViata;
  char *nume; // alocare dinamica
public:
  [[nodiscard]] int id1() const {
    return id;
  }

  [[nodiscard]] Pozitie poz1() const {
    return poz;
  }

  [[nodiscard]] bool in_viata() const {
    return inViata;
  }

  [[nodiscard]] char * nume1() const {
    return nume;
  }

  Entitate(int id, const Pozitie &poz, const char *nume)
      : id(id), poz(poz), inViata(true) {
    this->nume = new char[strlen(nume) + 1];
    strcpy(this->nume, nume);
  }

  // constructor de copiere
  Entitate(const Entitate &e) : id(e.id), poz(e.poz), inViata(e.inViata) {
    nume = new char[strlen(e.nume) + 1];
    strcpy(nume, e.nume);
  }

  // operator=
  Entitate &operator=(const Entitate &e) {
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


  ~Entitate() { delete[] nume; }

  [[nodiscard]] const Pozitie &getPoz() const { return poz; }
  [[nodiscard]] bool esteInViata() const { return inViata; }
  void setPoz(const Pozitie &p) { poz = p; }
  void setInViata(bool v) { inViata = v; }

  friend std::ostream &operator<<(std::ostream &os, const Entitate &e) {
    os << e.nume << " #" << e.id << " la " << e.poz;
    if (!e.inViata)
      os << " [mort]";
    return os;
  }
};

//  Class Inamic

class Inamic {
  Entitate entitate;
  char simbol;

public:
  Inamic(int id, const Pozitie &poz, char simbol)
      : entitate(id, poz, "Inamic"), simbol(simbol) {}

  // muta inamicul random intr-o directie
  void muta(int nrLinii, int nrColoane) {
    if (!entitate.esteInViata())
      return;
    constexpr int directii[][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    int d = rand() % 4;
    int linNou = entitate.getPoz().getLin() + directii[d][0];
    int colNou = entitate.getPoz().getCol() + directii[d][1];
    // verific limitele
    if (linNou < 0)
      linNou = 0;
    if (linNou >= nrLinii)
      linNou = nrLinii - 1;
    if (colNou < 0)
      colNou = 0;
    if (colNou >= nrColoane)
      colNou = nrColoane - 1;
    entitate.setPoz(Pozitie(linNou, colNou));
  }

  // verific daca inamicul e viu si la pozitia data

  [[nodiscard]] bool esteLA(const Pozitie &p) const {
    return entitate.esteInViata() && entitate.getPoz() == p;
  }

  // omoara inamicul
  void omoara() { entitate.setInViata(false); }

  [[nodiscard]] bool esteViu() const { return entitate.esteInViata(); }
  [[nodiscard]] const Pozitie &getPoz() const { return entitate.getPoz(); }
  [[nodiscard]] char getSimbol() const { return simbol; }

  friend std::ostream &operator<<(std::ostream &os, const Inamic &e) {
    os << "[" << e.simbol << "] " << e.entitate;
    return os;
  }
};

//  Clasa Jucator

class Jucator {
  Entitate entitate;
  int scor;
  int nrSliceuri;

public:
  explicit Jucator(const Pozitie &start)
      : entitate(0, start, "Jucator"), scor(0), nrSliceuri(0) {}

  // teleporteaza jucatorul
  void teleport(const Pozitie &p) { entitate.setPoz(p); }

  // face slice pe inamicii de pe traseul intre pozitia veche si cea noua doar daca randul sau coloana ramane aceeasi
  int slice(std::vector<Inamic> &inamici, const Pozitie &pozVeche) {
    Pozitie pozNoua = entitate.getPoz();
    int omorati = 0;

    if (pozVeche.getLin() == pozNoua.getLin()) {
      // acelasi rand -> omoara inamicii intre coloanele veche si noua
      int lin = pozNoua.getLin();
      int c1 = pozVeche.getCol();
      int c2 = pozNoua.getCol();
      if (c1 > c2) {
        int tmp = c1;
        c1 = c2;
        c2 = tmp;
      }
      for (auto & i : inamici) {
        if (i.esteViu() && i.getPoz().getLin() == lin &&
            i.getPoz().getCol() >= c1 &&
            i.getPoz().getCol() <= c2) {
          i.omoara();
          scor += 10;
          nrSliceuri++;
          omorati++;
        }
      }
    } else if (pozVeche.getCol() == pozNoua.getCol()) {
      // aceeasi coloana -> omoara inamicii intre randurile vechi si nou
      int col = pozNoua.getCol();
      int r1 = pozVeche.getLin();
      int r2 = pozNoua.getLin();
      if (r1 > r2) {
        int tmp = r1;
        r1 = r2;
        r2 = tmp;
      }
      for (auto & i : inamici) {
        if (i.esteViu() && i.getPoz().getCol() == col &&
            i.getPoz().getLin() >= r1 &&
            i.getPoz().getLin() <= r2) {
          i.omoara();
          scor += 10;
          nrSliceuri++;
          omorati++;
        }
      }
    }

    return omorati;
  }

  // verific daca un inamic e pe aceeasi pozitie cu jucatorul si dupa se termina jocul
  [[nodiscard]] bool atingeInamic(const std::vector<Inamic> &inamici) const {
    for (const auto &i : inamici)
      if (i.esteLA(entitate.getPoz()))
        return true;
    return false;
  }

  [[nodiscard]] int getScor() const { return scor; }
  [[nodiscard]] int getSliceuri() const { return nrSliceuri; }
  [[nodiscard]] const Pozitie &getPoz() const { return entitate.getPoz(); }

  friend std::ostream &operator<<(std::ostream &os, const Jucator &j) {
    os << j.entitate << " | scor: " << j.scor
       << " | sliceuri: " << j.nrSliceuri;
    return os;
  }
};

//  Clasa Matrice (compune vector<vector<Celula>>) 

class Matrice {
  int linii, coloane;
  std::vector<std::vector<Celula>> grila;

  void construiesteGrila() {
    for (int i = 0; i < linii; i++) {
      grila.emplace_back();
      for (int j = 0; j < coloane; j++) {
        constexpr char taste[] = "qwertyuiopasdfghjkl;zxcvbnm,./";
        int idx = i * coloane + j;
        char ch = (idx < 30) ? taste[idx] : '.';
        grila[i].emplace_back(Pozitie(i, j), ch);
      }
    }
  }

public:
  explicit Matrice(int linii = 3, int coloane = 10)
      : linii(linii), coloane(coloane) {
    construiesteGrila();
  }

  [[nodiscard]] int getLinii() const { return linii; }

  [[nodiscard]] int getColoane() const { return coloane; }

  // afisare margini
  void afiseaza(const Jucator &jucator, const std::vector<Inamic> &inamici) const {

    std::cout << "  +";
    for (int j = 0; j < coloane; j++)
      std::cout << "";
    std::cout << "-+" << std::endl;

    for (int i = 0; i < linii; i++) {
      std::cout << "  | ";
      for (int j = 0; j < coloane; j++) {
        Pozitie p(i, j);

        // verificam daca e jucatorul
        if (jucator.getPoz() == p) {
          std::cout << "@ ";
          continue;
        }

        // verificam daca e un inamic viu
        bool gasit = false;
        for (const auto &k : inamici) {
          if (k.esteViu() && k.esteLA(p)) {
            std::cout << k.getSimbol() << " ";
            gasit = true;
            break;
          }
        }
        if (gasit)
          continue;

        // altfel afisam litera din grila
        std::cout << grila[i][j].getSimbol() << " ";
      }
      std::cout << " |" << std::endl;
    }

    std::cout << "  +";
    for (int j = 0; j < coloane; j++)
      std::cout << "";
    std::cout << "-+" << std::endl;
  }

  // // returneaza simbolul de la o pozitie
  // [[nodiscard]] char simbolLa(const Pozitie &p) const {
  //   if (p.getLin() < 0 || p.getLin() >= linii)
  //     return '\0';
  //   if (p.getCol() < 0 || p.getCol() >= coloane)
  //     return '\0';
  //   return grila[p.getLin()][p.getCol()].getSimbol();
  // }

  // genereaza o pozitie random pe matrice
  [[nodiscard]] Pozitie pozitieRandom() const {
    return Pozitie(rand() % linii, rand() % coloane);
  }

  // cauta pozitia unei litere pe matrice (pentru teleportare)
  bool gasestePozitie(char ch, Pozitie &rezultat) const {
    for (int i = 0; i < linii; i++)
      for (int j = 0; j < coloane; j++)
        if (grila[i][j].getSimbol() == ch) {
          rezultat = Pozitie(i, j);
          return true;
        }
    return false;
  }

  friend std::ostream &operator<<(std::ostream &os, const Matrice &m) {
    os << "Matrice " << m.linii << "x" << m.coloane << ":" << std::endl;
    for (int i = 0; i < m.linii; i++) {
      for (int j = 0; j < m.coloane; j++)
        os << m.grila[i][j].getSimbol() << " ";
      os << std::endl;
    }
    return os;
  }
};

//  Clasa Timer 

class Timer {
  std::chrono::steady_clock::time_point ultimSpawn;
  std::chrono::steady_clock::time_point ultimaMiscare;
  double intervalSpawn;   // secunde
  double intervalMiscare; // secunde

  [[nodiscard]] static double secDe(const std::chrono::steady_clock::time_point &t) {
    return std::chrono::duration<double>(std::chrono::steady_clock::now() - t).count();
  }

public:
  explicit Timer(double spawn = 5.0, double miscare = 3.0)
      : ultimSpawn(std::chrono::steady_clock::now()),
        ultimaMiscare(std::chrono::steady_clock::now()), intervalSpawn(spawn),
        intervalMiscare(miscare) {}

  [[nodiscard]] bool trebuieSpawn() const {
    return secDe(ultimSpawn) >= intervalSpawn;
  }
  [[nodiscard]] bool trebuieMiscare() const {
    return secDe(ultimaMiscare) >= intervalMiscare;
  }
  void resetSpawn() { ultimSpawn = std::chrono::steady_clock::now(); }
  void resetMiscare() { ultimaMiscare = std::chrono::steady_clock::now(); }

  friend std::ostream &operator<<(std::ostream &os, const Timer &t) {
    os << "Timer[spawn la " << t.intervalSpawn << "s, miscare la "
       << t.intervalMiscare << "s]";
    return os;
  }
};

//  Clasa Joc (compune Matrice, Jucator, vector<Inamic>, Timer) 

class Joc {
  Matrice matrice;
  Jucator jucator;
  std::vector<Inamic> inamici;
  Timer timer;
  bool ruleaza;
  bool gameOver;
  int nextId;

  void curataMortii() {
    // stergem inamicii morti din vector
    std::vector<Inamic> vii;
    for (const auto &i : inamici)
      if (i.esteViu())
        vii.push_back(i);
    inamici = vii;
  }

public:
  Joc()
      : matrice(3, 10), jucator(Pozitie(1, 5)), timer(5.0, 6.0), ruleaza(false),
        gameOver(false), nextId(1) {}

  // adauga un inamic nou
  void adaugaInamic() {
    if (static_cast<int>(inamici.size()) >= 8)
      return; // max 8 inamici pe ecran
    constexpr char simboluri[] = "EFGHIJKL";
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
    if (tasta == 27 || tasta == '0') {
      ruleaza = false;
      return false;
    }
    char ch = static_cast<char>(tasta);
    if (ch >= 'A' && ch <= 'Z')
      ch = ch - 'A' + 'a';

    Pozitie dest(0, 0);
    if (matrice.gasestePozitie(ch, dest)) {
      Pozitie pozVeche = jucator.getPoz();
      jucator.teleport(dest);

      // daca m-am teleportat fix pe un inamic -> game over
      if (jucator.atingeInamic(inamici)) {
        gameOver = true;
        ruleaza = false;
        return true;
      }

      if (jucator.slice(inamici, pozVeche) > 0)
        curataMortii();
      return true;
    }
    return false;
  }

  void mutaInamici() {
    for (auto &i : inamici)
      i.muta(matrice.getLinii(), matrice.getColoane());
  }

  // afiseaza ecranul
  void afiseazaEcran() const {
    Tasta::clearScreen();
    std::cout << "SLICE GAME" << std::endl;
    std::cout << "Scor: " << jucator.getScor()
         << " | Sliceuri: " << jucator.getSliceuri()
         << " | Inamici: " << inamici.size() << std::endl;
    std::cout << "Apasa o litera de pe matrice = teleportare + slice" << std::endl;
    std::cout << "ESC = iesire" << std::endl;
    std::cout << std::endl;
    matrice.afiseaza(jucator, inamici);
    std::cout << std::endl;
  }

  void ruleazaJocul() {
    srand(static_cast<unsigned>(time(nullptr)));
    ruleaza = true;
    gameOver = false;
    adaugaInamic();
    afiseazaEcran();

    while (ruleaza) {
      int tasta = Tasta::citesteTasta();

      // daca input-ul s-a terminat (EOF), oprim jocul
      if (tasta == 0) {
        ruleaza = false;
        break;
      }

      bool trebuieRedesnat = proceseazaTasta(tasta);

      if (!ruleaza)
        break;

      // adauga un inamic nou daca a trecut intervalul de spawn
      if (timer.trebuieSpawn()) {
        adaugaInamic();
        timer.resetSpawn();
        trebuieRedesnat = true;
      }

      // muta inamicii daca a trecut intervalul de miscare
      if (timer.trebuieMiscare()) {
        mutaInamici();
        timer.resetMiscare();
        trebuieRedesnat = true;
        if (jucator.atingeInamic(inamici)) {
          gameOver = true;
          ruleaza = false;
        }
      }

      if (trebuieRedesnat)
        afiseazaEcran();
    }

    afiseazaEcran();
    if (gameOver) {
      std::cout << "GAME OVER! Un inamic te-a atins!" << std::endl;
      std::cout << "Scor final: " << jucator.getScor() << std::endl;
    }
    std::cout << "Apasa orice tasta pentru a iesi..." << std::endl;
    Tasta::citesteTasta();
  }

  friend std::ostream &operator<<(std::ostream &os, const Joc &j) {
    os << "Joc: " << j.jucator << " | " << j.timer;
    return os;
  }
};

int main() {
  // demo
  std::cout<< "Inamicii se vor spauna si trebuie sa ii elimini efectuand un slice(apasand orice tasta de pe matrice)"<<std::endl;
  std::cout<<"Pentru un inamic aflat in matrice pe lin:2 col:3 il poti elinima astfel:"<<std::endl;
  std::cout<<"Slice de pe lin:2 col:1 pe lin:2 col:5 sau vice versa, slice de pe lin:1 col:3 pe lin:3 col:3"<<std::endl;
  std::cout<<"! Mare atentie sa nu dai slice pe casuta cu inamicul in ea"<<std::endl;
  std::cout<<"Playerul este '@' "<<std::endl;



  Matrice mat(3, 10);
  std::cout <<std::endl<< mat;
  Joc joc;
  std::cout <<std::endl<< joc << std::endl;

  std::cout << std::endl << "Apasa orice tasta pentru a incepe jocul" << std::endl;
  Tasta::citesteTasta();
  joc.ruleazaJocul();

  return 0;
}
