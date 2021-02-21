#include "util.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <sstream>
#include <iostream>

using namespace std;

Modo comprobarParametros(int num_parametros, char **parametros){
	if(num_parametros >= NUM_PARAMETROS){
		if(parametros[1] == PARAMETRO_GREEDY) return GREEDY;
		if(parametros[1] == PARAMETRO_BUSQUEDA_LOCAL) return BUSQUEDA_LOCAL;
		if(parametros[1] == PARAMETRO_B_MULTIARRANQUE_BASICA) return B_MULTIARRANQUE_BASICA;
		if(parametros[1] == PARAMETRO_E_SIMULADO) return E_SIMULADO;
		if(parametros[1] == PARAMETRO_BL_REITERADA) return BL_REITERADA;
		if(parametros[1] == PARAMETRO_HIBRIDACION) return HIBRIDACION;
		cout << "Error al introducir el algoritmo"<< endl;
	} else cout << "Error en el numero de parametros introducidos"<< endl;
	mostrarUso(parametros[0]);
	exit(1);
}

Conjunto seleccionConjunto (char *nombre, char *parametro){
	if(parametro == PARAMETRO_IRIS) return IRIS;
	if(parametro == PARAMETRO_ECOLI) return ECOLI;
	if(parametro == PARAMETRO_RAND) return RAND;
	if(parametro == PARAMETRO_NEWTHYROID) return NEWTHYROID;
	cout << "Error al introducir el conjunto"<< endl;
	mostrarUso(nombre);
	exit(1);
}

string abrirFicheroElementos(Conjunto conjunto){
	switch (conjunto) {
		case IRIS: return "./datos/iris_set.dat"; break;
		case ECOLI: return "./datos/ecoli_set.dat"; break;
		case RAND: return "./datos/rand_set.dat"; break;
		case NEWTHYROID: return "./datos/newthyroid_set.dat"; break;
		default: break;;
	}
}

string abrirFicheroRestricciones(Conjunto conjunto, char *porcentaje){
	ifstream aux;
	switch (conjunto) {
		case IRIS:{
			if (porcentaje == PARAMETRO_10) return "./datos/iris_set_const_10.const";
			else if (porcentaje == PARAMETRO_20) return "./datos/iris_set_const_20.const";
			break;
		}
		case ECOLI: {
			if (porcentaje == PARAMETRO_10) return "./datos/ecoli_set_const_10.const";
			else if (porcentaje == PARAMETRO_20) return "./datos/ecoli_set_const_20.const";
			break;
		}
		case RAND: {
			if (porcentaje == PARAMETRO_10) return "./datos/rand_set_const_10.const";
			else if (porcentaje == PARAMETRO_20) return "./datos/rand_set_const_20.const";
			break;
		}
		case NEWTHYROID: {
			if (porcentaje == PARAMETRO_10) return "./datos/newthyroid_set_const_10.const";
			else if (porcentaje == PARAMETRO_20) return "./datos/newthyroid_set_const_20.const";
			break;
		}
		default: break;
	}
}

bool mostrarResultadoParticion(char *nombre, char *eleccion){
	if(eleccion == PARAMENTRO_MOSTRAR) return true;
	if (eleccion == PARAMENTRO_NO_MOSTRAR) return false;
	cout << "Error al introducir la preferencia de resultado" <<endl;
	mostrarUso(nombre);
	exit(1);
}

void mostrarUso(char *nombre_programa){
	cerr << "Error en el uso del programa" << endl << endl;
	cerr << "Uso: " << nombre_programa << " algoritmo conjunto porcentaje_restricciones n_repeticiones resultado [semillas]" << endl << endl;;
	cerr << "--- Algoritmos ---" << endl;
	cerr << "Greedy: " << PARAMETRO_GREEDY << endl;
	cerr << "Busqueda Local: " << PARAMETRO_BUSQUEDA_LOCAL << endl;
	cerr << "Busqueda Multiarranque Basica: " << PARAMETRO_B_MULTIARRANQUE_BASICA << endl;
	cerr << "Enfriamiento Simulado: " << PARAMETRO_E_SIMULADO << endl;
	cerr << "Busqueda Local Reiterada: " << PARAMETRO_BL_REITERADA << endl;
	cerr << "Hibridacion de " << PARAMETRO_E_SIMULADO << " y " << PARAMETRO_BL_REITERADA << ": " << PARAMETRO_HIBRIDACION << endl;
	cerr << "--- Conjuntos ---" << endl;
	cerr << "Iris -> " << PARAMETRO_IRIS << " Ecoli -> " << PARAMETRO_ECOLI << " Rand -> " << PARAMETRO_RAND << " Newthyroid -> " << PARAMETRO_NEWTHYROID << endl << endl;;
	cerr << "--- Porcentaje de restricciones ---" << endl;
	cerr << "10 20" << endl << endl;;
	cerr << "--- Opciones resultado ---" << endl;
	cerr << "Mostrar -> " << PARAMENTRO_MOSTRAR << " No mostrar -> " << PARAMENTRO_NO_MOSTRAR <<endl << endl << endl;
}

