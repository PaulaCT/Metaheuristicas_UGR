#include <math.h>
#include "../include/random.h"
#include <algorithm>
#include <random>

unsigned long Seed = 0L;

#define MASK 2147483647
#define PRIME 65539
#define SCALE 0.4656612875e-9

void Set_random (unsigned long x)
/* Inicializa la semilla al valor x.
   Solo debe llamarse a esta funcion una vez en todo el programa */
{
    Seed = (unsigned long) x;
}

unsigned long Get_random (void)
/* Devuelve el valor actual de la semilla */
{
    return Seed;
}

float Rand(void)
/* Genera un numero aleatorio real en el intervalo [0,1[
   (incluyendo el 0 pero sin incluir el 1) */
{
    return (( Seed = ( (Seed * PRIME) & MASK) ) * SCALE );
}

int Randint(int low, int high)
/* Genera un numero aleatorio entero en {low,...,high} */
{
    return (int) (low + (high-(low)+1) * Rand());
}

float Randfloat(float low, float high)
/* Genera un numero aleatorio real en el intervalo [low,...,high[
   (incluyendo 'low' pero sin incluir 'high') */
{
    return (low + (high-(low))*Rand());
}

std::vector<size_t> Randfromvector(std::vector<size_t> valores, size_t tam){
  Shuffle(valores);
  std::vector<size_t> random_vector;
  for (size_t i=0; i<tam; i++) random_vector.push_back(valores[i]);
  return random_vector;
}

void Shuffle(std::vector<size_t> valores){
  shuffle(valores.begin(), valores.end(),std::default_random_engine(Seed));
}
