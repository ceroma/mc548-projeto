/********************************************/
/*         MC548 - Trabalho Prático         */
/*                                          */
/* RA059369 - Bruno Makoto Nakamura Devidis */
/* RA059582 - Carlos Eduardo Rosa Machado   */
/* RA061465 - Hugo Hideki Yamashita         */
/********************************************/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    char c;
    int i, p;
    FILE *fin;
    double *costs;
    int M, N, **coverage;

    /* Open input file: */
    if (argc < 2) {
        printf("Usage: saveWorld inst\n");
        return 0;
    } else {
        fin = fopen(argv[1], "r");
    }

    /* Read the world: */
    fscanf(fin, "N %d\n", &N);
    fscanf(fin, "M %d\n", &M);
    costs = (double *) malloc(M * sizeof(double));
    coverage = (int **) malloc(M * sizeof(int*));
    for (i = 0; i < M; i++) {
        fscanf(fin, "%*s %lf%c ", &costs[i], &c); 
        coverage[i] = (int *) malloc(N * sizeof(int));
        memset(coverage[i], 0, N * sizeof(int));
        p = 0;
        while ((c = getc(fin)) != '\n') {
            if (isdigit(c)) {
                p = p * 10 + (c - '0');
            } else {
                coverage[i][p-1] = 1;
                p = 0;
            }
        }
    }

    /* Save the world: */
    // heuristic();
    printf("N: %d\n", N);
    printf("M: %d\n", M);
    for (i = 0; i < M; i++) {
        printf("S%d %lf ", i+1, costs[i]);
        for (p = 0; p < N; p++) {
            if (coverage[i][p]) {
                printf("%d ", p+1);
            }
        }
        printf("\n");
    }

    fclose(fin);
    return 0;
}
