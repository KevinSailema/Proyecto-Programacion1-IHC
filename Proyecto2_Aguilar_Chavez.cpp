#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h> //Para usar la funcion sleep
#include <cstdlib> //Para usar la funcion cls en windows
#include <cstdio> //Para usar la funcion remove

using namespace std;

void titulo(){
    cout<<"\n*****************************************************************"<<endl;
    cout<<"*    Bienvenido al sistema de gestion de tarjetas de credito    *"<<endl;
    cout<<"*****************************************************************"<<endl;
}

int imprimir_opciones(string tipo1, string tipo2, string tipo3, string tipo4, string tipo5, string tipo6){
    int n;
    cout << "\n1. " << tipo1 << endl;
    cout << "2. " << tipo2 << endl;
    cout << "3. " << tipo3 <<endl;
    cout << "4. " << tipo4 << endl;
    cout << "5. " << tipo5 <<endl;
    cout<<tipo6<<endl;
    cout << "Ingrese la opcion deseada: ";
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
    int cvc, limite_consumo=2000;
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
            cout<<"El numero de cedula debe tener diez digitos, intentelo de nuevo: ";
            cin>>cedula;
        }
        cout<<"Ingrese el numero de la tarjeta: ";
        cin>>numero_tarjeta;
        while(numero_tarjeta<=1000000000000000||numero_tarjeta>=10000000000000000){
            cout<<"El numero de tarjeta debe tener 16 digitos, intentelo de nuevo: ";
            cin>>numero_tarjeta;
        }
        cout<<"Ingrese el CVC de la tarjeta: ";
        cin>>cvc;
        while (cvc>=1000 || cvc<100){
            cout<<"El CVC debe tener al menos tres digitos, intentelo de nuevo: ";
            cin>>cvc;
        }
        archivo << nombres << ";" << apellidos << ";" << cedula << ";" 
                << numero_tarjeta << ";" << cvc << ";" << limite_consumo;
        
        string tarjeta_adicional = "";
        string cvc_adicional = "";
        string limite_adicional = "";
        
        // Solo añade los campos adicionales si tienen datos
        if (!tarjeta_adicional.empty()) {
            archivo << ";" << tarjeta_adicional << ";" << cvc_adicional << ";" << limite_adicional;
        }
        
        archivo << ";" << "0"; // consumo_temp_principal
        
        if (!tarjeta_adicional.empty()) {
            archivo << ";" << "0"; // consumo_temp_adicional
        }
        
        archivo << ";" << "0"; // deuda_total_principal
        
        if (!tarjeta_adicional.empty()) {
            archivo << ";" << "0"; // deuda_total_adicional
        }
        
        archivo << endl;
        archivo.close();
        cout<<"Cliente ingresado con exito"<<endl;
        cout<<"Regresando al menu principal";
        for(int i=0;i<3;i++){
            sleep(1);
            cout<<".";
        }
        system("cls");
    }else{
        cout << "Error al abrir el archivo" << endl;
        exit(1);
    }
}

//Crear archivo
void crear_archivo(){
    ofstream archivo("CRUD.csv",ios::app);
    if(archivo.is_open()){
        cout << "\nArchivo abierto correctamente" << endl;
        archivo.close();
    }else{
        cout << "Error al crear el archivo" << endl;
        exit(1);
  }
}

//Cálculo de consumos
float calcular_consumos(float descuento, float limite_descuento, float limite_credito){
    float consumo;
    float meses;
    cout<<"Ingrese el monto del consumo: $";
    cin>>consumo;
    while(consumo<=0){
        cout<<"El monto del consumo debe ser mayor a 0, intentelo de nuevo: ";
        cin>>consumo;
    }
    if(consumo >= limite_descuento && consumo <= 2000){
        consumo = consumo-consumo*(descuento);
        cout<<"A cuantos meses desea diferir? (1-3): ";
        cin>>meses;
        while(meses<1 || meses>3){
            cout<<"El numero de meses debe ser entre 1 y 3, intentelo de nuevo: ";
            cin>>meses;
        }
        consumo = consumo/meses;
        cout<<"El monto a pagar por mes aplicando descuento del "<<descuento*100<<"% es de: $"<<consumo<<" (diferido a "<<meses<< " mes/es)"<<endl;
    }else{
        if (consumo > limite_credito){
            cout<<"El monto excede el limite de credito ("<<limite_credito<<"), intentelo de nuevo"<<endl;
        }else{
            cout<<endl;
            cout<<"No se aplica descuento"<<endl;
            cout<<"El monto minimo para aplicar descuento es de: $"<<limite_descuento<<endl;
            cout<<"El monto a pagar es de: $"<<consumo<<endl;
            cout<<endl;
        }
    }
    return consumo;
}



