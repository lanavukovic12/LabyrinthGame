/*#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <queue>

using namespace std;


// funkcija koja proverava da li je lavirint progodan
// pretrazuje lavirint od pocetne tacke (startX, startY) do izlaza (exitRow, exitX)
// u pretrazi, sve celije osim zidova ('#') se tretiraju kao prolazne


bool isTraversable(const vector<vector<char>>& maze, int startX, int startY, int exitRow, int exitX) {
    int visina = maze.size();
    int sirina = maze[0].size();
    vector<vector<bool>> visited(visina, vector<bool>(sirina, false));

    // Koraci u cetiri smera: gore, dole, levo, desno
    int dx[4] = { 0, 0, -1, 1 };
    int dy[4] = { -1, 1, 0, 0 };

    queue<pair<int, int>> q;
    q.push({ startY, startX });
    visited[startY][startX] = true;

    while (!q.empty()) {
        auto [y, x] = q.front();
        q.pop();

        if (y == exitRow && x == exitX)
            return true;

        for (int i = 0; i < 4; i++) {
            int newY = y + dy[i];
            int newX = x + dx[i];

            // Provera granica
            if (newX < 0 || newX >= sirina || newY < 0 || newY >= visina)
                continue;

            // Ako nije zid i nije poseceno, dodaj u red
            if (!visited[newY][newX] && maze[newY][newX] != '#') {
                visited[newY][newX] = true;
                q.push({ newY, newX });
            }
        }
    }
    return false;
}







int main() {
    int sirina, visina;
    cout << "Unesite sirinu lavirinta (minimalno 15): ";
    cin >> sirina;
    cout << "Unesite visinu lavirinta (minimalno 15): ";
    cin >> visina;

    if (sirina < 15 || visina < 15) {
        cout << "Dimenzije moraju biti vece od 15 x 15!" << endl;
        return 1;
    }
    cout << "Lavirint ce imati dimenzije: " << sirina << " x " << visina << endl;

    // Kreiranje 2D matrice lavirinta
    vector<vector<char>> lavirint(visina, vector<char>(sirina, '.'));

    // Postavljanje spoljasnjih zidova
    for (int i = 0; i < sirina; i++) {
        lavirint[0][i] = '#';               // gornji zid
        lavirint[visina - 1][i] = '#';        // donji zid
    }
    for (int i = 0; i < visina; i++) {
        lavirint[i][0] = '#';               // levi zid
        lavirint[i][sirina - 1] = '#';        // desni zid
    }

    // Postavljanje unutrasnjih zidova
    int brojZidova = 2 * (sirina + visina);
    srand(time(0)); // Inicijalizacija generatora nasumicnih brojeva
    for (int i = 0; i < brojZidova; i++) {
        int x, y;
        do {
            x = rand() % (sirina - 2) + 1; // x u opsegu [1, sirina-2]
            y = rand() % (visina - 2) + 1;  // y u opsegu [1, visina-2]
        } while (lavirint[y][x] == '#');
        lavirint[y][x] = '#';
    }

    // Postavljanje ulaza (U) i robota (R)
    int ulazX = rand() % (sirina - 2) + 1;
    lavirint[0][ulazX] = 'U';
    lavirint[1][ulazX] = 'R';

    // Postavljanje izlaza (I)
    int izlazX = rand() % (sirina - 2) + 1;
    lavirint[visina - 1][izlazX] = 'I';

    // Unos broja predmeta i postavljanje predmeta (P)
    int brojPredmeta;
    cout << "Unesite broj predmeta (minimalno 4): ";
    cin >> brojPredmeta;
    if (brojPredmeta < 4) {
        cout << "Broj predmeta mora biti bar 4!" << endl;
        return 1;
    }
    for (int i = 0; i < brojPredmeta; i++) {
        int itemX, itemY;
        do {
            itemX = rand() % (sirina - 2) + 1;
            itemY = rand() % (visina - 2) + 1;
        } while (lavirint[itemY][itemX] != '.');
        lavirint[itemY][itemX] = 'P';
    }

    // Postavljanje minotaura (M)
    int minotaurX, minotaurY;
    do {
        minotaurX = rand() % (sirina - 2) + 1;
        minotaurY = rand() % (visina - 2) + 1;
    } while (lavirint[minotaurY][minotaurX] != '.');
    lavirint[minotaurY][minotaurX] = 'M';

    // Inicijalna pozicija robota
    int robotX = ulazX, robotY = 1;

    // Interaktivna petlja za kretanje robota
    char komanda;
    bool gameRunning = true;
    while (gameRunning) {
        // Ispis trenutnog lavirinta
        cout << "\nTrenutni lavirint:" << endl;
        for (int i = 0; i < visina; i++) {
            for (int j = 0; j < sirina; j++) {
                cout << lavirint[i][j] << " ";
            }
            cout << endl;
        }

        cout << "Unesite komandu (w/a/s/d za kretanje, q za izlaz): ";
        cin >> komanda;
        if (komanda == 'q' || komanda == 'Q') {
            cout << "Kraj igre." << endl;
            break;
        }

        // Odredjivanje nove pozicije robota
        int noviX = robotX, noviY = robotY;
        if (komanda == 'w' || komanda == 'W')
            noviY--;
        else if (komanda == 's' || komanda == 'S')
            noviY++;
        else if (komanda == 'a' || komanda == 'A')
            noviX--;
        else if (komanda == 'd' || komanda == 'D')
            noviX++;
        else {
            cout << "Nepoznata komanda. Probajte ponovo." << endl;
            continue;
        }

        // Provera da li je nova pozicija dozvoljena
        if (lavirint[noviY][noviX] == '#') {
            cout << "Zid blokira put! Probajte drugi smer." << endl;
            continue;
        }
        else if (lavirint[noviY][noviX] == 'I') {
            cout << "Cestitamo! Stigli ste do izlaza!" << endl;
            gameRunning = false;
        }
        else if (lavirint[noviY][noviX] == 'M') {
            cout << "Minotaur vas je uhvatio! Igra je zavrsena." << endl;
            gameRunning = false;
        }
        // Ako je celija prazna ili sadrzi predmet ('P'), robot se moze pomeriti
        // Brisemo staru poziciju robota
        lavirint[robotY][robotX] = '.';
        // Azuriramo poziciju
        robotX = noviX;
        robotY = noviY;
        // Postavljamo robota na novu poziciju
        lavirint[robotY][robotX] = 'R';
    }

    return 0;
}

*/
















