#ifndef ESTADISTICAS_H
#define ESTADISTICAS_H

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <map>
#include <vector>
#include <algorithm>
#include "../../include/listas/Lista.h"
#include "../sistema/SistemaElectoral.h"
#include "../indices/MultilistaElectoral.h"
#include "../gestores/GestorArchivos.h"
#include "../modelos/ResultadoElectoral.h"

using namespace std;

// Structure to hold simulation results for a city
struct ResultadoCiudad {
    int idCiudad;
    string nombreCiudad;
    int idRegion;
    int censo;
    int votosEmitidos;
    int votosEnBlanco;
    int votosNulos;
    int abstencion;
    string ganador;
    string partidoGanador;
    char sexoGanador;
    map<string, int> votosPorCandidato;
    map<string, int> votosPorPartido;
    int votosHombres;
    int votosMujeres;
    
    ResultadoCiudad() : idCiudad(0), idRegion(0), censo(0), votosEmitidos(0),
                        votosEnBlanco(0), votosNulos(0), abstencion(0),
                        sexoGanador('M'), votosHombres(0), votosMujeres(0) {}
};

class Estadisticas {
public:
    // =====================================================
    // ALCALDIAS: Reporte General por Region
    // Total votos, blancos, nulos, abstencion (cantidad y porcentaje)
    // =====================================================
    static void estadisticaAlcaldiaPorRegion(SistemaElectoral& sys, 
                                              const Lista<ResultadoCiudad>& resultados,
                                              const string& rutaOut) {
        string lines[512];
        int k = 0;
        lines[k++] = "ESTADISTICAS ALCALDIA POR REGION";
        lines[k++] = "================================";
        lines[k++] = "";

        // Group results by region
        map<int, vector<int>> ciudadesPorRegion; // idRegion -> indices of resultados
        for (int i = 0; i < resultados.obtenerTamano(); ++i) {
            ResultadoCiudad rc;
            if (!resultados.obtenerElemento(i, rc)) continue;
            ciudadesPorRegion[rc.idRegion].push_back(i);
        }

        Lista<int> regiones;
        sys.getMultilistaElectoral().listarRegiones(regiones);
        
        for (int r = 0; r < regiones.obtenerTamano(); ++r) {
            int idRegion;
            if (!regiones.obtenerElemento(r, idRegion)) continue;
            
            string nombreRegion = obtenerNombreRegion(sys, idRegion);
            
            int totalCenso = 0, totalVotos = 0, totalBlancos = 0, totalNulos = 0, totalAbst = 0;
            
            if (ciudadesPorRegion.find(idRegion) != ciudadesPorRegion.end()) {
                for (int idx : ciudadesPorRegion[idRegion]) {
                    ResultadoCiudad rc;
                    if (!resultados.obtenerElemento(idx, rc)) continue;
                    totalCenso += rc.censo;
                    totalVotos += rc.votosEmitidos;
                    totalBlancos += rc.votosEnBlanco;
                    totalNulos += rc.votosNulos;
                    totalAbst += rc.abstencion;
                }
            }
            
            lines[k++] = "Region: " + nombreRegion + " (ID: " + to_string(idRegion) + ")";
            lines[k++] = "  Censo electoral: " + to_string(totalCenso);
            lines[k++] = "  Total votos emitidos: " + to_string(totalVotos) + " (" + formatPct(totalVotos, totalCenso) + "%)";
            lines[k++] = "  Votos en blanco: " + to_string(totalBlancos) + " (" + formatPct(totalBlancos, totalCenso) + "%)";
            lines[k++] = "  Votos nulos: " + to_string(totalNulos) + " (" + formatPct(totalNulos, totalCenso) + "%)";
            lines[k++] = "  Abstencion: " + to_string(totalAbst) + " (" + formatPct(totalAbst, totalCenso) + "%)";
            lines[k++] = "";
        }
        
        GestorArchivos::escribirLineas(rutaOut, lines, k);
    }

