// Brunon Bobowski

#include <stdio.h>
#include <stdbool.h>

#ifndef POLA
#define POLA 5
#endif

#ifndef WIERSZE
#define WIERSZE 26
#endif

#ifndef KOLUMNY
#define KOLUMNY 26
#endif

#ifndef WYBOR
#define WYBOR 1000
#endif

//KONIEC to stała, dzięki której program wykryje poddanie się któregoś z graczy.
#define KONIEC 0

// char plansza[][] to dwuwymiarowa tablica, na której trwa rozgrywka. Posiada ona swoją kopię,
// char testowa_plansza[][], na której odbywa się wyszukiwanie najlepszego ruchu gracza prawego.
char plansza[WIERSZE][KOLUMNY] = {0};
char testowa_plansza[WIERSZE][KOLUMNY] = {0};

// koordynaty_ruchu_lewego[2] oraz koordynaty_ruchu_prawego[2] przechowują wczytane ruchy graczy.
int koordynaty_ruchu_lewego[2] = {0};
int koordynaty_ruchu_prawego[2] = {0};

// wiersze_ruchów_prawego[] oraz kolumny_ruchów_prawego[] to dwie pomocnicze tablice, które służą
// wyznaczeniu najlepszego ruchu gracza prawego, gdy jest ich kilka możliwych.
int wiersze_ruchów_prawego[WIERSZE * KOLUMNY] = {0};
int kolumny_ruchów_prawego[WIERSZE * KOLUMNY] = {0};

// Funkcja wczytanie_ruchu_lewego() wczytuje ruch gracza lewego ze standardowego wejścia i zapisuje
// w tablicy koordynaty_ruchu_lewego[]. Jeśli lewy nie wykonał ruchu, funkcja zapisuje wartość KONIEC,
// dzięki czemu program rozpozna zakończenie rozgrywki.
static void wczytanie_ruchu_lewego(void) {
    int i = 0;
    char znak;
    koordynaty_ruchu_lewego[0] = KONIEC;
    koordynaty_ruchu_lewego[1] = KONIEC;
    while ((znak = (char) getchar()) != '\n' && znak != EOF) {
       koordynaty_ruchu_lewego[i] = znak;
       i++;
    }
}

static int zamiana_na_wiersz(int a) {
    int b = (int) a - 'a';
    return b;
}

static int zamiana_na_kolumne(int a) {
    int b = (int) a - 'A';
    return b;
}

static void ruch_lewego(int kolumna, int wiersz) {
    for (int i = 0; i < POLA; i++) {
        plansza[wiersz + i][kolumna] = '#';
    }
}

static void ruch_prawego(int wiersz, int kolumna) {
    for (int i = 0; i < POLA; i++) {
        plansza[wiersz][kolumna + i] = '#';
    }
}

// Funkcja testowy_ruch_prawego() wykonuje testowy ruch prawego na kopii planszy.
static void testowy_ruch_prawego(int wiersz, int kolumna) {
    if (kolumna <= KOLUMNY - POLA) {
        for (int i = 0; i < POLA; i++) {
            testowa_plansza[wiersz][kolumna + i] = '#';
        }
    }
}

// Funkcja kopiowanie_planszy() tworzy kopię głównej planszy, dzięki czemu można wykonywać na niej
// wiele testowych ruchów prawego i wyznaczyć najlepszy z nich.
static void kopiowanie_planszy(void) {
    int wiersz;
    int kolumna;
    for (wiersz = 0; wiersz < WIERSZE; wiersz++) {
        for (kolumna = 0; kolumna < KOLUMNY; kolumna++) {
            testowa_plansza[wiersz][kolumna] = plansza[wiersz][kolumna];
        }
    }
}

// Funkcje ile_naraz_lewy() oraz ile_naraz_prawy() wyznaczają ile poziomek na raz mogą umieścić
// gracze lewy i prawy.
static int ile_naraz_lewy(void) {
    int wiersz;
    int kolumna;
    int ile_klocków_mozna_postawić = 0;
    int wolne_pola = 0;
    for (kolumna = 0; kolumna < KOLUMNY; kolumna++) {
        wolne_pola = 0;
        for (wiersz = 0; wiersz < WIERSZE; wiersz++) {
            if (testowa_plansza[wiersz][kolumna] != '#') {
                wolne_pola++;
            }
            else {
                ile_klocków_mozna_postawić += wolne_pola / POLA;
                wolne_pola = 0;
            }
        }
        ile_klocków_mozna_postawić += wolne_pola / POLA;
        wolne_pola = 0;
    }
    return ile_klocków_mozna_postawić;
}

