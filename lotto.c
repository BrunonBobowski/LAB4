#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_LOTTO 49
#define LICZBA_SKRESLEN 6

static int losowanie_z_zakresu() {
    int cyfra = rand() % 49 + 1;
    return cyfra;
}

bool sprawdzenie_powtorzen(int a[], int n) {
    int r = 0;
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            if(a[i] == a[j]) {
                r++;
            }
        }
    }
    if(r > 6) {
        return true;
    }
    else {
        return false;
    }
}

static int * losowanie_kuponu() {
    static int a[LICZBA_SKRESLEN] = {0};
    for(int i=0; i<LICZBA_SKRESLEN; i++) {
        a[i] = losowanie_z_zakresu();
    }
    if(sprawdzenie_powtorzen(a, LICZBA_SKRESLEN)) {
        return losowanie_kuponu();
    }
    else {
        return a;
    }
}

static int * losowanie_kuponu2() {
    static int a[LICZBA_SKRESLEN] = {0};
    for(int i=0; i<LICZBA_SKRESLEN; i++) {
        a[i] = losowanie_z_zakresu();
    }
    if(sprawdzenie_powtorzen(a, LICZBA_SKRESLEN)) {
        return losowanie_kuponu2();
    }
    else {
        return a;
    }
}

static int iloczyn_kuponow(int a[], int b[]) {
    int iloczyn = 0;
    for(int i=0; i<LICZBA_SKRESLEN; i++) {
        for(int j=0; j<LICZBA_SKRESLEN; j++) {
            if (a[i] == b[j]) {
                iloczyn++;
            }
        }
    }
    return iloczyn;
}

int main() {
    int n;
    int trafione = 0;
    static int tablica_trafionych[LICZBA_SKRESLEN + 1] = {0};
    int *kupon_glowny = losowanie_kuponu2();
    scanf("%d", &n);

    for(int i=0; i<n; i++) {
        int *kupon = losowanie_kuponu();
        trafione = iloczyn_kuponow(kupon_glowny, kupon);
        tablica_trafionych[trafione] += 1;
    }
    for(int j=0; j<LICZBA_SKRESLEN + 1; j++){
        printf("%d: %d\n", j, tablica_trafionych[j]);
    }
}

