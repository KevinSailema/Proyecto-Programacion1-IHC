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

//Ingreso de cliente
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
        while(numero_tarjeta<=1000000000000000||numero_tarjeta>=10000000000000000){
            cout<<"El número de tarjeta debe tener 16 dígitos, intentelo de nuevo: ";
            cin>>numero_tarjeta;
        }
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

//Crear archivo
void crear_archivo(){
    ofstream archivo("CRUD.csv",ios::app);
    if(archivo.is_open()){
        cout << "\nArchivo creado correctamente" << endl;
        archivo.close();
    }else{
        cout << "Error al crear el archivo" << endl;
        exit(1);
  }
}

//Consultar cliente
void consultar_cliente(){
    cout<<"******************"<<endl;
    cout<<"CONSULTAR CLIENTE"<<endl;
    cout<<"******************"<<endl;
    long long int cedula;
    cout<<"Ingrese la cedula del cliente a consultar: ";
    cin>>cedula;
    while(cedula<=100000000||cedula>=2500000000){
        cout<<"El número de cedula debe tener diez dígitos, ingrese nuevamente: ";
        cin>>cedula;
        cout<<endl;
    }
    ifstream archivo("CRUD.csv");
    if(archivo.is_open()){
        string linea;
        bool encontrado;
        string partes[6];
        while(getline(archivo, linea)){
                size_t start = 0, end = 0;
                for (int i = 0; i < 5; i++) {
                    end = linea.find(';', start);
                    if (end == string::npos) end = linea.length();
                    partes[i] = linea.substr(start, end - start);
                    start = end + 1;
                }
                linea = partes[0] + ";" + partes[1] + ";" + partes[2] + ";" + partes[3] + ";" + partes[4] + ";" + partes[5] + ";" + partes[6];
            if(linea.find(to_string(cedula))!=string::npos){ //Cuando encuentra la cedula, imprime la linea en donde la encontró
                cout<<"Cliente encontrado: ";
                cout<<partes[0]<<" "<<partes[1]<<" / "<<partes[2]<<endl;
                encontrado=true;
            }
        }
        if(!encontrado){ //Si no encuentra la cedula, imprime que no se encontró
            cout<<"Cliente no encontrado"<<endl;
        }
        int n;
        cout<<"1. Ver tarjetas registradas"<<endl;
        cout<<"2. Ingresar nueva tarjeta"<<endl;
        switch (n)
        {
        case 1:
            /* code */
            break;
        
        default:
            break;
        }
        archivo.close();
}
}

//

//Actualizar Datos
void actualizar_datos(string campo, int campo_num){
    bool encontrado = false;
    string linea, reemplazar;
    long long int cedula;
    cout << "Ingrese la cedula del cliente del cual se quieren actualizar los datos: ";
    cin >> cedula;
    while (cedula <= 100000000 || cedula >= 2500000000){
        cout << "El número de cedula debe tener diez dígitos, ingrese nuevamente: ";
        cin >> cedula;
        cout << endl;
    }

    ifstream archivo("CRUD.csv");
    ofstream archivo_temp("temp.csv");

    if (archivo.is_open() && archivo_temp.is_open()){
        while (getline(archivo, linea)){
            size_t pos = linea.find(to_string(cedula));
            if (pos != string::npos){
                cin.ignore();
                cout << "Ingrese " << campo << " a actualizar: ";
                getline(cin, reemplazar);
                encontrado = true;
                string partes[6];
                size_t start = 0, end = 0;
                for (int i = 0; i < 5; i++) {
                    end = linea.find(';', start);
                    if (end == string::npos) end = linea.length();
                    partes[i] = linea.substr(start, end - start);
                    start = end + 1;
                }
                partes[campo_num] = reemplazar;
                linea = partes[0] + ";" + partes[1] + ";" + partes[2] + ";" + partes[3] + ";" + partes[4] + ";" + partes[5] + ";" + partes[6];
            }
            archivo_temp << linea << endl;
        }
        archivo.close();
        archivo_temp.close();
        remove("CRUD.csv");
        rename("temp.csv", "CRUD.csv");
        if (encontrado){
            cout << "El dato ha sido actualizado con exito" << endl;
        } else {
            cout << "No se encontro el cliente" << endl;
        }
    } else {
        cout << "Error al abrir el archivo" << endl;
        exit(1);
    }
}

// Switch Actualizar Datos
void switch_actualizar_datos(){
    cout << "**************************" << endl;
    cout << "INGRESO A ACTUALIZAR DATOS" << endl;
    cout << "**************************" << endl;
    cout << "Seleccione el dato que desea actualizar: " << endl;
    int n;
    n = imprimir_opciones("Actualizar nombres", "Actualizar apellidos", "Actualizar cedula", "Actualizar numero de tarjeta", "Actualizar CVC");
    switch (n){
        case 1:
            actualizar_datos("el nombre", 0);
            break;
        case 2:
            actualizar_datos("los apellidos", 1);
            break;
        case 3:
            actualizar_datos("la cedula", 2);
            break;
        case 4:
            actualizar_datos("el numero de tarjeta", 3);
            break;
        case 5:
            actualizar_datos("el CVC (codigo de seguridad)", 4);
            break;
        default:
            cout << "Opcion no valida" << endl;
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
            if(linea.find(to_string(cedula))==string::npos){ //si NO se encuentra la cedula en el archivo original esta NO se copia al archivo temporal que pasa a ser el original
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
            consultar_cliente();
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