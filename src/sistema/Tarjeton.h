#ifndef TARJETON_H
#define TARJETON_H

#include <iostream>
#include <string>
#include "../../include/listas/Lista.h"
#include "../indices/MultilistaElectoral.h"
#include "../modelos/Candidato.h"
#include "../sistema/SistemaElectoral.h"

using namespace std;

class Tarjeton {
public:
    static bool generarTarjetonAlcaldia(SistemaElectoral& sys, int idCiudad, Lista<string>& lineasOut) {
        lineasOut.limpiar();
        Lista<int>* partidos = sys.getMultilistaElectoral().obtenerPartidosDeCiudad(idCiudad);
        if (partidos == NULL || partidos->estaVacia()) {
            lineasOut.insertarFinal(string("0. Voto en blanco"));
            return true;
        }
        lineasOut.insertarFinal(string("0. Voto en blanco"));
        int n = partidos->obtenerTamano();
        for (int i = 0; i < n; ++i) {
            int idPartido;
            if (!partidos->obtenerElemento(i, idPartido)) continue;
            int idCandidato = -1;
            if (!sys.getMultilistaElectoral().obtenerCandidatoAlcaldia(idCiudad, idPartido, idCandidato)) continue;

            // buscar candidato por identificacion en lista de alcaldia
            string ident = to_string(idCandidato);
            int pos = -1;
            if (!sys.obtenerPosCandidatoPorIdentificacion(ident, pos)) continue;
            if (pos < 0 || pos >= sys.getCandidatosAlcaldia().obtenerTamano()) continue;
            Candidato c;
            if (!sys.getCandidatosAlcaldia().obtenerElemento(pos, c)) continue;

            string item = to_string(i + 1) + ". " + c.getNombre() + " " + c.getApellido() + " | " + c.getPartido();
            lineasOut.insertarFinal(item);
        }
        return true;
    }

    static bool generarTarjetonPresidencia(SistemaElectoral& sys, Lista<string>& lineasOut) {
        lineasOut.limpiar();
        lineasOut.insertarFinal(string("0. Voto en blanco"));
        int n = sys.getCandidatosPresidencia().obtenerTamano();
        for (int i = 0; i < n; ++i) {
            Candidato c;
            if (!sys.getCandidatosPresidencia().obtenerElemento(i, c)) continue;
            string item = to_string(i + 1) + ". " + c.getPartido() + " | " + c.getNombre() + " " + c.getApellido() + " - " + c.getVicepresidente();
            lineasOut.insertarFinal(item);
        }
        return true;
    }
};

#endif // TARJETON_H