static int ile_naraz_prawy(void) {
    int wiersz;
    int kolumna;
    int ile_klocków_mozna_postawić = 0;
    int wolne_pola = 0;
    for (wiersz = 0; wiersz < WIERSZE; wiersz++) {
        wolne_pola = 0;
        for (kolumna = 0; kolumna < KOLUMNY; kolumna++) {
            if (testowa_plansza[wiersz][kolumna] != '#') {
                wolne_pola++;
            }
            else {
                ile_klocków_mozna_postawić += wolne_pola / POLA;
                wolne_pola = 0;
            }
        }
        ile_klocków_mozna_postawić += wolne_pola / POLA;
        wolne_pola = 0;
    }
    return ile_klocków_mozna_postawić;
}

static int ocena_testowej_planszy_dla_prawego(void) {
    return ile_naraz_prawy() - ile_naraz_lewy();
}

static void czyszczenie_tablicy_wiersze_ruchów_prawego(void) {
    for (int i = 0; i < KOLUMNY * WIERSZE; i++) {
        wiersze_ruchów_prawego[i] = 0;
        kolumny_ruchów_prawego[i] = 0;
    }
}

// Funkcja wyszukiwanie_najlepszego_ruchu_dla_prawego(), tak jak sugeruje nazwa, wyszukuje ruch prawego
// po którym ocena_testowej_planszy_dla_prawego() przyjmie największą wartość (będzie to najlepszy ruch).
// Jeśli takich ruchów jest kilka, program wyznacza ich ciąg uporządkowany w kolejności rosnących nazw wiersza
// a w ramach wiersza, w kolejności rosnących nazw kolumn. Zakładając, że długością tego ciągu jest n,
// numer wybranego ruchu, liczony od 0, będzie wartością wyrażenia WYBOR % n.
static void wyszukiwanie_najlepszego_ruchu_dla_prawego(void) {

    int wiersz = 0;
    int kolumna = 0;
    int n = 0;
    int maksymalna_ocena = -1000;
    int aktualna_ocena;
    bool można_postawić;

    for (wiersz = 0; wiersz < WIERSZE; wiersz++) {
        for (kolumna = 0; kolumna <= KOLUMNY - POLA; kolumna++) {
            można_postawić = true;
            kopiowanie_planszy();
            for (int i = 0; i < POLA; i++) {
                if (testowa_plansza[wiersz][kolumna + i] == '#') {
                    można_postawić = false;
                }
            }
            if (można_postawić) {
                testowy_ruch_prawego(wiersz, kolumna);
                aktualna_ocena = ocena_testowej_planszy_dla_prawego();
                if (aktualna_ocena > maksymalna_ocena) {
                    maksymalna_ocena = aktualna_ocena;
                    n = 0;
                    czyszczenie_tablicy_wiersze_ruchów_prawego();
                    wiersze_ruchów_prawego[0] = (int) wiersz + 'a';
                    kolumny_ruchów_prawego[0] = (int) kolumna + 'A';
                    n++;
                }
                else if (aktualna_ocena == maksymalna_ocena) {
                    wiersze_ruchów_prawego[n] = (int) wiersz + 'a';
                    kolumny_ruchów_prawego[n] = (int) kolumna + 'A';
                    n++;
                }
            }
            aktualna_ocena = 0;
            można_postawić = true;
            kopiowanie_planszy();
        }
    }

    // n == 0 oznacza, że prawy nie może już wykonać ruchu. W efekcie gracz poddaje się.
    if (n == 0) {
        koordynaty_ruchu_prawego[0] = KONIEC;
        koordynaty_ruchu_prawego[1] = KONIEC;
    }

    // n == 1 oznacza, że istnieje tylko jeden ruch, po którym ocena planszy dla prawego będzie maksymalna.
    else if (n == 1) {
        koordynaty_ruchu_prawego[0] = wiersze_ruchów_prawego[0];
        koordynaty_ruchu_prawego[1] = kolumny_ruchów_prawego[0];
    }

    // n > 1 oznacza, że istnieje więcej niż jeden ruch po którym ocena planszy dla prawego będzie maksymalna.
    // W tym przypadku ruch zostaje wybrany zgodnie z algorytmem opisanym w komentarzu przed funkcją.
    else {
        n = WYBOR % n;
        koordynaty_ruchu_prawego[0] = wiersze_ruchów_prawego[n];
        koordynaty_ruchu_prawego[1] = kolumny_ruchów_prawego[n];
    }
    czyszczenie_tablicy_wiersze_ruchów_prawego();
}