/*
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <queue>

using namespace std;

// Funkcija koja proverava da li je lavirint prohodan
// Pretrazuje lavirint od pocetne tacke (startX, startY) do izlaza (exitRow, exitX)
// U pretrazi, sve celije osim zidova ('#') se tretiraju kao prolazne.
bool isTraversable(const vector<vector<char>>& maze, int startX, int startY, int exitRow, int exitX) {
    int visina = maze.size();
    int sirina = maze[0].size();
    vector<vector<bool>> visited(visina, vector<bool>(sirina, false));

    // Koraci u cetiri smera: gore, dole, levo, desno
    int dx[4] = { 0, 0, -1, 1 };
    int dy[4] = { -1, 1, 0, 0 };

    queue<pair<int, int>> q;
    q.push({ startY, startX });
    visited[startY][startX] = true;

    while (!q.empty()) {
        auto current = q.front();
        q.pop();
        int y = current.first;
        int x = current.second;

        if (y == exitRow && x == exitX)
            return true;

        for (int i = 0; i < 4; i++) {
            int newY = y + dy[i];
            int newX = x + dx[i];

            // Provera granica
            if (newX < 0 || newX >= sirina || newY < 0 || newY >= visina)
                continue;

            // Ako nije zid i nije poseceno, dodaj u red
            if (!visited[newY][newX] && maze[newY][newX] != '#') {
                visited[newY][newX] = true;
                q.push({ newY, newX });
            }
        }
    }
    return false;
}

int main() {
    int sirina, visina;
    cout << "Unesite sirinu lavirinta (minimalno 15): ";
    cin >> sirina;
    cout << "Unesite visinu lavirinta (minimalno 15): ";
    cin >> visina;

    if (sirina < 15 || visina < 15) {
        cout << "Dimenzije moraju biti vece od 15 x 15!" << endl;
        return 1;
    }
    cout << "Lavirint ce imati dimenzije: " << sirina << " x " << visina << endl;

    int brojPredmeta;
    cout << "Unesite broj predmeta (minimalno 4): ";
    cin >> brojPredmeta;
    if (brojPredmeta < 4) {
        cout << "Broj predmeta mora biti bar 4!" << endl;
        return 1;
    }

    int ulazX, izlazX, robotX, robotY;
    vector<vector<char>> lavirint;
    bool validMaze = false;

    srand(time(0)); // Inicijalizacija generatora nasumicnih brojeva

    // Petlja generise lavirint sve dok se ne dobije prohodan
    while (!validMaze) {
        // Kreiramo prazan lavirint
        lavirint = vector<vector<char>>(visina, vector<char>(sirina, '.'));

        // Postavljanje spoljasnjih zidova
        for (int i = 0; i < sirina; i++) {
            lavirint[0][i] = '#';
            lavirint[visina - 1][i] = '#';
        }
        for (int i = 0; i < visina; i++) {
            lavirint[i][0] = '#';
            lavirint[i][sirina - 1] = '#';
        }

        // Postavljanje unutrasnjih zidova
        int brojZidova = 2 * (sirina + visina);
        for (int i = 0; i < brojZidova; i++) {
            int x, y;
            do {
                x = rand() % (sirina - 2) + 1;
                y = rand() % (visina - 2) + 1;
            } while (lavirint[y][x] == '#');
            lavirint[y][x] = '#';
        }

        // Postavljanje ulaza (U) u prvom redu
        ulazX = rand() % (sirina - 2) + 1;
        lavirint[0][ulazX] = 'U';

        // Postavljanje robota (R) odmah ispod ulaza
        robotX = ulazX;
        robotY = 1;
        lavirint[1][ulazX] = 'R';

        // Postavljanje izlaza (I) u poslednjem redu
        izlazX = rand() % (sirina - 2) + 1;
        lavirint[visina - 1][izlazX] = 'I';

        // Postavljanje predmeta (P)
        for (int i = 0; i < brojPredmeta; i++) {
            int itemX, itemY;
            do {
                itemX = rand() % (sirina - 2) + 1;
                itemY = rand() % (visina - 2) + 1;
            } while (lavirint[itemY][itemX] != '.');
            lavirint[itemY][itemX] = 'P';
        }

        // Postavljanje minotaura (M)
        int minotaurX, minotaurY;
        do {
            minotaurX = rand() % (sirina - 2) + 1;
            minotaurY = rand() % (visina - 2) + 1;
        } while (lavirint[minotaurY][minotaurX] != '.');
        lavirint[minotaurY][minotaurX] = 'M';

        // Provera: da li robot (na poziciji (robotX, robotY)) moze doci do izlaza (na (visina-1, izlazX))
        if (isTraversable(lavirint, robotX, robotY, visina - 1, izlazX)) {
            validMaze = true;
        }
        else {
            cout << "Lavirint nije prohodan. Generisem ponovo..." << endl;
        }
    }

    // Interaktivna petlja za kretanje robota
    char komanda;
    bool gameRunning = true;
    while (gameRunning) {
        // Ispis trenutnog lavirinta
        cout << "\nTrenutni lavirint:" << endl;
        for (int i = 0; i < visina; i++) {
            for (int j = 0; j < sirina; j++) {
                cout << lavirint[i][j] << " ";
            }
            cout << endl;
        }

        cout << "Unesite komandu (w/a/s/d za kretanje, q za izlaz): ";
        cin >> komanda;
        if (komanda == 'q' || komanda == 'Q') {
            cout << "Kraj igre." << endl;
            break;
        }

        // Izracunavanje nove pozicije robota
        int noviX = robotX, noviY = robotY;
        if (komanda == 'w' || komanda == 'W')
            noviY--;
        else if (komanda == 's' || komanda == 'S')
            noviY++;
        else if (komanda == 'a' || komanda == 'A')
            noviX--;
        else if (komanda == 'd' || komanda == 'D')
            noviX++;
        else {
            cout << "Nepoznata komanda. Probajte ponovo." << endl;
            continue;
        }

        // Provera da li je nova pozicija dozvoljena
        if (lavirint[noviY][noviX] == '#') {
            cout << "Zid blokira put! Probajte drugi smer." << endl;
            continue;
        }
        else if (lavirint[noviY][noviX] == 'I') {
            cout << "Cestitamo! Stigli ste do izlaza!" << endl;
            gameRunning = false;
        }
        else if (lavirint[noviY][noviX] == 'M') {
            cout << "Minotaur vas je uhvatio! Igra je zavrsena." << endl;
            gameRunning = false;
        }

        // Ako je celija prazna ili sadrzi predmet ('P'), robot se pomera
        lavirint[robotY][robotX] = '.';
        robotX = noviX;
        robotY = noviY;
        lavirint[robotY][robotX] = 'R';
    }

    return 0;
}
*/


