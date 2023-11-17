#include "../include/inputs.h"
#include "../include/algoritmo.h"
using namespace std;

int main(int argc, char* argv[]) {
    unsigned t0;
    t0=clock();

    // Lectura de primeros datos ///////////////////
    vector<string> first_line;
    first_line = obtain_first_line(argv[1]);
    
    string filename;
    int n_clientes, n_estaciones, t_max, d_max, t_srv, t_rcg;
    float vel;

    filename = first_line[0];
    n_clientes = stoi(first_line[1]);
    n_estaciones = stoi(first_line[2]);
    t_max = stoi(first_line[3]);
    d_max = stoi(first_line[4]);
    vel = stof(first_line[5]);
    t_srv = stoi(first_line[6]);
    t_rcg = stoi(first_line[7]);
    ///////////////////////////////////////////////
    
    // MATRIZ DE COSTOS ///////////////////////////
    int n = n_clientes + n_estaciones; // Tamaño de la matriz cuadrada
    vector<vector<float>> matriz_costos(n, vector<float>(n, 0));
    rellenar_matriz(matriz_costos, n, argv[1]);
    ///////////////////////////////////////////////

    // PARÁMETROS /////////////////////////////////
    int k = 1000;     
    int v_max = 100;
    float prob_estaciones = 0.40;
    float prob_mutacion = 0.2;
    ///////////////////////////////////////////////

    // POBLACIÓN INICIAL //////////////////////////
    vector<Cromosoma> poblacion(k);
    generar_poblacion(poblacion, matriz_costos, k, v_max, n_clientes, n_estaciones, prob_estaciones);
    evaluacion_poblacion(poblacion, k, n_estaciones, matriz_costos, d_max, t_max, vel, t_srv, t_rcg);
    ///////////////////////////////////////////////
    
    // ALGORITMO PRINCIPAL ////////////////////////
    for (int i = 0; i < stoi(argv[2]); i++)
    {
        main_algoritmo(poblacion, matriz_costos, k, n_estaciones, n_clientes, 
                    prob_mutacion, d_max, t_max, vel, t_srv, t_rcg);
    }

    int indiceMenor = 0;
    for (int i = 0; i < k; i++) {
        if (poblacion[i].evaluacion < poblacion[indiceMenor].evaluacion) {
            indiceMenor = i;
        }
    }
    caminos_finales(poblacion[indiceMenor], n_estaciones, matriz_costos, vel, t_srv, t_rcg, d_max, t0, n_clientes);
    ///////////////////////////////////////////////

    return 0;      
} 