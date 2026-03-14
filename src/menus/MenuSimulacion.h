#ifndef MENUSIMULACION_H
#define MENUSIMULACION_H

#include <iostream>
#include <string>
#include "../sistema/SistemaElectoral.h"
#include "../sistema/Simulador.h"
#include "../sistema/Escrutinio.h"

using namespace std;

class MenuSimulacion {
private:
    SistemaElectoral& sys;
public:
    MenuSimulacion(SistemaElectoral& s) : sys(s) {}

    void mostrar() {
        cout << "\n-- Menu Simulacion --\n";
        unsigned int seed; cout << "Seed (entero): "; if (!(cin >> seed)) { cin.clear(); cin.ignore(10000, '\n'); seed = 12345; }
        cin.ignore(10000, '\n');
        Simulador::seed(seed);
        sys.comenzarSimulacion();

        // ejemplo: simular una ciudad por id ingresado
        int idCiudad; cout << "Id ciudad para alcaldia (ej 1 Bogota): "; if (!(cin >> idCiudad)) { cin.clear(); cin.ignore(10000, '\n'); idCiudad = 1; }
        cin.ignore(10000, '\n');

        ResultadoElectoral resCiudad(0); // Placeholder; Simulador sets correct censo internally
        if (Simulador::simularAlcaldiaCiudad(sys, idCiudad, resCiudad)) {
            Escrutinio::escribirResultadosAlcaldiaPorCiudad("data/resultados/resultados_ciudad_alcaldia.txt", "CIUDAD_ID_"+to_string(idCiudad), resCiudad);
            cout << resCiudad.toString() << "\n";
        }

        ResultadoElectoral resPais(0); // Placeholder; Simulador sets correct censo internally
        if (Simulador::simularPresidenciaPais(sys, resPais)) {
            Escrutinio::escribirResultadosPresidenciaPais("data/resultados/resultados_pais_presidencia.txt", resPais);
            // Write segunda vuelta file if required
            if (resPais.getRequiereSegundaVuelta()) {
                Escrutinio::escribirSegundaVuelta("data/resultados/segunda_vuelta.txt", resPais, sys);
            }
            cout << resPais.toString() << "\n";
        }
        sys.finalizarSimulacion();
    }
};

#endif // MENUSIMULACION_H