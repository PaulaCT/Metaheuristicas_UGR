#ifndef TRAYECTORIA_H
#define TRAYECTORIA_H

#include <string>
#include <vector>
#include <list>
#include <utility>
#include "random.h"
#include <iostream>
#include <cstring>
#include "util.h"
#include <cstring>

const int EVALUACIONES_FOBJETIVO_ES = 100000;
const int EVALUACIONES_FOBJETIVO = 10000;
const int ITERACIONES = 10;
const int MAX_VECINOS = 10;
const double MAX_EXITOS = 0.1;
const double FI_MU = 0.3;
const double TEMPERATURA_FIN = 0.001;
const double P_MUTACION = 0.1;

// Genera la solución inicial y realiza el algoritmo correspondiente
std::vector<Cluster> generarInicial(Modo modo, size_t num_clus, std::vector<Node> &X, std::vector<std::vector<int>> M);

// Genera una soluación aleatoria
std::vector<size_t> solucion_aleatoria(std::vector<size_t> sol, size_t num_elem, size_t num_clus);

// Adapta un individuo a la representación usada para la primera práctica
std::vector<Cluster> convertir(std::vector<size_t> sol, size_t num_clus, std::vector<Node> &X);

// Verifica que ningún cluser quede vacio; si así fuera se cambiaría algún elemento aleatoriamente
std::vector<size_t> no_vacio(std::vector<size_t> sol, size_t num_clus);

// Función objetivo modificada para aceptar la representación de los datos usada para esta práctica
double funcion_objetivo_p3(std::vector<size_t> sol, std::vector<Node> x, std::vector<std::vector<int>> m, size_t num_clus);

// Modificación de la función infactibilidad adaptada a la nueva representación de los datos
int evaluar_restricciones_p3(size_t & num_res, std::vector<size_t> sol, std::vector<std::vector<int>> m);

// Operador de generación de vecinos
std::vector<size_t> cambio_cluster(std::vector<size_t> sol, size_t num_clus, size_t elem);

// Algoritmo de búsqueda local adaptado
std::vector<size_t> busqueda_local(std::vector<size_t> sol, size_t num_clus, std::vector<Node> X, std::vector<std::vector<int>> M);

// Algoritmo de enfriamiento simulado
std::vector<size_t> enfriamiento_simulado(Modo modo,std::vector<size_t> sol, size_t num_clus, std::vector<Node> X, std::vector<std::vector<int>> M);

// Operador de mutación brusco
std::vector<size_t> cambio_segmento(std::vector<size_t> sol, size_t num_clus);

// Algoritmo de búsqueda multiarranque básica
std::vector<size_t> busqueda_multiarranque(std::vector<size_t> sol, size_t num_clus, std::vector<Node> X, std::vector<std::vector<int>> M);

// Algoritmo de búsqueda local reiterada
std::vector<size_t> busqueda_local_reiterada(std::vector<size_t> sol, size_t num_clus, std::vector<Node> X, std::vector<std::vector<int>> M);

// Algoritmo de selección, devuelve la mejor solución entre dos
std::vector<size_t> torneo(std::vector<size_t> sol_1,std::vector<size_t> sol_2, std::vector<Node> x, std::vector<std::vector<int>> m, size_t num_clus);

// Algoritmo de hibridación: búsqueda local reiterada con enfriamiento simulado
std::vector<size_t> hibridacion(Modo modo, std::vector<size_t> sol, size_t num_clus, std::vector<Node> X, std::vector<std::vector<int>> M);

#endif