    // =====================================================
    // ALCALDIAS: Reporte Detallado por Region
    // Numero de alcaldes por partido; totales hombres y mujeres
    // =====================================================
    static void estadisticaAlcaldiaDetallePorRegion(SistemaElectoral& sys,
                                                     const Lista<ResultadoCiudad>& resultados,
                                                     const string& rutaOut) {
        string lines[1024];
        int k = 0;
        lines[k++] = "ESTADISTICAS ALCALDIA DETALLE POR REGION";
        lines[k++] = "=========================================";
        lines[k++] = "";

        // Group results by region
        map<int, vector<int>> ciudadesPorRegion;
        for (int i = 0; i < resultados.obtenerTamano(); ++i) {
            ResultadoCiudad rc;
            if (!resultados.obtenerElemento(i, rc)) continue;
            ciudadesPorRegion[rc.idRegion].push_back(i);
        }

        Lista<int> regiones;
        sys.getMultilistaElectoral().listarRegiones(regiones);
        
        for (int r = 0; r < regiones.obtenerTamano(); ++r) {
            int idRegion;
            if (!regiones.obtenerElemento(r, idRegion)) continue;
            
            string nombreRegion = obtenerNombreRegion(sys, idRegion);
            map<string, int> alcaldesPorPartido;
            int totalHombres = 0, totalMujeres = 0;
            int totalAlcaldes = 0;
            
            if (ciudadesPorRegion.find(idRegion) != ciudadesPorRegion.end()) {
                for (int idx : ciudadesPorRegion[idRegion]) {
                    ResultadoCiudad rc;
                    if (!resultados.obtenerElemento(idx, rc)) continue;
                    if (!rc.partidoGanador.empty()) {
                        alcaldesPorPartido[rc.partidoGanador]++;
                        totalAlcaldes++;
                        if (rc.sexoGanador == 'M') totalHombres++;
                        else totalMujeres++;
                    }
                }
            }
            
            lines[k++] = "Region: " + nombreRegion + " (ID: " + to_string(idRegion) + ")";
            lines[k++] = "  Total alcaldes electos: " + to_string(totalAlcaldes);
            lines[k++] = "  Alcaldes por partido:";
            for (const auto& p : alcaldesPorPartido) {
                lines[k++] = "    " + p.first + ": " + to_string(p.second) + " (" + formatPct(p.second, totalAlcaldes) + "%)";
            }
            lines[k++] = "  Hombres: " + to_string(totalHombres) + " (" + formatPct(totalHombres, totalAlcaldes) + "%)";
            lines[k++] = "  Mujeres: " + to_string(totalMujeres) + " (" + formatPct(totalMujeres, totalAlcaldes) + "%)";
            lines[k++] = "";
        }
        
        GestorArchivos::escribirLineas(rutaOut, lines, k);
    }

    // =====================================================
    // ALCALDIAS: Totales Nacionales
    // Totales por partido; totales por hombres y mujeres
    // =====================================================
    static void estadisticaAlcaldiaTotalesNacionales(SistemaElectoral& sys,
                                                      const Lista<ResultadoCiudad>& resultados,
                                                      const string& rutaOut) {
        string lines[512];
        int k = 0;
        lines[k++] = "ESTADISTICAS ALCALDIA TOTALES NACIONALES";
        lines[k++] = "=========================================";
        lines[k++] = "";

        map<string, int> alcaldesPorPartido;
        int totalHombres = 0, totalMujeres = 0;
        int totalAlcaldes = 0;
        int totalCenso = 0, totalVotos = 0, totalBlancos = 0, totalNulos = 0, totalAbst = 0;
        
        for (int i = 0; i < resultados.obtenerTamano(); ++i) {
            ResultadoCiudad rc;
            if (!resultados.obtenerElemento(i, rc)) continue;
            
            totalCenso += rc.censo;
            totalVotos += rc.votosEmitidos;
            totalBlancos += rc.votosEnBlanco;
            totalNulos += rc.votosNulos;
            totalAbst += rc.abstencion;
            
            if (!rc.partidoGanador.empty()) {
                alcaldesPorPartido[rc.partidoGanador]++;
                totalAlcaldes++;
                if (rc.sexoGanador == 'M') totalHombres++;
                else totalMujeres++;
            }
        }
        
        lines[k++] = "RESUMEN NACIONAL";
        lines[k++] = "  Censo electoral total: " + to_string(totalCenso);
        lines[k++] = "  Total votos emitidos: " + to_string(totalVotos) + " (" + formatPct(totalVotos, totalCenso) + "%)";
        lines[k++] = "  Votos en blanco: " + to_string(totalBlancos) + " (" + formatPct(totalBlancos, totalCenso) + "%)";
        lines[k++] = "  Votos nulos: " + to_string(totalNulos) + " (" + formatPct(totalNulos, totalCenso) + "%)";
        lines[k++] = "  Abstencion: " + to_string(totalAbst) + " (" + formatPct(totalAbst, totalCenso) + "%)";
        lines[k++] = "";
        lines[k++] = "ALCALDES ELECTOS POR PARTIDO";
        lines[k++] = "  Total alcaldes: " + to_string(totalAlcaldes);
        for (const auto& p : alcaldesPorPartido) {
            lines[k++] = "  " + p.first + ": " + to_string(p.second) + " (" + formatPct(p.second, totalAlcaldes) + "%)";
        }
        lines[k++] = "";
        lines[k++] = "DISTRIBUCION POR SEXO";
        lines[k++] = "  Hombres: " + to_string(totalHombres) + " (" + formatPct(totalHombres, totalAlcaldes) + "%)";
        lines[k++] = "  Mujeres: " + to_string(totalMujeres) + " (" + formatPct(totalMujeres, totalAlcaldes) + "%)";
        
        GestorArchivos::escribirLineas(rutaOut, lines, k);
    }

