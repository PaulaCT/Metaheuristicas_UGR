#include "trayectoria.h"
#include "util.h"
#include "random.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <random>


using namespace std;

vector<Cluster> generarInicial(Modo modo, size_t num_clus, vector<Node> &X, vector<vector<int>> M){
  vector<size_t> sol;
  size_t num_elem = X.size();
  sol = solucion_aleatoria(sol, num_elem, num_clus);
  switch(modo){
    case B_MULTIARRANQUE_BASICA: {
      sol = busqueda_multiarranque(sol,num_clus,X,M);
    } break;
    case E_SIMULADO: {
      sol = enfriamiento_simulado(modo,sol,num_clus,X,M);
    } break;
    case BL_REITERADA: {
      sol = busqueda_local_reiterada(sol,num_clus,X,M);
    } break;
    case HIBRIDACION: {
      sol = hibridacion(modo,sol,num_clus,X,M);
    } break;
    default: break;
  }
  vector<Cluster> C = convertir(sol,num_clus, X);
  return C;
}

vector<size_t> solucion_aleatoria(vector<size_t> sol, size_t num_elem, size_t num_clus){
  for (size_t j=0; j<num_elem; j++)
    sol.push_back(Randint(0,num_clus-1));
  return no_vacio(sol, num_clus);
}

vector<Cluster> convertir(vector<size_t> sol, size_t num_clus, vector<Node> &X){
  vector<Cluster> C;
  C.resize(num_clus);
  size_t num_elem = sol.size();
  for (size_t i=0; i<num_elem; i++){
    size_t aux = sol[i];
    X[i].cluster = aux;
    C[aux].nodos.push_back(i);
  }
  centroides(C,X);
  return C;
}

vector<size_t> no_vacio(vector<size_t> sol, size_t num_clus){
  vector<size_t> check;
  size_t num_elem = sol.size();
  for (size_t i=0; i<num_clus; i++) check.push_back(0);
  for (size_t i=0; i<num_elem; i++) check[sol[i]]++;
  bool no_vacio = false;
  while (!no_vacio){
    for (size_t i=0; i<num_clus; i++){
      if (check[i]==0){
        size_t rand = Randint (0, num_elem-1);
        size_t aux = sol[rand];
        sol[rand] = i;
        check[i]++;
        check[aux]--;
        break;
      }
      if (check[i]!=0 && i==num_clus-1) no_vacio=true;
    }
  }
  return sol;
}

double funcion_objetivo_p3(vector<size_t> sol, vector<Node> x, vector<vector<int>> m, size_t num_clus){
  size_t num_res = 0;
  double max_dist = 0;
  vector<double> vec_dist;
  int restricciones = evaluar_restricciones_p3(num_res,sol,m);
  vector<Cluster> c = convertir(sol, num_clus,x);
  double distancias = evaluarDistancias(vec_dist,c,x);
	for(size_t i=0; i<vec_dist.size(); i++){
		if (vec_dist[i] > max_dist) max_dist = vec_dist[i];
	}
  return  distancias+(restricciones*max_dist/num_res);
}

int evaluar_restricciones_p3(size_t & num_res, vector<size_t> sol, vector<vector<int>> m){
  size_t tam = m.size();
	int incumplidas=0;
	for (size_t i = 0; i < tam ; i++) {
		for (size_t j = i; j < tam; j++) {
			if (m[i][j]==1){
        num_res++;
				if (sol[i] != sol[j]) incumplidas++;
			} else if (m[i][j]==-1){
        num_res++;
				if (sol[i] == sol[j]) incumplidas++;
			}
		}
	}
  return incumplidas;
}

vector<size_t> cambio_cluster(vector<size_t> sol, size_t num_clus, size_t elem){
  size_t nuevo_clus = Randint (0, num_clus-1);
  sol[elem] = nuevo_clus;
  return no_vacio(sol, num_clus);
}

vector<size_t> busqueda_local(vector<size_t> sol, size_t num_clus, vector<Node> X, vector<vector<int>> M, size_t & evaluaciones){
  size_t num_elem = sol.size();
  bool cambios = true;
  vector<size_t> nueva_sol = sol;
  vector<size_t> orden;
  for (size_t i=0; i<num_elem; i++) orden.push_back(i);
  Shuffle(orden);
  double obj = funcion_objetivo_p3(sol,X,M,num_clus);
  while(evaluaciones<EVALUACIONES_FOBJETIVO && cambios){
    cambios = false;
    for(size_t i=0; i<num_elem; i++){
      nueva_sol = cambio_cluster(nueva_sol, num_clus, nueva_sol[orden[i]]);
      double obj_vecino = funcion_objetivo_p3(nueva_sol,X,M,num_clus);
      evaluaciones++;
      if(obj_vecino<obj){
        sol = nueva_sol;
        obj = obj_vecino;
        cambios = true;
        Shuffle(orden);
        break;
      }
    }
  }
  return sol;
}

