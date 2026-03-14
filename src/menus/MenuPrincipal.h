#ifndef MENUPRINCIPAL_H
#define MENUPRINCIPAL_H

#include <iostream>
#include <string>
#include <limits>
#include "../sistema/SistemaElectoral.h"
#include "../sistema/Tarjeton.h"
#include "../sistema/Consultas.h"
#include "../sistema/Simulador.h"
#include "../sistema/Escrutinio.h"
#include "../sistema/Estadisticas.h"
#include "MenuEstadisticas.h"

using namespace std;

class MenuPrincipal {
private:
    SistemaElectoral& sys;

    void pausar() {
        cout << "Presione ENTER para continuar...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

public:
    MenuPrincipal(SistemaElectoral& s) : sys(s) {}

    void mostrar() {
        bool salir = false;
        while (!salir) {
            cout << "\n=== SISTEMA ELECTORAL ===\n";
            cout << "1. Consultas\n";
            cout << "2. Tarjetones\n";
            cout << "3. Simulacion\n";
            cout << "4. Escrutinio (demo)\n";
            cout << "5. Estadisticas\n";
            cout << "6. Salir\n";
            cout << "Opcion: ";
            int op;
            if (!(cin >> op)) { cin.clear(); cin.ignore(10000, '\n'); continue; }
            cin.ignore(10000, '\n');

            if (op == 1) consultas();
            else if (op == 2) tarjetones();
            else if (op == 3) simulacion();
            else if (op == 4) escrutinioDemo();
            else if (op == 5) estadisticas();
            else if (op == 6) salir = true;
            else cout << "Opcion invalida\n";
        }
    }

    void consultas() {
        bool salir = false;
        while (!salir) {
            cout << "\n-- Consultas --\n";
            cout << "1. Listar formulas presidenciales\n";
            cout << "2. Candidatos a alcaldia por partido en capitales\n";
            cout << "3. Censo total\n";
            cout << "4. Volver\n";
            cout << "Opcion: ";
            int op;
            if (!(cin >> op)) { cin.clear(); cin.ignore(10000, '\n'); continue; }
            cin.ignore(10000, '\n');
            
            if (op == 1) {
                Lista<string> out;
                Consultas::listarPresidencia(sys, out);
                cout << "\nFormulas presidenciales:\n";
                for (int i = 0; i < out.obtenerTamano(); ++i) {
                    string s; out.obtenerElemento(i, s);
                    cout << "  " << s << "\n";
                }
                pausar();
            } else if (op == 2) {
                cout << "Nombre del partido: ";
                string partido;
                getline(cin, partido);
                Lista<string> out;
                Consultas::candidatosAlcaldiaPorPartidoEnCapitales(sys, partido, out);
                cout << "\nCandidatos a alcaldia de " << partido << " en capitales:\n";
                if (out.obtenerTamano() == 0) {
                    cout << "  No se encontraron candidatos\n";
                } else {
                    for (int i = 0; i < out.obtenerTamano(); ++i) {
                        string s; out.obtenerElemento(i, s);
                        cout << "  " << s << "\n";
                    }
                }
                pausar();
            } else if (op == 3) {
                cout << "\nCenso total nacional: " << Consultas::censoTotal(sys) << " habitantes\n";
                pausar();
            } else if (op == 4) {
                salir = true;
            }
        }
    }

    void tarjetones() {
        cout << "\n-- Tarjetones --\n";
        int idBogota = -1;
        if (sys.obtenerIdCiudadPorNombre("Bogota", idBogota)) {
            Lista<string> out;
            Tarjeton::generarTarjetonAlcaldia(sys, idBogota, out);
            cout << "Tarjeton alcaldia (Bogota):\n";
            for (int i = 0; i < out.obtenerTamano(); ++i) { string s; out.obtenerElemento(i, s); cout << s << "\n"; }
        }
        Lista<string> out2;
        Tarjeton::generarTarjetonPresidencia(sys, out2);
        cout << "Tarjeton presidencia:\n";
        for (int i = 0; i < out2.obtenerTamano(); ++i) { string s; out2.obtenerElemento(i, s); cout << s << "\n"; }
        pausar();
    }

    void simulacion() {
        cout << "\n-- Simulacion --\n";
        sys.comenzarSimulacion();
        Simulador::seed(12345);

        int idBogota = -1;
        if (sys.obtenerIdCiudadPorNombre("Bogota", idBogota)) {
            ResultadoElectoral resBog(0); // Placeholder; Simulador sets correct censo internally
            if (Simulador::simularAlcaldiaCiudad(sys, idBogota, resBog)) {
                Escrutinio::escribirResultadosAlcaldiaPorCiudad("data/resultados/resultados_ciudad_alcaldia.txt", "Bogota", resBog);
                cout << resBog.toString() << "\n";
            }
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
        pausar();
    }

    void escrutinioDemo() {
        cout << "Ver archivos en data/resultados/ para los escrutinios generados.\n";
        pausar();
    }

    void estadisticas() {
        MenuEstadisticas menuEst(sys);
        menuEst.mostrar();
    }
};

#endif // MENUPRINCIPAL_H