#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <cmath> // za abs()

using namespace std;

// 1) Provera prohodnosti lavirinta (BFS od robota do izlaza)
bool isTraversable(const vector<vector<char>>& maze, int startX, int startY, int exitRow, int exitX) {
    int visina = maze.size();
    int sirina = maze[0].size();
    vector<vector<bool>> visited(visina, vector<bool>(sirina, false));

    // Koraci u 4 smera: gore, dole, levo, desno
    int dx[4] = { 0, 0, -1, 1 };
    int dy[4] = { -1, 1, 0, 0 };

    queue<pair<int, int>> q;
    q.push({ startY, startX });
    visited[startY][startX] = true;

    while (!q.empty()) {
        auto current = q.front();
        q.pop();
        int y = current.first;
        int x = current.second;

        // Ako smo stigli do izlaza, lavirint je prohodan
        if (y == exitRow && x == exitX)
            return true;

        for (int i = 0; i < 4; i++) {
            int newY = y + dy[i];
            int newX = x + dx[i];

            // Provera granica
            if (newX < 0 || newX >= sirina || newY < 0 || newY >= visina)
                continue;
            // Ako nije zid i nije poseceno
            if (!visited[newY][newX] && maze[newY][newX] != '#') {
                visited[newY][newX] = true;
                q.push({ newY, newX });
            }
        }
    }
    return false;
}

