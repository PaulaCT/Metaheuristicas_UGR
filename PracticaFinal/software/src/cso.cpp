#include "cso.h"
#include "util.h"
#include "random.h"
#include "gato.h"
#include "pantera.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <random>

using namespace std;

float MR = 0.7;
float MR_P = 0.5;
float CL = 0.1;
int SMP = 100;
int SRD = 6;
int CDC = 50;
int CT = 6;
bool SPC = true;

double max_dist_cso;
size_t restricciones_cso;


void set_SMP(int smp){SMP = smp;}
void set_SRD(int srd){SRD = srd;}
void set_CDC(int cdc){CDC = cdc;}
void set_SPC(bool spc){SPC = spc;}
void set_MR(int mr){MR = mr;}
void set_CT(int ct){CT = ct;}
void set_MR_P(float mr){MR_P = mr;}
void set_CL(float cl){CL = cl;}

vector<Cluster> cso(size_t num_clus, vector<Node> &X, vector<vector<int>> M){
  //Inicialización
  size_t dimensiones = X.size();
  set_SMP(dimensiones);
  set_SRD(num_clus);
  set_CDC(dimensiones/3);
  set_MR(0.6);
  vector<Gato *> Swarm;
  size_t mejor_gato;
  double min = 1000;
  //Generación de gatos
  Swarm.resize(NUM_GATOS);
  for(size_t i=0; i<NUM_GATOS; i++){
    Swarm[i] = new Gato(dimensiones,num_clus-1);
    double aux = Swarm[i]->funcion_objetivo_gato(X,M);
    if (aux < min){
      mejor_gato = i;
      min = aux;
    }
  }
  //Comienzo iteraciones
  size_t iteraciones = 0;
  size_t fallos = 0;
  while (iteraciones < MAX_ITER && fallos < TOLERANCIA){
    //Asignación de comportamiento
    vector<size_t> indices = Rand_indices(NUM_GATOS, round(MR*NUM_GATOS));
    for(size_t i=0; i<NUM_GATOS; i++){
      if (poll(indices,i)) Swarm[i]->cambiar_comportamiento(SEEKING);
      else Swarm[i]->cambiar_comportamiento(TRACING);
    }
    bool cambio = false;
    //Ejecución de acciones
    for(size_t i=0; i<NUM_GATOS; i++){
      if (Swarm[i]->get_comportamiento() == SEEKING)
        Swarm[i]->seeking(SMP,SRD,CDC,SPC,X,M);
      else
        Swarm[i]->tracing(CT, Swarm[mejor_gato]->get_posicion());
      //Evaluación
      double aux = Swarm[i]->funcion_objetivo_gato(X,M);
      if (aux < min){
        mejor_gato = i;
        min = aux;
        cambio = true;
      }
    }
    if (!cambio) fallos++;
    iteraciones++;
  }
  //Conversión
  vector<Cluster> C = convertir(Swarm[mejor_gato]->get_posicion(), num_clus, X);
  return C;
}

bool poll(vector<size_t> &vector, size_t buscado){
  for(std::vector<size_t>::iterator it=vector.begin();it!=vector.end();it++){
  	if((*it)==buscado){
      vector.erase(it);
  		return true;
  	}
  }
  return false;
}

vector<Cluster> convertir(vector<size_t> posicion, size_t num_clus, vector<Node> &X){
  vector<Cluster> C;
  C.resize(num_clus);
  size_t num_elem = posicion.size();
  for (size_t i=0; i<num_elem; i++){
    size_t aux = posicion[i];
    X[i].cluster = aux;
    C[aux].nodos.push_back(i);
  }
  centroides(C,X);
  return C;
}

vector<size_t> busqueda_local_suave(vector<size_t> posicion, size_t num_clus, vector<Node> X, vector<vector<int>> M){
  //Inicialización
  size_t num_elem = posicion.size();
  size_t fallos = 0;
  size_t contador = 0;
  double obj = funcion_objetivo_gato_ext(posicion, num_clus, X,M);
  //Comienzo iteraciones
  while (fallos < MAX_FALLOS*num_elem && contador < num_elem){
    //Generación de vecino
    vector<size_t> posicion_vecino = mutacion(posicion, num_clus-1);
    double obj_vecino = funcion_objetivo_gato_ext(posicion_vecino, num_clus,X,M);
    //Evaluación
    if (obj_vecino < obj){
      posicion = posicion_vecino;
      obj=obj_vecino;
    } else fallos++;
    contador++;
  }
  return posicion;
}

vector<size_t> mutacion(vector<size_t> posicion, size_t num_clus){
  size_t num_elem = posicion.size();
  size_t pos = Randint(0, num_elem-1);
  size_t nuevo_clus = Randint(0, num_clus-1);
  while (nuevo_clus == posicion[pos]) nuevo_clus = Randint(0, num_clus-1);
  posicion[pos]=nuevo_clus;
  return posicion;
}