//Tarjeta a usar
void usar_tarjeta() {
    system("cls");
    cout << "\n**************" << endl;
    cout << "USO DE TARJETAS" << endl;
    cout << "**************" << endl;
    int cvc, resp;
    float consumo, consumo_temp = 0;
    long long int cedula;
    bool encontrado = false, cvc_correcto = false;
    cout<<"Ingrese la cedula del tutor de la(s) tarjetas: ";
    cin>>cedula;
    while(cedula<=100000000||cedula>=2500000000){
        cout<<"El numero de cedula debe tener diez digitos, intentelo de nuevo: ";
        cin>>cedula;
        cout<<endl;
    }
    ifstream archivo("CRUD.csv"); // abrir archivo
    ofstream archivo_temp("temp.csv"); // abrir archivo temporal
    if (archivo.is_open() && archivo_temp.is_open()) { // si se abren los archivos
        string linea;
        while (getline(archivo, linea)) { // leer linea por linea
            size_t start = 0, end = 0; // inicio y fin de la linea
            string partes[13]; // partes de la linea
            int i = 0; // contador
            while ((end = linea.find(';', start)) != string::npos && i < 13) { // separar la linea en partes
                partes[i++] = linea.substr(start, end - start); // guardar las partes en los espacios del arreglo
                start = end + 1; // actualizar el inicio
            }
            if (i < 13) { // si no se han guardado todas las partes
                partes[i] = linea.substr(start); // guardar la parte restante
                while (++i < 13) { // completar las partes faltantes
                    partes[i] = "0"; // con un string vacio
                }
            }

            if(partes[2]==to_string(cedula)){
                encontrado=true;
                cout<<endl;
                cout << "Cliente encontrado: " << partes[0] << " " << partes[1] << endl;
                cout << "Tarjeta principal: " << partes[3] << endl;
                if (partes[6] != "0") {
                    cout << "Tarjeta adicional: " << partes[6] << endl;
                }
                cout << endl;
                while(!cvc_correcto){
                    cout << "Ingrese el CVC de la tarjeta a usar: ";
                    cin >> cvc;
                    while (cvc >= 1000 || cvc < 100) {
                        cout << "El CVC debe tener al menos tres digitos, intentelo de nuevo: ";
                        cin >> cvc;
                    }
                    if (partes[4] == to_string(cvc) || partes[7] == to_string(cvc)) { // si el cvc es igual al ingresado
                        cvc_correcto = true;
                        if(partes[4]==to_string(cvc)){
                            cout<<endl;
                            cout<<"Este CVC corresponde a la tarjeta principal de "<<partes[0]<<" "<<partes[1]<<endl;
                            cout << "Numero de tarjeta: " << partes[3] << " (Limite de credito: $" << stoi(partes[5]) << ")" << endl;
                            cout<<""<<endl;
                            cout<<"Deuda total: $"<<partes[11]<<endl;
                            cout << "Fecha de corte: 15 de cada mes" << endl;
                            cout << "Fecha de pago: 30 de cada mes" << endl;
                            cout<<""<<endl;
                            cout << "Beneficios: " << endl;
                            cout << "- Por consumos desde $50, 10% de descuento en Osaka, Casa Res, Bocatto da Fiorentino, Friday's" << endl;
                            cout << "- Por consumos desde $500, 15% de descuento en tu proximo vuelo con LATAM Airlines" << endl;
                            cout << "- Por consumos desde $40, 20% de descuento en consultas medicas en METRORED" << endl;
                            int n1;
                            do {
                            cout << "\nEscoja el tipo de consumo: " << endl;
                            cout << "1. Consumo en restaurantes" << endl;
                            cout << "2. Consumo en vuelos" << endl;
                            cout << "3. Consumo en consultas medicas" << endl;
                            cout << "4. Salir al menu principal" << endl;
                            cout << "Ingrese la opcion deseada: "; 
                            cin >> n1;
                            switch (n1) {
                                case 1:
                                    consumo = calcular_consumos(0.1, 50, stoi(partes[5])); // calcular el consumo
                                    consumo_temp += consumo; // sumar el consumo al total
                                    break;
                                case 2:
                                    consumo = calcular_consumos(0.15, 500, stoi(partes[5]));
                                    consumo_temp += consumo;
                                    break;
                                case 3:
                                    consumo = calcular_consumos(0.2, 40, stoi(partes[5]));
                                    consumo_temp += consumo;
                                    break;
                                case 4:
                                    cout << "Regresando al menu principal";
                                    for (int i = 0; i < 4; i++) {
                                        sleep(1);
                                        cout << ".";
                                    }
                                    system("cls");
                                    break;
                                default:
                                    cout << "Opcion no valida" << endl;
                                    break;
                            }
                            if(n1!=4){
                                cout << "Desea registrar otro consumo? (1. Si / 2. No): ";
                                cin >> resp;
                                while (resp != 1 && resp != 2) {
                                    cout << "Opcion no valida, intentelo de nuevo: ";
                                    cin >> resp;
                                }

                            }
                        } while (n1 != 4 && resp == 1); // mientras no se quiera salir y se quiera registrar otro consumo
                            if (consumo_temp > 0) { // si hay un consumo
                                float consumo_total = stoi(partes[11]) + consumo_temp; // sumar el consumo al total
                                float limite_actual = stoi(partes[5]) - consumo_temp; // restar el consumo al limite

                                partes[11] = to_string(consumo_total); // actualizar la deuda
                                partes[9] = to_string(consumo_temp); // reiniciar consumo temporal
                                partes[5] = to_string(limite_actual);  // actualizar el limite
                            }
                        }else if(partes[7]==to_string(cvc)){
                                cout<<"Este CVC corresponde a la tarjeta adicional de "<<partes[0]<<" "<<partes[1]<<endl;
                                cout << "Numero de tarjeta: " <<partes[6] << " (Limite de credito: $" << stoi(partes[8]) << ")" << endl;
                                cout<<""<<endl;
                                cout<<"Deuda total: $"<<partes[12]<<endl;
                                cout << "Fecha de corte: 10 de cada mes" << endl;
                                cout << "Fecha de pago: 25 de cada mes" << endl;
                                cout<<""<<endl;
                                cout << "Beneficios: " << endl;
                                cout << "- Por consumos desde $50, 10% de descuento en Chez Jerome, Fogo de Chao, Carmine" << endl;
                                cout << "- Por consumos desde $500, 15% de descuento en tu proximo vuelo con AVIANCA" << endl;
                                cout << "- Por consumos desde $40, 20% de descuento en consultas medicas en SIME USFQ" << endl;
                                int n2;
                                do {
                                    cout << "\nEscoja el tipo de consumo: " << endl;
                                    cout << "1. Consumo en restaurantes" << endl;
                                    cout << "2. Consumo en vuelos" << endl;
                                    cout << "3. Consumo en consultas medicas" << endl;
                                    cout << "4. Salir al menu principal" << endl;
                                    cout<< "Ingrese la opcion deseada: ";
                                    cin >> n2;
                                    switch (n2) {
                                        case 1:
                                            consumo = calcular_consumos(0.1, 50, stoi(partes[8]));
                                            consumo_temp += consumo;
                                            break;
                                        case 2:
                                            consumo = calcular_consumos(0.15, 500, stoi(partes[8]));
                                            consumo_temp += consumo;
                                            break;
                                        case 3:
                                            consumo = calcular_consumos(0.2, 40, stoi(partes[8]));
                                            consumo_temp += consumo;
                                            break;
                                        case 4:
                                            cout << "Regresando al menu principal";
                                            for (int i = 0; i < 4; i++) {
                                                sleep(1);
                                                cout << ".";
                                            }
                                            system("cls");
                                            break;
                                        default:
                                            cout << "Opcion no valida" << endl;
                                            break;
                                    }
                                    if(n2!=4){
                                        cout<<endl;
                                        cout << "Desea registrar otro consumo? (1. Si / 2. No): ";
                                        cin >> resp;
                                        while (resp != 1 && resp != 2) {
                                            cout << "Opcion no valida, intentelo de nuevo: ";
                                            cin >> resp;
                                        }

                                    }
                                } while (n2 != 4 && resp == 1);
                                if (consumo_temp > 0) {
                                    float consumo_total = stoi(partes[12]) + consumo_temp;
                                    float limite_actual = stoi(partes[8]) - consumo_temp;

                                    partes[12] = to_string(consumo_total); // actualizar deuda
                                    partes[10] = to_string(consumo_temp); // reiniciar consumo temporal
                                    partes[8] = to_string(limite_actual); // actualizar limite
                                }
                            }
                        }else{
                        cout << "Este CVC no corresponde a ninguna tarjeta registrada para este cliente" << endl;
                        cout << "¿Desea intentar de nuevo? (1: Sí / 0: No): ";
                        int opcion;
                        cin >> opcion;
                        if (opcion == 0) {
                            break;
                        }
                        }
            }
            // Escribir en el archivo temporal
            archivo_temp << partes[0] << ";" << partes[1] << ";" << partes[2] << ";" 
                        << partes[3] << ";" << partes[4] << ";" << partes[5] << ";"
                        << partes[6] << ";" << partes[7] << ";" << partes[8] << ";"
                        << partes[9] << ";" << partes[10] << ";" << partes[11] << ";"
                        << partes[12] << endl; // escribir la linea
        }else{
            archivo_temp << linea << endl; // escribir la linea
        }
        }
        archivo.close();
        archivo_temp.close();

        ifstream archivo_temp_final("temp.csv");
        ofstream archivo_final("CRUD.csv", ios::trunc);  // Abre el archivo original en modo truncado

        if (archivo_temp_final.is_open() && archivo_final.is_open()) {
            archivo_final << archivo_temp_final.rdbuf();  // Copia todo el contenido
            archivo_temp_final.close();
            archivo_final.close();
            remove("temp.csv");  // Elimina el archivo temporal
        } else {
            cout << "Error al abrir los archivos para la actualización final" << endl;
        }
        
        
    }else{
        cout << "Error al abrir el archivo" << endl;
        exit(1);
    }
    if(!encontrado){
        cout<<"Cliente no encontrado, regresando al menu principal";
        for(int i=0;i<3;i++){
            sleep(1);
            cout<<".";
        }
        system("cls");
    }
    if (!cvc_correcto) { // si no se encuentra la tarjeta
        cout << "Tarjeta no encontrada, regresando";
        for (int i = 0; i < 4; i++) {
            sleep(1);
            cout << ".";
        }
        system("cls");
    }
}


