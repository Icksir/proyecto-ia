#include "../include/algoritmo.h" 
using namespace std;

void obtener_indices(Cromosoma &cromosoma, int m){    
    vector<int> indexes;
    for (size_t i = 0; i < cromosoma.clientes.size(); i++)
    {        
        if (cromosoma.clientes[i] == 0) indexes.insert(indexes.end(), i);
    }
    cromosoma.indices = indexes;
}

vector<int> crossover_points(vector<int> indices){
    random_device rd;
    mt19937 generador(rd());
    shuffle(indices.begin(), indices.end(), generador);
    vector<int> puntos;
    puntos.insert(puntos.end(), indices[0]);
    puntos.insert(puntos.end(), indices[1]);
    sort(puntos.begin(), puntos.end());
    return puntos;
}

pair<vector<int>, vector<int>> crossover(Cromosoma &padre1, Cromosoma &padre2, int m){
    int size1 = padre1.clientes.size();
    int size2 = padre2.clientes.size();
    obtener_indices(padre1, m);
    obtener_indices(padre2, m);
    vector<int> crossover_points1 = crossover_points(padre1.indices);
    vector<int> crossover_points2 = crossover_points(padre2.indices);

    if (crossover_points1[0] == 0 && crossover_points1[1] == size1 - 1) crossover_points(padre1.indices);
    if (crossover_points2[0] == 0 && crossover_points2[1] == size2 - 1) crossover_points(padre2.indices);
    
    vector<int> hijo1(size1, -1);
    vector<int> hijo2(size2, -1);

    for (int i = crossover_points1[0]; i <= crossover_points1[1]; ++i) {
        hijo1[i] = padre1.clientes[i];
    }
    for (int i = crossover_points2[0]; i <= crossover_points2[1]; ++i) {
        hijo2[i] = padre2.clientes[i];
    }

    for (size_t i = 0; i < padre1.indices.size(); i++) hijo1[padre1.indices[i]] = 0;    
    for (size_t i = 0; i < padre2.indices.size(); i++) hijo2[padre2.indices[i]] = 0;

    unordered_set<int> usados_hijo1(hijo1.begin(), hijo1.end());
    unordered_set<int> usados_hijo2(hijo2.begin(), hijo2.end());

    int indexChild1 = 0;
    int indexChild2 = 0;

    for (int i = 0; i < size1; ++i) {
        if (hijo1[i] == -1) {
            while (usados_hijo1.count(padre2.clientes[indexChild2]) > 0) {
                ++indexChild2;
            }
            hijo1[i] = padre2.clientes[indexChild2];
            usados_hijo1.insert(padre2.clientes[indexChild2]);
        }
    }

    for (int i = 0; i < size2; ++i) {
        if (hijo2[i] == -1) {
            while (usados_hijo2.count(padre1.clientes[indexChild1]) > 0) {
                ++indexChild1;
            }
            hijo2[i] = padre1.clientes[indexChild1];
            usados_hijo2.insert(padre1.clientes[indexChild1]);
        }
    }

    return make_pair(hijo1, hijo2);
}

void mutacion_swap(Cromosoma &cromosoma, int m, int n){
    obtener_indices(cromosoma, m);
    int size = cromosoma.indices.size();
    int indice1 = 0;
    int indice2 = 0;

    while (cromosoma.clientes[indice1] == 0) indice1 = index(0, n+size-1);
    while (cromosoma.clientes[indice2] == 0) indice2 = index(1, n+size-1);

    swap(cromosoma.clientes[indice1], cromosoma.clientes[indice2]);
}

void mutacion_afs(Cromosoma &cromosoma, int m, int n){
    int estacion_aleatoria = index(0, m-1);
    int size = cromosoma.estaciones.size();
    int index_aleatorio = index(0, size-2);

    while (cromosoma.estaciones[index_aleatorio] == -2) index_aleatorio = index(0, size-2);
    cromosoma.estaciones[index_aleatorio] = estacion_aleatoria;    
}

void main_algoritmo(vector<Cromosoma> &poblacion, vector<vector<float>> &matriz_costos, 
                int k, int m, int n, float prob_mutacion, int d_max, int t_max, float vel, 
                int t_srv, int t_rcg){
    int crom_1 = index(0, k-1);
    int crom_2 = index(0, k-1);
    while (crom_1 == crom_2) crom_2 = index(0, k-1);

    pair<vector<int>, vector<int>> hijos = crossover(poblacion[crom_1], poblacion[crom_2], m);
    int aleatorio = index(0,1);
    vector<int> hijo_elegido;
    int index_elegido;
    if (aleatorio == 0) 
    {
        hijo_elegido = hijos.first;
        index_elegido = crom_1;
    }
    else 
    {
        hijo_elegido = hijos.second;
        index_elegido = crom_2;
    }

    Cromosoma temporal = poblacion[index_elegido];
    temporal.clientes = hijo_elegido;

    aleatorio = index(0,1);
    if (random_number() < prob_mutacion) {
        if (aleatorio == 0) mutacion_swap(temporal, m, n);
        else mutacion_afs(temporal, m, n);
    }

    evaluacion_cromosoma(temporal, matriz_costos, m, d_max, t_max, vel, t_srv, t_rcg);

    // Buscar al peor cromosoma
    int indiceMayor = 0;
    for (int i = 0; i < k; i++) {
        if (poblacion[i].evaluacion > poblacion[indiceMayor].evaluacion) {
            indiceMayor = i;
        }
    }

    if (temporal.evaluacion < poblacion[indiceMayor].evaluacion) 
    {
        poblacion[indiceMayor] = temporal;
    }
}

/*
void marcoco(int kpop, int :3){
    if (:3 === true)
    {
        :3.stay();
        keep.:3();
    }
}
*/ 