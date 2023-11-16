#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include "poblacion.h"
#include <unordered_set>
using namespace std;

void obtener_indices(Cromosoma &cromosoma, int m);
vector<int> crossover_points(vector<int> indices);
pair<vector<int>, vector<int>>  crossover(Cromosoma &padre1, Cromosoma &padre2, int m);
void mutacion_swap(Cromosoma &cromosoma, int m, int n);
void main_algoritmo(vector<Cromosoma> &poblacion, vector<vector<float>> &matriz_costos, int k, int m, int n, float prob_mutacion, int d_max, int t_max, float vel, int t_srv, int t_rcg);