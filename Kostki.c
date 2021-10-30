#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

int suma_oczek(void) {
    int liczba_oczek_1 = 1 + (int) (rand() / (RAND_MAX + 1.0) * 6);
    int liczba_oczek_2 = 1 + (int) (rand() / (RAND_MAX + 1.0) * 6);
    return liczba_oczek_1 + liczba_oczek_2;
}

int main(void) {
    srand((unsigned) time(NULL));
    int n, suma;
    int max = 0;
    int A[13] = {0};
    double skalar;
    scanf("%i", &n);
    for(int i=0; i<n; i++) {
        suma = (int) suma_oczek();
        A[suma]++;
    }
    for(int i=2; i<=12; i++) {
        if (A[i] > max) max = A[i];
    }
    skalar = (float) max / 20;
    for (int i = 2; i <= 12; i++) {
        A[i] /= skalar;
    }

    printf("+-----------------------+\n");
    for(int i=20; i>=2; i--) {
        printf("| ");
        for (int j = 0; j<11; j++) {
            if(A[j+2] > i) {
                printf("* ");
            }
            else printf("  ");
        }
        printf("|\n");
    }
    printf("+-----------------------+\n");
}





