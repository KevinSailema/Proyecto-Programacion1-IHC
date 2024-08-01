#include <iostream>
<<<<<<< HEAD
#include <fstream>
#include <vector>
#include <string>

// Clase Cliente
class Cliente {
public:
    std::string id;
    std::string nombre;

    Cliente(std::string id, std::string nombre) : id(id), nombre(nombre) {}
};

// Clase TarjetaCredito
class TarjetaCredito {
public:
    std::string numeroTarjeta;
    std::string idCliente;
    double cupoMaximo;
    std::vector<std::string> movimientos;

    TarjetaCredito(std::string numeroTarjeta, std::string idCliente, double cupoMaximo)
        : numeroTarjeta(numeroTarjeta), idCliente(idCliente), cupoMaximo(cupoMaximo) {}

    void agregarMovimiento(std::string movimiento) {
        movimientos.push_back(movimiento);
    }

    double calcularTotal() {
        // Calcula el total basado en los movimientos registrados
        double total = 0;
        for (auto& movimiento : movimientos) {
            total += std::stod(movimiento);
        }
        return total;
    }
};

// Clase Beneficio
class Beneficio {
public:
    std::string idBeneficio;
    std::string descripcion;

    Beneficio(std::string idBeneficio, std::string descripcion) : idBeneficio(idBeneficio), descripcion(descripcion) {}
};

// Clase GestorTarjetas
class GestorTarjetas {
    std::vector<Cliente> clientes;
    std::vector<TarjetaCredito> tarjetas;
    std::vector<Beneficio> beneficios;

public:
    void agregarCliente(std::string id, std::string nombre) {
        clientes.emplace_back(id, nombre);
    }

    void agregarTarjeta(std::string numero, std::string idCliente, double cupo) {
        tarjetas.emplace_back(numero, idCliente, cupo);
    }

    void agregarMovimientoATarjeta(std::string numeroTarjeta, std::string movimiento) {
        for (auto& tarjeta : tarjetas) {
            if (tarjeta.numeroTarjeta == numeroTarjeta) {
                tarjeta.agregarMovimiento(movimiento);
                break;
            }
        }
    }

    void agregarBeneficio(std::string idBeneficio, std::string descripcion) {
        beneficios.emplace_back(idBeneficio, descripcion);
    }

    void guardarDatosCSV(const std::string& filename) {
        std::ofstream file(filename);
        for (auto& tarjeta : tarjetas) {
            file << "Tarjeta: " << tarjeta.numeroTarjeta << ", Cliente: " << tarjeta.idCliente << ", Total: " << tarjeta.calcularTotal() << "\n";
        }
        file.close();
    }
};

int main() {
    GestorTarjetas gestor;
    gestor.agregarCliente("1", "Juan Perez");
    gestor.agregarTarjeta("1234567890", "1", 5000);
    gestor.agregarMovimientoATarjeta("1234567890", "100.50");
    gestor.agregarBeneficio("B1", "Descuento en restaurantes");

    gestor.guardarDatosCSV("datos_tarjetas.csv");

    return 0;
}
=======
using namespace std;
>>>>>>> 04ec46d5ad3480b0b0bc9826251838f86d5c36f0