int main(void) {

    // bool koniec; służy do wyjścia z pętli rozgrywki.
    // Jeśli któryś z graczy podda się, program nada wartość koniec = true,
    // dzięki czemu nastąpi wyjście z pętli, a rozgrywka się zakończy.
    bool koniec = false;

    int wiersz_lewego;
    int kolumna_lewego;
    int wiersz_prawego;
    int kolumna_prawego;

    // Następuje wczytanie pierwszego ruchu gracza lewego.
    wczytanie_ruchu_lewego();
    wiersz_lewego = zamiana_na_wiersz(koordynaty_ruchu_lewego[1]);
    kolumna_lewego = zamiana_na_kolumne(koordynaty_ruchu_lewego[0]);

    while (!koniec) {

        // Przypadek, gdy gracz lewy się poddaje, więc program kończy działanie.
        if (koordynaty_ruchu_lewego[0] == '.') {

            koniec = true;
            printf("\n");
        }

        // Przypadek, gdy gracz lewy oddaje pierwszy ruch prawemu.
        else if (koordynaty_ruchu_lewego[0] == '-') {

            kopiowanie_planszy();
            wyszukiwanie_najlepszego_ruchu_dla_prawego();
            wiersz_prawego = zamiana_na_wiersz(koordynaty_ruchu_prawego[0]);
            kolumna_prawego = zamiana_na_kolumne(koordynaty_ruchu_prawego[1]);
            ruch_prawego(wiersz_prawego, kolumna_prawego);
            printf("%c%c\n", koordynaty_ruchu_prawego[0], koordynaty_ruchu_prawego[1]);
        }

        // Przypadek "normalnego" ruch gracza lewego. To znaczy, że  ze standardowego wejścia
        // wczytano ruch gracza lewego w postaci 'Aa'.
        else {

            ruch_lewego(kolumna_lewego, wiersz_lewego);
            kopiowanie_planszy();
            wyszukiwanie_najlepszego_ruchu_dla_prawego();

            // Przypadek, gdy gracz prawy nie może wykonać już ruchu, więc poddaje się.
            if (koordynaty_ruchu_prawego[0] == KONIEC && koordynaty_ruchu_prawego[1] == KONIEC) {

                koniec = true;
                printf(".\n");
            }

            // Przypadek wykonania "normalnego" ruchu przez gracza prawego.
            // Na przykład ruchu zapisanego jako 'aA'.
            else {

                wiersz_prawego = zamiana_na_wiersz(koordynaty_ruchu_prawego[0]);
                kolumna_prawego = zamiana_na_kolumne(koordynaty_ruchu_prawego[1]);
                ruch_prawego(wiersz_prawego, kolumna_prawego);
                printf("%c%c\n", koordynaty_ruchu_prawego[0], koordynaty_ruchu_prawego[1]);
            }
        }

        // Wczytanie następnego ruchu gracza lewego.
        wczytanie_ruchu_lewego();
        wiersz_lewego = zamiana_na_wiersz(koordynaty_ruchu_lewego[1]);
        kolumna_lewego = zamiana_na_kolumne(koordynaty_ruchu_lewego[0]);

        // Sprawdzenie, czy gracz lewy poddał się, lub nie wykonuje kolejnego ruchu.
        if (koordynaty_ruchu_lewego[1] == KONIEC && koordynaty_ruchu_lewego[0] == KONIEC) {

            koniec = true;
        }
        if (koordynaty_ruchu_lewego[0] == '.') {

            koniec = true;
        }
    }
}
