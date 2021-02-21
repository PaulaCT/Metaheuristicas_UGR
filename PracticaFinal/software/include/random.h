/* GENERADOR DE NUMEROS ALEATORIOS */

#ifndef __RANDOM_PPIO_H
#define __RANDOM_PPIO_H

#include <vector>

/* Inicializa la semilla al valor 'x'.
   Solo debe llamarse a esta funcion una vez en todo el programa */
void Set_random (unsigned long x);

/* Devuelve el valor actual de la semilla */
unsigned long Get_random (void);

/* Genera un numero aleatorio real en el intervalo [0,1[
   (incluyendo el 0 pero sin incluir el 1) */
float Rand(void);

/* Genera un numero aleatorio entero en {low,...,high} */
int Randint(int low, int high);

/* Genera un numero aleatorio real en el intervalo [low,...,high[
   (incluyendo 'low' pero sin incluir 'high') */
float Randfloat(float low, float high);

/* Genera un vector que contiente tam elementos del vector valores
   aleatoriamente generado */
std::vector<size_t> Randfromvector(std::vector<size_t> valores, size_t tam);

/* Genera un vector desordenado de índices de tamaño tam y devuelve otro
   vector con tantos elementos como indique small_tam */
std::vector<size_t> Rand_indices(size_t tam, size_t small_tam);

/* Desordena un vector */
void Shuffle(std::vector<size_t> &valores);

/* Devuelve 1 o +1 aleatoriamente */
int Rand_sign();

#endif
