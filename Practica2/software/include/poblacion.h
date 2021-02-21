#ifndef POBLACION_H
#define POBLACION_H

#include <string>
#include <vector>
#include <list>
#include <utility>
#include "random.h"
#include <iostream>
#include <cstring>
#include "util.h"
#include <cstring>

const int EVALUACIONES_FOBJETIVO = 100000;
const int POBLACION_AG = 50;
const int POBLACION_AM = 10;
const int PADRES_ESTACIONARIO = 2;
const int PADRES_GENERACIONAL = 50;
const double P_CRUCE_AGG = 0.7;
const double P_CRUCE_AGE = 1;
const double P_MUTACION_AG = 0.001;
const double P_CROMOSOMA_AM = 0.7;
const double P_GEN_AM = 0.001;
const double MAX_FALLOS = 0.1; //0.1 * n

// Genera la población y realiza el algoritmo correspondiente
std::vector<Cluster> generarPoblacion(Modo modo, size_t num_clus, std::vector<Node> &X, std::vector<std::vector<int>> M);

//Adapta un individuo a la representación usada para la práctica anterior
std::vector<Cluster> convertir(std::vector<size_t> individuo, size_t num_clus, std::vector<Node> &X);

//Operador de cruce uniforme
std::vector<size_t> cruce_uniforme(std::vector<size_t> padre1, std::vector<size_t> padre2, size_t num_clus);

//Operador de cruce por segmento fijo
std::vector<size_t> cruce_segmento_fijo(std::vector<size_t> padre1, std::vector<size_t> padre2, size_t num_clus);

//Verifica que ningún cluser quede vacio; si así fuera se cambiaría algún elemento aleatoriamente
std::vector<size_t> no_vacio(std::vector<size_t> hijo, size_t num_clus);

//Operador de mutación uniforme
std::vector<size_t> mutacion_uniforme(std::vector<size_t> hijo, size_t num_clus);

//Algoritmo de búsqueda local que solo recorre el individuo una vez
std::vector<size_t> busqueda_local_suave(std::vector<size_t> individuo, /*size_t max_fallos,*/ size_t num_clus, std::vector<Node> X, std::vector<std::vector<int>> M, size_t & evaluaciones);

//Operador de selección por torneo binario, selecciona dos individuos aleaorios y devuelve el mejor de ellos
std::vector<size_t> seleccion_torneo_binario(std::vector<std::vector<size_t>> poblacion, std::vector<Node> X, std::vector<std::vector<int>> M, size_t num_clus);

//Función objetivo modificada para aceptar la representación de los datos usada para esta práctica
double funcion_objetivo_p2(std::vector<size_t> individuo, std::vector<Node> x, std::vector<std::vector<int>> m, size_t num_clus);

//Modificación de la función infactibilidad adaptada a la nueva representación de los datos
int evaluar_restricciones_p2(size_t & num_res, std::vector<size_t> individuo, std::vector<std::vector<int>> m);

//Genera un número aleatorio y devuelve true si se encuentra por debajo de probabilidad
bool tirar_dado(double probabilidad);

//Algoritmo genérico
std::vector<std::vector<size_t>> algoritmo_generico(std::vector<std::vector<size_t>> poblacion, Modo modo, std::vector<Node> X, std::vector<std::vector<int>> M, size_t num_clus);

//Proceso de cruce para el modelo estacionario, selección del operador de cruce
std::vector<std::vector<size_t>> cruzar_estacionario(std::vector<std::vector<size_t>> poblacion, Modo modo, std::vector<Node> X, std::vector<std::vector<int>> M, size_t num_clus, size_t &evaluacion);

//Proceso de cruce para el modelo generacional, selección del operador de cruce
std::vector<std::vector<size_t>> cruzar_generacional(std::vector<std::vector<size_t>> poblacion, Modo modo, std::vector<Node> X, std::vector<std::vector<int>> M, size_t num_clus, size_t &evaluacion, std::vector<size_t> &posiciones);

//Proceso de mutación
std::vector<std::vector<size_t>> mutar(std::vector<std::vector<size_t>> poblacion, Modo modo, size_t num_clus, std::vector<size_t> & posiciones);

//Selecciona
//  modo -1: el peor cromosoma
//  modo  1: el mejor cromosoma
size_t sustituciones(std::vector<std::vector<size_t>> poblacion, int modo, size_t & evaluacion, std::vector<Node> X, std::vector<std::vector<int>> M, size_t num_clus);

//Devuelve la media de las funciones objetivo de todos los cromosomas de una población
double evaluar_poblacion(std::vector<std::vector<size_t>> poblacion, std::vector<Node> x, std::vector<std::vector<int>> m, size_t num_clus, size_t &evaluacion);

//Algoritmo memético
std::vector<std::vector<size_t>> algoritmo_memetico(std::vector<std::vector<size_t>> poblacion, Modo modo, std::vector<Node> X, std::vector<std::vector<int>> M, size_t num_clus);

//Proceso de selección de búsqueda local
std::vector<std::vector<size_t>> optimizar(std::vector<std::vector<size_t>> poblacion, Modo modo,/*size_t max_fallos,*/ size_t num_clus, std::vector<Node> X, std::vector<std::vector<int>> M, size_t & evaluaciones);

//Proceso de cruce y mutación para el modelo generacional
std::vector<std::vector<size_t>> cruzar_mejor(std::vector<std::vector<size_t>> poblacion, std::vector<Node> X, std::vector<std::vector<int>> M, size_t num_clus, size_t &evaluacion, std::vector<size_t> &posiciones);

//Devuelve true si vector contiene buscado
bool contains(std::vector<size_t> vector, size_t buscado);

//Si vector contiene buscado, lo elimina y devuelve true
bool poll(std::vector<size_t> &vector, size_t buscado);

//Imprime un cromosoma
void imprimirIndividuo(std::vector<size_t> individuo);

#endif
