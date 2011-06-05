#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "problem.h"

#define MAX_TIME 60

/* Perform a Greedy Randomized Adaptive Search on problem p. */
solution_t * grasp(problem_t * p) {
    int i;
    double best_cost, sol_cost = 0.0;
    solution_t *solution, *gr_solution, *best_solution;
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
        gr_solution = greedy_randomized_solution(p);

        /* Local search: */
        solution = local_search(p, gr_solution);

        /* Compare with best solution: */
        if (solution->cost < best_solution->cost) {
            problem_solution_destroy(best_solution);
            best_solution = solution;
        }
    }

    if (solution != best_solution) {
        free(solution);
    }

    return best_solution;
}
