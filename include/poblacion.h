#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
using namespace std;

struct Cromosoma {
    int v;
    int evaluacion;
    vector<int> clientes;
    vector<int> estaciones;
    vector<int> indices;
};

vector<int> permutacion(vector<int> cromosoma);
void generar_cliente(Cromosoma &crom_entero, int n);
int index(int inf, int sup);
void generar_estaciones(Cromosoma &crom_entero, int n, int m);
float random_number();
void generar_poblacion(vector<Cromosoma> &poblacion, vector<vector<float>> &matriz_costos, int k, int v_max, int n, int m);
bool es_invalido(Cromosoma crom_entero);
vector<int> generar_camino(Cromosoma &crom_entero, int m);
void evaluacion_cromosoma(Cromosoma &crom_entero, vector<vector<float>> &matriz_costos, int m, int d_max, int t_max, int vel, int t_srv, int t_rcg);
void evaluacion_poblacion(vector<Cromosoma> &poblacion, int k, int m, vector<vector<float>> &matriz_costos, int d_max, int t_max, int vel, int t_srv, int t_rcg);