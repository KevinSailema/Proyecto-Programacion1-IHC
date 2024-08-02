#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int imprimir_opciones(string tipo1, string tipo2, string tipo3, string tipo4, string tipo5){
    int n;
    cout << "\n1. " << tipo1 << endl;
    cout << "2. " << tipo2 << endl;
    cout << "3. " << tipo3 <<endl;
    cout << "4. " << tipo4 << endl;
    cout << "5. " << tipo5 <<endl;
    cin>>n;
    return n;
}

//ingreso de cliente
void ingreso_cliente(){
    cout<<"********************"<<endl;
    cout<<"INGRESO DE CLIENTES"<<endl;
    cout<<"********************"<<endl;
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

//Actualizar Datos
void actualizar_datos(string tipo){
    bool encontrado = false;
    string linea, buscar, reemplazar;
    cin.ignore();
    cout << "Ingrese " << tipo << " a actualizar: ";
    getline(cin, buscar);
    ifstream archivo("CRUD.csv");
    ofstream archivo_temp("temp.csv");
    if (archivo.is_open() && archivo_temp.is_open()) {
        while (getline(archivo, linea)) {
            size_t pos = linea.find(buscar);
            if (pos != string::npos) {
                cout << "Ingrese el nuevo dato: ";
                getline(cin, reemplazar);
                linea.replace(pos, buscar.length(), reemplazar);
                encontrado = true;
            }
            archivo_temp << linea << endl;
        }
        if (encontrado) {
            cout << "Dato actualizado con exito" << endl;
        } else {
            cout << "No se encontro el dato" << endl;
        }
        archivo.close();
        archivo_temp.close();
        remove("CRUD.csv");
        rename("temp.csv", "CRUD.csv");
    } else {
        cout << "Error al abrir el archivo" << endl;
        exit(1);
    }
}

//Switch Actualizar Datos
void switch_actualizar_datos(){
    cout<<"**************************"<<endl;
    cout<<"INGRESO A ACTUALIZAR DATOS"<<endl;
    cout<<"**************************"<<endl;
    cout<<"Seleccione el dato que desea actualizar: "<<endl;
    int n;
    n = imprimir_opciones("Actualizar nombres", "Actualizar apellidos", "Actualizar cedula", "Actualizar numero de tarjeta", "Actualizar CVC");
    switch (n)
    {
    case 1:
        actualizar_datos("los nombres");
        break;
    case 2:
        actualizar_datos("los apellidos");
        break;
    case 3:
        actualizar_datos("la cedula");
        break;
    case 4:
        actualizar_datos("el numero de tarjeta");
        break;
    case 5:
        actualizar_datos("el CVC");
        break;
    default:
        cout<<"Opcion no valida"<<endl;
        break;
    }
}


//Eliminar Datos
void eliminar_cliente(){
    cout<<"**************************"<<endl;
    cout<<"INGRESO A ELIMINAR CLIENTE"<<endl;
    cout<<"**************************"<<endl;
    long long int cedula;
    cout<<"Ingrese la cedula del cliente a eliminar: ";
    cout<<"(Se borraran todos los datos adjuntos al cliente)"<<endl;
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
    int n;
    crear_archivo();
    do
    {
        n = imprimir_opciones("Ingresar cliente nuevo", "Consultar cliente", "Actualizar cliente", "Eliminar cliente", "Salir del programa");
        switch (n)
        {
        case 1: //ingreso de clientes
            ingreso_cliente();
            break;
        case 2:
            break;
        case 3:
            switch_actualizar_datos();
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