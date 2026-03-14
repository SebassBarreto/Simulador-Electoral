#ifndef ESCRUTINIO_H
#define ESCRUTINIO_H

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include "../../include/listas/Lista.h"
#include "../sistema/SistemaElectoral.h"
#include "../modelos/ResultadoElectoral.h"
#include "../gestores/GestorArchivos.h"

using namespace std;

class Escrutinio {
public:
    static bool escribirResultadosAlcaldiaPorCiudad(const string& ruta, const string& nombreCiudad, const ResultadoElectoral& res) {
        string out[64];
        int k = 0;
        out[k++] = string("CIUDAD: ") + nombreCiudad;
        out[k++] = string("Censo: ") + to_string(res.getCensoElectoral());
        out[k++] = string("Emitidos: ") + to_string(res.getTotalVotosEmitidos());
        out[k++] = string("Abstencion: ") + to_string(res.getAbstencion());
        out[k++] = string("Votos nulos: ") + to_string(res.getVotosNulos());
        out[k++] = string("Votos en blanco: ") + to_string(res.getVotosEnBlanco());
        out[k++] = string("Ganador: ") + res.getGanador();
        return GestorArchivos::escribirLineas(ruta, out, k);
    }

    static bool escribirResultadosPresidenciaPais(const string& ruta, const ResultadoElectoral& res) {
        string out[64];
        int k = 0;
        out[k++] = string("PAIS");
        out[k++] = string("Censo: ") + to_string(res.getCensoElectoral());
        out[k++] = string("Emitidos: ") + to_string(res.getTotalVotosEmitidos());
        out[k++] = string("Abstencion: ") + to_string(res.getAbstencion());
        out[k++] = string("Votos nulos: ") + to_string(res.getVotosNulos());
        out[k++] = string("Votos en blanco: ") + to_string(res.getVotosEnBlanco());
        out[k++] = string("Ganador: ") + res.getGanador();
        out[k++] = string("Segunda vuelta: ") + string(res.getRequiereSegundaVuelta() ? "SI" : "NO");
        return GestorArchivos::escribirLineas(ruta, out, k);
    }

    // Get top-2 candidates from ResultadoElectoral and write to segunda_vuelta file
    static bool escribirSegundaVuelta(const string& ruta, const ResultadoElectoral& res, SistemaElectoral& sys) {
        if (!res.getRequiereSegundaVuelta()) {
            return false; // No segunda vuelta required
        }

        const map<string, int>& votos = res.getVotosPorCandidato();
        if (votos.size() < 2) {
            return false; // Not enough candidates
        }

        // Convert to vector and sort by votes descending
        vector<pair<string, int>> candidatos(votos.begin(), votos.end());
        sort(candidatos.begin(), candidatos.end(),
             [](const pair<string, int>& a, const pair<string, int>& b) {
                 return a.second > b.second;
             });

        int totalValidos = res.getTotalVotosValidos();
        
        string out[64];
        int k = 0;
        out[k++] = "SEGUNDA VUELTA PRESIDENCIAL";
        out[k++] = "===========================";
        out[k++] = "";

        // Write top 2 candidates
        for (int i = 0; i < 2 && i < (int)candidatos.size(); ++i) {
            const string& nombreCompleto = candidatos[i].first;
            int votosC = candidatos[i].second;
            float porcentaje = totalValidos > 0 ? (static_cast<float>(votosC) / totalValidos) * 100.0f : 0.0f;
            
            // Try to find party from presidential candidates
            string partido = buscarPartidoCandidato(sys, nombreCompleto);
            
            stringstream ss;
            ss << "Candidato " << (i + 1) << ": " << nombreCompleto;
            out[k++] = ss.str();
            out[k++] = "  Partido: " + partido;
            out[k++] = "  Votos: " + to_string(votosC);
            ss.str("");
            ss << fixed << setprecision(2) << porcentaje;
            out[k++] = "  Porcentaje: " + ss.str() + "%";
            out[k++] = "";
        }

        return GestorArchivos::escribirLineas(ruta, out, k);
    }

private:
    static string buscarPartidoCandidato(SistemaElectoral& sys, const string& nombreCompleto) {
        const Lista<Candidato>& candidatos = sys.getCandidatosPresidencia();
        for (int i = 0; i < candidatos.obtenerTamano(); ++i) {
            Candidato c;
            if (!candidatos.obtenerElemento(i, c)) continue;
            string fullName = c.getNombre() + " " + c.getApellido();
            if (fullName == nombreCompleto) {
                return c.getPartido();
            }
        }
        return "Desconocido";
    }
};

#endif // ESCRUTINIO_H