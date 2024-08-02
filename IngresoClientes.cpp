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
    long long int cedula, numero_tarjeta;
    int cvc;
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
        cout<<"Ingrese el número de la tarjeta: ";
        cin>>numero_tarjeta;
        cout<<"Ingrese el CVC de la tarjeta: ";
        cin>>cvc;
        while (cvc>=1000 || cvc<100){
            cout<<"El CVC debe tener al menos tres dígitos, intentelo de nuevo: ";
            cin>>cvc;
        }
        archivo<<nombres<<";"<<apellidos<<";"<<cedula<<";"<<numero_tarjeta<<";"<<cvc<<endl;
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

//Eliminar Datos
void eliminar_cliente(){
    long long int cedula;
    cout<<"Ingrese la cedula del cliente a eliminar: ";
    cin>>cedula;
    while(cedula<=100000000||cedula>=2500000000){
        cout<<"El número de cedula debe tener diez dígitos, ingrese nuevamente: ";
        cin>>cedula;
        cout<<endl;
    }
    ifstream archivo("CRUD.csv");
    ofstream archivo_temp("temp.csv");

    if(archivo.is_open() && archivo_temp.is_open()){
        string linea;
        while(getline(archivo, linea)){
            if(linea.find(to_string(cedula))==string::npos){
                archivo_temp<<linea<<endl;
            }
        }
        archivo.close();
        archivo_temp.close();
        remove("CRUD.csv");
        rename("temp.csv","CRUD.csv");
        cout<<"Datos del cliente eliminados con exito "<<endl;
    }else{
        cout<<"Error al abrir los archivos"<<endl;
        exit(1);
    }
}


int main(){
    setlocale(LC_ALL,"spanish");
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
        case 4:
            eliminar_cliente();
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