vector<Cluster> cso_mem(size_t num_clus, vector<Node> &X, vector<vector<int>> M){
  //Inicialización
  size_t dimensiones = X.size();
  set_SMP(dimensiones);
  set_SRD(num_clus);
  set_CDC(dimensiones/3);
  set_MR(0.6);
  vector<Gato *> Swarm;
  size_t mejor_gato;
  double min = 1000;
  //Generación de gatos
  Swarm.resize(NUM_GATOS);
  for(size_t i=0; i<NUM_GATOS; i++){
    Swarm[i] = new Gato(dimensiones,num_clus-1);
    double aux = Swarm[i]->funcion_objetivo_gato(X,M);
    if (aux < min){
      mejor_gato = i;
      min = aux;
    }
  }
  //Comienzo iteraciones
  size_t iteraciones = 0;
  size_t fallos = 0;
  while (iteraciones < MAX_ITER && fallos < TOLERANCIA){
    //Asignación de comportamiento
    vector<size_t> indices = Rand_indices(NUM_GATOS, MR*NUM_GATOS);
    for(size_t i=0; i<NUM_GATOS; i++) indices.push_back(i);
    indices = Randfromvector(indices, MR*NUM_GATOS);
    for(size_t i=0; i<NUM_GATOS; i++){
      if (poll(indices,i)) Swarm[i]->cambiar_comportamiento(SEEKING);
      else Swarm[i]->cambiar_comportamiento(TRACING);
    }
    cout << endl << endl;
    bool cambio = false;
    //Ejecución de acciones
    for(size_t i=0; i<NUM_GATOS; i++){
      if (Swarm[i]->get_comportamiento() == SEEKING)
        Swarm[i]->seeking(SMP,SRD,CDC,SPC,X,M);
      if (Swarm[i]->get_comportamiento() == TRACING)
        Swarm[i]->tracing(CT, Swarm[mejor_gato]->get_posicion());
      double aux = Swarm[i]->funcion_objetivo_gato(X,M);
      //Evaluación
      if (aux < min){
        mejor_gato = i;
        min = aux;
        cambio = true;
      }
    }
    if (!cambio) fallos++;
    //Optimización
    if (iteraciones % REPETICIONES == 0){
      vector<size_t> mejor_posicion = Swarm[mejor_gato]->get_posicion();
      mejor_posicion = busqueda_local_suave(mejor_posicion, num_clus, X, M);
      Swarm[mejor_gato]->cambiar_posicion(mejor_posicion);
      min = Swarm[mejor_gato]->funcion_objetivo_gato(X,M);
    }
    iteraciones++;
  }
  //Conversión
  vector<Cluster> C = convertir(Swarm[mejor_gato]->get_posicion(), num_clus, X);
  return C;
}

double funcion_objetivo_gato_ext(vector<size_t> posicion, size_t numero_clusters, vector<Node> X, vector<vector<int>> M){
  getVariables(max_dist_cso, restricciones_cso);
  vector<Cluster> C;
  C.resize(numero_clusters);
  size_t num_elem = posicion.size();
  for (size_t i=0; i<num_elem; i++){
    size_t aux = posicion[i];
    X[i].cluster = aux;
    C[aux].nodos.push_back(i);
  }
  centroides(C,X);
  int rest = evaluarRestricciones(C,X,M);
  double distancias = evaluarDistancias(C,X);
  return  distancias+(rest*max_dist_cso/restricciones_cso);
}

vector<Cluster> cso_p(size_t num_clus, vector<Node> &X, vector<vector<int>> M){
  //Inicialización
  size_t dimensiones = X.size();
  set_SRD(num_clus);
  set_CDC(dimensiones/4);
  set_MR_P(0.47);
  set_CL(0.23);
  set_CT(7);
  vector<Pantera *> Swarm;
  size_t mejor_pantera;
  double min = 1000;
  //Generación de panteras
  Swarm.resize(NUM_GATOS);
  for(size_t i=0; i<NUM_GATOS; i++){
    Swarm[i] = new Pantera(dimensiones);
    double aux = Swarm[i]->funcion_objetivo_pantera(X,M);
    if (aux < min){
      mejor_pantera = i;
      min = aux;
    }
  }
  //Comienzo iteraciones
  size_t iteraciones = 0;
  size_t fallos = 0;
  while (iteraciones < MAX_ITER && fallos < TOLERANCIA_P){
    //Asignación de comportamiento
    vector<size_t> indices_seeking = Rand_indices(NUM_GATOS, round(MR_P*NUM_GATOS + CL*NUM_GATOS));
    vector<size_t> indices_cleaning = dividir(indices_seeking, CL*NUM_GATOS);
    for(size_t i=0; i<NUM_GATOS; i++){
      if (poll(indices_seeking,i)) Swarm[i]->cambiar_comportamiento(SEEKING);
      if (poll(indices_cleaning,i)) Swarm[i]->cambiar_comportamiento(CLEANING);
      else Swarm[i]->cambiar_comportamiento(TRACING);
    }
    bool cambio = false;
    //Ejecución de acciones
    for(size_t i=0; i<NUM_GATOS; i++){
      if (Swarm[i]->get_comportamiento() == SEEKING)
        Swarm[i]->seeking(SRD,SPC,X,M);
      else if (Swarm[i]->get_comportamiento() == TRACING)
        Swarm[i]->tracing(CT, Swarm[mejor_pantera]->get_posicion(),X,M);
      else
        Swarm[i]->cleaning(CDC,X,M);
      double aux = Swarm[i]->funcion_objetivo_pantera(X,M);
      if (aux < min){
        mejor_pantera = i;
        min = aux;
        cambio = true;
      }
    }
    if (!cambio) fallos++;
    iteraciones++;
  }
  //Conversión
  vector<Cluster> C = convertir(Swarm[mejor_pantera]->get_posicion(), num_clus, X);
  return C;
}

vector<size_t> dividir(vector<size_t> &ind, size_t tam){
  vector<size_t> peque;
  size_t num_elem = ind.size();
  for(size_t i=0; i<tam; i++){
    peque.push_back(ind[tam-i]);
    ind.pop_back();
  }
  return peque;
}
