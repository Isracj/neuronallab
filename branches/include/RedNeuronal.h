#ifndef REDNEURONAL_H
#define REDNEURONAL_H

#include "CapaNeuronal.h"


class RedNeuronal
{
    public:
        RedNeuronal(int ncapas, int *n_neuronas);
        RedNeuronal(char *archivo);
        virtual ~RedNeuronal();
        void CreateNetwork(FILE *archivo);
        void RunNetwork(char *archivito);
        double GetSalida(int neurona);
        long int TrainNetwork(char *archivito);
        void SaveNetwork(char *file_name);
        void LoadNetwork(char *file_name);

    private:
        void FeedPropagate();
        void BackPropagate();
        void ComputeTotalError();
        void Train();
        void PlotTraining();
        void PrepareTraining(char *archivo);

        double *Target;
        double *Input;
        double *Output;
        double ErrorTotal;
        double **patron;
        int n_casos;
        int correc;
        int cant_entradas, cant_salidas;
        int *n_neuronas;
        int numero_de_capas;
        CapaNeuronal *capa0;
};

#endif // REDNEURONAL_H