vector<int> crearVectorSemillas(int num_parametros, char **parametros){
	vector<int> semillas;
	if(num_parametros==NUM_PARAMETROS) semillas = {1,5,10,20,25};
	else {
		for (int i = NUM_PARAMETROS; i < num_parametros; i++) semillas.push_back(stof(parametros[i]));
	}
	return semillas;
}

void get_token(istringstream &Line_stream1,string &Token1){
	getline(Line_stream1,Token1,',');
}

bool get_new_line(ifstream &File,istringstream &Line_stream){
	string Line;
	if (getline(File,Line)){
		Line_stream.str(Line);
		Line_stream.seekg(0);
		return true;
	}
	else return false;
}


vector<Node> leerNodos(Conjunto con, ifstream &file){
	vector<Node> x;
	Node nuevo_nodo;
	size_t caract, num_datos;
	switch (con) {
		case IRIS: caract = 4; num_datos = 150; break;
		case ECOLI: caract = 7; num_datos = 336; break;
		case RAND: caract = 2; num_datos = 150; break;
		case NEWTHYROID: caract = 5; num_datos = 215; break;
		default: break;
	}
	string Token;
	istringstream Line_stream;
	for(size_t k=0; k<num_datos; k++){
		get_new_line(file,Line_stream);
		for(size_t i = 0; i <caract; i++){
			get_token(Line_stream,Token);
			nuevo_nodo.datos.push_back(stof(Token));
		}
		x.push_back(nuevo_nodo);
		nuevo_nodo.datos.clear();
	}
	return x;
}

vector<vector<int>> leerRestricciones(Conjunto con, ifstream &file){
	vector<vector<int>> m;
	vector<int> fila;
	size_t num_datos;
	switch (con) {
		case IRIS: num_datos = 150; break;
		case ECOLI: num_datos = 336; break;
		case RAND: num_datos = 150; break;
		case NEWTHYROID: num_datos = 215; break;
		default: break;
	}
	string Token;
	istringstream Line_stream;
	for(size_t k = 0; k < num_datos; k++){
		get_new_line(file,Line_stream);
		for(size_t i = 0; i < num_datos; ++i){
			get_token(Line_stream,Token);
			fila.push_back(stof(Token));
		}
		m.push_back(fila);
		fila.clear();
	}
	return m;
}

void dimensionarParticion(Conjunto conjunto, vector<Cluster> &C){
	switch (conjunto) {
		case IRIS: C.resize(3); break;
		case ECOLI: C.resize(8); break;
		case RAND: C.resize(3); break;
		case NEWTHYROID: C.resize(3); break;
		default: break;
	}
}

vector<double> calcularCentroide(vector<Cluster> c, vector<Node> x, size_t clus){
	size_t dim = x[0].datos.size();
	size_t elementos = c[clus].nodos.size();
	vector<double> sumatoria;
	for (size_t i=0; i<dim; i++) sumatoria.push_back(0.0);
	for(size_t i=0; i<elementos; i++){
		for(size_t j=0; j<dim; j++){
			sumatoria[j] += x[c[clus].nodos[i]].datos[j];

		}
	}
	for(size_t i=0; i<dim; i++) sumatoria[i] = sumatoria[i]/elementos;
	return sumatoria;
}

void centroides(vector<Cluster> &c, vector<Node> x){
	size_t num_clus = c.size();
	for(size_t i=0; i<num_clus; i++) c[i].centroide = calcularCentroide(c,x,i);
}

void centroidesAleatorios(vector<Cluster> & c, vector<Node> x){
	size_t num_clus = c.size();
	size_t num_datos = x[0].datos.size();
	for(size_t i=0; i<num_clus; i++) {
		vector<double> aux;
		for(size_t j = 0; j < num_datos; j++) {
			float rand = Randfloat(1, 5);
			aux.push_back(rand);
		}
		c[i].centroide = aux;
	}
}

