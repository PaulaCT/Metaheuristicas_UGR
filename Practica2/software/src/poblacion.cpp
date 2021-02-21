#include "poblacion.h"
#include "util.h"
#include "random.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <sstream>
#include <iostream>

using namespace std;

vector<Cluster> generarPoblacion (Modo modo, size_t num_clus, vector<Node> &X, vector<vector<int>> M){
  vector<vector<size_t>> poblacion;
  size_t num_elem = X.size();
  size_t tam_poblacion;
  switch(modo){
    case MEMETICO_10:
    case MEMETICO_01:
    case MEMETICO_MEJOR:
    case MEMETICO_10_1:
    case MEMETICO_01_1:
    case MEMETICO_MEJOR_1: tam_poblacion=POBLACION_AM; break;
    case GENETICO_GENERACIONAL_UN:
    case GENETICO_GENERACIONAL_SF:
    case GENETICO_ESTACIONARIO_UN:
    case GENETICO_ESTACIONARIO_SF: tam_poblacion=POBLACION_AG; break;
    default: break;
  }
  poblacion.resize(tam_poblacion);
  for (size_t i=0; i<tam_poblacion; i++){
    for (size_t j=0; j<num_elem; j++)
      poblacion[i].push_back(Randint(0,num_clus-1));
    poblacion[i]=no_vacio(poblacion[i], num_clus);
  }
  switch(modo){
    case MEMETICO_10:
    case MEMETICO_01:
    case MEMETICO_MEJOR:
    case MEMETICO_10_1:
    case MEMETICO_01_1:
    case MEMETICO_MEJOR_1: poblacion = algoritmo_memetico(poblacion,modo,X,M,num_clus); break;
    case GENETICO_GENERACIONAL_UN:
    case GENETICO_GENERACIONAL_SF:
    case GENETICO_ESTACIONARIO_UN:
    case GENETICO_ESTACIONARIO_SF: poblacion = algoritmo_generico(poblacion,modo,X,M,num_clus); break;
    default: break;
  }
  size_t aux = 0;
  size_t mejor = sustituciones(poblacion,1,aux,X,M,num_clus);
  vector<Cluster> C = convertir(poblacion[mejor],num_clus, X);
  return C;
}

vector<Cluster> convertir(vector<size_t> individuo, size_t num_clus, vector<Node> &X){
  vector<Cluster> C;
  C.resize(num_clus);
  size_t num_elem = individuo.size();
  for (size_t i=0; i<num_elem; i++){
    size_t aux = individuo[i];
    X[i].cluster = aux;
    C[aux].nodos.push_back(i);
  }
  centroides(C,X);
  return C;
}

vector<size_t> cruce_uniforme(vector<size_t> padre1, vector<size_t> padre2, size_t num_clus){
  vector<size_t> seleccion;
  vector<size_t> hijo;
  size_t num_elem = padre1.size();
  for (size_t i=0; i<num_elem/2; i++) seleccion.push_back(Randint(0,num_elem-1));
  for (size_t i=0; i<num_elem; i++){
    if (contains(seleccion,i)) hijo.push_back(padre1[i]);
    else hijo.push_back(padre2[i]);
  }
  return no_vacio(hijo, num_clus);
}

vector<size_t> cruce_segmento_fijo(vector<size_t> padre1, vector<size_t> padre2,size_t num_clus){
  vector<size_t> seleccion;
  vector<size_t> hijo;
  size_t num_elem = padre1.size();
  size_t inicio = Randint(0, num_elem-1);
  size_t tam_segmento = Randint (0, num_elem-1);
  vector<size_t> restantes;
  vector<size_t> pos_segmento;
  for (size_t i=0; i<tam_segmento; i++) pos_segmento.push_back((inicio+i)%num_elem);
  for (size_t i=0; i<num_elem; i++){
    hijo.push_back(-1);
    if (!poll(pos_segmento,i)) restantes.push_back(i);
    else hijo[i]=padre1[i];
  }
  seleccion = Randfromvector(restantes,restantes.size()/2);
  for (size_t i=0; i<num_elem; i++){
    if (poll(restantes,i)){
      if (poll(seleccion,i)) hijo[i]=padre1[i];
      else hijo[i]=padre2[i];
    }
  }
  return no_vacio(hijo, num_clus);
}

