/* Set of structs to represent an instance of the problem: */

/* 
 * Struct that represents an station:
 * cost: cost to build the station;
 * n_covered: number of points covered by the station;
 * coverage: points covered by the station.
 */
typedef struct {
    double cost;
    int n_covered;
    int * coverage;
} station_t;

/* 
 * Create an ordered list of covered points from a vector where the i-th posi-
 * tion is 1 if the i-th point is covered by the station. N is the number of
 * points in the world and n is the number of covered points.
 */
int * problem_coverage_create(int N, int n, int * points);

/* 
 * Struct that represents an instance of the problem:
 * n_points: number of points in the world;
 * n_stations: number of possible stations;
 * stations: list of possible stations.
 */
typedef struct {
    int n_points;
    int n_stations;
    station_t * stations;
} problem_t;

/*
 * Struct that represents a solution of the problem:
 * cost: cost of building proposed stations;
 * plan: 1 if i-th station should be built, 0 otherwise.
 */
typedef struct {
    double cost;
    char * plan;
} solution_t;

/* Create an empty solution, given the number M of stations. */
solution_t * problem_solution_create(int M);

/* Destroy (free memory of) given solution pointer. */
void problem_solution_destroy(solution_t * solution);
