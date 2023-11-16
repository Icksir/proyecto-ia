#include "../include/inputs.h"
#include "../include/poblacion.h"
using namespace std;

int main(int argc, char* argv[]) {

    unsigned t0;
 
    t0=clock();

    // -------------------------------------------------------------------------------------
    // Lectura de primeros datos

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

    // -------------------------------------------------------------------------------------
    // Lectura de nodos

    int n = n_clientes + n_estaciones; // Tamaño de la matriz cuadrada
    vector<vector<float>> matriz_costos(n, vector<float>(n, 0));
    rellenar_matriz(matriz_costos, n, argv[1]);
    
    // Print de matriz de costos

    // print_matriz(matriz_costos, n); // Ejecución debug para visualizar matriz de costos

    // -------------------------------------------------------------------------------------
    // Generar población inicial

    int k = 5; // Cantidad de cromosomas en la población
    vector<Cromosoma> poblacion(k);

    // -------------------------------------------------------------------------------------

    int v_max = 10;

    generar_poblacion(poblacion, matriz_costos, k, v_max, n_clientes, n_estaciones);
    evaluacion_cromosoma(poblacion[0], matriz_costos, n_estaciones, d_max, t_max, vel, t_srv, t_rcg);
    // evaluacion_poblacion(poblacion, k, n_estaciones, matriz_costos, d_max, t_max, vel, t_srv, t_rcg);
    caminos_finales(poblacion[0], n_estaciones, matriz_costos, vel, t_srv, t_rcg, d_max, t0, n_clientes);
    // cout << "t_max: " << poblacion[0].v << "\n";



    return 0;      
} 