vector<size_t> no_vacio(vector<size_t> hijo, size_t num_clus){
  vector<size_t> check;
  size_t num_elem = hijo.size();
  for (size_t i=0; i<num_clus; i++) check.push_back(0);
  for (size_t i=0; i<num_elem; i++) check[hijo[i]]++;
  bool no_vacio = false;
  while (!no_vacio){
    for (size_t i=0; i<num_clus; i++){
      if (check[i]==0){
        size_t rand = Randint (0, num_elem-1);
        size_t aux = hijo[rand];
        hijo[rand] = i;
        check[i]++;
        check[aux]--;
        break;
      }
      if (check[i]!=0 && i==num_clus-1) no_vacio=true;
    }
  }
  return hijo;
}

vector<size_t> mutacion_uniforme(vector<size_t> hijo, size_t num_clus){
  size_t num_elem = hijo.size();
  size_t posicion = Randint(0, num_elem-1);
  size_t nuevo_clus = Randint(0, num_clus-1);
  while (nuevo_clus == hijo[posicion]) nuevo_clus = Randint(0, num_clus-1);
  hijo[posicion]=nuevo_clus;
  return no_vacio(hijo,num_clus);
}

vector<size_t> busqueda_local_suave(vector<size_t> individuo/*, size_t max_fallos*/, size_t num_clus, vector<Node> X, vector<vector<int>> M, size_t & evaluaciones){
  size_t num_elem = individuo.size();
  bool mejora = true;
  size_t fallos = 0;
  size_t contador = 0;
  double obj = funcion_objetivo_p2(individuo,X,M,num_clus);
  while ((mejora || fallos < MAX_FALLOS*num_elem) && contador < num_elem){//num_elem){
    mejora = false;
    vector<size_t> vecino = mutacion_uniforme(individuo,num_clus);
    double obj_vecino = funcion_objetivo_p2(vecino,X,M,num_clus);
    if (obj_vecino < obj){
      mejora = true;
      individuo = vecino;
      obj=obj_vecino;
    } else fallos++;
    contador++;
    if (++evaluaciones==EVALUACIONES_FOBJETIVO) break;
  }
  return individuo;
}

vector<size_t> seleccion_torneo_binario(vector<vector<size_t>> poblacion, vector<Node> X, vector<vector<int>> M, size_t num_clus){
  vector<vector<size_t>> torneo = Randfromvector(poblacion, 2);
  double obj_aux = 10000000;
  size_t best=0;
  for (size_t i=0; i<2; i++){
    double obj = funcion_objetivo_p2(torneo[i],X,M,num_clus);
    if (obj < obj_aux){
      obj_aux=obj;
      best = i;
    }
  }
  return torneo[best];
}

double funcion_objetivo_p2(vector<size_t> individuo, vector<Node> x, vector<vector<int>> m, size_t num_clus){
  vector<double> vec_dist;
  double max_dist = 0;
  size_t num_res = 0;
  int restricciones = evaluar_restricciones_p2(num_res,individuo,m);
  vector<Cluster> c = convertir(individuo, num_clus,x);
  double distancias = evaluarDistancias(vec_dist,c,x);
  for(size_t i=0; i<vec_dist.size(); i++){
    if (vec_dist[i] > max_dist) max_dist = vec_dist[i];
  }
  return  distancias+(restricciones*max_dist/num_res);
}

int evaluar_restricciones_p2(size_t & num_res, vector<size_t> individuo, vector<vector<int>> m){
  size_t tam = m.size();
	int incumplidas=0;
	for (size_t i = 0; i < tam ; i++) {
		for (size_t j = i; j < tam; j++) {
			if (m[i][j]==1){
        num_res++;
				if (individuo[i] != individuo[j]) incumplidas++;
			} else if (m[i][j]==-1){
        num_res++;
				if (individuo[i] == individuo[j]) incumplidas++;
			}
		}
	}
  return incumplidas;
}

bool tirar_dado(double probabilidad){
  float dado = Rand();
  return (dado<probabilidad);
}