    // =====================================================
    // PRESIDENCIA: Reporte General por Region
    // Total votos, blancos, nulos, abstencion
    // =====================================================
    static void estadisticaPresidenciaPorRegion(SistemaElectoral& sys,
                                                 const Lista<ResultadoCiudad>& resultados,
                                                 const string& rutaOut) {
        string lines[512];
        int k = 0;
        lines[k++] = "ESTADISTICAS PRESIDENCIA POR REGION";
        lines[k++] = "====================================";
        lines[k++] = "";

        // Group results by region
        map<int, vector<int>> ciudadesPorRegion;
        for (int i = 0; i < resultados.obtenerTamano(); ++i) {
            ResultadoCiudad rc;
            if (!resultados.obtenerElemento(i, rc)) continue;
            ciudadesPorRegion[rc.idRegion].push_back(i);
        }

        Lista<int> regiones;
        sys.getMultilistaElectoral().listarRegiones(regiones);
        
        for (int r = 0; r < regiones.obtenerTamano(); ++r) {
            int idRegion;
            if (!regiones.obtenerElemento(r, idRegion)) continue;
            
            string nombreRegion = obtenerNombreRegion(sys, idRegion);
            
            int totalCenso = 0, totalVotos = 0, totalBlancos = 0, totalNulos = 0, totalAbst = 0;
            
            if (ciudadesPorRegion.find(idRegion) != ciudadesPorRegion.end()) {
                for (int idx : ciudadesPorRegion[idRegion]) {
                    ResultadoCiudad rc;
                    if (!resultados.obtenerElemento(idx, rc)) continue;
                    totalCenso += rc.censo;
                    totalVotos += rc.votosEmitidos;
                    totalBlancos += rc.votosEnBlanco;
                    totalNulos += rc.votosNulos;
                    totalAbst += rc.abstencion;
                }
            }
            
            lines[k++] = "Region: " + nombreRegion + " (ID: " + to_string(idRegion) + ")";
            lines[k++] = "  Censo electoral: " + to_string(totalCenso);
            lines[k++] = "  Total votos emitidos: " + to_string(totalVotos) + " (" + formatPct(totalVotos, totalCenso) + "%)";
            lines[k++] = "  Votos en blanco: " + to_string(totalBlancos) + " (" + formatPct(totalBlancos, totalCenso) + "%)";
            lines[k++] = "  Votos nulos: " + to_string(totalNulos) + " (" + formatPct(totalNulos, totalCenso) + "%)";
            lines[k++] = "  Abstencion: " + to_string(totalAbst) + " (" + formatPct(totalAbst, totalCenso) + "%)";
            lines[k++] = "";
        }
        
        GestorArchivos::escribirLineas(rutaOut, lines, k);
    }

