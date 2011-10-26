#include "../include/RedNeuronal.h"



RedNeuronal::RedNeuronal(int n_capas, int *n_neur)
{
    //ctor
    this->numero_de_capas = n_capas;
    this->n_neuronas = new int[n_capas];
    int i;
    for(i=0; i<n_capas;i++)
    {
        this->n_neuronas[i] = n_neur[i];
    }
    this->CreateNetwork(NULL);

}

RedNeuronal::RedNeuronal(char *archivo)
{
    //Esta funcion lee el numero de capas y el vector de neuronas luegoo se llama a CreateNetwork para crear la red
    this->LoadNetwork(archivo);


}

RedNeuronal::~RedNeuronal()
{
    //dtor
    delete [] Input;
    delete [] Output;
    delete [] Target;
    delete [] capa0;
    delete [] n_neuronas;
    //fcloseall();
}

//Funcion que alimenta la red y genera una salida
void RedNeuronal::FeedPropagate()
{
    int i;
    for(i=0; i<this->numero_de_capas; i++)
    {
        this->capa0[i].ProcessOutput();
    }
    i--;
    for(int j = 0; j<this->n_neuronas[i];j++)
    {
        this->Output[j] = this->capa0[i].GetSalida(j);
        cout<<endl;
        //cout<<"Salida :"<<this->Output[j]<<endl;
    }

}

//Funcion que retropropaga el error para corregir los pesos
void RedNeuronal::BackPropagate()
{
    //this->capa0[0].SetEntrada(this->Input);
    this->FeedPropagate();
    this->ComputeTotalError();
}

//Funcion que calcula el error de la red en cada capa
void RedNeuronal::ComputeTotalError()
{
    int nc1 = this->numero_de_capas -1;
    this->ErrorTotal = 0;
    this->ErrorTotal += this->capa0[nc1].ComputeError(this->Target);

    //cout<<"Error Total: "<<this->ErrorTotal<<endl;

    // nc1 se decrementa por q la capa de salida tiene que calcular el error con la funcion ComputeError(double *target)
    // Mientras que las de la capa oculta no lo debe hacer

    for(--nc1; nc1>0; nc1--)
    {
        this->capa0[nc1].ComputeError();
    }
        //this->correc--;
    for(nc1=1; nc1<this->numero_de_capas;nc1++)
    {
            this->capa0[nc1].GetRightWeight();
    }

}


//Funcion que crea la red
void RedNeuronal::CreateNetwork(FILE *archivo)
{
    int i= 0;
    //cout<<"Creamos la capa: 0"<<endl;
    //cout<<"Numero de neuronas : "<< n_neuronas[i]<<endl;
    this->capa0 = new CapaNeuronal[this->numero_de_capas];
    this->capa0[0].New(n_neuronas[0], NULL, NULL);
    this->cant_entradas = n_neuronas[i];
    int n;
    for (i = 1; i<this->numero_de_capas; i++)
    {
        //cout<<"Creamos la capa: "<<i<<endl;
        this->capa0[i].New((int)(n_neuronas[i]),&this->capa0[i-1],archivo);
    }
    i--;
    this->cant_salidas = n_neuronas[i];
    this->Target = new double[cant_salidas];
    this->Input = new double[cant_entradas];
    this->Output = new double[cant_salidas];


    fcloseall();

}


void RedNeuronal::SaveNetwork(char *file_name)
{
    FILE *f;
    f = fopen(file_name, "w");
    if(!f)
    {
        cout <<"Error al cargar la red!!"<<endl;
        perror(file_name);
        exit(-2);
    }
    else
    {
        //Leer el numero de capas
        fprintf(f, "Numero de capas: %d\n", this->numero_de_capas);
        int i;
        for(i=0; i<numero_de_capas;i++)
        {
            while(fprintf(f, "Neuronas capa%d: %d\n",i, this->n_neuronas[i])==0);
        }
        i = 1;
        fprintf(f, "Cargar pesos? cero=NO uno=SI: %d\n", i);
        if(i==1)
        {
            for(;i<numero_de_capas; i++)
            {
                fprintf(f, "Pesos capa%d:\n", i);
                this->capa0[i].SaveLayer(f);
                fprintf(f, "\n");
            }
        }
        fclose(f);
    }

}

void RedNeuronal::LoadNetwork(char *file_name)
{
    FILE *f = fopen(file_name, "r");
    if(!f)
    {
        cout <<"Error al cargar la red!!"<<endl;
        perror(file_name);
        exit(-2);
    }
    else
    {
        //Leer el numero de capas
        while(fscanf(f, "%*s %d", &this->numero_de_capas)==0);
        int i;
        int estado;
        this->n_neuronas = new int[numero_de_capas];
        for(i=0; i<numero_de_capas;i++)
        {
            while(fscanf(f, "%*s %d", &this->n_neuronas[i])==0);
            //cout<<"Leemos la capa numero : "<<i<<endl;
            //cout<<"Numero de neuronas: "<<this->n_neuronas[i]<<endl;

        }
        while(fscanf(f, "%*s %d", &estado)==0);
        //cout<<"Se encuentra entrenada la red? : "<<estado<<endl;
        if(estado==0)
        {
            fcloseall();
            f = NULL;
        }
        this->CreateNetwork(f);
    }


}

