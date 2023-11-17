#include "../include/poblacion.h" 
using namespace std;

// -------------------------------------------------------
// POBLACIÓN
// -------------------------------------------------------

/*
permutacion: permuta un cromosoma
--------------------------------------------------------- 
vector<int> cromosoma: un cromosoma
*/
vector<int> permutacion(vector<int> cromosoma){
    random_device rd;
    mt19937 gen(rd());
    shuffle(cromosoma.begin(), cromosoma.end(), gen);
    return cromosoma;
}

/*
index: da un número aleatorio entre un rango
--------------------------------------------------------- 
int inf: rango inferior
int sup: rango superior
*/
int index(int inf, int sup){
    random_device                  rand_dev;
    mt19937                        generator(rand_dev());
    uniform_int_distribution<int>  distr(inf, sup);

    return distr(generator);
}

/*
generar_cliente: Genera un cromosoma de forma aleatoria con nodos de clientes
---------------------------------------------------------
Cromosoma &crom_entero: cromosoma
int v: cantidad de vehiculos
int n: cantidad de clientes
*/
void generar_cliente(Cromosoma &crom_entero, int n){

    // Vector con clientes
    vector<int> secuencia(n);
    for (int i = 0; i < n; i++) secuencia[i] = i + 1;
    vector<int> crom_permutado(n);
    crom_permutado = permutacion(secuencia);

    // Vector dada representación
    int v = crom_entero.v;
    vector<int> crom(n + v + 1, -1);
    vector<int> indexes(v - 1);
    int v_index;

    // Generar índices para cerrar las rutas
    for (int i = 0; i < v - 1; i++)
    {
        v_index = index(2, n + v - 1);
        if(find(indexes.begin(), indexes.end(), v_index) != indexes.end()) {
            i--;
        } else {
            indexes[i] = v_index;
            crom[v_index] = 0;
        }
    }
    crom[0] = 0;
    crom[n+v] = 0;

    // Completar rutas con permutación
    for (int i = 0; i < n + v + 1; i++)
    {
        if (crom[i] != 0)
        {
            crom[i] = crom_permutado.front();
            crom_permutado.erase(crom_permutado.begin());
        }
    }

    crom_entero.clientes = crom;
    crom_entero.indices = indexes;
    
    // For debug
    // for (auto i: crom) cout << i << ' ';
}

/*
random_number: Genera un número random entre 0 y 1
---------------------------------------------------------
Sin argumentos
*/
float random_number(){
    random_device rd;
    mt19937 e2(rd());
    uniform_real_distribution<float> dist(0, 1);
    return dist(e2);
}

/*
generar_estaciones: Genera un cromosoma de forma aleatoria con nodos de estaciones de recarga
---------------------------------------------------------
Cromosoma &crom_entero: cromosoma
int v: cantidad de vehiculos
int n: cantidad de clientes
*/
void generar_estaciones(Cromosoma &crom_entero, int n, int m, float prob_estaciones){
    
    // Inicializa cromosoma e inhabilita zonas prohibidas
    int v = crom_entero.v;
    vector<int> crom(n + v, -1);
    crom[0] = -2;
    crom[n + v - 1] = -2;

    for (int i: crom_entero.indices)
    {
        crom[i] = -2;
        crom[i-1] = -2;
    }

    // Rellenar con estaciones de servicio aleatorias con 0.3 de probabilidad
    for (int i = 0; i < n + v; i++)
    {
        if (random_number() < 0.45 && crom[i] != -2)
        {
            crom[i] = index(0, m);
        }
    }

    crom_entero.estaciones = crom;

    // For debug
    // for (auto i: crom) cout << i << ' ';
}

/*
es_invalido: comprueba que dos depot no estén juntos
---------------------------------------------------------
Cromosoma crom_entero: cromosoma a verificar
*/
bool es_invalido(Cromosoma crom_entero){
    for (size_t i = 1; i < crom_entero.clientes.size(); i++)
    {
        if (crom_entero.clientes[i] == crom_entero.clientes[i-1])
        {
            return true;
        }
        
    }
    return false;
}

