#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "problem.h"

#define NAME_SIZE_MULT 10

/* Allocate memory and read station's name from file. */
char * problem_station_name_read(FILE * fin) {
    int size = 0, alloc_count = 1;
    char c, *name = realloc(NULL, NAME_SIZE_MULT);

    /* Read and realloc memory if needed: */
    while ((c = getc(fin)) != ' ') {
       name[size++] = c;
       if (size >= NAME_SIZE_MULT * alloc_count) {
           alloc_count++;
           name = realloc(name, NAME_SIZE_MULT * alloc_count);
       }
    }
    name[size] = '\0';

    return name;
}

/* 
 * Create an ordered list of covered points from a vector where the i-th posi-
 * tion is 1 if the i-th point is covered by the station. N is the number of
 * points in the world and n is the number of covered points.
 */
int * problem_station_coverage_create(int N, int n, int * points) {
    int i, k = 0;
    int * coverage;

    coverage = (int *) malloc(n * sizeof(int));
    for (i = 1; i <= N; i++) {
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
