#ifndef SISTEMA_ELECTORAL_H
#define SISTEMA_ELECTORAL_H

#include <iostream>
#include <string>

// modelos
#include "../modelos/Region.h"
#include "../modelos/Ciudad.h"
#include "../modelos/Partido.h"
#include "../modelos/Candidato.h"
#include "../modelos/Voto.h"
#include "../modelos/ResultadoElectoral.h"

// gestores
#include "../gestores/GestorArchivos.h"
#include "../gestores/GestorRegiones.h"
#include "../gestores/GestorCiudades.h"
#include "../gestores/GestorPartidos.h"
#include "../gestores/GestorCandidatos.h"

// indices
#include "../indices/IndiceAVL.h"
#include "../indices/MultilistaElectoral.h"

// listas genericas
#include "../../include/listas/Lista.h"

using namespace std;

class SistemaElectoral {
private:
    Lista<Region> regiones;
    Lista<Ciudad> ciudades;
    Lista<Partido> partidos;
    Lista<Candidato> candidatosAlcaldia;
    Lista<Candidato> candidatosPresidencia;

    IndiceAVL idxCiudadPorNombre;
    IndiceAVL idxPartidoPorNombre;
    IndiceAVL idxCandidatoPorIdent;
    IndiceAVL idxPresidentePorPartido;

    MultilistaElectoral mElectoral;
    bool simulando;

    static string kCiudadNombre(const string& nombre) { return string("Ciudad:") + nombre; }
    static string kPartidoNombre(const string& nombre) { return string("Partido:") + nombre; }
    static string kCandId(const string& id) { return string("Cand:") + id; }
    static string kPresPorPartido(int idPartido) { return string("Presidente:") + to_string(idPartido); }

    int generarIdCiudad() const {
        int maxId = 0; Ciudad c;
        for (int i = 0; i < ciudades.obtenerTamano(); ++i) {
            if (ciudades.obtenerElemento(i, c)) if (c.getId() > maxId) maxId = c.getId();
        }
        return maxId + 1;
    }
    int generarIdPartido() const {
        int maxId = 0; Partido p;
        for (int i = 0; i < partidos.obtenerTamano(); ++i) {
            if (partidos.obtenerElemento(i, p)) if (p.getId() > maxId) maxId = p.getId();
        }
        return maxId + 1;
    }
    int generarIdRegion() const {
        int maxId = 0; Region r;
        for (int i = 0; i < regiones.obtenerTamano(); ++i) {
            if (regiones.obtenerElemento(i, r)) if (r.getId() > maxId) maxId = r.getId();
        }
        return maxId + 1;
    }

    // Crea ciudad si no existe
    int ensureCiudad(const string& nombreCiudad) {
        int id = -1;
        if (idxCiudadPorNombre.buscar(kCiudadNombre(nombreCiudad), id)) return id;
        int nuevoId = generarIdCiudad();
        // idRegion desconocido; usar 0 y ajustar multilista por nombre de region si luego se conoce
        Ciudad nueva(nuevoId, nombreCiudad, 0, 300000, true);
        ciudades.insertarFinal(nueva);
        idxCiudadPorNombre.insertar(kCiudadNombre(nombreCiudad), nuevoId);
        return nuevoId;
    }

    // Crea partido si no existe
    int ensurePartido(const string& nombrePartido) {
        int id = -1;
        if (idxPartidoPorNombre.buscar(kPartidoNombre(nombrePartido), id)) return id;
        int nuevoId = generarIdPartido();
        partidos.insertarFinal(Partido(nombrePartido, "Rep " + nombrePartido, nuevoId));
        idxPartidoPorNombre.insertar(kPartidoNombre(nombrePartido), nuevoId);
        return nuevoId;
    }

public:
    SistemaElectoral() : simulando(false) {}

    bool inicializar(const string& rutaRegiones,
                    const string& rutaCiudades,
                    const string& rutaPartidos,
                    const string& rutaCandidatosAlcaldia,
                    const string& rutaCandidatosPresidencia) {
        // si falta algún archivo, igual inicializar estructuras vacías
        GestorRegiones::cargar(rutaRegiones, regiones);
        GestorCiudades::cargar(rutaCiudades, ciudades);
        GestorPartidos::cargar(rutaPartidos, partidos);
        GestorCandidatos::cargarAlcaldia(rutaCandidatosAlcaldia, candidatosAlcaldia);
        GestorCandidatos::cargarPresidencia(rutaCandidatosPresidencia, candidatosPresidencia);

        construirIndicesBasicos();
        mElectoral.construirRegionesCiudades(ciudades);
        poblarCiudadPartidoCandidatoAlcaldia();
        return true;
    }

    bool guardar(const string& rutaRegiones,
                const string& rutaCiudades,
                const string& rutaPartidos,
                const string& rutaCandidatosAlcaldia,
                const string& rutaCandidatosPresidencia,
                const string& rutaRegionesCiudades,
                const string& rutaCiudadPartidos,
                const string& rutaCiudadPartidoCandidato) {

        bool ok = true;
        ok = ok && GestorRegiones::guardar(rutaRegiones, regiones);
        ok = ok && GestorCiudades::guardar(rutaCiudades, ciudades);
        ok = ok && GestorPartidos::guardar(rutaPartidos, partidos);
        ok = ok && GestorCandidatos::guardar(rutaCandidatosAlcaldia, candidatosAlcaldia);
        ok = ok && GestorCandidatos::guardar(rutaCandidatosPresidencia, candidatosPresidencia);
        ok = ok && mElectoral.guardarEnArchivos(rutaRegionesCiudades, rutaCiudadPartidos, rutaCiudadPartidoCandidato);
        return ok;
    }