/*
generar_camino: Genera camino único con los clientes y la estación de servicio
---------------------------------------------------------
Cromosoma crom_entero
*/
vector<int> generar_camino(Cromosoma &crom_entero, int m){
    vector<int> ruta;
    ruta.insert(ruta.end(),0);
    for (size_t i = 0; i < crom_entero.clientes.size() - 1; i++)
    {
        if (crom_entero.estaciones[i] < 0)
        {   
            if (crom_entero.clientes[i+1] < 1) ruta.insert(ruta.end(),crom_entero.clientes[i+1]);
            else {ruta.insert(ruta.end(),crom_entero.clientes[i+1] + m - 1);}
        }
        else{
            ruta.insert(ruta.end(),crom_entero.estaciones[i]);
            if (crom_entero.clientes[i+1] < 1) ruta.insert(ruta.end(),crom_entero.clientes[i+1]);
            else {ruta.insert(ruta.end(),crom_entero.clientes[i+1] + m - 1);}
        }
    }
    // For debug
    // for (auto i: ruta) cout << i << ' ';
    // cout << "\n\n";
    return ruta;
}

/*
evaluacion_cromosoma: Evalúa un cromosoma
---------------------------------------------------------
Cromosoma &crom_entero
vector<vector<float>> matriz_costos
int m
*/
void evaluacion_cromosoma(Cromosoma &crom_entero, vector<vector<float>> &matriz_costos, 
                            int m, int d_max, int t_max, float vel, int t_srv, int t_rcg){
    float valor = 0;
    float d_recorrida = 0;
    int t_tomado = 0;
    vector<int> ruta = generar_camino(crom_entero, m);    

    // For debug
    // for (auto i: ruta) cout << i << ' ';
    // cout << "\n\n";

    for (size_t i = 0; i < ruta.size() - 1; i++)
    {   
        valor += matriz_costos[ruta[i]][ruta[i+1]];
        d_recorrida += matriz_costos[ruta[i]][ruta[i+1]];
        t_tomado += matriz_costos[ruta[i]][ruta[i+1]] * vel;

        if (ruta[i+1] < m) t_tomado += t_rcg;
        else{ t_tomado += t_srv;}

        // Penalización por distancia sin estación de recarga
        // La penalización es por millas excedidas y se reinicia al haber una estación de servicio
        if (ruta[i+1] < m)
        {
            if (d_recorrida > d_max)
            {
                valor += (d_recorrida - d_max) * 100;
            }
            d_recorrida = 0;
        }

        // Penalización por excederse del tiempo
        if (ruta[i+1] == 0)
        {
            if (t_tomado > t_max)
            {
                valor += (t_tomado - t_max) * 100;
            }
            t_tomado = 0;
        }
    }
    crom_entero.evaluacion = valor;
}

/*
generar_poblacion: Genera la cantidad de cromosomas indicados
---------------------------------------------------------
vector<Cromosomas> poblacion: población de cromosomas
int k: cantidad de cromosomas
int v_max: cantidad de vehiculos máximos a generar
int n: cantidad de clientes
int m: cantidad de estaciones
*/
void generar_poblacion(vector<Cromosoma> &poblacion, vector<vector<float>> &matriz_costos, int k, int v_max, int n, int m, float prob_estaciones){
    int v;
    for (int i = 0; i < k; i++)
    {
        v = index(3, v_max);
        poblacion[i].v = v;
        generar_cliente(poblacion[i], n);
        generar_estaciones(poblacion[i], n, m, prob_estaciones);
        if (es_invalido(poblacion[i])) i--;
    }
}

