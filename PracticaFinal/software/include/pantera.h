#ifndef PANTERA_H
#define PANTERA_H

#include <vector>
#include <utility>
#include "random.h"
#include "util.h"
#include "cso.h"
#include "gato.h"

void set_num_clus_p(size_t num);

class Pantera{
private:

	//Atributos
	Comportamiento modo;
  std::vector<size_t> posicion;
  std::vector<double> velocidad;

  //Comprueba que ningún cluster se quede vacío
  void no_vacio();

	//Devuelve true si la posición candidata obtiene mejor evaluación que la actual
	bool mejor_fobj(std::vector<size_t> candidata, std::vector<Node> X, std::vector<std::vector<int>> M);

	//Operador de cambio de cluster (generador de vecinos)
	std::vector<size_t> cambio_cluster(size_t elem);

public:

  //Constructor
  Pantera(size_t num_pos);

  //Modificador
  void cambiar_comportamiento(Comportamiento accion);

	//Consultor
  Comportamiento get_comportamiento();
  std::vector<size_t> get_posicion();

	//Operador seeking: genera tantas posiciones como dimensiones, alterando solo una dimensión.
	//Si mejora la función objetivo, se copia dicha dimensión en la posición actual.
  void seeking(size_t SRD, bool SPC, std::vector<Node> X, std::vector<std::vector<int>> M);

	//Operador tracing: No existe límite de velocidad, y se comprueba la mejoría antes de actualizar.
	void tracing(size_t coeficiente,std::vector<size_t> mejor_posicion, std::vector<Node> X, std::vector<std::vector<int>> M);

	//Operador cleaning: Aplica una búsqueda_local pequeñita CDC
	void cleaning(size_t CDC, std::vector<Node> X, std::vector<std::vector<int>> M);

  //Función objetivo
  double funcion_objetivo_pantera(std::vector<Node> X, std::vector<std::vector<int>> M);

	//Destructor
  ~Pantera();
};

#endif