//Consultar cliente
void consultar_cliente(){
    cout<<"******************"<<endl;
    cout<<"CONSULTAR CLIENTE"<<endl;
    cout<<"******************"<<endl;
    long long int cedula, tarjeta_adicional = 0;
    int cvc_adicional, n;
    bool tarjeta_adicional_ingresada = false;
    cout<<"Ingrese la cedula del cliente a consultar: ";
    cin>>cedula;
    while(cedula<=100000000||cedula>=2500000000){
        cout<<"El numero de cedula debe tener diez digitos, ingrese nuevamente: ";
        cin>>cedula;
        cout<<endl;
    }
    ifstream archivo("CRUD.csv");
    ofstream archivo_temp("temp.csv"); // abrir archivo temporal
    if(archivo.is_open() && archivo_temp.is_open()){
        string linea;
        bool encontrado = false;
        while(getline(archivo, linea)){
            size_t start = 0, end = 0;
            string partes[13];
            int i = 0;
            while ((end = linea.find(';', start)) != string::npos && i<13) {
                partes[i++] = linea.substr(start, end - start);
                start = end + 1;
            }
            if(i<13){
                partes[i] = linea.substr(start);
                while(++i<13){
                    partes[i] = "0";
                }
            }
            if(partes[2] == to_string(cedula)){
                encontrado = true;
                cout<<"Cliente encontrado: ";
                cout<<partes[0]<<" "<<partes[1]<<" / "<<partes[2]<<endl;
                do{
                    cout<<endl;
                    cout<<"*************************"<<endl;
                    cout<<"Visualizacion de tarjetas"<<endl;
                    cout<<"*************************"<<endl;
                    cout<<"1. Ver tarjetas registradas"<<endl;
                    cout<<"2. Ingresar tarjeta adicional"<<endl;
                    cout<<"3. Ingresar pagos"<<endl;
                    cout<<"4. Salir al menu principal"<<endl;
                    cout<<"Ingrese la opcion deseada: ";
                    cin>>n;
                    switch(n){
                        case 1:
                            cout<<endl;
                            cout<<"Tarjetas registradas de "<<partes[0]<<" "<<partes[1]<<":"<<endl;
                            cout<<"Numero de tarjeta principal: "<<partes[3]<<endl;
                            cout<<"Deuda de tarjeta principal: $"<<partes[11]<<endl;
                            if(partes[6] != "0"){
                                cout<<"Numero de tarjeta adicional: "<<partes[6]<<endl;
                                cout<<"Deuda de tarjeta adicional: $"<<partes[12]<<endl;
                            }
                            cout<<endl;
                            cout<<"Ingrese cualquier tecla para continuar"<<endl;
                            cin.ignore();
                            cin.get();
                            system("cls");
                            break;
                        case 2:
                            tarjeta_adicional_ingresada = true;
                            if(partes[6]!="0"){
                                cout<<"Ya existe una tarjeta adicional registrada, desea reemplazarla? (1: Si / 0: No): ";
                                int opcion;
                                cin>>opcion;
                                if(opcion==0){
                                    cout<<endl;
                                    cout<<"Ingrese cualquier tecla para continuar"<<endl;
                                    cin.ignore();
                                    cin.get();
                                    system("cls");
                                    break;
                                }
                            }
                            cout<<"Ingrese el numero de la tarjeta adicional: ";
                            cin>>tarjeta_adicional;
                            while(tarjeta_adicional<=1000000000000000||tarjeta_adicional>=10000000000000000){
                                cout<<"El numero de tarjeta debe tener 16 digitos, intentelo de nuevo: ";
                                cin>>tarjeta_adicional;
                            }
                            cout<<"Ingrese el CVC de la tarjeta adicional: ";
                            cin>>cvc_adicional;    
                            while(cvc_adicional>=1000 || cvc_adicional<100){
                                cout<<"El CVC debe tener al menos tres digitos, intentelo de nuevo: ";
                                cin>>cvc_adicional;
                            }
                            partes[6] = to_string(tarjeta_adicional);
                            partes[7] = to_string(cvc_adicional);
                            partes[8] = "2000";
                            cout<<"Tarjeta adicional ingresada con exito"<<endl;
                            cout<<endl;
                            cout<<"Ingrese cualquier tecla para continuar"<<endl;
                            cin.ignore();
                            cin.get();
                            system("cls");
                            break;
                        case 3:
                            usar_tarjeta();
                            cout<<endl;
                            cout<<"Ingrese cualquier tecla para continuar"<<endl;
                            cin.ignore();
                            cin.get();
                            system("cls");
                            break;
                        case 4:
                            cout<<"Regresando al menu principal";
                            for(int i=0;i<3;i++){
                                sleep(1);
                                cout<<".";
                            }
                            system("cls");
                            break;
                        default:
                            cout<<"Opcion no valida"<<endl;
                            break;
                    }
                } while (n!=4);
                archivo_temp<<partes[0]<<";"<<partes[1]<<";"<<partes[2]<<";"<<partes[3]<<";"<<partes[4]<<";"<<partes[5]<<";"<<partes[6]<<";"<<partes[7]<<";"<<partes[8]<<";"<<partes[9]<<";"<<partes[10]<<";"<<partes[11]<<";"<<partes[12]<<endl;
            } else {
                archivo_temp<<linea<<endl;
            }
        }
        if(!encontrado){ // Si no encuentra la cedula, imprime que no se encontró
            cout<<"Cliente no encontrado, regresando al menu principal";
            for(int i=0;i<3;i++){
                sleep(1);
                cout<<".";
            }
            system("cls");
        }
        archivo.close(); // cerrar archivo
        archivo_temp.close(); // cerrar archivo temporal
        if(tarjeta_adicional_ingresada){
            ifstream archivo_temp_final("temp.csv");
            ofstream archivo_final("CRUD.csv", ios::trunc);  // Abre el archivo original en modo truncado

            if (archivo_temp_final.is_open() && archivo_final.is_open()) {
                archivo_final << archivo_temp_final.rdbuf();  // Copia todo el contenido
                archivo_temp_final.close();
                archivo_final.close();
                remove("temp.csv");  // Elimina el archivo temporal
            } else {
                cout << "Error al abrir los archivos para la actualización final" << endl;
            }
        }
    }else{
        cout << "Error al abrir el archivo" << endl;
        exit(1);
    }
}