int main() {
    srand(time(0)); // Inicijalizacija generatora slucajnih brojeva

    int sirina, visina;
    cout << "Unesite sirinu lavirinta (minimalno 15): ";
    cin >> sirina;
    cout << "Unesite visinu lavirinta (minimalno 15): ";
    cin >> visina;

    if (sirina < 15 || visina < 15) {
        cout << "Dimenzije moraju biti vece od 15 x 15!" << endl;
        return 1;
    }
    cout << "Lavirint ce imati dimenzije: " << sirina << " x " << visina << endl;

    int brojPredmeta;
    cout << "Unesite broj predmeta (minimalno 4): ";
    cin >> brojPredmeta;
    if (brojPredmeta < 4) {
        cout << "Broj predmeta mora biti bar 4!" << endl;
        return 1;
    }

    // Deklaracije koje cemo puniti nakon generisanja lavirinta
    vector<vector<char>> lavirint;
    bool validMaze = false;

    // Koordinate robota, minotaura, itd.
    int ulazX, robotX, robotY;
    int izlazX;
    int minotaurX, minotaurY;

    // 2) Generisanje lavirinta dok ne bude prohodan
    while (!validMaze) {
        // Kreiramo prazan lavirint
        lavirint = vector<vector<char>>(visina, vector<char>(sirina, '.'));

        // Postavljanje spoljašnjih zidova
        for (int i = 0; i < sirina; i++) {
            lavirint[0][i] = '#';
            lavirint[visina - 1][i] = '#';
        }
        for (int i = 0; i < visina; i++) {
            lavirint[i][0] = '#';
            lavirint[i][sirina - 1] = '#';
        }

        // Postavljanje unutrasnjih zidova
        int brojZidova = 2 * (sirina + visina);
        for (int i = 0; i < brojZidova; i++) {
            int x, y;
            do {
                x = rand() % (sirina - 2) + 1;
                y = rand() % (visina - 2) + 1;
            } while (lavirint[y][x] == '#');
            lavirint[y][x] = '#';
        }

        // Postavljanje ulaza (U) u prvom redu
        ulazX = rand() % (sirina - 2) + 1;
        lavirint[0][ulazX] = 'U';

        // Robot (R) ispod ulaza
        robotX = ulazX;
        robotY = 1;
        lavirint[robotY][robotX] = 'R';

        // Postavljanje izlaza (I) u poslednjem redu
        izlazX = rand() % (sirina - 2) + 1;
        lavirint[visina - 1][izlazX] = 'I';

        // Postavljanje predmeta (P)
        for (int i = 0; i < brojPredmeta; i++) {
            int itemX, itemY;
            do {
                itemX = rand() % (sirina - 2) + 1;
                itemY = rand() % (visina - 2) + 1;
            } while (lavirint[itemY][itemX] != '.');
            lavirint[itemY][itemX] = 'P';
        }

        // Postavljanje minotaura (M)
        do {
            minotaurX = rand() % (sirina - 2) + 1;
            minotaurY = rand() % (visina - 2) + 1;
        } while (lavirint[minotaurY][minotaurX] != '.');
        lavirint[minotaurY][minotaurX] = 'M';

        // Proveravamo da li je lavirint prohodan (robot -> izlaz)
        if (isTraversable(lavirint, robotX, robotY, visina - 1, izlazX)) {
            validMaze = true;
        }
        else {
            cout << "Lavirint nije prohodan. Generisem ponovo..." << endl;
        }
    }

    // 3) Interaktivna petlja
    char komanda;
    bool gameRunning = true;

    // Pomocne tabele za 4 smera (gore, dole, levo, desno)
    int dX[4] = { 0,  0, -1,  1 };
    int dY[4] = { -1,  1,  0,  0 };

    while (gameRunning) {
        // Ispis trenutnog lavirinta
        cout << "\nTrenutni lavirint:" << endl;
        for (int i = 0; i < visina; i++) {
            for (int j = 0; j < sirina; j++) {
                cout << lavirint[i][j] << " ";
            }
            cout << endl;
        }

        // Unos komande
        cout << "Unesite komandu (w/a/s/d za kretanje, q za izlaz): ";
        cin >> komanda;
        if (komanda == 'q' || komanda == 'Q') {
            cout << "Kraj igre." << endl;
            break;
        }

        // Pomeranje robota
        int noviX = robotX, noviY = robotY;
        if (komanda == 'w' || komanda == 'W') noviY--;
        else if (komanda == 's' || komanda == 'S') noviY++;
        else if (komanda == 'a' || komanda == 'A') noviX--;
        else if (komanda == 'd' || komanda == 'D') noviX++;
        else {
            cout << "Nepoznata komanda. Probajte ponovo." << endl;
            continue;
        }

        // Provera nove pozicije robota
        if (lavirint[noviY][noviX] == '#') {
            cout << "Zid blokira put! Probajte drugi smer." << endl;
        }
        else if (lavirint[noviY][noviX] == 'I') {
            // Robot stigao do izlaza
            lavirint[robotY][robotX] = '.';
            robotX = noviX;
            robotY = noviY;
            lavirint[robotY][robotX] = 'R';
            cout << "Cestitamo! Stigli ste do izlaza!" << endl;
            gameRunning = false;
        }
        else if (lavirint[noviY][noviX] == 'M') {
            // Robot naleteo direktno na Minotaura
            cout << "Minotaur vas je uhvatio! Igra je zavrsena." << endl;
            gameRunning = false;
        }
        else {
            // Polje je '.' ili 'P' – robot može da predje
            lavirint[robotY][robotX] = '.';
            robotX = noviX;
            robotY = noviY;
            lavirint[robotY][robotX] = 'R';
        }

        if (!gameRunning) break; // ako je igra vec gotova, prekidamo petlju

        // 4) Kretanje Minotaura:
        // a) Provera da li je robot na susednom polju (Minotaur odmah jede robota)
        if (abs(minotaurX - robotX) + abs(minotaurY - robotY) == 1) {
            cout << "Minotaur je pored vas i pojeo vas je! Kraj igre." << endl;
            gameRunning = false;
            break;
        }

        // b) Ako nije, Minotaur se nasumicno pomera
        bool minotaurSePomerao = false;
        for (int pokusaj = 0; pokusaj < 4; pokusaj++) {
            int r = rand() % 4; // biramo slucajan smer
            int newMX = minotaurX + dX[r];
            int newMY = minotaurY + dY[r];

            // Provera da li je unutar granica
            if (newMX < 0 || newMX >= sirina || newMY < 0 || newMY >= visina)
                continue;
            // Ako je zid, preskacemo
            if (lavirint[newMY][newMX] == '#')
                continue;
            if (lavirint[newMY][newMX] == 'I')
                continue;

            // Ako je 'P', Minotaur unistava predmet
            // Ako je '.', samo prelazi
            // Ako je 'R', robot je pojeden
            if (lavirint[newMY][newMX] == 'R') {
                // Minotaur pojede robota
                cout << "Minotaur je naleteo na vas! Igra je zavrsena." << endl;
                gameRunning = false;
                break;
            }

            // Prethodna pozicija Minotaura postaje '.'
            lavirint[minotaurY][minotaurX] = '.';
            // Pomerimo Minotaura
            minotaurX = newMX;
            minotaurY = newMY;
            lavirint[minotaurY][minotaurX] = 'M';

            minotaurSePomerao = true;
            break; // izadjemo iz for petlje (Minotaur se pomerio)
        }

        if (!gameRunning) break; // ako je Minotaur upravo pojeo robota

        // c) Nakon pomeranja, ponovo proverimo da li su se Minotaur i robot sudarili
        if (minotaurX == robotX && minotaurY == robotY) {
            cout << "Minotaur vas je uhvatio! Igra je zavrsena." << endl;
            gameRunning = false;
        }
    }

    return 0;
}
