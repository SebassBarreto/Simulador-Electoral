#ifndef MENUMODIFICACION_H
#define MENUMODIFICACION_H

#include <iostream>
#include <string>
#include "../sistema/SistemaElectoral.h"

using namespace std;

class MenuModificacion {
private:
    SistemaElectoral& sys;
public:
    MenuModificacion(SistemaElectoral& s) : sys(s) {}

    void mostrar() {
        cout << "\n-- Menu Modificacion --\n";
        if (sys.estaSimulando()) {
            cout << "No se pueden modificar candidatos durante la simulacion.\n";
            return;
        }
        cout << "1. Guardar maestros y relaciones\n";
        cout << "Opcion: ";
        int op; if (!(cin >> op)) { cin.clear(); cin.ignore(10000, '\n'); return; }
        cin.ignore(10000, '\n');
        if (op == 1) {
            bool ok = sys.guardar(
                "data/maestros/regiones.txt",
                "data/maestros/ciudades.txt",
                "data/maestros/partidos.txt",
                "data/maestros/candidatos_alcaldia.txt",
                "data/maestros/candidatos_presidencia.txt",
                "data/resultados/estadisticas_alcaldia_por_region.txt",
                "data/resultados/estadisticas_alcaldia_detalle_por_region.txt",
                "data/resultados/estadisticas_alcaldia_totales_nacionales.txt"
            );
            cout << (ok ? "Guardado ok\n" : "Error guardando\n");
        }
    }
};

#endif // MENUMODIFICACION_H