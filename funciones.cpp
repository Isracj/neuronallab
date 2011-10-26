#include "../include/funciones.h"


//Crear una matriz de valores "double"
double **create_array( int Row, int Col )
{
    double **array = new double*[Row];
    for ( int i = 0; i < Row; i++ )
        array[i] = new double[Col];
    return array;
}

//Generador de numeros aleatorios, desde un minimo a un maximo
double Random( int High, int Low )
{
    time_t segs;
    //se usa la funcion time(NULL) para no tener siempre la misma secuencia de aleatorios
    srand( ( unsigned int )(segs % 65536) );
    //retorna el numero aleatorio
    return (((double)rand()/RAND_MAX) * (High - Low) + Low);
}

void mostrar_matriz(double **matriz, int filas, int cols)
{
    for(int i = 0; i <filas; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            cout<<"Valor ["<<i<<"] ["<<j<<"]: "<<matriz[i][j]<<endl;
        }

    }
}


double sumatoria_de_pesos(int cant, double *peso, double *ent)
{
    int i;
    double sum = 0.0;
    for(i = 0; i<cant; i++)
    {
        sum += peso[i]*ent[i];
    }

    return sum;
}

double funcion_activacion(double potencial_sinaptico,int tipo)
{
    double salida = 0.0;
    switch(tipo)
    {
        case 0:
            // Identidad
            salida = potencial_sinaptico;
            break;
        case 1:
            //  Sigmoidea
            salida = 1/(1+ (exp(-1*potencial_sinaptico)));
            break;
        case 2:
            // Signo
            if(potencial_sinaptico>0.0)
                salida = 1;
            else
                salida = 0;
            break;
        case 3:
            //Gaussiana
            salida = exp(potencial_sinaptico*potencial_sinaptico);
            break;
        default:
            //Error
            cout<<"La funcion no esta definida se tomara la lineal por defecto";
            salida = potencial_sinaptico;
    }
    return salida;

}
