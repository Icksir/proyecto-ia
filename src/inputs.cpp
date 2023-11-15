#include "../include/inputs.h" 
using namespace std;

// -------------------------------------------------------
// LECTURA DE ARCHIVOS
// -------------------------------------------------------

/*
obtain_first_line: lee la primera linea
--------------------------------------------------------- 
string file: archivo a leer
*/
vector<string> obtain_first_line(string file){
    ifstream exprFile(file);
    string singleExpr;
    getline(exprFile, singleExpr);

    istringstream iss(singleExpr);
    vector<string> tokens;
    string token;

    while (iss >> token) {
        tokens.push_back(token);
    }

    return tokens;
}

/*
print_variables: printea un vector de strings (para debug)
--------------------------------------------------------- 
vector<string> variables: vector con strings
*/
void print_variables(vector<string> variables){
    for (auto i: variables) cout << i << ' ';
}

/*
haversine: calcula la distancia en millas dada latitudes y longitudes
https://www.geeksforgeeks.org/haversine-formula-to-find-distance-between-two-points-on-a-sphere/
--------------------------------------------------------- 
double lon1: longitud 1
double lat1: latitud 1
double lon2: longitud 2
double lat2: latitud 2
*/
double haversine(double lon1, double lat1, double lon2, double lat2){
        // distance between latitudes
        // and longitudes
        double dLat = (lat2 - lat1) *
                      M_PI / 180.0;
        double dLon = (lon2 - lon1) * 
                      M_PI / 180.0;
 
        // convert to radians
        lat1 = (lat1) * M_PI / 180.0;
        lat2 = (lat2) * M_PI / 180.0;
 
        // apply formulae
        double a = pow(sin(dLat / 2), 2) + 
                   pow(sin(dLon / 2), 2) * 
                   cos(lat1) * cos(lat2);
        double rad = 4182.45;
        double c = 2 * asin(sqrt(a));
        return rad * c;
    }

/*
rellenar_matriz: rellena la matriz de costos
--------------------------------------------------------- 
vector<vector<float>> &matriz_costos
int n: número de clientes y estaciones de servicio
string file: archivo a leer
*/
void rellenar_matriz(vector<vector<float>> &matriz_costos, int n, string file){
    // Ignorar las dos primeras líneas de los archivos
    ifstream exprFile(file);
    string singleExpr;
    getline(exprFile, singleExpr);
    getline(exprFile, singleExpr);

    vector<string> tokens(5);
    string token;
    vector<vector<double>> long_lat(n, vector<double>(2, 0));

    for (int i = 0; i < n; i++) {
        getline(exprFile, singleExpr);
        istringstream iss(singleExpr);
        for (int j = 0; j < 5; j++){
            tokens[j] = token;
            iss >> token;
        }   
        long_lat[i][0] = stod(tokens[3]);
        long_lat[i][1] = stod(tokens[4]);
    }
    
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            matriz_costos[i][j] = haversine(long_lat[i][0], long_lat[i][1], long_lat[j][0], long_lat[j][1]);
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            matriz_costos[i][j] = haversine(long_lat[i][0], long_lat[i][1], long_lat[j][0], long_lat[j][1]);
        }
    }
}

/*
print_matriz: printea la matriz de costos
--------------------------------------------------------- 
vector<int> cromosoma: un cromosoma
*/
void print_matriz(vector<vector<float>> &matriz_costos, int n){
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << matriz_costos[i][j] << " ";
        }
        std::cout << std::endl;
    }
}