//Pagar deudas
void pagar_deudas(){
    cout<<"\n***********"<<endl;
    cout<<"PAGAR DEUDA"<<endl;
    cout<<"***********"<<endl;
    int cvc, opcion;
    float pago_minimo, monto, adeudado, pagar;
    long long int cedula;
    bool encontrado = false, cvc_correcto=false;
    cout<<"Ingrese la cedula del cliente que va a pagar: ";
    cin>>cedula;
    while(cedula<=100000000||cedula>=2500000000){
        cout<<"El numero de cedula debe tener diez digitos, ingrese nuevamente: ";
        cin>>cedula;
        cout<<endl;
    }
    ifstream archivo("CRUD.csv");
    ofstream archivo_temp("temp.csv"); // abrir archivo temporal
    if(archivo.is_open() && archivo_temp.is_open()){
        string linea;
        while(getline(archivo, linea)){
            size_t start = 0, end = 0;
            string partes[13];
            int i = 0;
            while ((end = linea.find(';', start)) != string::npos && i<13) {
                partes[i++] = linea.substr(start, end - start);
                start = end + 1;
            }
            if(i<13){
                partes[i] = linea.substr(start);
                while(++i<13){
                    partes[i] = "";
                }
            }
            if(partes[2] == to_string(cedula)){
                encontrado = true;
                cout<<"Cedula encontrada "<<endl;
                cout<<endl;
                cout<<"Tarjeta principal: "<<partes[3]<<endl;
                if(partes[6] != "0"){
                    cout<<"Tarjeta adicional: "<<partes[6]<<endl;
                }
                cout<<"Tutor de la tarjeta: "<<partes[0]<<" "<<partes[1]<<endl;
                cout<<endl;
                while(!cvc_correcto){
                    cout<<"Ingrese el CVC de la tarjeta a pagar: ";
                    cin>>cvc;
                    while(cvc>=1000 || cvc<100){
                        cout<<"El CVC debe tener al menos tres digitos, intentelo de nuevo: ";
                        cin>>cvc;
                    }
                
                if(partes[4]==to_string(cvc) || partes[7]== to_string(cvc)){
                    cvc_correcto=true;
                    if(partes[4] == to_string(cvc)){
                    cout<<endl;
                    cout<<"Este CVC corresponde a la tarjeta principal"<<endl;
                    do{
                    cout<<"\n*****************************************"<<endl;
                    cout<<"La deuda total es de: $"<<partes[11]<<endl;
                    cout<<"*****************************************"<<endl;
                    cout<<"Como desea pagar la deuda?"<<endl;
                    cout<<"1. Pagar la totalidad"<<endl;
                    cout<<"2. Pagar el minimo"<<endl;
                    cout<<"3. Salir al menu principal"<<endl;
                    cout<<"Ingrese la opcion deseada: ";
                    cin>>opcion;
                        switch(opcion){
                        case 1:
                            if(partes[11]=="0"){
                                cout<<"No hay deuda pendiente"<<endl;
                                break;
                            }
                            cout<<"Pagando la totalidad de la deuda mensual";
                            for(int i=0;i<3;i++){
                                sleep(1);
                                cout<<".";
                            }
                            partes[11] = "0";
                            partes[5] = "2000";
                            cout<<endl;
                            cout<<"Pago realizado con exito"<<endl;
                            sleep(1);
                            cout<<"Restaurando limite de credito";
                            for(int i=0;i<3;i++){
                                sleep(1);
                                cout<<".";
                            }
                            cout<<endl;
                            cout<<"Ingrese cualquier tecla para continuar"<<endl;
                            cin.ignore();
                            cin.get();
                            system("cls");
                            break;
                        case 2:
                            if(partes[11]=="0"){
                                cout<<"No hay deuda pendiente"<<endl;
                                break;
                            }
                            pago_minimo=stoi(partes[11])*0.05;
                            cout<<"El pago minimo de la deuda mensual es: $"<<pago_minimo<<endl;
                            cout<<"Ingrese el monto a pagar (Si el monto es menor al pago minimo entra a buro de credito): ";
                            cin>>monto;
                            cout<<endl;
                            if(monto<pago_minimo){
                                cout<<"El monto ingresado es menor al pago minimo, entra a buro de credito"<<endl;
                                adeudado=stoi(partes[11])-monto;
                                pagar=adeudado+adeudado*0.025;
                                cout<<"Pagando monto ingresado";
                                for(int i=0;i<3;i++){
                                    sleep(1);
                                    cout<<".";
                                }
                                cout<<endl;
                                cout<<"Pago realizado con exito"<<endl;
                                cout<<"Calculando nueva deuda con intereses";
                                for(int i=0;i<3;i++){
                                    sleep(1);
                                    cout<<".";
                                }
                                cout<<""<<endl;
                                cout<<"La nueva deuda es de: $"<<pagar<<endl;
                                cout<<"Recuerde que ha entrado a buro de credito, su historial crediticio se vera afectado"<<endl;
                                partes[11]=to_string(pagar);
                                partes[5]="2000";
                                cout<<endl;
                                cout<<"Ingrese cualquier tecla para continuar"<<endl;
                                cin.ignore();
                                cin.get();
                                system("cls");
                            }else{
                                cout<<"El monto ingresado es mayor o igual al pago minimo, NO entra a buro de credito"<<endl;
                                adeudado=stoi(partes[11])-monto;
                                pagar=adeudado+adeudado*0.0232;
                                cout<<endl;
                                cout<<"Pagando monto ingresado";
                                for(int i=0;i<3;i++){
                                    sleep(1);
                                    cout<<".";
                                }
                                cout<<endl;
                                cout<<"Pago realizado con exito"<<endl;
                                cout<<"Calculando nueva deuda con intereses";
                                for(int i=0;i<3;i++){
                                    sleep(1);
                                    cout<<".";
                                }
                                cout<<""<<endl;
                                cout<<"La nueva deuda es de: $"<<pagar<<endl;
                                cout<<"Recuerde pagar al banco puntualmente"<<endl;
                                partes[11]=to_string(pagar);
                                partes[5]="2000";
                                cout<<endl;
                                cout<<"Ingrese cualquier tecla para continuar"<<endl;
                                cin.ignore();
                                cin.get();
                                system("cls");
                            }
                            break;
                        case 3:
                            cout<<"Regresando al menu principal";
                            for(int i=0;i<3;i++){
                                sleep(1);
                                cout<<".";
                            }
                            system("cls");
                            break;
                        default:
                            cout<<"Opcion no valida"<<endl;
                            break;
                    }
                    }while(opcion!=3);
                    
                    archivo_temp<<partes[0]<<";"<<partes[1]<<";"<<partes[2]<<";"<<partes[3]<<";"<<partes[4]<<";"<<partes[5]<<";"<<partes[6]<<";"<<partes[7]<<";"<<partes[8]<<";"<<partes[9]<<";"<<partes[10]<<";"<<partes[11]<<";"<<partes[12]<<endl;
                    system("cls");
                }else{
                    if(partes[7]==to_string(cvc)){
                        cout<<"Este CVC corresponde a la tarjeta adicional"<<endl;
                        do{
                    cout<<"\n*****************************************"<<endl;
                    cout<<"La deuda total es de: $"<<partes[12]<<endl;//REVISAR ESTO
                    cout<<"*****************************************"<<endl;
                    cout<<"Como desea pagar la deuda?"<<endl;
                    cout<<"1. Pagar la totalidad"<<endl;
                    cout<<"2. Pagar el minimo"<<endl;
                    cout<<"3. Salir al menu principal"<<endl;
                    cout<<"Ingrese la opcion deseada: ";
                    cin>>opcion;
                        switch(opcion){
                        case 1:
                            if(partes[12]=="0"){
                                cout<<"No hay deuda pendiente"<<endl;
                                break;
                            }
                            cout<<"Pagando la totalidad de la deuda mensual";
                            for(int i=0;i<3;i++){
                                sleep(1);
                                cout<<".";
                            }
                            partes[12] = "0";
                            partes[8] = "2000";
                            cout<<endl;
                            cout<<"Pago realizado con exito"<<endl;
                            sleep(1);
                            cout<<"Restaurando limite de credito";
                            for(int i=0;i<3;i++){
                                sleep(1);
                                cout<<".";
                            }
                            break;
                        case 2:
                            if(partes[12]=="0"){
                                cout<<"No hay deuda pendiente"<<endl;
                                break;
                            }
                            pago_minimo=stoi(partes[12])*0.05;
                            cout<<"El pago minimo de la deuda mensual es: $"<<pago_minimo<<endl;
                            cout<<"Ingrese el monto a pagar (Si el monto es menor al pago minimo entra a buro de credito): ";
                            cin>>monto;
                            cout<<endl;
                            if(monto<pago_minimo){
                                cout<<"El monto ingresado es menor al pago minimo, entra a buro de credito"<<endl;
                                adeudado=stoi(partes[12])-monto;
                                pagar=adeudado+adeudado*0.025;
                                cout<<endl;
                                cout<<"Pagando monto ingresado";
                                for(int i=0;i<3;i++){
                                    sleep(1);
                                    cout<<".";
                                }
                                cout<<"Pago realizado con exito"<<endl;
                                cout<<"Calculando nueva deuda con intereses";
                                for(int i=0;i<3;i++){
                                    sleep(1);
                                    cout<<".";
                                }
                                cout<<""<<endl;
                                cout<<"La nueva deuda es de: $"<<pagar<<endl;
                                cout<<"Recuerde que ha entrado a buro de credito, su historial crediticio se vera afectado"<<endl;
                                partes[12]=to_string(pagar);
                                partes[8]="2000";
                            }else{
                                cout<<"El monto ingresado es mayor o igual al pago minimo, NO entra a buro de credito";
                                adeudado=stoi(partes[12])-monto;
                                pagar=adeudado+adeudado*0.0232;
                                cout<<endl;
                                cout<<"Pagando monto ingresado";
                                for(int i=0;i<3;i++){
                                    sleep(1);
                                    cout<<".";
                                }
                                cout<<endl;
                                cout<<"Pago realizado con exito"<<endl;
                                cout<<"Calculando nueva deuda con intereses";
                                for(int i=0;i<3;i++){
                                    sleep(1);
                                    cout<<".";
                                }
                                cout<<""<<endl;
                                cout<<"La nueva deuda es de: $"<<pagar<<endl;
                                cout<<"Recuerde pagar al banco puntualmente"<<endl;
                                partes[12]=to_string(pagar);
                                partes[8]="2000";
                            }
                            break;
                        case 3:
                            cout<<"Cambios guardados"<<endl;
                            cout<<"Regresando al menu principal";
                            for(int i=0;i<3;i++){
                                sleep(1);
                                cout<<".";
                            }
                            system("cls");
                            break;
                        default:
                            cout<<"Opcion no valida"<<endl;
                            break;
                        }
                    }while(opcion!=3);
                    archivo_temp<<partes[0]<<";"<<partes[1]<<";"<<partes[2]<<";"<<partes[3]<<";"<<partes[4]<<";"<<partes[5]<<";"<<partes[6]<<";"<<partes[7]<<";"<<partes[8]<<";"<<partes[9]<<";"<<partes[10]<<";"<<partes[11]<<";"<<partes[12]<<endl; 
                    system("cls");
                    }else{
                        cout<<"Este CVC no corresponde a ninguna tarjeta registrada"<<endl;
                        cout<<"Regresando al menu principal";
                        for(int i=0;i<3;i++){
                            sleep(1);
                            cout<<".";
                        }
                        system("cls");
                    }
                }
                }else{
                    cout<<"Este CVC no corresponde a ninguna tarjeta registrada para este cliente"<<endl;
                    cout<<"¿Desea intentar de nuevo? (1: Sí / 0: No): ";
                    int opcion;
                    cin>>opcion;
                    if(opcion == 0){
                        break;
                    }
                }
                }
            }else{
                archivo_temp<<linea<<endl;
            }
        }
        archivo.close(); 
        archivo_temp.close(); 
        ifstream archivo_temp_final("temp.csv");
        ofstream archivo_final("CRUD.csv", ios::trunc);  // Abre el archivo original en modo truncado

        if (archivo_temp_final.is_open() && archivo_final.is_open()) {
            archivo_final << archivo_temp_final.rdbuf();  // Copia todo el contenido
            archivo_temp_final.close();
            archivo_final.close();
            remove("temp.csv");  // Elimina el archivo temporal
        } else {
            cout << "Error al abrir los archivos para la actualización final" << endl;
        }
    }else{
        cout << "Error al abrir el archivo" << endl;
        exit(1);
    }
    if(!encontrado){
        cout<<"Cliente no encontrado, regresando al menu principal";
        for(int i=0;i<3;i++){
            sleep(1);
            cout<<".";
        }
        system("cls");
    }
}

