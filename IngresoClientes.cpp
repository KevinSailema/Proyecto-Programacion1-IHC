#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int imprimir_opciones(){
    int n;
    cout << "\n1. Ingresar cliente nuevo" << endl;
    cout << "2. Consultar cliente" << endl;
    cout << "3. Actualizar cliente" << endl;
    cout << "4. Eliminar cliente" << endl;
    cout << "5. Salir del programa" << endl;
    cin>>n;
    return n;
}

//ingreso de cliente
void ingreso_cliente(){
    string nombres, apellidos;
    long long int cedula;
    ofstream archivo("CRUD.csv",ios::app);
    if(archivo.is_open()){
        cin.ignore();
        cout<<"Ingrese sus nombres completos: ";
        getline(cin, nombres);
        cout<<"Ingrese sus apellidos completos: ";
        getline(cin, apellidos);
        cout<<"Ingrese su numero de cedula: ";
        cin>>cedula;
        while(cedula<=100000000||cedula>=2500000000){
        cout<<"Número de cedula incorrecto, ingrese nuevamente: ";
        cin>>cedula;
        cout<<endl;
    }
        archivo<<"Nombres: "<<";"<<nombres<<endl; 
        archivo<<"Apellidos: "<<";"<<apellidos<<endl, 
        archivo<<"Cedula: "<<";"<<cedula<<endl;
        archivo.close();
    }else{
        cout << "Error al abrir el archivo" << endl;
        exit(1);
    }
}

//crear archivo
void crear_archivo(){
    ofstream archivo("CRUD.csv",ios::app);
    if(archivo.is_open()){
        cout << "Archivo creado correctamente" << endl;
        archivo.close();
    }else{
        cout << "Error al crear el archivo" << endl;
        exit(1);
    }
}

int main(){
    int n;
    crear_archivo();
    do
    {
        n = imprimir_opciones();
        switch (n)
        {
        case 1: //ingreso de clientes
            ingreso_cliente();
            break;
        case 5:
            cout<<"Gracias por usar el programa"<<endl;
            exit(1);
            break;
        default:
            cout<<"Opcion no valida"<<endl;
            break;
        }
    } while (n <= 4 || n >= 1);
    return 0;
}