long int RedNeuronal::TrainNetwork(char *archivito)
{
    int epocas_max = 200;
    this->ErrorTotal = 0;
    long int epocas = 0;
    int corregido, i, j;
    this->PrepareTraining(archivito);
    FILE *f;
    f = fopen("error.dat", "w");
    if(!f)
    {
        perror("Error al abrir el archivo\n");
        exit(-1);
    }
    if(n_casos >0 )
    {

        do
        {
            for(i=0; i<n_casos; i++)
            {
                //correc = n_casos;
                this->capa0[0].SetEntrada(patron[i]);
                //this->Input[j] = patron[i][j];
                //cout<<"Entrada("<<j<<"):"<<this->Input[j]<<endl;
                //cout<<"Entrada("<<j<<"):"<<patron[i][j]<<endl;
                for(int h = 0, j = cant_entradas; h<this->cant_salidas ; h++, j++)
                {
                    this->Target[h] = patron[i][j];
                    //cout <<"Target en Red: "<<this->Target[h]<<endl;
                }
                //cout<<"Entrenando...."<<endl;
                this->Train();
            }
            fprintf(f, "%d %lf \n",epocas, this->ErrorTotal);
            cout<<"Epoca Numero:"<<epocas<<" Error Total de red: "<<this->ErrorTotal<<endl;
            epocas++;
        }while( epocas < epocas_max);
    }
    fclose(f);
    this->PlotTraining();
    delete [] patron;
    return epocas;

}

void RedNeuronal::Train()
{
        this->BackPropagate();
}


void RedNeuronal::PrepareTraining(char *archivo)
{
    FILE *f = fopen(archivo, "r");
    if(!f)
    {
        perror(archivo);
    }
    else{
    int casos, i, j;
    while(fscanf(f, "%*s %d", &casos)==0);
    //cout <<"Se leyeron "<<casos<<" casos"<<endl;
    //patrones = create_array(casos, this->cant_entradas+this->cant_salidas);
    //cout<<"Se creo un array para entrenar de "<<casos<< "x "<<this->cant_entradas+this->cant_salidas<<" dimensiones"<<endl;
    patron = new double*[casos];
    for(i=0; i<casos; i++)
    {
        patron[i] = new double[this->cant_entradas+this->cant_salidas];
        for(j=0; j<(this->cant_entradas+this->cant_salidas);j++)
        {
            while(fscanf(f, "%*s %lf", &patron[i][j])==0);
            //cout<<"Entrada leida numero "<<j<<" :"<<patron[i][j]<<endl;
        }
    }
    fclose(f);
    //cout<<"Se termino la preparacion"<<endl;
    this->n_casos = casos;}
}

void RedNeuronal::RunNetwork(char *archivito)
{
    FILE *f = fopen(archivito, "r");
    if(!f)
    {
        perror(archivito);
    }
    else
    {
        int i, j, casos;
        while(fscanf(f, "%*s %d", &casos)==0);
        for(i=0; i<casos; i++)
        {
            for(j=0; j<this->cant_entradas;j++)
            {
                double tmp;
                while(fscanf(f, "%*s %lf", &tmp)==0);
                this->Input[j] = tmp;
                //cout<<"Entrada leida "<<j<<" : "<<this->Input[j]<<endl;
            }
            this->capa0[0].SetEntrada(this->Input);
            this->FeedPropagate();
            cout<<"Caso numero: "<<i<<endl;
            for(j = 0; j<this->cant_salidas; j++)
            {
                cout<<"Salida de la red("<<j<<") :"<<this->Output[j]<<endl;
            }

        }
        fclose(f);
    }
}


double RedNeuronal::GetSalida(int neurona)
{
    if(neurona>this->cant_salidas)
    {
        return 0.0f;
    }
    else
    return(this->Output[neurona]);
}

void RedNeuronal::PlotTraining()
{

	/*
	fprintf(pipe, "set samples 40\n");
	fprintf(pipe, "set isosamples 40\n");
	fprintf(pipe, "set hidden3d\n");
	fprintf(pipe, "set xrange [-8.000:8.000]\n");
	fprintf(pipe, "set yrange [-8.000:8.000]\n");
	fprintf(pipe, "set zrange [-2.000:2.000]\n");
	fprintf(pipe, "set terminal png\n");
	fprintf(pipe, "splot cos(x)+cos(y)\n");
	fprintf(pipe, "set output 'graph.png'\n");
	*/
	FILE *pipe = popen("gnuplot -persist","w");
	fprintf(pipe, "set title 'Entrenamiento de la red Neuronal'\n");
	fprintf(pipe, "set xlabel 'Cantidad de epocas'\n");
	fprintf(pipe, "set ylabel 'Error cuadratico medio'\n");
	fprintf(pipe, "plot 'error.dat' using 1:2\n");

	pclose(pipe);

}