    // =====================================================
    // PRESIDENCIA: Reporte General para todo el Pais
    // =====================================================
    static void estadisticaPresidenciaPais(SistemaElectoral& sys,
                                           const ResultadoElectoral& resultado,
                                           const string& rutaOut) {
        string lines[512];
        int k = 0;
        lines[k++] = "ESTADISTICAS PRESIDENCIA PAIS";
        lines[k++] = "==============================";
        lines[k++] = "";
        
        int censo = resultado.getCensoElectoral();
        int votos = resultado.getTotalVotosEmitidos();
        int blancos = resultado.getVotosEnBlanco();
        int nulos = resultado.getVotosNulos();
        int abst = resultado.getAbstencion();
        
        lines[k++] = "Censo electoral: " + to_string(censo);
        lines[k++] = "Total votos emitidos: " + to_string(votos) + " (" + formatPct(votos, censo) + "%)";
        lines[k++] = "Votos en blanco: " + to_string(blancos) + " (" + formatPct(blancos, censo) + "%)";
        lines[k++] = "Votos nulos: " + to_string(nulos) + " (" + formatPct(nulos, censo) + "%)";
        lines[k++] = "Abstencion: " + to_string(abst) + " (" + formatPct(abst, censo) + "%)";
        lines[k++] = "";
        lines[k++] = "Ganador: " + resultado.getGanador();
        lines[k++] = "Requiere segunda vuelta: " + string(resultado.getRequiereSegundaVuelta() ? "SI" : "NO");
        
        GestorArchivos::escribirLineas(rutaOut, lines, k);
    }

    // =====================================================
    // PRESIDENCIA: Reporte Detallado por Departamento (ciudad)
    // Votos por partido; votos por hombres y mujeres
    // =====================================================
    static void estadisticaPresidenciaDetallePorDepartamento(SistemaElectoral& sys,
                                                              const Lista<ResultadoCiudad>& resultados,
                                                              const string& rutaOut) {
        string lines[1024];
        int k = 0;
        lines[k++] = "ESTADISTICAS PRESIDENCIA DETALLE POR DEPARTAMENTO";
        lines[k++] = "==================================================";
        lines[k++] = "";

        for (int i = 0; i < resultados.obtenerTamano(); ++i) {
            ResultadoCiudad rc;
            if (!resultados.obtenerElemento(i, rc)) continue;
            
            int totalVotosValidos = rc.votosEmitidos - rc.votosNulos;
            
            lines[k++] = "Ciudad/Departamento: " + rc.nombreCiudad;
            lines[k++] = "  Censo: " + to_string(rc.censo);
            lines[k++] = "  Votos emitidos: " + to_string(rc.votosEmitidos) + " (" + formatPct(rc.votosEmitidos, rc.censo) + "%)";
            lines[k++] = "  Votos por partido:";
            for (const auto& p : rc.votosPorPartido) {
                lines[k++] = "    " + p.first + ": " + to_string(p.second) + " (" + formatPct(p.second, totalVotosValidos) + "%)";
            }
            lines[k++] = "  Votos por sexo del votante:";
            int totalVotos = rc.votosHombres + rc.votosMujeres;
            lines[k++] = "    Hombres: " + to_string(rc.votosHombres) + " (" + formatPct(rc.votosHombres, totalVotos) + "%)";
            lines[k++] = "    Mujeres: " + to_string(rc.votosMujeres) + " (" + formatPct(rc.votosMujeres, totalVotos) + "%)";
            lines[k++] = "";
        }
        
        GestorArchivos::escribirLineas(rutaOut, lines, k);
    }