vector<vector<size_t>> algoritmo_generico(vector<vector<size_t>> poblacion, Modo modo, vector<Node> X, vector<vector<int>> M, size_t num_clus){
  size_t evaluaciones = 0;
  vector<vector<size_t>> nueva_poblacion=poblacion;
  double f_poblacion = evaluar_poblacion(poblacion,X,M,num_clus,evaluaciones);
  double f_nueva_poblacion = 0.00;
  while (evaluaciones < EVALUACIONES_FOBJETIVO && f_poblacion >= f_nueva_poblacion){
    poblacion=nueva_poblacion;
    vector<vector<size_t>> hijos;
    vector<vector<size_t>> mutados;
    vector<size_t> posiciones;
    switch (modo) {
      case GENETICO_ESTACIONARIO_SF:
      case GENETICO_ESTACIONARIO_UN: { //ESTACIONARIO
        hijos = cruzar_estacionario(nueva_poblacion,modo,X,M,num_clus, evaluaciones); //Genera hijos
        mutados = mutar(nueva_poblacion,modo,num_clus, posiciones); //Mutacion
        for (size_t i=0; i<posiciones.size();i++) nueva_poblacion[posiciones[i]]=mutados[i]; //Añade mutación
        size_t peor = sustituciones(nueva_poblacion,-1,evaluaciones,X,M,num_clus); //Sustituye hijos
        nueva_poblacion[peor]=hijos[0];
        peor = sustituciones(nueva_poblacion,-1,evaluaciones,X,M,num_clus);
        nueva_poblacion[peor]=hijos[1];
        break;
      }
      case GENETICO_GENERACIONAL_SF:
      case GENETICO_GENERACIONAL_UN:{ //GENERACIONAL
        vector<size_t> posiciones_hijos;
        hijos = cruzar_generacional(nueva_poblacion,modo,X,M,num_clus, evaluaciones,posiciones); //Genera hijos
        mutados = mutar(nueva_poblacion,modo,num_clus, posiciones); //Mutacion
        for (size_t i=0; i<posiciones.size();i++) nueva_poblacion[posiciones[i]]=mutados[i]; //Añade mutación
        size_t mejor = sustituciones(nueva_poblacion,1,evaluaciones,X,M,num_clus); //Sustituye hijos
        bool elitista=false;
        vector<size_t> aux;
        for (size_t i=0; i<posiciones_hijos.size();i++){
          if (posiciones_hijos[i]==mejor){
            elitista = true;
            aux = nueva_poblacion[mejor];
          }
          nueva_poblacion[posiciones_hijos[i]]=hijos[i];
        }
        if (elitista){
          size_t peor = sustituciones(hijos,-1,evaluaciones,X,M,num_clus);
          nueva_poblacion[peor]=aux;
        }
        break;
      }
      default: break;
    }
    f_nueva_poblacion = evaluar_poblacion(nueva_poblacion,X,M,num_clus,evaluaciones);
  }
  return poblacion;
}

vector<vector<size_t>> cruzar_estacionario(vector<vector<size_t>> poblacion, Modo modo, vector<Node> X, vector<vector<int>> M, size_t num_clus, size_t &evaluacion){
  vector<vector<size_t>> padres;
  vector<vector<size_t>> hijos;
  //size_t tam_poblacion = poblacion.size();
  size_t esperanza = P_CRUCE_AGE*PADRES_ESTACIONARIO;
  for (size_t i=0; i<esperanza; i++){
    padres.push_back(seleccion_torneo_binario(poblacion,X,M,num_clus));
    evaluacion+=2;
  }
  switch (modo) {
    case GENETICO_ESTACIONARIO_UN:
      for (size_t i=0; i<esperanza; i++)
        hijos.push_back(cruce_uniforme(padres[0],padres[1],num_clus));
      break;
    case GENETICO_ESTACIONARIO_SF:
      for (size_t i=0; i<esperanza; i++)
        hijos.push_back(cruce_segmento_fijo(padres[0],padres[1],num_clus));
      break;
    default: break;
  }
  return hijos;
}

