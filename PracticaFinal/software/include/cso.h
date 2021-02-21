#ifndef CSO_H
#define CSO_H

#include <vector>
#include <list>
#include "random.h"
#include "util.h"
#include "gato.h"
#include "pantera.h"

const int NUM_GATOS = 100;
const int MAX_ITER = 1000000;
const int MAX_ITER_MEM = 10000;
const int REPETICIONES = 10;
const int MAX_FALLOS = 3;
const int TOLERANCIA = 75;
const int TOLERANCIA_P = 10;

//Metodos para cambiar parámetros
void set_SMP(int smp);
void set_SRD(int srd);
void set_CDC(int cdc);
void set_SPC(bool spc);
void set_MR(int mr);
void set_CT(int ct);
void set_MR_P(float mr);
void set_CL(float cl);

//Algoritmo CSO
std::vector<Cluster> cso(size_t num_clus, std::vector<Node> &X, std::vector<std::vector<int>> M);

//Devuelve true si el elemento buscado pertenece a vector y lo elimina
bool poll(std::vector<size_t> &vector, size_t buscado);

//Algoritmo de conversión: convierte una posición gatuna a un vector de clusters
std::vector<Cluster> convertir(std::vector<size_t> posicion, size_t num_clus, std::vector<Node> &X);

//Algoritmo de búsqueda local suave
std::vector<size_t> busqueda_local_suave(std::vector<size_t> posicion, size_t num_clus, std::vector<Node> X, std::vector<std::vector<int>> M);

//Operador de mutación uniforme
std::vector<size_t> mutacion(std::vector<size_t> posicion, size_t num_clus);

//Algoritmo CSO memético
std::vector<Cluster> cso_mem(size_t num_clus, std::vector<Node> &X, std::vector<std::vector<int>> M);

//Función objetivo
double funcion_objetivo_gato_ext(std::vector<size_t> posicion, size_t numero_clusters, std::vector<Node> X, std::vector<std::vector<int>> M);

//Algoritmo CSO mejorado
std::vector<Cluster> cso_p(size_t num_clus, std::vector<Node> &X, std::vector<std::vector<int>> M);

//Divide el vector ind en dos vectores de tamaño tam e ind.size-tam
std::vector<size_t> dividir(std::vector<size_t> &ind, size_t tam);

#endif
