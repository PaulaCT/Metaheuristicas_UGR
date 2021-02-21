#include "gato.h"
#include "cso.h"
#include "util.h"
#include "random.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <tgmath.h>

using namespace std;

size_t numero_clusters;

void set_num_clus(size_t num){numero_clusters = num;};

Gato::Gato(size_t num_pos, double limite){
	this->limite = limite;
  for(size_t i=0; i<num_pos; i++){
    posicion.push_back(Randint(0, numero_clusters-1));
    velocidad.push_back(Randfloat(0,1));
  }
	no_vacio();
}

Gato::Gato(vector<size_t> posicion, double limite){
  size_t num_pos = posicion.size();
	this->limite = limite;
  for(size_t i=0; i<num_pos; i++){
    this->posicion.push_back(posicion[i]);
    velocidad.push_back(Randfloat(0,1));
  }
  no_vacio();
}

void Gato::cambiar_comportamiento(Comportamiento accion){
  modo = accion;
}

void Gato::cambiar_posicion(vector<size_t> pos){
	for (size_t i=0; i<posicion.size(); i++)
		posicion[i] = pos[i];
	no_vacio();
}

Comportamiento Gato::get_comportamiento(){
  return modo;
}

vector<size_t> Gato::get_posicion(){
  return posicion;
}

vector<double> Gato::probabilidad(vector<double> f_objetivo, size_t max, size_t min){
  size_t tam = f_objetivo.size();
  vector<double> prob;
  if (f_objetivo[min] != f_objetivo[max]){
    for(size_t i=0; i<tam; i++)
      prob.push_back((f_objetivo[i]-f_objetivo[max])/(f_objetivo[max]-f_objetivo[min])); ///
  } else {
    for(size_t i=0; i<tam; i++)
      prob.push_back(1);
  }
  return prob;
}

size_t Gato::seleccionar_mejor(vector<double> probabilidad){
  double mejor = 0;
  size_t tam = probabilidad.size();
  for (size_t i=0; i<tam; i++){
    if (probabilidad[i]>mejor) mejor = probabilidad[i];
    if (mejor == 1) break;
  }
  vector<size_t> mejores;
  for (size_t i=0; i<tam; i++){
    if (probabilidad[i] == mejor) mejores.push_back(i);
  }
	mejores = Randfromvector(mejores,1);
  return mejores[0];
}

void Gato::no_vacio(){
  vector<size_t> check;
  size_t num_elem = posicion.size();
  for (size_t i=0; i<numero_clusters; i++) check.push_back(0);
  for (size_t i=0; i<num_elem; i++) check[posicion[i]]++;
  bool no_vacio = false;
  while (!no_vacio){
    for (size_t i=0; i<numero_clusters; i++){
      if (check[i]==0){
        size_t rand = Randint (0, num_elem-1);
        size_t aux = posicion[rand];
        posicion[rand] = i;
        check[i]++;
        check[aux]--;
        break;
      }
      if (check[i]!=0 && i==numero_clusters-1) no_vacio=true;
    }
  }
}

void Gato::seeking(size_t SMP, size_t SRD, size_t CDC, bool SPC, vector<Node> X, vector<vector<int>> M){
  vector<vector<size_t>> posiciones;
  vector<double> f_objetivo;
  vector<size_t> candidatos;
  if (SPC) candidatos = Rand_indices(posicion.size(),SMP-1);
  else candidatos = Rand_indices(posicion.size(),SMP);
  double max = 0;
  double min = 100000;
  size_t f_max;
  size_t f_min;
  for (size_t i=0; i<candidatos.size(); i++){
    posiciones.push_back(posicion);
    vector<size_t> indices = Rand_indices(posicion.size(), CDC);
    for(size_t j=0; j<CDC; j++)
			posiciones[candidatos[i]][indices[j]] = (int)round(posiciones[candidatos[i]][indices[j]]*Randfloat(-1,1)*SRD)%numero_clusters; //SRD es entero
		double aux = funcion_objetivo_gato(X,M);
    if (aux > max){
      max = aux;
      f_max = candidatos[i];
    }
    if (aux < min){
      min = aux;
      f_min = candidatos[i];
    }
    f_objetivo.push_back(aux);
  }
	if (SPC){
			posiciones.push_back(posicion);
			double f_obj = funcion_objetivo_gato(X,M);
			f_objetivo.push_back(f_obj);
			if (f_obj > max){
				max = f_obj;
				f_max = candidatos[SMP-1];
			}
			if (f_obj < min){
				min = f_obj;
				f_min = candidatos[SMP-1];
			}
	}
  vector<double> prob = probabilidad(f_objetivo, f_max, f_min);
  posicion = posiciones[seleccionar_mejor(prob)];
  no_vacio();
}

void Gato::tracing(size_t coeficiente, vector<size_t> mejor_posicion){
  size_t tam = velocidad.size();
  for(size_t i=0; i<tam; i++){
    float r = Randfloat (0,1);
    if (velocidad[i]+r*coeficiente*(mejor_posicion[i]-posicion[i]) < limite)
      velocidad[i]+=r*coeficiente*(mejor_posicion[i]-posicion[i]);
    else velocidad[i] = limite;
    posicion[i] = (int)round(posicion[i] + velocidad[i]) % numero_clusters;
  }
	no_vacio();
}

double Gato::funcion_objetivo_gato(vector<Node> X, vector<vector<int>> M){
	return funcion_objetivo_gato_ext(posicion, numero_clusters, X, M);
}

Gato::~Gato(){
	delete[] this;
}
