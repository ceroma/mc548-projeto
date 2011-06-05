#include <stdlib.h>
#include <string.h>
#include "problem.h"

/* 
 * Create an ordered list of covered points from a vector where the i-th posi-
 * tion is 1 if the i-th point is covered by the station. N is the number of
 * points in the world and n is the number of covered points.
 */
int * problem_coverage_create(int N, int n, int * points) {
    int i, k = 0;
    int * coverage;

    coverage = (int *) malloc(n * sizeof(int));
    for (i = 0; i <= N; i++) {
        if (points[i]) {
            coverage[k++] = i;
        }
    }
    return coverage;
}

/* Create an empty solution, given the number M of stations. */
solution_t * problem_solution_create(int M) {
    solution_t * solution = (solution_t *) malloc(sizeof(solution_t));
    solution->cost = 0.0;
    solution->plan = (char *) malloc(M * sizeof(char));
    memset(solution->plan, 0, M);
    return solution;
}

/* Destroy (free memory of) given solution pointer. */
void problem_solution_destroy(solution_t * solution) {
    free(solution->plan);
    free(solution);
}