double calcularDistancia(vector<Cluster> c, vector<Node> x, size_t clus){
	size_t dim = x[0].datos.size();
	size_t elementos = c[clus].nodos.size();
	vector<double> sumatoria;
	for (size_t i=0; i<dim; i++) sumatoria.push_back(0.0);
	for(size_t i=0; i<elementos; i++){
		for(size_t j=0; j<dim; j++){
			sumatoria[j] += pow(c[clus].centroide[j] - x[c[clus].nodos[i]].datos[j], 2);
		}
	}
	double distancia = 0;
	for(size_t i=0; i<dim; i++) distancia += sumatoria[i];
	return sqrt(distancia)/elementos;
}

double evaluarDistancias(vector<Cluster> c, vector<Node> x){
	size_t k = c.size();
	double distancia = 0;
	for (size_t i=0; i<k; i++) distancia += calcularDistancia(c, x, i);
	return distancia/k;
}


double evaluarDistancias(vector<double> &vec_dist, vector<Cluster> c, vector<Node> x){
	size_t k = c.size();
	double distancia = 0;
	for (size_t i=0; i<k; i++){
		double aux = calcularDistancia(c, x, i);
		distancia+=aux;
		vec_dist.push_back(aux);
	}
	return distancia/k;
}

double buscarDistanciaMaxima(vector<Node> x){
	double max = 0;
	size_t num_elem = x.size();
	for (size_t i=0; i<num_elem; i++){
		for (size_t j=i+1; j<num_elem; j++){
			double aux = distanciaElem(x[i].datos, x[j].datos);
			if (aux>max) max = aux;
		}
	}
	return max;
}

int evaluarRestricciones(size_t &num_res, vector<Cluster> c, vector<Node> x, vector<vector<int>> m){
	size_t tam = m.size();
	int incumplidas=0;
	for (size_t i = 0; i < tam ; i++) {
		for (size_t j = i; j < tam; j++) {
			if (m[i][j]==1){
				num_res++;
				if (x[i].cluster != x[j].cluster) incumplidas++;
			} else if (m[i][j]==-1){
				num_res++;
				if (x[i].cluster == x[j].cluster) incumplidas++;
			}
		}
	}
	return incumplidas;
}

int evaluarRestricciones(vector<Cluster> c, vector<Node> x, vector<vector<int>> m){
	size_t tam = m.size();
	int incumplidas=0;
	for (size_t i = 0; i < tam ; i++) {
		for (size_t j = i; j < tam; j++) {
			if (m[i][j]==1){
				if (x[i].cluster != x[j].cluster) incumplidas++;
			} else if (m[i][j]==-1){
				if (x[i].cluster == x[j].cluster) incumplidas++;
			}
		}
	}
	return incumplidas;
}

void greedy(vector<Cluster> &C, vector<Node> &X, vector<vector<int>> M){
	bool diferencias = true;
	int dif = evaluarRestricciones(C,X,M);
	size_t contador=1;
	centroidesAleatorios(C,X);
	size_t num_elem = X.size();
	size_t num_clus = C.size();
	double distancia = 0;
	while (diferencias){
		int restricciones;
		list<size_t> empate_restricciones;
		for(size_t i = 0; i < num_elem; i++){
			int aux_max=3000;
			bool vacio = false;
			for(size_t j = 0; j < num_clus; j++){
				restricciones = probarCluster(C,X,M,i,j,contador);
				if (restricciones < aux_max){
					empate_restricciones.clear();
					aux_max = restricciones;
					empate_restricciones.push_back(j);
				} else if (restricciones == aux_max) empate_restricciones.push_back(j);
			}
			size_t cluster;
			if (empate_restricciones.size()>1){
				for(auto it=empate_restricciones.begin(); it!=empate_restricciones.end() && !vacio; it++){
					if(C[(*it)].nodos.size()==0){
						cluster = (*it);
						vacio=true;
					}
				}
				if (!vacio) cluster = desempatar(aux_max,i,empate_restricciones,C,X);
			} else cluster = empate_restricciones.front();
			empate_restricciones.clear();
			if (contador==1){
				C[cluster].nodos.push_back(i);
				X[i].cluster=cluster;
			} else if(cluster!=X[i].cluster){
				vector<size_t>::iterator pos;
				bool buscando=true;
				for(vector<size_t>::iterator it=C[X[i].cluster].nodos.begin();buscando;it++){
					if((*it)==i){
						pos=it;
						buscando=false;
					}
				}
				C[X[i].cluster].nodos.erase(pos);
				C[cluster].nodos.push_back(i);
				X[i].cluster=cluster;
			}
		}
		if (evaluarRestricciones(C,X,M)==dif) diferencias = false;
		else dif=evaluarRestricciones(C,X,M);
		contador++;
	}
	centroides(C,X);
}

