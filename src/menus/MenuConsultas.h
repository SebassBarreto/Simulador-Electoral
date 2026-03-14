#ifndef MENUCONSULTAS_H
#define MENUCONSULTAS_H

#include <iostream>
#include <string>
#include "../sistema/SistemaElectoral.h"
#include "../sistema/Consultas.h"

using namespace std;

class MenuConsultas {
private:
    SistemaElectoral& sys;
public:
    MenuConsultas(SistemaElectoral& s) : sys(s) {}

    void mostrar() {
        cout << "\n-- Menu Consultas --\n";
        cout << "1. Listar presidencia\n";
        cout << "2. Candidato alcaldia por ciudad y partido\n";
        cout << "3. Candidatos por partido y region\n";
        cout << "Opcion: ";
        int op; if (!(cin >> op)) { cin.clear(); cin.ignore(10000, '\n'); return; }
        cin.ignore(10000, '\n');

        if (op == 1) {
            Lista<string> out; Consultas::listarPresidencia(sys, out);
            for (int i = 0; i < out.obtenerTamano(); ++i) { string s; out.obtenerElemento(i, s); cout << s << "\n"; }
        } else if (op == 2) {
            string ciudad, partido;
            cout << "Ciudad: "; getline(cin, ciudad);
            cout << "Partido: "; getline(cin, partido);
            string linea;
            if (Consultas::candidatoAlcaldiaPorCiudadYPartido(sys, ciudad, partido, linea)) cout << linea << "\n";
            else cout << "No encontrado\n";
        } else if (op == 3) {
            string partido; int idRegion;
            cout << "Id region: "; cin >> idRegion; cin.ignore(10000, '\n');
            cout << "Partido: "; getline(cin, partido);
            Lista<string> out;
            Consultas::candidatosAlcaldiaPorPartidoYRegion(sys, idRegion, partido, out);
            for (int i = 0; i < out.obtenerTamano(); ++i) { string s; out.obtenerElemento(i, s); cout << s << "\n"; }
        }
    }
};

#endif // MENUCONSULTAS_H