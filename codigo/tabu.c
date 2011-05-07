#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "problem.h"

#define MAX_TIME 60
#define MEM_INIT 5

char * tabu_search(problem_t * p) {
    int i, j, point, next_flip, skip;
    double min_cost, max_cost, best_cost, sol_cost = 0.0;
    int *memory = (int *) malloc(p->n_stations * sizeof(int));
    char *solution = (char *) malloc(p->n_stations * sizeof(char));
    char *best_solution = (char *) malloc(p->n_stations * sizeof(char));
    int *sol_coverage = (int *) malloc((p->n_points + 1) * sizeof(int));
    time_t t0 = time(NULL); 

    /* Building all stations is a valid solution: */
    memset(solution, 1, p->n_stations);
    memset(best_solution, 1, p->n_stations);
    memset(memory, 0, p->n_stations * sizeof(int));
    memset(sol_coverage, 0, (p->n_points + 1) * sizeof(int));
    for (i = 0; i < p->n_stations; i++) {
        sol_cost += p->stations[i].cost;
        for (j = 0; j < p->stations[i].n_covered; j++) {
            sol_coverage[ p->stations[i].coverage[j] ]++;
        }
    }
    best_cost = min_cost = max_cost = sol_cost;

    /* Tabu search: */
    while ((time(NULL) - t0) < MAX_TIME) {
        next_flip = -1;
        min_cost = max_cost;
        /* Choose station to be built/destroyed: */
        for (i = 0; i < p->n_stations; i++) { 
            skip = 0;
            if (!memory[i]) {
                /* Destroy: */
                if (solution[i]) {
                    /* Check if it will still be a solution: */
                    for (j = 0; j < p->stations[i].n_covered; j++) {
                        point = p->stations[i].coverage[j];
                        if ((sol_coverage[point] - 1) <= 0) {
                            skip = 1;
                            break;
                        }
                    }
                    if (skip) continue;
                    /* Check if it is least costly neighbour: */
                    if ((sol_cost - p->stations[i].cost) < min_cost) {
                        next_flip = i;
                        min_cost = sol_cost - p->stations[i].cost;
                    }
                /* Build: */
                } else {
                    /* Check if it will be a solution: */
                    for (j = 0; j < p->stations[i].n_covered; j++) {
                        point = p->stations[i].coverage[j];
                        if ((sol_coverage[point] + 1) <= 0) {
                            skip = 1;
                            break;
                        }
                    }
                    if (skip) continue;
                    /* Check if it is least costly neighbour: */
                    if ((sol_cost + p->stations[i].cost) < min_cost) {
                        next_flip = i;
                        min_cost = sol_cost - p->stations[i].cost;
                    }
                }
                memory[i] = MEM_INIT;
            } else {
                memory[i]--;
            }
        }
        if (next_flip == -1) continue;

        /* Destroy/build i-th station: */
        if (solution[next_flip]) {
            solution[next_flip] = 0;
            sol_cost -= p->stations[next_flip].cost;
            for (j = 0; j < p->stations[next_flip].n_covered; j++) {
                sol_coverage[ p->stations[next_flip].coverage[j] ]--;
            }
        } else {
            solution[next_flip] = 1;
            sol_cost += p->stations[next_flip].cost;
            for (j = 0; j < p->stations[next_flip].n_covered; j++) {
                sol_coverage[ p->stations[next_flip].coverage[j] ]++;
            }
        }

        /* Compare with best solution: */
        if (sol_cost < best_cost) {
            best_cost = sol_cost;
            memcpy(best_solution, solution, p->n_stations);
        }
    }

    free(memory);
    free(solution);
    free(sol_coverage);

    return best_solution;
}
