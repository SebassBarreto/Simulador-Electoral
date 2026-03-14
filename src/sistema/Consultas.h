#ifndef CONSULTAS_H
#define CONSULTAS_H

#include <iostream>
#include <string>
#include "../../include/listas/Lista.h"
#include "../sistema/SistemaElectoral.h"
#include "../indices/MultilistaElectoral.h"
#include "../modelos/Candidato.h"

using namespace std;

class Consultas {
public:
    // candidatos a alcaldia por partido y region
    static void candidatosAlcaldiaPorPartidoYRegion(SistemaElectoral& sys, int idRegion, const string& nombrePartido, Lista<string>& out) {
        out.limpiar();
        int idPartido = -1;
        if (!sys.obtenerIdPartidoPorNombre(nombrePartido, idPartido)) return;

        Lista<int>* ciudadesRegion = sys.getMultilistaElectoral().obtenerCiudadesDeRegion(idRegion);
        if (ciudadesRegion == NULL) return;

        int n = ciudadesRegion->obtenerTamano();
        for (int i = 0; i < n; ++i) {
            int idCiudad;
            if (!ciudadesRegion->obtenerElemento(i, idCiudad)) continue;
            int idCandidato = -1;
            if (sys.getMultilistaElectoral().obtenerCandidatoAlcaldia(idCiudad, idPartido, idCandidato)) {
                string ident = to_string(idCandidato);
                int pos = -1;
                if (!sys.obtenerPosCandidatoPorIdentificacion(ident, pos)) continue;
                if (pos < 0 || pos >= sys.getCandidatosAlcaldia().obtenerTamano()) continue;
                Candidato c;
                if (!sys.getCandidatosAlcaldia().obtenerElemento(pos, c)) continue;
                out.insertarFinal(c.getNombre() + " " + c.getApellido() + " | " + c.getCiudadResidencia() + " | " + c.getPartido());
            }
        }
    }

    // candidato a alcaldia por ciudad y partido
    static bool candidatoAlcaldiaPorCiudadYPartido(SistemaElectoral& sys, const string& nombreCiudad, const string& nombrePartido, string& lineaOut) {
        int idCiudad = -1;
        int idPartido = -1;
        if (!sys.obtenerIdCiudadPorNombre(nombreCiudad, idCiudad)) return false;
        if (!sys.obtenerIdPartidoPorNombre(nombrePartido, idPartido)) return false;

        int idCandidato = -1;
        if (!sys.getMultilistaElectoral().obtenerCandidatoAlcaldia(idCiudad, idPartido, idCandidato)) return false;

        string ident = to_string(idCandidato);
        int pos = -1;
        if (!sys.obtenerPosCandidatoPorIdentificacion(ident, pos)) return false;
        if (pos < 0 || pos >= sys.getCandidatosAlcaldia().obtenerTamano()) return false;

        Candidato c;
        if (!sys.getCandidatosAlcaldia().obtenerElemento(pos, c)) return false;

        lineaOut = c.getNombre() + " " + c.getApellido() + " | " + c.getPartido() + " | " + c.getCiudadResidencia();
        return true;
    }

    // listar formulas presidenciales
    static void listarPresidencia(SistemaElectoral& sys, Lista<string>& out) {
        out.limpiar();
        int n = sys.getCandidatosPresidencia().obtenerTamano();
        for (int i = 0; i < n; ++i) {
            Candidato c;
            if (!sys.getCandidatosPresidencia().obtenerElemento(i, c)) continue;
            out.insertarFinal(c.getPartido() + " | " + c.getNombre() + " " + c.getApellido() + " - " + c.getVicepresidente());
        }
    }

    // censo por ciudad y total
    static int censoTotal(SistemaElectoral& sys) {
        int total = 0;
        int n = sys.getCiudades().obtenerTamano();
        for (int i = 0; i < n; ++i) {
            Ciudad c;
            if (sys.getCiudades().obtenerElemento(i, c)) {
                total += c.getPoblacion();
            }
        }
        return total;
    }

    // NEW: Dado un partido, listar candidatos a alcaldías por cada capital
    static void candidatosAlcaldiaPorPartidoEnCapitales(SistemaElectoral& sys, const string& nombrePartido, Lista<string>& out) {
        out.limpiar();
        int idPartido = -1;
        if (!sys.obtenerIdPartidoPorNombre(nombrePartido, idPartido)) return;

        const Lista<Ciudad>& ciudades = sys.getCiudades();
        int n = ciudades.obtenerTamano();
        
        for (int i = 0; i < n; ++i) {
            Ciudad ciudad;
            if (!ciudades.obtenerElemento(i, ciudad)) continue;
            
            // Filter by capital cities only
            if (!ciudad.getEsCapital()) continue;
            
            int idCandidato = -1;
            if (!sys.getMultilistaElectoral().obtenerCandidatoAlcaldia(ciudad.getId(), idPartido, idCandidato)) continue;
            
            string ident = to_string(idCandidato);
            int pos = -1;
            if (!sys.obtenerPosCandidatoPorIdentificacion(ident, pos)) continue;
            if (pos < 0 || pos >= sys.getCandidatosAlcaldia().obtenerTamano()) continue;
            
            Candidato c;
            if (!sys.getCandidatosAlcaldia().obtenerElemento(pos, c)) continue;
            
            out.insertarFinal(ciudad.getNombre() + " | " + c.getNombre() + " " + c.getApellido() + " | " + c.getPartido());
        }
    }
};

#endif // CONSULTAS_H