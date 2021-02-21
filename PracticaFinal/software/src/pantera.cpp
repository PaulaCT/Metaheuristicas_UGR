#include "pantera.h"
#include "cso.h"
#include "util.h"
#include "random.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <tgmath.h>

using namespace std;

size_t numero_clusters_p;
size_t max_fallos = 3;

void set_num_clus_p(size_t num){numero_clusters_p = num;};

Pantera::Pantera(size_t num_pos){
  for(size_t i=0; i<num_pos; i++){
    posicion.push_back(Randint(0, numero_clusters_p-1));
    velocidad.push_back(Randfloat(0,1));
  }
	no_vacio();
}

void Pantera::cambiar_comportamiento(Comportamiento accion){
  modo = accion;
}

Comportamiento Pantera::get_comportamiento(){
  return modo;
}

vector<size_t> Pantera::get_posicion(){
  return posicion;
}

void Pantera::no_vacio(){
  vector<size_t> check;
  size_t num_elem = posicion.size();
  for (size_t i=0; i<numero_clusters_p; i++) check.push_back(0);
  for (size_t i=0; i<num_elem; i++) check[posicion[i]]++;
  bool no_vacio = false;
  while (!no_vacio){
    for (size_t i=0; i<numero_clusters_p; i++){
      if (check[i]==0){
        size_t rand = Randint (0, num_elem-1);
        size_t aux = posicion[rand];
        posicion[rand] = i;
        check[i]++;
        check[aux]--;
        break;
      }
      if (check[i]!=0 && i==numero_clusters_p-1) no_vacio=true;
    }
  }
}


bool Pantera::mejor_fobj(vector<size_t> candidata, vector<Node> X, vector<vector<int>> M){
	double f_cand = funcion_objetivo_gato_ext(candidata, numero_clusters_p, X, M);
	if (f_cand < funcion_objetivo_pantera(X,M)) return true;
	return false;
}


vector<size_t> Pantera::cambio_cluster(size_t elem){
	size_t nuevo_cluster = Randint(0,numero_clusters_p-1);
	while (nuevo_cluster == posicion[elem]){
		nuevo_cluster = Randint(0,numero_clusters_p-1);
	}
	vector<size_t> vecina = posicion;
	vecina[elem] = nuevo_cluster;
	return vecina;
}

void Pantera::seeking(size_t SRD, bool SPC, vector<Node> X, vector<vector<int>> M){
	vector<vector<size_t>> posiciones;
	size_t dimensiones = posicion.size();
	double obj = funcion_objetivo_pantera(X,M);
	size_t actual = -1;
	if (SPC) actual = Randint(0, dimensiones-1);
	vector<size_t> indices = vector_desordenado(dimensiones);
	for (size_t i=0; i<dimensiones; i++){
		posiciones.push_back(posicion);
		if (i != actual) {
			posiciones[i][indices[i]] = (int)round(posiciones[i][indices[i]]*Randfloat(-1,1)*SRD)%numero_clusters_p;
		}
	}
	for (size_t i=0; i<dimensiones; i++){
		if (mejor_fobj(posiciones[i],X,M))
			posicion[indices[i]] = posiciones[i][indices[i]];
	}
  no_vacio();
}

void Pantera::tracing(size_t coeficiente, vector<size_t> mejor_posicion, vector<Node> X, vector<vector<int>> M){
  size_t tam = velocidad.size();
	vector<size_t> auxiliar;
	auxiliar.resize(posicion.size());
  for(size_t i=0; i<tam; i++){
    float r = Randfloat (0,1);
		//No tiene limite de velocidad
    velocidad[i]+=r*coeficiente*(mejor_posicion[i]-posicion[i]);
		velocidad[i] = (int)round(velocidad[i]) % numero_clusters_p;
    auxiliar[i] = (int)round(posicion[i] + velocidad[i]) % numero_clusters_p;
  }
	if (mejor_fobj(auxiliar,X,M)){
		for(size_t i=0; i<tam; i++){
	    posicion[i] = auxiliar[i];
	  }
	}
	no_vacio();
}

void Pantera::cleaning(size_t CDC, vector<Node> X, vector<vector<int>> M){
	size_t num_elem = posicion.size();
	vector<size_t> indices = Rand_indices(num_elem, CDC);
  size_t contador = 0;
  double obj = funcion_objetivo_pantera(X,M);
  while (contador < CDC){
    vector<size_t> posicion_vecino = cambio_cluster(indices[contador]);
    double obj_vecino = funcion_objetivo_gato_ext(posicion_vecino, numero_clusters_p,X,M);
    if (obj_vecino < obj){
      posicion = posicion_vecino;
      obj=obj_vecino;
    }
    contador++;
  }
  no_vacio();
}

double Pantera::funcion_objetivo_pantera(vector<Node> X, vector<vector<int>> M){
  return funcion_objetivo_gato_ext(posicion, numero_clusters_p, X, M);
}

Pantera::~Pantera(){
	delete[] this;
}