/*
evaluacion_poblacion: Evaluación población
---------------------------------------------------------
vector<Cromosomas> poblacion: población de cromosomas
int k: cantidad de cromosomas
int m: cantidad de estaciones
*/
void evaluacion_poblacion(vector<Cromosoma> &poblacion, int k, int m, vector<vector<float>> &matriz_costos, 
                            int d_max, int t_max, float vel, int t_srv, int t_rcg){
    for (int i = 0; i < k; i++)
    {
        evaluacion_cromosoma(poblacion[i], matriz_costos, m, d_max, t_max, vel, t_srv, t_rcg);
    }   
}

/*
caminos_finales: Genera caminos para printearlos por consola
---------------------------------------------------------
Cromosoma &mejor_evaluado: cromosoma mejor evaluado
int m: cantidad de estaciones
vector<vector<float>> &matriz_costos: matriz de costos
float vel: velocidad de los vehículos
int t_srv: tiempo de servicio a los clientes
int t_rcg: tiempo de recarga
int d_max: distancia máxima 
unsigned t0: tiempo para calcular tiempo de ejecución
int n: cantidad de clientes
*/
void caminos_finales(Cromosoma &crom_entero, int m, vector<vector<float>> &matriz_costos,
                    float vel, int t_srv, int t_rcg, int d_max, unsigned t0, int n){
    vector<int> ruta = generar_camino(crom_entero, m);  

    float distancia = 0;
    float distancia_total = 0;
    int tiempo_total = 0;
    float distancia_excedida = 0;
    float distancia_dmax = 0;
    float distancia_excedida_total = 0;
    int vehiculos = 0;
    
    vector<int> camino;

    // For debug
    // for (auto i: ruta) cout << i << ' ';
    // cout << "\n\n";

    for (size_t i = 0; i < ruta.size() - 1; i++)
    {
        camino.insert(camino.end(), ruta[i+1]);
        if (ruta[i+1] == 0)
        {
            camino.insert(camino.begin(), 0);
            vehiculos++;

            // Calcular tiempo y distancia 
            for (size_t j = 0; j < camino.size() - 1; j++)
            {
                distancia += matriz_costos[camino[j]][camino[j+1]];      
            }        
            camino.clear();
            distancia_total += distancia;
            distancia = 0;
            tiempo_total = 0;
            distancia_excedida = 0;
            distancia_excedida_total = 0;
        }
    }

    unsigned t1 = clock();
    double time = (double(t1-t0)/CLOCKS_PER_SEC);

    cout << distancia_total << " " << n << " " << vehiculos << " " << time << "\n\n";

    for (size_t i = 0; i < ruta.size() - 1; i++)
    {
        camino.insert(camino.end(), ruta[i+1]);
        if (ruta[i+1] == 0)
        {
            camino.insert(camino.begin(), 0);
            for (size_t j = 0; j < camino.size() - 1; j++)
            {
                if (camino[j] < m && camino[j] != 0) cout << "F" << camino[j] << "-";
                else if (camino[j] > m - 1) cout << "C" << camino[j] - m + 1 << "-";
                else if (camino[j] == 0) cout << "D0-";
            }
            cout << "D0 ";

            // Calcular tiempo y distancia 
            for (size_t j = 0; j < camino.size() - 1; j++)
            {
                distancia += matriz_costos[camino[j]][camino[j+1]];
                distancia_dmax += matriz_costos[camino[j]][camino[j+1]];
                tiempo_total += matriz_costos[camino[j]][camino[j+1]] * vel;
                if (camino[j] < m && camino[j] != 0) tiempo_total += t_rcg;
                else if (camino[j] > m - 1) tiempo_total += t_srv;

                if (distancia_dmax > d_max) distancia_excedida = distancia_dmax - d_max;
                if (camino[j+1] < m)
                {
                    distancia_excedida_total += distancia_excedida;
                    distancia_dmax = 0;
                    distancia_excedida = 0;
                }                
            }        
            cout << distancia << " " << tiempo_total << " " << distancia_excedida_total;
            cout << "\n\n";
            camino.clear();
            distancia = 0;
            tiempo_total = 0;
            distancia_excedida = 0;
            distancia_excedida_total = 0;
        }
    }
}