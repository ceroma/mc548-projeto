/* Set of structs to represent an instance of the problem: */

/* 
 * Struct that represents an area covered by a station:
 * n: number of points covered by the station;
 * points: list of points covered by the station.
 */
typedef struct {
    int n;
    int * points;
} coverage_t;

/* 
 * Create an ordered list of covered points from a vector where the i-th posi-
 * tion is 1 if the i-th point is covered by the station. N is the number of
 * points in the world.
 */
coverage_t problem_coverage_create(int N, int n, int * points);

/* 
 * Struct that represents an station:
 * cost: cost to build the station;
 * coverage: area covered by the station.
 */
typedef struct {
    double cost;
    coverage_t coverage;
} station_t;

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
