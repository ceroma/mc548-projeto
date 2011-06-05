#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "problem.h"

#define MAX_TIME 60
#define MEM_INIT 400

/* Perform a Tabu Search on problem p. */
solution_t * tabu_search(problem_t * p) {
    double min_cost, max_cost;
    int i, j, point, next_flip, skip;
    int *memory = (int *) malloc(p->n_stations * sizeof(int));
    int *sol_coverage = (int *) malloc((p->n_points + 1) * sizeof(int));
    solution_t *solution = problem_solution_create(p->n_stations);
    solution_t *best_solution = problem_solution_create(p->n_stations);
    time_t t0 = time(NULL);

    /* Building all stations is a valid solution: */
    memset(solution->plan, 1, p->n_stations);
    memset(best_solution->plan, 1, p->n_stations);
    memset(memory, 0, p->n_stations * sizeof(int));
    memset(sol_coverage, 0, (p->n_points + 1) * sizeof(int));
    for (i = 0; i < p->n_stations; i++) {
        solution->cost += p->stations[i].cost;
        for (j = 0; j < p->stations[i].n_covered; j++) {
            sol_coverage[ p->stations[i].coverage[j] ]++;
        }
    }
    best_solution->cost = min_cost = max_cost = solution->cost;

    /* Tabu search: */
    while ((time(NULL) - t0) < MAX_TIME) {
        next_flip = -1;
        min_cost = max_cost;
        /* Choose station to be built/destroyed: */
        for (i = 0; i < p->n_stations; i++) {
            skip = 0;
            if (!memory[i]) {
                /* Destroy: */
                if (solution->plan[i]) {
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
                    if ((solution->cost - p->stations[i].cost) < min_cost) {
                        next_flip = i;
                        min_cost = solution->cost - p->stations[i].cost;
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
                    if ((solution->cost + p->stations[i].cost) < min_cost) {
                        next_flip = i;
                        min_cost = solution->cost + p->stations[i].cost;
                    }
                }
            } else {
                memory[i]--;
            }
        }
        if (next_flip == -1) continue;

        /* Destroy/build i-th station: */
        memory[next_flip] = MEM_INIT;
        if (solution->plan[next_flip]) {
            solution->plan[next_flip] = 0;
            solution->cost -= p->stations[next_flip].cost;
            for (j = 0; j < p->stations[next_flip].n_covered; j++) {
                sol_coverage[ p->stations[next_flip].coverage[j] ]--;
            }
        } else {
            solution->plan[next_flip] = 1;
            solution->cost += p->stations[next_flip].cost;
            for (j = 0; j < p->stations[next_flip].n_covered; j++) {
                sol_coverage[ p->stations[next_flip].coverage[j] ]++;
            }
        }

        /* Compare with best solution: */
        if (solution->cost < best_solution->cost) {
            best_solution->cost = solution->cost;
            memcpy(best_solution->plan, solution->plan, p->n_stations);
        }
    }

    free(memory);
    free(sol_coverage);
    problem_solution_destroy(solution);

    return best_solution;
}