vector<vector<size_t>> cruzar_generacional(vector<vector<size_t>> poblacion, Modo modo, vector<Node> X, vector<vector<int>> M, size_t num_clus, size_t &evaluacion, vector<size_t> & posiciones){
  vector<vector<size_t>> padres;
  vector<vector<size_t>> hijos;
  //size_t tam_poblacion = poblacion.size();
  size_t esperanza = P_CRUCE_AGG*PADRES_GENERACIONAL;
  for (size_t i=0; i<esperanza; i++){
    padres.push_back(seleccion_torneo_binario(poblacion,X,M,num_clus));
    evaluacion+=2;
  }
  esperanza/=2;
  switch (modo){
    case GENETICO_GENERACIONAL_UN:
      for (size_t i=0; i<esperanza; i++){
        hijos.push_back(cruce_uniforme(padres[i],padres[i+esperanza],num_clus));
        hijos.push_back(cruce_uniforme(padres[i],padres[i+esperanza],num_clus));
      } break;
    case GENETICO_GENERACIONAL_SF:
      for (size_t i=0; i<esperanza; i++){
        double obj1 = funcion_objetivo_p2(padres[i],X,M,num_clus);
        double obj2 = funcion_objetivo_p2(padres[i+esperanza],X,M,num_clus);
        evaluacion+=2;
        if (obj1 < obj2){
          hijos.push_back(cruce_segmento_fijo(padres[i],padres[i+esperanza],num_clus));
          hijos.push_back(cruce_segmento_fijo(padres[i],padres[i+esperanza],num_clus));
        } else {
          hijos.push_back(cruce_segmento_fijo(padres[i+esperanza],padres[i],num_clus));
          hijos.push_back(cruce_segmento_fijo(padres[i+esperanza],padres[i],num_clus));
        }
      } break;
    default: break;
  }
  return hijos;
}

vector<vector<size_t>> mutar(vector<vector<size_t>> poblacion, Modo modo, size_t num_clus, vector<size_t> & posiciones){
  vector<vector<size_t>> mutados;
  size_t tam_poblacion = poblacion.size();
  size_t num_elem = poblacion[0].size();
  size_t probabilidad;
  switch (modo) {
    case MEMETICO_10:
    case MEMETICO_01:
    case MEMETICO_MEJOR:
    case MEMETICO_10_1:
    case MEMETICO_01_1:
    case MEMETICO_MEJOR_1: probabilidad = P_GEN_AM; break;
    case GENETICO_GENERACIONAL_SF:
    case GENETICO_GENERACIONAL_UN:
    case GENETICO_ESTACIONARIO_SF:
    case GENETICO_ESTACIONARIO_UN: probabilidad = P_MUTACION_AG; break;
    default: break;
  }
  for (size_t i=0; i<tam_poblacion; i++){
    if (tirar_dado(probabilidad*num_elem)){
      mutados.push_back(mutacion_uniforme(poblacion[i], num_clus));
      posiciones.push_back(i);
    }
  }
  return mutados;
}


size_t sustituciones(vector<vector<size_t>> poblacion, int modo, size_t & evaluacion, vector<Node> X, vector<vector<int>> M, size_t num_clus){
  size_t tam_poblacion = poblacion.size();
  size_t sustituto;
  if (modo==-1){ //peor
    size_t aux = 0;
    sustituto = 0;
    for (size_t i=0; i<tam_poblacion; i++){
      double obj = funcion_objetivo_p2(poblacion[i],X,M,num_clus);
      if (obj > aux){
        aux = obj;
        sustituto = i;
      }
    }
  } else if (modo==0){

  } else if (modo==1){ //mejor
    size_t aux = 1000000;
    sustituto = 0;
    for (size_t i=0; i<tam_poblacion; i++){
      double obj = funcion_objetivo_p2(poblacion[i],X,M,num_clus);
      if (obj < aux){
        aux = obj;
        sustituto = i;
      }
    }
  }
  evaluacion += tam_poblacion;
  return sustituto;
}

double evaluar_poblacion(vector<vector<size_t>> poblacion, vector<Node> x, vector<vector<int>> m, size_t num_clus, size_t &evaluacion){
  size_t tam_poblacion = poblacion.size();
  double obj;
  for (size_t i=0; i<tam_poblacion; i++){
    obj += funcion_objetivo_p2(poblacion[i],x,m,num_clus);
  }
  evaluacion+=tam_poblacion;
  return obj/tam_poblacion;
}


