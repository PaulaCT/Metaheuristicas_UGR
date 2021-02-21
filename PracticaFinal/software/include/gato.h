#ifndef GATO_H
#define GATO_H

#include <string>
#include <vector>
#include <list>
#include <utility>
#include "random.h"
#include "util.h"
#include "cso.h"

void set_num_clus(size_t num);

class Gato{
private:

	//Atributos
	Comportamiento modo;
  std::vector<size_t> posicion;
  std::vector<double> velocidad;
  double limite;

  //Genera el vector de probabilidades para el modo seeking
  std::vector<double> probabilidad(std::vector<double> f_objetivo, size_t max, size_t min);

  //Selecciona la posición con mayor probabilidad
  size_t seleccionar_mejor(std::vector<double> probabilidad);

  //Comprueba que ningún cluster se quede vacío
  void no_vacio();

public:

  //Constructores
  Gato(size_t num_pos, double limite);
  Gato(std::vector<size_t> posicion, double limite);

  //Modificadores
  void cambiar_comportamiento(Comportamiento accion);
	void cambiar_posicion(std::vector<size_t> pos);

	//Consultores
  Comportamiento get_comportamiento();
  std::vector<size_t> get_posicion();

  //Operadores para las acciones seeking y tracing
  void seeking(size_t SMP, size_t SRD, size_t CDC, bool SPC, std::vector<Node> X, std::vector<std::vector<int>> M);
  void tracing(size_t coeficiente,std::vector<size_t> mejor_posicion);

  //Función objetivo
  double funcion_objetivo_gato(std::vector<Node> X, std::vector<std::vector<int>> M);

	//Destructor
  ~Gato();
};

#endif
