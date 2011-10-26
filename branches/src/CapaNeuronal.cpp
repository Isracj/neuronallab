#include "../include/CapaNeuronal.h"


CapaNeuronal::CapaNeuronal()
{
/*    //ctor
    cout<<"creamos la capa";
    this->cant_neuronas = cant_deneuronas;
    this->anterior = ant;
    //Crea la capa neuronal
    this->CreateLayer();
    if (arch !=NULL && ant!=NULL)
    {
        cout<<"Cargamos los pesos";
        //Carga los pesos de la capa desde el archvo arch
        this->LoadLayer(arch);
    }
*/

}

void CapaNeuronal::New(int cant_deneuronas, CapaNeuronal *ant, FILE *arch)
{
    //ctor
    this->cant_neuronas = cant_deneuronas;
    this->anterior = ant;

    //Crea la capa neuronal
    this->CreateLayer();
    if (arch!=NULL&&anterior!=NULL)
    {
        //cout<<"Cargamos los pesos"<<endl;
        //Carga los pesos de la capa desde el archvo arch
        this->LoadLayer(arch);
    }
}

void CapaNeuronal::KnowingWithBeforeLayer()
{
    //Se conecta con la capa anterior para la retropropagacion
    this->anterior->SetNextLayer(this);
}

void CapaNeuronal::CreateLayer()
{
    // Si la capa no tiene capa anterior es la denominada capa de entrada, por lo tanto no se crean pesos sinapticos ni salidas
    if(anterior==NULL)
    {
        //La salida en este caso sera igual a la entrada
        //Esta capa cumple la funcion de propagar los pesos hacia adentro
        //cout <<"Creado capa de entrada"<<endl;
        //cout <<"Cantidad de entradas :"<<cant_neuronas<<endl;
        this->cant_entradas = cant_neuronas;
        this->entrada = new double[cant_entradas];
        this->salida = this->entrada;
    }
    else if(anterior!=NULL)
    {
        //cout <<"Creado capa oculta"<<endl;
        //cout <<"Cantidad de entradas :"<<cant_neuronas<<endl;
        this->KnowingWithBeforeLayer();
        this->cant_entradas = this->anterior->GetCantNeuronas();
        this->salida = new double[cant_neuronas];
        this->Umbral = new double[cant_neuronas];
        this->DeltaH = new double[cant_neuronas];
        this->SetRandomWeight();
        this->entrada = new double[cant_entradas];
    }
    this->Momento = 0.5;
    this->siguiente = NULL;
    this->Epsilon = -0.05;


}

CapaNeuronal::~CapaNeuronal()
{
    //dtor
    delete [] entrada;
    if(anterior!=NULL)
    {
        delete [] DeltaH;
        delete [] *weights;
        delete [] salida;
        delete [] this->Umbral;
    }

}

double CapaNeuronal::GetCantNeuronas(void)
{
	return(this->cant_neuronas);
}

double CapaNeuronal::GetSalida(int neurona)
{
	if(neurona > this->cant_neuronas)
	{
		return 0;
	}
	else
	{
	return this->salida[neurona];
	}
}

void CapaNeuronal::SetNextLayer(CapaNeuronal *Sig)
{
    this->siguiente = Sig;
}

void CapaNeuronal::SetRandomWeight()
{
    int i,j;
    this->weights = new double*[cant_neuronas];
    this->dweights = new double*[cant_neuronas];
    for(i = 0; i<cant_neuronas; i++)
    {
        this->weights[i] = new double[cant_entradas];
        this->dweights[i] = new double[cant_entradas];
        for(j = 0; j<cant_entradas; j++)
        {
            cout<< "Pesos Neurona ("<<i;
            cout<<", "<<j<<") :";
            this->weights[i][j] = Random(2, -2);
            cout<<this->weights[i][j]<<endl;
        }
    }

}

void CapaNeuronal::ProcessOutput()
{
    if(anterior!=NULL)
    {
        this->GetInputFromBackLayer();
        int i ,j;
        for(i=0; i<cant_neuronas;i++)
        {
            //Realiza la multiplicacion de las entradas por los pesos sinapticos correspondientes
            this->salida[i] = sumatoria_de_pesos(this->cant_entradas, this->weights[i], this->entrada);
            //cout<<"Sumatoria:"<<this->salida[i]<<endl;
            //Le resta el umbral
            this->salida[i] += this->Umbral[i];
             //cout<<"Se le resta el umbral:"<<this->Umbral[i]<<endl;
             //cout<<"Y nos queda: "<<this->salida[i]<<endl;
            //Pasa el potencial a la funcion de salida, el uno indida que se va a utilizar la func sigmoide
            this->salida[i] = funcion_activacion(this->salida[i], 1);
            //cout<<"La salida de la funcion:"<<this->salida[i]<<endl;
        }

    }
}

