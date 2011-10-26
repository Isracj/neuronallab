#ifndef CAPANEURONAL_H
#define CAPANEURONAL_H

#include "funciones.h"

class CapaNeuronal
{
    public:
        CapaNeuronal();
        void SetNextLayer(CapaNeuronal *Sig);
        virtual ~CapaNeuronal();
        void New(int cant_deneuronas, CapaNeuronal *ant, FILE *arch);
        double GetCantNeuronas(void);
        double GetSalida(int neurona);
        void SetEntrada(double *entrada);
        void SaveLayer(FILE *archivo);
        void ProcessOutput();
        void ComputeError();
        double ComputeError(double *Target);
        double GetDeltaH(int neurona);
        void GetRightWeight();
        void SetRandomWeight();
        double GetPeso(int Neurona_capa_siguiente, int Neurona_capa_oculta);

    private:
        void GetInputFromBackLayer();
        void KnowingWithBeforeLayer();
        void LoadLayer(FILE *archivo);
        void CreateLayer();
        int cant_neuronas, cant_entradas, capa;
        CapaNeuronal *anterior, *siguiente;
        double **weights;
        double **dweights;
        double *entrada;
        double *salida;

        double *Umbral;
        double Momento;
        double Epsilon;
        double Error_capa;          //El error capa es el LMS(least means square) o regla del minimo error cuadrado
        double *DeltaH;

};

#endif // CAPANEURONAL_H
