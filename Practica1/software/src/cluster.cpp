#include "util.h"
#include "random.h"
#include <iostream>
#include <chrono>
#include <cstring>

using namespace std;


int main(int argc, char *argv[]){
	Modo modo = comprobarParametros(argc, argv);
	Conjunto conjunto = seleccionConjunto(argv[0],argv[2]);
	ifstream read_file(abrirFicheroElementos(conjunto));
  ifstream restriction_file(abrirFicheroRestricciones(conjunto, argv[3]));
	size_t num_ejecuciones = stof(argv[4]);
	vector<int> semillas = crearVectorSemillas(argc, argv);
	if(!read_file or !restriction_file){
		cerr << "Error con los ficheros de entrada" << endl;
		return 1;
	}
	vector<Node> X = leerNodos(conjunto, read_file);
	vector<vector<int>> M = leerRestricciones(conjunto, restriction_file);
	vector<Cluster> C;
	dimensionarParticion(conjunto,C);
	bool resultado = mostrarResultadoParticion(argv[0],argv[5]);


	for(size_t s=0; s<semillas.size(); s++){
		Set_random(semillas[s]);
		cout<< endl<< "Semilla = "<<semillas[s] <<endl;
		if (!resultado) cout<< "Tasa_C|Tasa_i|Agreg|Tiempo" << endl;

		for(size_t i=0; i<num_ejecuciones; i++){
			for(size_t i=0; i<C.size(); i++) C[i].nodos.clear();
			chrono::time_point<std::chrono::high_resolution_clock> tiempo_inicial, tiempo_final;
			switch(modo){
				case GREEDY:
					tiempo_inicial = chrono::high_resolution_clock::now();
					greedy(C,X,M);
					tiempo_final = chrono::high_resolution_clock::now();
					break;
				case BUSQUEDA_LOCAL:
					tiempo_inicial = chrono::high_resolution_clock::now();
					busquedaLocal(C,X,M);
					tiempo_final = chrono::high_resolution_clock::now();
					break;
				default: break;
			}
			if (resultado)cout<< endl << endl << "Tasa_C|Tasa_i|Agreg|Tiempo - repetición " << i+1 << endl;
			unsigned long tiempo_ejecucion = chrono::duration_cast<std::chrono::nanoseconds>(tiempo_final - tiempo_inicial).count();
			cout << evaluarDistancias(C,X) << " " << evaluarRestricciones(C,X,M) << " " << funcionObjetivo(C,X,M) << " " << tiempo_ejecucion<< endl;
			if (resultado) imprimirAsignaciones(C);
		}
	}

}
