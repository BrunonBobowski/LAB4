#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ROZMIAR_TABLICY 12
#define MAXWIERSZ 1000

int akumulator[ROZMIAR_TABLICY - 1] = {0};
int wielomian[ROZMIAR_TABLICY] = {0};
char wiersz[MAXWIERSZ] = {0};
char znak;

void wczytaj_wiersz() {
    int i;
    for (i = 0; i < ROZMIAR_TABLICY; i++) {
        wielomian[i] = 0;
    }
    i = 0;
    while (wiersz[i] != 0) {
        wiersz[i] = 0;
        i++;
    }
    i = 0;
    while ((znak = getchar()) != '\n' && i < MAXWIERSZ) {
        if (znak != ' ' && znak != '\n') {
            wiersz[i] = znak;
            i++;
        }
    }
}

void zamień_wiersz_na_wielomian() {
    wczytaj_wiersz();
    int cyfra = 0;
    int a = 1;
    int i = 0;
    int potęga = 0;
    wielomian[ROZMIAR_TABLICY - 1] = wiersz[0];
    while ((znak = wiersz[i]) != 0 && i < MAXWIERSZ) {
        if (znak == '+') {
            a = 1;
            i++;
        }
        else if (znak == '-') {
            a = -1;
            i++;
        }
        else if (znak >= '0' && znak <= '9') {
            cyfra *= 10;
            cyfra += znak - '0';
            if (wiersz[i + 1] < '0' || wiersz[i + 1] > '9') {
                cyfra *= a;
                if (wiersz[i + 1] != 'x') {
                    wielomian[0] += cyfra;
                    cyfra = 0;
                }
                if (wiersz[i + 1] == 'x') {
                    if (wiersz[i + 2] != '^') {
                        wielomian[1] += cyfra;
                        cyfra = 0;
                        i++;
                    }
                    else {
                        if (wiersz[i + 4] == '0') {
                            wielomian[10] += cyfra;
                            cyfra = 0;
                            i += 4;
                        }
                        else {
                            potęga = wiersz[i + 3] - '0';
                            wielomian[potęga] += cyfra;
                            cyfra = 0;
                            i += 3;
                        }
                    }
                }
            }
            i++;
        }
        else if (znak == 'x') {
            cyfra = a;
            if (wiersz[i + 1] != '^') {
                wielomian[0] += cyfra;
                cyfra = 0;
            }
            else {
                if (wiersz[i + 3] == '0') {
                    wielomian[10] += cyfra;
                    cyfra = 0;
                    i += 3;
                }
                else {
                    potęga = wiersz[i + 2] - '0';
                    wielomian[potęga] += cyfra;
                    cyfra = 0;
                    i += 2;
                }
            }
            i++;
        }
        else {
            i++;
        }
    }
}
void dodawanie() {
    for (int i = 0 ; i < ROZMIAR_TABLICY - 1; i++) {
        akumulator[i] += wielomian[i];
    }
}

void mnozenie() {
    int tablica_pomocnicza[ROZMIAR_TABLICY - 1] = {0};
    for (int i = 0; i < ROZMIAR_TABLICY - 1; i++) {
        for (int j = 0; j < ROZMIAR_TABLICY - 1; j++) {
            tablica_pomocnicza[i + j] += akumulator[i] * wielomian[j];
        }
    }
    for (int z = 0; z < ROZMIAR_TABLICY - 1; z++) {
        akumulator[z] = tablica_pomocnicza[z];
    }
}
void wypisywanie_wielomianu() {
    int s = ROZMIAR_TABLICY - 1;
    bool czypierwsze = false;
    while (s >= 0 && akumulator[s] == 0){
        s--;
    }
    if (s >= 0) {
        for (int i = s; i >= 0; i--) {
            int współczynnik = akumulator[i];
            if (współczynnik < 0 && !czypierwsze) {
                if (i == 0) {
                    printf("%d", -współczynnik);
                }
                else if (i == 1) {
                    printf("-x");
                }
                else{
                    if (współczynnik == -1) {
                        printf("-x^%d", i);
                    }
                    else {
                        printf("-%dx^%d", -współczynnik, i);
                    }
                }
                czypierwsze = true;
            }
            else if (współczynnik > 0 && !czypierwsze) {
                if (i == 0) {
                    printf("%d", współczynnik);
                }
                else if (i == 1) {
                    printf("x");
                }
                else {
                    if (współczynnik == 1) {
                        printf("x^%d", i);
                    }
                    else {
                        printf("%dx^%d", współczynnik, i);
                    }
                }
                czypierwsze = true;
            }
            else if (współczynnik > 0) {
                if (i == 0) {
                    printf(" + %d", współczynnik);
                }
                else if (i == 1){
                    if (współczynnik == 1) {
                        printf(" + x");
                    }
                    else {
                        printf(" + %dx", współczynnik);
                    }
                }
                else{
                    if (współczynnik == 1) {
                        printf(" + x^%d", i);
                    }
                    else {
                        printf(" + %dx^%d", współczynnik, i);
                    }
                }
            }
            else if (współczynnik < 0) {
                if (i == 0) {
                    printf(" - %d", -współczynnik);
                }
                else if (i == 1){
                    if (współczynnik == -1) {
                        printf(" - x");
                    }
                    else {
                        printf(" - %dx", -współczynnik);
                    }
                }
                else {
                    if (współczynnik == -1) {
                        printf(" - x^%d", i);
                    }
                    else {
                        printf(" - %dx^%d", -współczynnik, i);
                    }
                }
            }
        }
    }
    else {
        printf("0");
    }
}

int main() {
    zamień_wiersz_na_wielomian();
    int operacja = wielomian[ROZMIAR_TABLICY - 1];
    while (operacja != '.') {
        if (operacja == '+') {
            dodawanie();
            wypisywanie_wielomianu();
        }
        if (operacja == '*') {
            mnozenie();
            wypisywanie_wielomianu();
        }
        zamień_wiersz_na_wielomian();
        if ((operacja = wielomian[ROZMIAR_TABLICY - 1]) != '.') {
            printf("\n");
        }
    }
    return 0;
}