// Obtiene las entradas pidiendolas a la capa anterior
void CapaNeuronal::GetInputFromBackLayer()
{

    int i;
    for(i = 0; i<this->cant_entradas; i++)
    {
        this->entrada[i] = this->anterior->GetSalida(i);

    }
}
/**************************************************************************
*   Funcion que obtiene el delta de las capas ocultas.
*   Aqui el error o delta esta dado por:
*   delta= sumatoria[(delta de la neurona de la siguiente capa)*(peso de la conexion)]
*   Luego delta *= f'(entrada) que es igual a delta = delta *(1-salida)*salida
**************************************************************************/
void CapaNeuronal::ComputeError()
{
    this->Error_capa = 0.0f;
    int i,j, neuronas_capa_siguiente = this->siguiente->GetCantNeuronas();

    for(i=0; i<this->cant_neuronas; i++)
    {
        //cout <<"Calculando el error de la capa oculta neurona: "<<i<<endl;
        double Error_tmp = 0.0f;
        for(j=0; j<neuronas_capa_siguiente; j++)
        {
            Error_tmp += this->siguiente->GetDeltaH(j)*this->siguiente->GetPeso(j, i);
            //cout <<"Error: "<<Error_tmp<<endl;

            this->DeltaH[i] = (1 - this->salida[i]) * this->salida[i] * Error_tmp;
            //cout <<"DeltaH["<<i<<"] :"<<this->DeltaH[i]<<endl;
        }

    }

}

double CapaNeuronal::ComputeError(double *Target)
{
    this->Error_capa = 0.0;
    int i,j;
    double Error_tmp = 0.0;
    for(j=0; j<this->cant_neuronas; j++)
    {
        Error_tmp = (Target[j] - this->salida[j]);
        //cout <<"Diferencia con la entrada esperada: "<<Error_tmp<<endl;
        //cout <<"Salida :"<< this->salida[j]<<endl;
        this->DeltaH[j] = (1 - this->salida[j]) * this->salida[j] * Error_tmp;
        //cout << "DeltaH["<<j<<"] :"<<this->DeltaH[j]<<endl;
        Error_capa += (0.5*Error_tmp*Error_tmp);
        //cout <<"Error capa: "<<Error_capa<<endl;
    }
    return Error_capa;
}

double CapaNeuronal::GetDeltaH(int neurona)
{
    if(neurona>this->cant_neuronas)
    {
        perror("El valor sobrepasa la cantidad de neuronas de la capa");
        return 0;
    }
    else
    {
        return( this->DeltaH[neurona]);
    }
}

double CapaNeuronal::GetPeso(int Neurona_capa_siguiente, int Neurona_capa_oculta)
{
    return (this->weights[Neurona_capa_siguiente][Neurona_capa_oculta]);
}

void CapaNeuronal::GetRightWeight()
{
    int i,j;
    double tmp;
    for(i = 0; i<this->cant_neuronas; i++)
    {
        for(j = 0; j<this->cant_entradas;j++)
        {
            tmp = Epsilon*this->DeltaH[i]*this->entrada[i];
            if(this->siguiente!=NULL)
            {
                int aux = this->siguiente->GetCantNeuronas();
                for(int h=0; h< aux; h++ )
                {
                    tmp *= this->siguiente->GetPeso(h, i);
                }
            }
            this->weights[i][j] += tmp + (this->Momento*this->dweights[i][j]);
            this->dweights[i][j] = Epsilon*this->DeltaH[i]*this->entrada[i];
            //cout <<"Valor adicionado al peso ("<<i<<","<<j<<"): "<<tmp<<endl;
            //cout <<"Delta["<<i<<"] :"<<this->DeltaH[i]<<endl;
            //cout <<"Salida "<<i<<": "<<this->salida[i]<<endl;
            //cout << "Nuevo peso ("<<i<<","<<j<<"): "<< this->weights[i][j]<<endl;
        }
        this->Umbral[i] += this->Epsilon*this->DeltaH[i];
    }

}

void CapaNeuronal::SetEntrada(double *In)
{
    if(this->anterior==NULL)
    {
        int i;
        for(i=0; i<this->cant_neuronas;i++)
        {
            this->entrada[i] = In[i];
            //cout <<"Seteando Entrada ("<<i<<") :"<<In[i]<<endl;
        }
    }

}

//Funcion que carga los pesos desde un archivo ya abierto por la clase RedNeuronal
void CapaNeuronal::LoadLayer(FILE *archivo)
{
    if(this->anterior!=NULL)
    {
        double tmp;
        int i,j;
        for(i=0; i<cant_neuronas;i++)
        {
            for(j=0; j<cant_entradas; j++)
            {
                while(fscanf(archivo, "%*s %lf", &tmp)==0);
                this->weights[i][j] = tmp;
                //cout<<"Pesos cargados("<<i<<","<<j<<"): "<<this->weights[i][j]<<endl;
            }
            while(fscanf(archivo, "%*s %lf", &this->Umbral[i])==0);
            //cout <<"Umbral cargado("<<i<<"): "<<this->Umbral[i]<<endl;
        }
    }
}

void CapaNeuronal::SaveLayer(FILE *archivo)
{

    int i,j;
    for(i=0; i<cant_neuronas;i++)
    {
        for(j=0; j<cant_entradas; j++)
        {
            fprintf(archivo, "Peso(%d,%d): ", i,j);
            fprintf(archivo, "%lf \n", this->weights[i][j]);
        }
        fprintf(archivo, "Umbral(%d): %lf \n",i, this->Umbral[i]);
    }

}


