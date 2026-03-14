#ifndef SIMULADOR_H
#define SIMULADOR_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <map>
#include "../../include/listas/Lista.h"
#include "../sistema/SistemaElectoral.h"
#include "../modelos/ResultadoElectoral.h"
#include "../modelos/Candidato.h"

using namespace std;

// Forward declaration for ResultadoCiudad from Estadisticas.h
struct ResultadoCiudad;

// simulador simple reproducible si se pasa una semilla
class Simulador {
private:
    // Helper functions
    static int censoTotal(SistemaElectoral& sys) {
        int total = 0;
        int n = sys.getCiudades().obtenerTamano();
        for (int i = 0; i < n; ++i) {
            Ciudad c;
            if (!sys.getCiudades().obtenerElemento(i, c)) continue;
            total += c.getPoblacion();
        }
        return total;
    }

    static bool obtenerCiudadPorId(SistemaElectoral& sys, int idCiudad, Ciudad& out) {
        int n = sys.getCiudades().obtenerTamano();
        for (int i = 0; i < n; ++i) {
            Ciudad c;
            if (!sys.getCiudades().obtenerElemento(i, c)) continue;
            if (c.getId() == idCiudad) { out = c; return true; }
        }
        return false;
    }

public:
    static void seed(unsigned int s) {
        srand(s);
    }

    static int randInt(int minV, int maxV) {
        if (maxV <= minV) return minV;
        return minV + (rand() % (maxV - minV + 1));
    }

    // simula alcaldia por ciudad, devuelve ResultadoElectoral por ciudad
    static bool simularAlcaldiaCiudad(SistemaElectoral& sys, int idCiudad, ResultadoElectoral& resultadoOut) {
        // censo: poblacion de la ciudad
        Ciudad c;
        if (!obtenerCiudadPorId(sys, idCiudad, c)) return false;
        int censo = c.getPoblacion();
        ResultadoElectoral res(censo);

        // lista de partidos y candidatos en la ciudad
        Lista<int>* partidos = sys.getMultilistaElectoral().obtenerPartidosDeCiudad(idCiudad);
        int emitidos = 0;
        if (partidos != NULL) {
            int np = partidos->obtenerTamano();
            for (int i = 0; i < np; ++i) {
                int idPartido;
                if (!partidos->obtenerElemento(i, idPartido)) continue;

                int idCandidato = -1;
                if (!sys.getMultilistaElectoral().obtenerCandidatoAlcaldia(idCiudad, idPartido, idCandidato)) continue;

                string ident = to_string(idCandidato);
                int pos = -1;
                if (!sys.obtenerPosCandidatoPorIdentificacion(ident, pos)) continue;
                if (pos < 0 || pos >= sys.getCandidatosAlcaldia().obtenerTamano()) continue;

                Candidato cand;
                if (!sys.getCandidatosAlcaldia().obtenerElemento(pos, cand)) continue;

                int votos = randInt(0, censo / (np + 3)); // distribucion simple
                emitidos += votos;
                for (int v = 0; v < votos; ++v) res.agregarVoto(cand.getNombre() + " " + cand.getApellido());
            }
        }

        // blanco y nulos y abstencion
        int blancos = randInt(0, censo / 20);
        int nulos = randInt(0, censo / 30);
        int abst = censo - (emitidos + blancos + nulos);
        if (abst < 0) abst = 0;

        for (int i = 0; i < blancos; ++i) res.agregarVotoEnBlanco();
        for (int i = 0; i < nulos; ++i) res.agregarVotoNulo();
        for (int i = 0; i < abst; ++i) res.agregarAbstencion();

        res.calcularResultados();
        resultadoOut = res;
        return true;
    }

