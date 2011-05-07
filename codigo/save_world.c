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

#include "problem.h"

int main(int argc, char **argv) {
    char c;
    FILE *fin;
    problem_t p;
    int i, d, n, *coverage;

    /* Open input file: */
    if (argc < 2) {
        printf("Usage: saveWorld inst\n");
        return 0;
    } else {
        fin = fopen(argv[1], "r");
    }

    /* Read the world: */
    fscanf(fin, "N %d\n", &p.n_points);
    fscanf(fin, "M %d\n", &p.n_stations);
    p.stations = (station_t *) malloc(p.n_stations * sizeof(station_t));
    coverage = (int *) malloc((p.n_points + 1) * sizeof(int));
    for (i = 0; i < p.n_stations; i++) {
        fscanf(fin, "%*s %lf%c ", &p.stations[i].cost, &c);
        memset(coverage, 0, (p.n_points + 1) * sizeof(int));
        d = n = 0;
        while ((c = getc(fin)) != '\n') {
            if (isdigit(c)) {
                d = d * 10 + (c - '0');
            } else {
                coverage[d] = 1;
                d = 0;
                n++;
            }
        }
        p.stations[i].coverage =
            problem_coverage_create(p.n_points, n, coverage);
    }

    /* Save the world: */
    // heuristic();
    printf("N: %d\n", p.n_points);
    printf("M: %d\n", p.n_stations);
    for (i = 0; i < p.n_stations; i++) {
        printf("S%d %lf ", i+1, p.stations[i].cost);
        for (d = 0; d < p.stations[i].coverage.n; d++) {
            printf("%d ", p.stations[i].coverage.points[d]);
        }
        printf("\n");
    }

    /* Save the whales: */
    for (i = 0; i < p.n_stations; i++) {
        free(p.stations[i].coverage.points);
    }
    free(p.stations);
    free(coverage);
    fclose(fin);
    return 0;
}
