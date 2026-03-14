#include <iostream>
#include <string>
#include "src/sistema/SistemaElectoral.h"
#include "src/menus/MenuPrincipal.h"

using namespace std;

int main() {
    SistemaElectoral sistema;

    bool ok = sistema.inicializar(
        "data/maestros/regiones.txt",
        "data/maestros/ciudades.txt",
        "data/maestros/partidos.txt",
        "data/maestros/candidatos_alcaldia.txt",
        "data/maestros/candidatos_presidencia.txt"
    );

    if (!ok) {
        cout << "Error cargando datos iniciales.\n";
        return 1;
    }

    MenuPrincipal menu(sistema);
    menu.mostrar();

    return 0;
}