#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>
#include <ctime> 
using namespace std;

vector<string> obtain_first_line(string file);
double haversine(double lon1, double lat1, double lon2, double lat2);
void rellenar_matriz(vector<vector<float>> &matriz_costos, int n, string file);
void print_variables(vector<string> variables);
void print_matriz(vector<vector<float>> &matriz_costos, int n);