int probarCluster(vector<Cluster> c, vector<Node> x, vector<vector<int>> m, size_t nodo, size_t clus, size_t iteracion){
	if (iteracion > 1){
		bool buscando=true;
		vector<size_t>::iterator posicion;
		for(vector<size_t>::iterator it=c[x[nodo].cluster].nodos.begin();buscando;it++){
			if((*it)==nodo){
				posicion=it;
				buscando=false;
			}
		}
		c[x[nodo].cluster].nodos.erase(posicion);
	}
	c[clus].nodos.push_back(nodo);
	return evaluarRestricciones(c,x,m);
}

size_t desempatar(int rest, size_t nodo, list<size_t> lista, vector<Cluster> c, vector<Node> x){
	double aux_max=1000;
	size_t best;
	for(auto it=lista.begin();it!=lista.end();it++){
		double distancia = distanciaElem(c[(*it)].centroide, x[nodo].datos);
		if (distancia < aux_max) {
			aux_max = distancia;
			best = (*it);
		}
	}
	return best;
}

double distanciaElem(vector<double> centroide, vector<double> elemento){
	size_t dim = elemento.size();
	vector<double> sumatoria;
	for (size_t i=0; i<dim; i++) sumatoria.push_back(0.0);
	for(size_t i=0; i<dim; i++) sumatoria[i] += pow(centroide[i] - elemento[i], 2);
	double distancia = 0;
	for(size_t i=0; i<dim; i++) distancia += sumatoria[i];
	return sqrt(distancia);
}

void busquedaLocal(vector<Cluster> &C, vector<Node> &X, vector<vector<int>> M){
	size_t num_elem = X.size();
	size_t num_clus = C.size();
	int clus=0;
	for(size_t i=0; i<num_elem; i++){
		clus = Randint(0, num_clus-1);
		X[i].cluster = clus;
		C[clus].nodos.push_back(i);
	}
	centroides(C,X);
	double obj = funcionObjetivo(C,X,M);
	size_t contador=0;
	bool cambios=true;
	while(contador<100000 && cambios){
		cambios=false;
		for (size_t i=0; i<num_elem; i++){
			bool mejor = false;
			centroides(C,X);
			double aux_obj = funcionObjetivo(C,X,M);
			for (size_t j=0; j<num_clus && !mejor; j++){
				if (C[j].nodos.size()>1){
					size_t nuevo_clus = (X[i].cluster+j)%num_clus;
					if (probarVecino(C,X,M,aux_obj,nuevo_clus,i)){
						vector<size_t>::iterator pos;
						bool buscando=true;
						for(vector<size_t>::iterator it=C[X[i].cluster].nodos.begin();buscando;it++){
							if((*it)==i){
								pos=it;
								buscando=false;
							}
						}
						C[X[i].cluster].nodos.erase(pos);
						X[i].cluster=nuevo_clus;
						C[nuevo_clus].nodos.push_back(i);
						mejor=true;
						cambios=true;
					}
				}
			}
		}
		contador++;
	}
	centroides(C,X);
}

bool probarVecino(vector<Cluster> c, vector<Node> x, vector<vector<int>> m,double obj_antes, size_t clus, size_t elem){
	bool buscando=true;
	vector<size_t>::iterator posicion;
	for(vector<size_t>::iterator it=c[x[elem].cluster].nodos.begin();buscando;it++){
		if((*it)==elem){
			posicion=it;
			buscando=false;
		}
	}
	c[x[elem].cluster].nodos.erase(posicion);
	c[clus].nodos.push_back(elem);
	centroides(c,x);
	if(funcionObjetivo(c,x,m) < obj_antes) return true;
	else return false;
}

double funcionObjetivo(vector<Cluster> c, vector<Node> x, vector<vector<int>> m){
	vector<double> vec_dist;
	double max_dist = 0;
	size_t num_res = 0;
	int restricciones = evaluarRestricciones(num_res,c,x,m);
	double distancias = evaluarDistancias(vec_dist,c,x);
	for(size_t i=0; i<vec_dist.size(); i++){
		if (vec_dist[i] > max_dist) max_dist = vec_dist[i];
	}
	return  distancias+(restricciones*max_dist/num_res);
}

void imprimirAsignaciones(vector<Cluster> C){
	for (int i=0; i<C.size();i++){
		cout << "Cluster "<< i << ":" <<endl;
		cout << "{ ";
		for (int j=0; j<C[i].nodos.size();j++){
			cout<<C[i].nodos[j]<<" ";
		}
		cout << "}" << endl;
	}
}