    bool estaSimulando() const { return simulando; }
    void comenzarSimulacion() { simulando = true; }
    void finalizarSimulacion() { simulando = false; }

    const Lista<Region>& getRegiones() const { return regiones; }
    const Lista<Ciudad>& getCiudades() const { return ciudades; }
    const Lista<Partido>& getPartidos() const { return partidos; }
    const Lista<Candidato>& getCandidatosAlcaldia() const { return candidatosAlcaldia; }
    const Lista<Candidato>& getCandidatosPresidencia() const { return candidatosPresidencia; }

    MultilistaElectoral& getMultilistaElectoral() { return mElectoral; }
    const MultilistaElectoral& getMultilistaElectoral() const { return mElectoral; }

    bool obtenerIdCiudadPorNombre(const string& nombreCiudad, int& idOut) const {
        return idxCiudadPorNombre.buscar(kCiudadNombre(nombreCiudad), idOut);
    }
    bool obtenerIdPartidoPorNombre(const string& nombrePartido, int& idOut) const {
        return idxPartidoPorNombre.buscar(kPartidoNombre(nombrePartido), idOut);
    }
    bool obtenerPosCandidatoPorIdentificacion(const string& ident, int& posOut) const {
        return idxCandidatoPorIdent.buscar(kCandId(ident), posOut);
    }
    bool obtenerPresidentePorPartido(int idPartido, int& idCandidatoOut) const {
        return idxPresidentePorPartido.buscar(kPresPorPartido(idPartido), idCandidatoOut);
    }

    void reconstruirIndices() {
        idxCiudadPorNombre.limpiar();
        idxPartidoPorNombre.limpiar();
        idxCandidatoPorIdent.limpiar();
        idxPresidentePorPartido.limpiar();
        construirIndicesBasicos();
    }

    void limpiar() {
        regiones.limpiar();
        ciudades.limpiar();
        partidos.limpiar();
        candidatosAlcaldia.limpiar();
        candidatosPresidencia.limpiar();
        idxCiudadPorNombre.limpiar();
        idxPartidoPorNombre.limpiar();
        idxCandidatoPorIdent.limpiar();
        idxPresidentePorPartido.limpiar();
        mElectoral.limpiar();
        simulando = false;
    }

private:
    void construirIndicesBasicos() {
        for (int i = 0; i < ciudades.obtenerTamano(); ++i) {
            Ciudad c;
            if (ciudades.obtenerElemento(i, c)) {
                idxCiudadPorNombre.insertar(kCiudadNombre(c.getNombre()), c.getId());
            }
        }
        for (int i = 0; i < partidos.obtenerTamano(); ++i) {
            Partido p;
            if (partidos.obtenerElemento(i, p)) {
                idxPartidoPorNombre.insertar(kPartidoNombre(p.getNombre()), p.getId());
            }
        }
        for (int i = 0; i < candidatosAlcaldia.obtenerTamano(); ++i) {
            Candidato c;
            if (candidatosAlcaldia.obtenerElemento(i, c)) {
                idxCandidatoPorIdent.insertar(kCandId(c.getIdentificacion()), i);
            }
        }
        for (int j = 0; j < candidatosPresidencia.obtenerTamano(); ++j) {
            Candidato c;
            if (candidatosPresidencia.obtenerElemento(j, c)) {
                idxCandidatoPorIdent.insertar(kCandId(c.getIdentificacion()), 100000 + j);
            }
        }
        for (int i = 0; i < candidatosPresidencia.obtenerTamano(); ++i) {
            Candidato c;
            if (!candidatosPresidencia.obtenerElemento(i, c)) continue;
            int idPartido = ensurePartido(c.getPartido());
            int idCandidatoNum = 0;
            try { idCandidatoNum = stoi(c.getIdentificacion()); } catch (...) { continue; }
            idxPresidentePorPartido.insertar(kPresPorPartido(idPartido), idCandidatoNum);
        }
    }

    void poblarCiudadPartidoCandidatoAlcaldia() {
        for (int i = 0; i < candidatosAlcaldia.obtenerTamano(); ++i) {
            Candidato c;
            if (!candidatosAlcaldia.obtenerElemento(i, c)) continue;

            int idCiudad = ensureCiudad(c.getCiudadResidencia());
            int idPartido = ensurePartido(c.getPartido());

            int idCandidatoNum = 0;
            try { idCandidatoNum = stoi(c.getIdentificacion()); } catch (...) { continue; }

            mElectoral.insertarPartidoEnCiudad(idCiudad, idPartido);
            mElectoral.registrarCandidatoAlcaldia(idCiudad, idPartido, idCandidatoNum);
        }
    }
};

#endif // SISTEMA_ELECTORAL_H