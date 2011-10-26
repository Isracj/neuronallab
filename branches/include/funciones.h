#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <math.h>


#define MAX 5
#define MIN -5


using namespace std;


double **create_array( int Row, int Col );
double Random( int High, int Low );
void mostrar_matriz(double **matriz, int filas, int cols);
double sumatoria_de_pesos(int cant, double *peso, double *ent);
double funcion_activacion(double potencial_sinaptico,int tipo);

#endif // FUNCIONES_H_INCLUDED
