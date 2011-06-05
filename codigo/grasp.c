#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "problem.h"

#define MAX_TIME 60

/* Perform a Greedy Randomized Adaptive Search on problem p. */
char * grasp(problem_t * p) {
    double best_cost, sol_cost = 0.0;
    char *solution, *gr_solution, *best_solution;
    time_t t0 = time(NULL);

    /* Initialize empty solutions: */
    best_solution = (char *) malloc(p->n_stations * sizeof(char));
    for (i = 0; i < p->n_stations; i++) {
        best_cost += p->stations[i].cost;
    }

    /* GRASP: */
    while ((time(NULL) - t0) < MAX_TIME) {
        /* Greedy randomized solution: */
        gr_solution = greedy_randomized_solution(p);

        /* Local search: */
        solution = local_search(gr_solution);
        sol_cost = X;

        /* Compare with best solution: */
        if (sol_cost < best_cost) {
            best_cost = sol_cost;
            memcpy(best_solution, solution, p->n_stations);
        }
        
    }

    free(solution);

    return best_solution;
}
