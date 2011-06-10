#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "problem.h"

#define MAX(a,b) (a > b) ? a : b;
#define MIN(a,b) (a < b) ? a : b;

#define ALPHA    0.6
#define MAX_TIME 60

/* Construct a greedy-randomized solution. */
solution_t * greedy_randomized_solution(problem_t * p) {
    int i, j, station, point;
    int num_covered_points = 0;
    double rand_prob, max_prob;
    double f_min, f_max, threshold, max_cost = 0.0;
    double *prob = (double *) malloc(p->n_stations * sizeof(double));
    int rcl_size, *rcl = (int *) malloc(p->n_stations * sizeof(int));
    solution_t *solution = problem_solution_create(p->n_stations);
    int *sol_coverage = (int *) malloc((p->n_points + 1) * sizeof(int));

    /* Find most expensive station: */
    for (i = 0; i < p->n_stations; i++) {
        max_cost = MAX(max_cost, p->stations[i].cost);
    }
    memset(sol_coverage, 0, (p->n_points + 1) * sizeof(int));

    while (num_covered_points != p->n_points) {
        /* Build restricted candidates list: */
        /* RCL - find f_min and f_max: */
        f_max = 0.0;
        f_min = max_cost;
        for (i = 0; i < p->n_stations; i++) {
            if (!solution->plan[i]) {
                f_max = MAX(f_max, p->stations[i].cost);
                f_min = MIN(f_min, p->stations[i].cost);
            }
        }

        /* RCL - select candidates: */
        rcl_size = 0;
        threshold = f_min + ALPHA * (f_max - f_min);
        for (i = 0; i < p->n_stations; i++) {
            if ((!solution->plan[i]) && (p->stations[i].cost <= threshold)) {
                rcl[rcl_size++] = i;
            }
        }

        /* Random-greedy choice: */
        /* RGC - calculate probabilities: */
        max_prob = 0.0;
        for (i = 0; i < rcl_size; i++) {
            prob[i] = 0.0;
            station = rcl[i];
            for (j = 0; j < p->stations[station].n_covered; j++) {
                point = p->stations[station].coverage[j];
                if (!sol_coverage[point]) {
                    prob[i]++;
                }
            }
            prob[i] = prob[i] / p->stations[station].cost;
            max_prob += prob[i];
        }

        /* RGC - choose one candidate: */
        rand_prob = (max_prob * ((double) rand())) / ((double) RAND_MAX);
        for (i = 0; i < rcl_size; i++) {
            if (rand_prob - prob[i] <= 0.0) {
                station = rcl[i];
                break;
            }
            rand_prob = rand_prob - prob[i];
        }

        /* New solution: */
        solution->plan[station] = 1;
        solution->cost += p->stations[station].cost;
        for (j = 0; j < p->stations[station].n_covered; j++) {
            if (sol_coverage[ p->stations[station].coverage[j] ] == 0) {
                num_covered_points++;
            }
            sol_coverage[ p->stations[station].coverage[j] ]++;
        }
    }

    free(rcl);
    free(prob);
    free(sol_coverage);

    return solution;
}

/* Perform a search for a local minimum starting with given solution. */
void local_search(problem_t * p, solution_t * solution) {
    int i, j, point, next_flip, skip;
    double max_cost = 0.0, min_cost = 0.0;
    int *sol_coverage = (int *) malloc((p->n_points + 1) * sizeof(int));

    /* Calculate coverage of given solution: */
    memset(sol_coverage, 0, (p->n_points + 1) * sizeof(int));
    for (i = 0; i < p->n_stations; i++) {
        if (solution->plan[i]) {
            max_cost += p->stations[i].cost;
            for (j = 0; j < p->stations[i].n_covered; j++) {
                sol_coverage[ p->stations[i].coverage[j] ]++;
            }
        }
    }

    /* Local search: */
    while (min_cost != max_cost) {
        next_flip = -1;
        min_cost = max_cost;
        /* Choose station to be built/destroyed: */
        for (i = 0; i < p->n_stations; i++) {
            skip = 0;
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
        }
        if (next_flip == -1) continue;

        /* Destroy/build i-th station, if better then current solution: */
        if (min_cost < solution->cost) {
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
        } else {
            min_cost = max_cost;
        }
    }

    free(sol_coverage);
}

/* Perform a Greedy Randomized Adaptive Search on problem p. */
solution_t * grasp(problem_t * p) {
    int i;
    solution_t *solution, *best_solution;
    time_t t0 = time(NULL);

    /* Initialize empty solutions: */
    best_solution = problem_solution_create(p->n_stations);
    for (i = 0; i < p->n_stations; i++) {
        best_solution->plan[i] = 1;
        best_solution->cost += p->stations[i].cost;
    }

    /* GRASP: */
    while ((time(NULL) - t0) < MAX_TIME) {
        /* Greedy randomized solution: */
        solution = greedy_randomized_solution(p);

        /* Local search: */
        local_search(p, solution);

        /* Compare with best solution: */
        if (solution->cost < best_solution->cost) {
            problem_solution_destroy(best_solution);
            best_solution = solution;
        } else {
            problem_solution_destroy(solution);
        }
    }

    return best_solution;
}