//Actualizar Datos
void actualizar_datos(string campo, int campo_num){
    bool encontrado = false;
    string linea, reemplazar;
    long long int cedula;
    cout << "Ingrese la cedula del cliente para actualizacion de datos: ";
    cin >> cedula;
    while (cedula <= 100000000 || cedula >= 2500000000){
        cout << "El numero de cedula debe tener diez digitos, ingrese nuevamente: ";
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
                cout << "Ingrese " << campo <<":";
                getline(cin, reemplazar);
                encontrado = true;
                string partes[13];
                size_t start = 0, end = 0;
                for (int i = 0; i < 13; i++) {
                    end = linea.find(';', start);
                    if (end == string::npos) {
                        partes[i] = linea.substr(start);
                        while (++i < 13) {
                            partes[i] = "0";
                        }
                        break;
                    }
                    partes[i] = linea.substr(start, end - start);
                    start = end + 1;
                }
                partes[campo_num] = reemplazar;
                linea = partes[0] + ";" + partes[1] + ";" + partes[2] + ";" + partes[3] + ";" + partes[4] + ";" + partes[5] + ";" + partes[6] + ";" + partes[7] + ";" + partes[8] + ";" + partes[9] + ";" + partes[10] + ";" + partes[11] + ";" + partes[12];
            }
            archivo_temp << linea << endl;
        }
        if (encontrado){
            cout << "El dato ha sido actualizado con exito";
        } else {
            cout << "No se encontro el cliente, intentelo de nuevo";
        }
        
        for (int i = 0; i < 4; i++){
            sleep(1);
            cout << ".";
        }
    } else {
        cout << "Error al abrir el archivo" << endl;
        exit(1);
    }
    archivo.close();
    archivo_temp.close();
    ifstream archivo_temp_final("temp.csv");
    ofstream archivo_final("CRUD.csv", ios::trunc);
    if (archivo_temp_final.is_open() && archivo_final.is_open()) {
        archivo_final << archivo_temp_final.rdbuf();
        archivo_temp_final.close();
        archivo_final.close();
        remove("temp.csv");
    } else {
        cout << "Error al abrir los archivos para la actualización final" << endl;
    }
    system("cls");
}

