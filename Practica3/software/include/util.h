#ifndef UTIL_H
#define UTIL_H

#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <utility>
#include "random.h"
#include <sstream>
#include <iostream>
#include <cstring>

const int NUM_PARAMETROS = 6;
const std::string PARAMETRO_10 = "10";
const std::string PARAMETRO_20 = "20";
const std::string PARAMETRO_GREEDY = "COPKM";
const std::string PARAMETRO_BUSQUEDA_LOCAL = "BL";
const std::string PARAMETRO_B_MULTIARRANQUE_BASICA = "BMB";
const std::string PARAMETRO_E_SIMULADO = "ES";
const std::string PARAMETRO_BL_REITERADA = "ILS";
const std::string PARAMETRO_HIBRIDACION = "ILS-ES";
const std::string PARAMETRO_IRIS = "I";
const std::string PARAMETRO_ECOLI = "E";
const std::string PARAMETRO_RAND = "R";
const std::string PARAMETRO_NEWTHYROID = "NT";
const std::string PARAMENTRO_MOSTRAR = "S";
const std::string PARAMENTRO_NO_MOSTRAR = "N";

enum Modo{GREEDY,BUSQUEDA_LOCAL,B_MULTIARRANQUE_BASICA, E_SIMULADO, BL_REITERADA, HIBRIDACION};
enum Conjunto{IRIS,ECOLI,RAND,NEWTHYROID};

struct Node{
	int cluster = -1;
	std::vector<double> datos;
};

struct Cluster{
	std::vector<double> centroide;
	std::vector<size_t> nodos;
};

// Compueba que los parametros son correctos y devuelve el metodo de resolucion del problema
Modo comprobarParametros(int num_parametros, char **parametros);

//Comprueba que el conjunto indicado es correcto y lo devuelve
Conjunto seleccionConjunto(char *nombre, char *parametro);

//Selecciona el archivo del que se leeran los elementos en funcion del conjunto
std::string abrirFicheroElementos(Conjunto conjunto);

//Selecciona el archivo del que se leeran las restricciones en funcion del conjunto
std::string abrirFicheroRestricciones(Conjunto conjunto, char *porcentaje);

//Almacena las semillas introducidas en un vector o devuelve por defecto {1, 5, 10, 20, 25}
std::vector<int> crearVectorSemillas(int num_parametros, char **parametros);

//Imprime los elementos agrupados en cada cluster
bool mostrarResultadoParticion(char *nombre, char *eleccion);

// Muestra por pantalla la forma de uso del programa
void mostrarUso(char*);

//Obtiene un token o palabra de una linea de texto
void get_token(std::istringstream &Line_stream1,std::string &Token1);

//Lee una nueva linea en un determinado fichero
bool get_new_line(std::ifstream &File,std::istringstream &Line_stream);

// Guarda la informacion de los nodos de un fichero en un vector de nodos
std::vector<Node> leerNodos(Conjunto con, std::ifstream&);

//Guarda la informacion de las restricciones debiles en una matriz de enteros
std::vector<std::vector<int>> leerRestricciones(Conjunto con, std::ifstream&);

//Calcula el vector promedio de las instancias de X que componen un cluster
std::vector<double> calcularCentroide(std::vector<Cluster> c, std::vector<Node> x, size_t clus);

void dimensionarParticion(Conjunto conjunto, std::vector<Cluster> &C);

//Asigna a cada cluster el vector promedio de las instancias de X que los componen
void centroides(std::vector<Cluster> &c, std::vector<Node> x);

//Asigna a cada cluster un centroide generado aleatoriamente
void centroidesAleatorios(std::vector<Cluster> &c, std::vector<Node> x);

//Calcula la distancia media intra-cluster de un cluster dado
double calcularDistancia(std::vector<Cluster> c, std::vector<Node> x, size_t clus);

//Calcula la desviacion general de la particion c
double evaluarDistancias(std::vector<Cluster> c, std::vector<Node> x);

//Calcula la desviacion general de la particion c (almacenando las distancias)
double evaluarDistancias(std::vector<double> &vec_dist, std::vector<Cluster> c, std::vector<Node> x);

//Obtiene la distancia máxima entre los elementos de un cluster
double buscarDistanciaMaxima(std::vector<Node> x);

//Calcula la infactibilidad o número de violaciones de restricciones
int evaluarRestricciones(std::vector<Cluster> c, std::vector<Node> x, std::vector<std::vector<int>> m);

//Calcula la infactibilidad o número de violaciones de restricciones (almacenando el número total de restricciones)
int evaluarRestricciones(size_t &num_res, std::vector<Cluster> c, std::vector<Node> x, std::vector<std::vector<int>> m);

//Algoritmo Greedy
void greedy(std::vector<Cluster> &C, std::vector<Node> &X, std::vector<std::vector<int>> M);

//Devuelve la infactibilidad que se obtendría al asignar un elemento a un determinado cluster
int probarCluster(std::vector<Cluster> c, std::vector<Node> x, std::vector<std::vector<int>> m, size_t nodo, size_t clus, size_t iteracion);

//Dada una lista de cluster, devuelve aquel cuyo centroide sea menos lejano a un elemento dado
size_t desempatar(int rest, size_t nodo, std::list<size_t> lista, std::vector<Cluster> c, std::vector<Node> x);

//Calcula la distancia entre dos elementos
double distanciaElem(std::vector<double> centroide, std::vector<double> elemento);

//Indica si la función objetivo al cambiar un elemento a un determinado cluster sería reducida
bool probarVecino(std::vector<Cluster> c, std::vector<Node> x, std::vector<std::vector<int>> m, double obj_antes, size_t clus, size_t elem);

//Calcula la función objetivo o función a minimizar
double funcionObjetivo(std::vector<Cluster> c, std::vector<Node> x, std::vector<std::vector<int>> m);

//Algoritmo de búsqueda local
void busquedaLocal(std::vector<Cluster> &C, std::vector<Node> &X, std::vector<std::vector<int>> M);

//Muestra por pantalla la partición C
void imprimirAsignaciones(std::vector<Cluster> C);

#endif