vector<size_t> enfriamiento_simulado(Modo modo,vector<size_t> sol, size_t num_clus, vector<Node> X, vector<vector<int>> M){
  size_t evaluaciones = 0;
  size_t num_elem = sol.size();
  vector<size_t> mejor = sol;
  double obj = funcion_objetivo_p3(sol,X,M,num_clus);
  evaluaciones++;
  size_t max_evaluaciones;
	switch (modo) {
		case E_SIMULADO: max_evaluaciones = EVALUACIONES_FOBJETIVO_ES; break;
		case HIBRIDACION: max_evaluaciones = EVALUACIONES_FOBJETIVO; break;
		default: break;
	}
  double obj_mejor = obj;
  double max_vecinos = MAX_VECINOS*num_elem;
  double max_exitos = MAX_EXITOS*max_vecinos;
  double num_enfriamientos = max_evaluaciones/max_vecinos;
  double temperatura_ini = -(FI_MU * obj) / log(FI_MU);
  double temperatura = temperatura_ini;
  while (temperatura < TEMPERATURA_FIN) temperatura *= TEMPERATURA_FIN;
  double beta = (temperatura_ini - TEMPERATURA_FIN) / (num_enfriamientos * temperatura_ini * TEMPERATURA_FIN);
  bool exitoso = true;
  while (exitoso && temperatura > TEMPERATURA_FIN){
    size_t vecinos = 0;
    size_t exitos = 0;
    while (exitos < max_exitos && vecinos < max_vecinos){
      vector<size_t> sol_vecino = cambio_cluster(sol, num_clus, Randint(0, num_elem-1));
      double obj_vecino = funcion_objetivo_p3(sol_vecino,X,M,num_clus);
      vecinos++;
      double mejora = obj_vecino - obj;
      double prob = Rand();
      if (mejora < 0 || exp(-mejora / temperatura) >= prob){
        sol = sol_vecino;
        obj = obj_vecino;
        if (obj_mejor > obj){
          mejor = sol;
          obj_mejor = obj;
          exitos++;
        }
      }
      if (++evaluaciones>=max_evaluaciones) break;
    }
    temperatura /= (1 + beta * temperatura);
    if (exitos == 0) exitoso = false;
    if (evaluaciones>=max_evaluaciones) break;
  }
  return mejor;
}

vector<size_t> busqueda_multiarranque(vector<size_t> sol, size_t num_clus, vector<Node> X, vector<vector<int>> M){
  double min = 1000;
  size_t evaluaciones;
  vector<size_t> aux;
  size_t num_elem = sol.size();
  for (size_t i=0; i<ITERACIONES; i++){
    aux = solucion_aleatoria(aux, num_elem, num_clus);
    evaluaciones = 0;
    aux = busqueda_local(aux,num_clus,X,M,evaluaciones);
    double obj_aux = funcion_objetivo_p3(aux,X,M,num_clus);
    if(obj_aux < min){
      min = obj_aux;
      sol = aux;
    }
    aux.clear();
  }
  return sol;
}


vector<size_t> cambio_segmento(vector<size_t> sol, size_t num_clus){
  size_t num_elem = sol.size();
  size_t ini = Randint (0, num_elem-1);
  size_t tam = P_MUTACION * num_elem;
  size_t nuevo_clus;
  vector<size_t> seleccionados;
  for (size_t i=0; i<tam; i++) seleccionados.push_back((ini+i)%num_elem);
  for (size_t i=0; i<tam; i++){
    nuevo_clus = Randint (0, num_clus-1);
    sol[seleccionados[i]] = nuevo_clus;
  }
  return no_vacio(sol, num_clus);
}

vector<size_t> busqueda_local_reiterada(vector<size_t> sol, size_t num_clus, vector<Node> X, vector<vector<int>> M){
  double min = 1000;
  size_t evaluaciones=0;
  sol = busqueda_local(sol,num_clus,X,M,evaluaciones);
  vector<size_t> mejor = sol;
  vector<size_t> aux;
  for (size_t i=0; i<ITERACIONES-1; i++){
    aux = mejor;
    mejor = cambio_segmento(mejor,num_clus);
    evaluaciones = 0;
    mejor = busqueda_local(mejor,num_clus,X,M,evaluaciones);
    mejor = torneo(aux,mejor,X,M,num_clus);
  }
  return mejor;
}

vector<size_t> torneo(vector<size_t> sol_1,vector<size_t> sol_2,vector<Node> x,vector<vector<int>> m,size_t num_clus){
  double obj_1 = funcion_objetivo_p3(sol_1,x,m,num_clus);
  double obj_2 = funcion_objetivo_p3(sol_2,x,m,num_clus);
  if (obj_1 > obj_2) return sol_2;
  else return sol_1;
}

vector<size_t> hibridacion(Modo modo, vector<size_t> sol, size_t num_clus, vector<Node> X, vector<vector<int>> M){
  double min = 1000;
  sol = enfriamiento_simulado(modo,sol,num_clus,X,M);
  vector<size_t> mejor = sol;
  vector<size_t> aux;
  for (size_t i=0; i<ITERACIONES-1; i++){
    aux = mejor;
    mejor = cambio_segmento(mejor,num_clus);
    mejor = enfriamiento_simulado(modo,mejor,num_clus,X,M);
    mejor = torneo(aux,mejor,X,M,num_clus);
  }
  return mejor;
}
