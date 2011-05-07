#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TIME 60
#define MEM_INIT 5

char * tabu_search(int **coverage, double *costs, int N, int M) {
    int i, j, next_flip, skip;
    double min_cost, max_cost, best_cost, sol_cost = 0.0;
    int *memory = (int *) malloc(M * sizeof(int));
    char *solution = (char *) malloc(M * sizeof(char));
    char *best_solution = (char *) malloc(M * sizeof(char));
    int *sol_coverage = (int *) malloc(N * sizeof(int));
    time_t t0 = time(NULL); 

    /* Building all stations is a valid solution: */
    memset(solution, 1, M);
    memset(best_solution, 1, M);
    memset(memory, 0, M * sizeof(int));
    memset(sol_coverage, 0, N * sizeof(int));
    for (i = 0; i < M; i++) {
        sol_cost += costs[i];
        for (j = 0; j < N; j++) {
          sol_coverage[j] += coverage[i][j];
        }
    }
    best_cost = min_cost = max_cost = sol_cost;

    /* Tabu search: */
    while ((time(NULL) - t0) < MAX_TIME) {
        min_cost = max_cost;
        /* Choose station to be built/destroyed: */
        for (i = 0; i < M; i++) { 
            skip = 0;
            if (!memory[i]) {
                /* Destroy: */
                if (solution[i]) {
                    /* Check if it will still be a solution: */
                    for (j = 0; j < N; j++) {
                        if ((sol_coverage[j] - coverage[i][j]) <= 0) {
                            skip = 1;
                        }
                    }
                    if (skip) continue;
                    /* Check if it is least costly neighbour: */
                    if ((sol_cost - costs[i]) < min_cost) {
                        next_flip = i;
                        min_cost = sol_cost - costs[i];
                    }
                /* Build: */
                } else {
                    /* Check if it will be a solution: */
                    for (j = 0; j < N; j++) {
                        if ((sol_coverage[j] + coverage[i][j]) <= 0) {
                            skip = 1;
                        }
                    }
                    if (skip) continue;
                    /* Check if it is least costly neighbour: */
                    if ((sol_cost + costs[i]) < min_cost) {
                        next_flip = i;
                        min_cost = sol_cost - costs[i];
                    }
                }
                memory[i] = MEM_INIT;
            } else {
                memory[i]--;
            }
        }

        /* Destroy/build i-th station: */
        if (solution[next_flip]) {
            solution[next_flip] = 0;
            sol_cost -= costs[next_flip];
            for (j = 0; j < N; j++) {
                sol_coverage[j] -= coverage[next_flip][j];
            }
        } else {
            solution[next_flip] = 1;
            sol_cost += costs[next_flip];
            for (j = 0; j < N; j++) {
                sol_coverage[j] += coverage[next_flip][j];
            }
        }

        /* Compare with best solution: */
        if (sol_cost < best_cost) {
            best_cost = sol_cost;
            memcpy(best_solution, solution, M);
        }
    }

    free(memory);
    free(solution);
    free(sol_coverage);

    return best_solution;
}
