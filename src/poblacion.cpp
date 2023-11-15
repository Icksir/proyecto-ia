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
void generar_estaciones(Cromosoma &crom_entero, int n, int m){
    
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
        if (random_number() < 0.33 && crom[i] != -2)
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
                            int m, int d_max, int t_max, int vel, int t_srv, int t_rcg){
    int valor = 0;
    int d_recorrida = 0;
    int t_tomado = 0;
    vector<int> ruta = generar_camino(crom_entero, m);    

    for (auto i: ruta) cout << i << ' ';
    cout << "\n\n";

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
                valor += (d_recorrida - d_max);
            }
            d_recorrida = 0;
        }

        // Penalización por excederse del tiempo
        if (ruta[i+1] == 0)
        {
            if (t_tomado > t_max)
            {
                valor += (t_tomado - t_max) * 20;
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
void generar_poblacion(vector<Cromosoma> &poblacion, vector<vector<float>> &matriz_costos, int k, int v_max, int n, int m){
    int v;
    for (int i = 0; i < k; i++)
    {
        v = index(3, v_max);
        poblacion[i].v = v;
        generar_cliente(poblacion[i], n);
        generar_estaciones(poblacion[i], n, m);
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
                            int d_max, int t_max, int vel, int t_srv, int t_rcg){
    for (int i = 0; i < k; i++)
    {
        evaluacion_cromosoma(poblacion[i], matriz_costos, m, d_max, t_max, vel, t_srv, t_rcg);
        
        // Debug
        cout << "v: " << poblacion[i].v << "\n";
        cout << "evaluacion: " << poblacion[i].evaluacion << "\n\n";
    }   
}