    // Simula alcaldía con datos extendidos para estadísticas
    static bool simularAlcaldiaCiudadExtendido(SistemaElectoral& sys, int idCiudad,
                                                ResultadoElectoral& resultadoOut,
                                                string& partidoGanador, char& sexoGanador,
                                                map<string, int>& votosPorPartido,
                                                int& votosHombres, int& votosMujeres) {
        Ciudad c;
        if (!obtenerCiudadPorId(sys, idCiudad, c)) return false;
        int censo = c.getPoblacion();
        ResultadoElectoral res(censo);

        votosPorPartido.clear();
        votosHombres = 0;
        votosMujeres = 0;

        Lista<int>* partidos = sys.getMultilistaElectoral().obtenerPartidosDeCiudad(idCiudad);
        int emitidos = 0;
        string candidatoMasVotos = "";
        int maxVotos = -1;
        
        if (partidos != NULL) {
            int np = partidos->obtenerTamano();
            for (int i = 0; i < np; ++i) {
                int idPartido;
                if (!partidos->obtenerElemento(i, idPartido)) continue;

                int idCandidato = -1;
                if (!sys.getMultilistaElectoral().obtenerCandidatoAlcaldia(idCiudad, idPartido, idCandidato)) continue;

                string ident = to_string(idCandidato);
                int pos = -1;
                if (!sys.obtenerPosCandidatoPorIdentificacion(ident, pos)) continue;
                if (pos < 0 || pos >= sys.getCandidatosAlcaldia().obtenerTamano()) continue;

                Candidato cand;
                if (!sys.getCandidatosAlcaldia().obtenerElemento(pos, cand)) continue;

                int votos = randInt(0, censo / (np + 3));
                emitidos += votos;
                
                string nombrePartido = cand.getPartido();
                votosPorPartido[nombrePartido] += votos;
                
                for (int v = 0; v < votos; ++v) {
                    res.agregarVoto(cand.getNombre() + " " + cand.getApellido());
                    // Simulate voter gender (50/50 split roughly)
                    if (randInt(0, 1) == 0) votosHombres++;
                    else votosMujeres++;
                }
                
                if (votos > maxVotos) {
                    maxVotos = votos;
                    candidatoMasVotos = cand.getNombre() + " " + cand.getApellido();
                    partidoGanador = cand.getPartido();
                    sexoGanador = cand.getSexo();
                }
            }
        }

        int blancos = randInt(0, censo / 20);
        int nulos = randInt(0, censo / 30);
        int abst = censo - (emitidos + blancos + nulos);
        if (abst < 0) abst = 0;

        for (int i = 0; i < blancos; ++i) res.agregarVotoEnBlanco();
        for (int i = 0; i < nulos; ++i) res.agregarVotoNulo();
        for (int i = 0; i < abst; ++i) res.agregarAbstencion();

        res.calcularResultados();
        resultadoOut = res;
        return true;
    }

    // simula presidencia nacional
    static bool simularPresidenciaPais(SistemaElectoral& sys, ResultadoElectoral& resultadoOut) {
        int censo = censoTotal(sys);
        ResultadoElectoral res(censo);

        int n = sys.getCandidatosPresidencia().obtenerTamano();
        int emitidos = 0;
        for (int i = 0; i < n; ++i) {
            Candidato c;
            if (!sys.getCandidatosPresidencia().obtenerElemento(i, c)) continue;
            int votos = randInt(0, censo / (n + 5));
            emitidos += votos;
            for (int v = 0; v < votos; ++v) res.agregarVoto(c.getNombre() + " " + c.getApellido());
        }

        int blancos = randInt(0, censo / 25);
        int nulos = randInt(0, censo / 35);
        int abst = censo - (emitidos + blancos + nulos);
        if (abst < 0) abst = 0;

        for (int i = 0; i < blancos; ++i) res.agregarVotoEnBlanco();
        for (int i = 0; i < nulos; ++i) res.agregarVotoNulo();
        for (int i = 0; i < abst; ++i) res.agregarAbstencion();

        res.calcularResultados();
        resultadoOut = res;
        return true;
    }

    // Simula presidencia por ciudad (para estadísticas por departamento)
    static bool simularPresidenciaPorCiudad(SistemaElectoral& sys, int idCiudad,
                                            ResultadoElectoral& resultadoOut,
                                            map<string, int>& votosPorPartido,
                                            int& votosHombres, int& votosMujeres) {
        Ciudad c;
        if (!obtenerCiudadPorId(sys, idCiudad, c)) return false;
        int censo = c.getPoblacion();
        ResultadoElectoral res(censo);
        
        votosPorPartido.clear();
        votosHombres = 0;
        votosMujeres = 0;

        int n = sys.getCandidatosPresidencia().obtenerTamano();
        int emitidos = 0;
        for (int i = 0; i < n; ++i) {
            Candidato cand;
            if (!sys.getCandidatosPresidencia().obtenerElemento(i, cand)) continue;
            int votos = randInt(0, censo / (n + 5));
            emitidos += votos;
            
            string nombrePartido = cand.getPartido();
            votosPorPartido[nombrePartido] += votos;
            
            for (int v = 0; v < votos; ++v) {
                res.agregarVoto(cand.getNombre() + " " + cand.getApellido());
                if (randInt(0, 1) == 0) votosHombres++;
                else votosMujeres++;
            }
        }

        int blancos = randInt(0, censo / 25);
        int nulos = randInt(0, censo / 35);
        int abst = censo - (emitidos + blancos + nulos);
        if (abst < 0) abst = 0;

        for (int i = 0; i < blancos; ++i) res.agregarVotoEnBlanco();
        for (int i = 0; i < nulos; ++i) res.agregarVotoNulo();
        for (int i = 0; i < abst; ++i) res.agregarAbstencion();

        res.calcularResultados();
        resultadoOut = res;
        return true;
    }
};

#endif // SIMULADOR_H