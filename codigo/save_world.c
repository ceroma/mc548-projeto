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
#include "grasp.h"
#include "tabu.h"

int main(int argc, char **argv) {
    char c;
    FILE *fin;
    problem_t p;
    solution_t * solution;
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
        p.stations[i].name = problem_station_name_read(fin);
        fscanf(fin, "%lf%c ", &p.stations[i].cost, &c);
        memset(coverage, 0, (p.n_points + 1) * sizeof(int));
        d = n = 0;
        do {
            c = getc(fin);
            if (isdigit(c)) {
                d = d * 10 + (c - '0');
            } else {
                n += ((d == 0) || (coverage[d] != 0)) ? 0 : 1;
                coverage[d] = 1;
                d = 0;
            }
        } while (c != '\n');
        p.stations[i].n_covered = n;
        p.stations[i].coverage =
            problem_station_coverage_create(p.n_points, n, coverage);
    }

    /* Save the world: */
    solution = grasp(&p);
    d = 0;
    for (i = 0; i < p.n_stations; i++) {
        if (solution->plan[i]) {
            d++;
        }
    }
    printf("Valor: %lf\n", solution->cost);
    printf("Total: %d\n", d);
    for (i = 0; i < p.n_stations; i++) {
        if (solution->plan[i]) {
            printf("%s\n", p.stations[i].name);
        }
    }
    problem_solution_destroy(solution);

    /* Save the whales: */
    for (i = 0; i < p.n_stations; i++) {
        free(p.stations[i].name);
        free(p.stations[i].coverage);
    }
    free(p.stations);
    free(coverage);
    fclose(fin);
    return 0;
}