    // =====================================================
    // PRESIDENCIA: Reporte Detallado para todo el Pais
    // Votos por partido; votos por hombres y mujeres
    // =====================================================
    static void estadisticaPresidenciaDetallePais(SistemaElectoral& sys,
                                                   const ResultadoElectoral& resultado,
                                                   const Lista<ResultadoCiudad>& resultados,
                                                   const string& rutaOut) {
        string lines[512];
        int k = 0;
        lines[k++] = "ESTADISTICAS PRESIDENCIA DETALLE PAIS";
        lines[k++] = "======================================";
        lines[k++] = "";
        
        int censo = resultado.getCensoElectoral();
        int votosValidos = resultado.getTotalVotosValidos();
        
        lines[k++] = "Censo electoral: " + to_string(censo);
        lines[k++] = "Total votos validos: " + to_string(votosValidos) + " (" + formatPct(votosValidos, censo) + "%)";
        lines[k++] = "";
        
        // Votos por partido (using candidate names to infer party)
        lines[k++] = "VOTOS POR PARTIDO:";
        const map<string, int>& votosCandidato = resultado.getVotosPorCandidato();
        map<string, int> votosPorPartido;
        for (const auto& vc : votosCandidato) {
            string partido = buscarPartidoCandidatoPresidencial(sys, vc.first);
            votosPorPartido[partido] += vc.second;
        }
        for (const auto& p : votosPorPartido) {
            lines[k++] = "  " + p.first + ": " + to_string(p.second) + " (" + formatPct(p.second, votosValidos) + "%)";
        }
        lines[k++] = "";
        
        // Aggregate gender votes from city results
        int totalHombres = 0, totalMujeres = 0;
        for (int i = 0; i < resultados.obtenerTamano(); ++i) {
            ResultadoCiudad rc;
            if (!resultados.obtenerElemento(i, rc)) continue;
            totalHombres += rc.votosHombres;
            totalMujeres += rc.votosMujeres;
        }
        int totalGenero = totalHombres + totalMujeres;
        
        lines[k++] = "VOTOS POR SEXO DEL VOTANTE:";
        lines[k++] = "  Hombres: " + to_string(totalHombres) + " (" + formatPct(totalHombres, totalGenero) + "%)";
        lines[k++] = "  Mujeres: " + to_string(totalMujeres) + " (" + formatPct(totalMujeres, totalGenero) + "%)";
        
        GestorArchivos::escribirLineas(rutaOut, lines, k);
    }

    // =====================================================
    // Legacy function: total censo por region
    // =====================================================
    static void estadisticaCensoPorRegion(SistemaElectoral& sys, const string& rutaOut) {
        Lista<int> regiones;
        sys.getMultilistaElectoral().listarRegiones(regiones);
        string lines[512];
        int k = 0;
        for (int i = 0; i < regiones.obtenerTamano(); ++i) {
            int idRegion;
            if (!regiones.obtenerElemento(i, idRegion)) continue;
            Lista<int>* ciudades = sys.getMultilistaElectoral().obtenerCiudadesDeRegion(idRegion);
            int censoRegion = 0;
            if (ciudades != NULL) {
                for (int j = 0; j < ciudades->obtenerTamano(); ++j) {
                    int idCiudad;
                    if (!ciudades->obtenerElemento(j, idCiudad)) continue;
                    Ciudad c;
                    if (!obtenerCiudadPorId(sys, idCiudad, c)) continue;
                    censoRegion += c.getPoblacion();
                }
            }
            lines[k++] = string("Region ") + to_string(idRegion) + "|Censo|" + to_string(censoRegion);
        }
        GestorArchivos::escribirLineas(rutaOut, lines, k);
    }

private:
    static bool obtenerCiudadPorId(SistemaElectoral& sys, int idCiudad, Ciudad& out) {
        int n = sys.getCiudades().obtenerTamano();
        for (int i = 0; i < n; ++i) {
            Ciudad c;
            if (!sys.getCiudades().obtenerElemento(i, c)) continue;
            if (c.getId() == idCiudad) { out = c; return true; }
        }
        return false;
    }
    
    static string obtenerNombreRegion(SistemaElectoral& sys, int idRegion) {
        const Lista<Region>& regiones = sys.getRegiones();
        for (int i = 0; i < regiones.obtenerTamano(); ++i) {
            Region r;
            if (!regiones.obtenerElemento(i, r)) continue;
            if (r.getId() == idRegion) return r.getNombre();
        }
        return "Region " + to_string(idRegion);
    }
    
    static string buscarPartidoCandidatoPresidencial(SistemaElectoral& sys, const string& nombreCompleto) {
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
    
    static string formatPct(int value, int total) {
        if (total == 0) return "0.00";
        float pct = (static_cast<float>(value) / total) * 100.0f;
        stringstream ss;
        ss << fixed << setprecision(2) << pct;
        return ss.str();
    }
};

#endif // ESTADISTICAS_H