vector<vector<size_t>> algoritmo_memetico(vector<vector<size_t>> poblacion, Modo modo, vector<Node> X, vector<vector<int>> M, size_t num_clus){
  size_t evaluaciones = 0;
  vector<vector<size_t>> nueva_poblacion=poblacion;
  double f_poblacion = evaluar_poblacion(poblacion,X,M,num_clus,evaluaciones);
  double f_nueva_poblacion = 0.00;
  size_t contador = 0;
  while (evaluaciones < EVALUACIONES_FOBJETIVO && f_poblacion >= f_nueva_poblacion){
    poblacion=nueva_poblacion;
    vector<vector<size_t>> hijos;
    vector<vector<size_t>> mutados;
    vector<size_t> posiciones;
    vector<size_t> posiciones_hijos;
    hijos = cruzar_mejor(nueva_poblacion,X,M,num_clus, evaluaciones,posiciones); //GENETICO_GENERACIONAL_SF
    mutados = mutar(nueva_poblacion,modo,num_clus, posiciones); //Mutacion
    for (size_t i=0; i<posiciones.size();i++) nueva_poblacion[posiciones[i]]=mutados[i]; //Añade mutación
    size_t mejor = sustituciones(nueva_poblacion,1,evaluaciones,X,M,num_clus); //Sustituye hijos
    bool elitista=false;
    vector<size_t> aux;
    for (size_t i=0; i<posiciones_hijos.size();i++){
      nueva_poblacion[posiciones_hijos[i]]=hijos[i];
      if (posiciones_hijos[i]==mejor){
        elitista = true;
        aux = nueva_poblacion[mejor];
      }
    }
    if (elitista){
      size_t peor = sustituciones(hijos,-1,evaluaciones,X,M,num_clus);
      nueva_poblacion[peor]=aux;
    }
    switch (modo) {
      case MEMETICO_10:
      case MEMETICO_01:
      case MEMETICO_MEJOR: if (contador%10==0) nueva_poblacion = optimizar(nueva_poblacion, modo, num_clus, X, M, evaluaciones);
      case MEMETICO_10_1:
      case MEMETICO_01_1:
      case MEMETICO_MEJOR_1: nueva_poblacion = optimizar(nueva_poblacion, modo, num_clus, X, M, evaluaciones);
      default: break;
    }
    f_nueva_poblacion = evaluar_poblacion(nueva_poblacion,X,M,num_clus,evaluaciones);
    contador++;
  }
  return poblacion;
}

vector<vector<size_t>> optimizar(vector<vector<size_t>> poblacion, Modo modo,/*size_t max_fallos,*/ size_t num_clus, vector<Node> X, vector<vector<int>> M, size_t & evaluaciones){
  size_t tam_poblacion = poblacion.size();
  poblacion = Randfromvector(poblacion, tam_poblacion);
  switch (modo) {
    case MEMETICO_10:
    case MEMETICO_10_1: {
      for (size_t i = 0; i<tam_poblacion; i++){
        poblacion[i]=busqueda_local_suave(poblacion[i], num_clus,X,M,evaluaciones);
      } break;
    }
    case MEMETICO_01:
    case MEMETICO_01_1: {
      for (size_t i=0; i<tam_poblacion; i++){
        if (tirar_dado(0.1)) poblacion[i]=busqueda_local_suave(poblacion[i], num_clus,X,M,evaluaciones);
      } break;
    }
    case MEMETICO_MEJOR:
    case MEMETICO_MEJOR_1: {
      size_t mejor = sustituciones(poblacion,1,evaluaciones,X,M,num_clus);
      poblacion[mejor]=busqueda_local_suave(poblacion[mejor], num_clus,X,M,evaluaciones);
      break;
    }
    default: break;
  }
  return poblacion;
}

vector<vector<size_t>> cruzar_mejor(vector<vector<size_t>> poblacion, vector<Node> X, vector<vector<int>> M, size_t num_clus, size_t &evaluacion, vector<size_t> &posiciones){
  vector<vector<size_t>> padres;
  vector<vector<size_t>> hijos;
  size_t tam_poblacion = poblacion.size();
  size_t esperanza = P_CROMOSOMA_AM*tam_poblacion;
  for (size_t i=0; i<esperanza; i++){
    padres.push_back(seleccion_torneo_binario(poblacion,X,M,num_clus));
    evaluacion+=2;
  }
  esperanza/=2;
  for (size_t i=0; i<esperanza; i++){
    hijos.push_back(cruce_uniforme(padres[i],padres[i+esperanza],num_clus));
    hijos.push_back(cruce_uniforme(padres[i],padres[i+esperanza],num_clus));
  }
  return hijos;
}

bool contains(vector<size_t> vector, size_t buscado){
  if(vector.empty()){
    return false;
  } else {
    size_t tam = vector.size();
    for (size_t i=0; i<tam; i++)
      if (vector[tam]==buscado) return true;
  }
  return false;
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

void imprimirIndividuo(vector<size_t> individuo){
  size_t num_elem = individuo.size();
  cout << "( ";
  for (size_t i=0; i<num_elem; i++) cout << individuo[i] << " ";
  cout << ")" << endl;
}
