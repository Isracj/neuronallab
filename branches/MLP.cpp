#include "include/RedNeuronal.h"


int Menu(void);


int main(int argc, char **argv)
{
    int opc;
    RedNeuronal *red;
    red = NULL;
    char aux[32];
    do
    {
        opc = Menu();

        switch(opc)
        {
            case 1:
            if(red==NULL)
            {
                cin.width(31);
                cout<<"Ingrese el archivo con la estructura de la red: ";
                cin>>aux;
                red = new RedNeuronal(aux);
                cout<<"Se ha cargado la red"<<endl;
            }
            else
            {
                cout<<"Primero elimine la red actual"<<endl;
            }
            break;
            case 2:
            cout<<"Ingrese el archivo con los patrones para entrenar :";
            cin.width(31);
            cin>>aux;
            if(red!=NULL)
            {
                int a = red->TrainNetwork(aux);
                cout<<"Se ha entrenado la red en :"<<a<<" epocas"<<endl;
            }
            else
            {
                cout<<"Primero Cree la red"<<endl;
            }
            break;
            case 3:
            cout<<"Ingrese el archivo con los patrones para ejecutar :";
            cin.width(31);
            cin>>aux;
            if(red!=NULL)
            {
                red->RunNetwork(aux);
            }
            else
            {
                cout<<"Primero Cree la red"<<endl;
            }
            break;
            case 4:
            cout<<"Ingrese el archivo donde quiere guardar la red actualmente :";
            cin.width(31);
            cin>>aux;
            if(red!=NULL)
            {
                red->SaveNetwork(aux);
            }
            else
            {
                cout<<"Primero Cree la red"<<endl;
            }
            break;
            case 5:
            case 0:
            if(red!=NULL)
            {
            delete red;
            }
            break;
            default:
            cout<<"Ingrese una opcion correcta...."<<endl;
        }


    }while(opc!=0);


    return 0;
}

int Menu(void)
{
    cout<<"*************************************************************"<<endl;
    cout<<"*\t Programa para utilizar la clase RedNeuronal \t    *"<<endl;
    cout<<"*\t Dicha clase maneja un Perceptron multicapa de \t  *"<<endl;
    cout<<"*\t Una o varias capas ocultas \t\t   *"<<endl;
    cout<<"*\t Licenciado bajo GPL2 \t\t            *"<<endl;
    cout<<"*************************************************************"<<endl<<endl;
    cout<< "\tIngrese una opcion:"<<endl;
    cout<< "\t\t1 - Para cargar la red"<<endl;
    cout<< "\t\t2 - Para entrenar la red"<<endl;
    cout<< "\t\t3 - Para ejecutar"<<endl;
    cout<< "\t\t4 - Para guardar la red"<<endl;
    cout<< "\t\t5 - Para eliminar la red actual"<<endl;
    cout<< "\t\t0 - Para salir"<<endl;
    int aux;
    cin>>aux;
    return aux;
}
