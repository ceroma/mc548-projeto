/* Construct a greedy-randomized solution. */
solution_t * greedy_randomized_solution(problem_t * p);

/* Perform a search for a local minimum starting with given solution. */
void local_search(problem_t * p, solution_t * solution);

/* Perform a Greedy Randomized Adaptive Search on problem p. */
solution_t * grasp(problem_t * p);