// Switch Actualizar Datos
void switch_actualizar_datos(){
    int n;
    do{
    cout << "**************************" << endl;
    cout << "INGRESO A ACTUALIZAR DATOS" << endl;
    cout << "**************************" << endl;
    cout << "Seleccione el campo que desea actualizar: " << endl;
    n = imprimir_opciones("Actualizar nombres", "Actualizar apellidos", "Actualizar cedula", "Actualizar numero de tarjeta", "Actualizar CVC","6. Salir al menu principal");
        switch (n){
        case 1:
            actualizar_datos("los nuevos nombres", 0);
            system("cls");
            break;
        case 2:
            actualizar_datos("los nuevos apellidos", 1);
            system("cls");
            break;
        case 3:
            actualizar_datos("la nueva cedula", 2);
            system("cls");
            break;
        case 4:
            actualizar_datos("el nuevo numero de tarjeta", 3);
            system("cls");
            break;
        case 5:
            actualizar_datos("el nuevo CVC (codigo de seguridad)", 4);
            system("cls");
            break;
        case 6:
            cout << "Regresando al menu principal";
            for(int i=0;i<3;i++){
                sleep(1);
                cout<<".";
            }
            system("cls");
            break;
        default:
            cout << "Opcion no valida" << endl;
            break;
    }
    }while(n!=6);
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
        cout<<"El numero de cedula debe tener diez digitos, ingrese nuevamente: ";
        cin>>cedula;
        cout<<endl;
    }
    ifstream archivo("CRUD.csv");
    ofstream archivo_temp("temp.csv");

    if(archivo.is_open() && archivo_temp.is_open()){
        string linea;
        bool encontrado = false;
        while(getline(archivo, linea)){
            if(linea.find(to_string(cedula))==string::npos){
                archivo_temp<<linea<<endl;
            } else {
                encontrado = true;
            }
        }
        archivo.close();
        archivo_temp.close();
        
        if(encontrado) {
            remove("CRUD.csv");
            rename("temp.csv","CRUD.csv");
            cout<<"Datos del cliente eliminados con exito "<<endl;
        } else {
            remove("temp.csv");
            cout<<"No se encontro al cliente con la cedula proporcionada"<<endl;
        }
        
        cout << "Regresando al menu principal";
        for(int i=0;i<3;i++){
            sleep(1);
            cout<<".";
        }
        system("cls");
    }else{
        cout<<"Error al abrir los archivos"<<endl;
        exit(1);
    }
}

//Menu Principal
void menu(){
    int n;
    crear_archivo();
    do
    {
        titulo();
        n = imprimir_opciones("Ingresar cliente nuevo", "Consultar cliente", "Pagar deudas", "Actualizar datos", "Eliminar cliente", "6. Salir del programa");
        switch (n)
        {
        case 1: //ingreso de clientes
            system("cls");
            ingreso_cliente();
            break;
        case 2:
            system("cls");
            consultar_cliente();
            break;
        case 3:
            system("cls");
            pagar_deudas();
            break;
        case 4:
            system("cls");
            switch_actualizar_datos();
            break;
        case 5:
            system("cls");
            eliminar_cliente();
            break;
        case 6:
            system("cls");
            cout<<"Gracias por usar el programa"<<endl;
            exit(1);
            break;
        default:
            cout<<"Opcion no valida"<<endl;
            sleep(1.2);
            system("cls");
            break;
        }
    } while (n!=6);
}

//Main
int main(){
    menu();
    return 0;
}