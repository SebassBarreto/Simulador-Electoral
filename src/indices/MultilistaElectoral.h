#ifndef MULTILISTAELECTORAL_H
#define MULTILISTAELECTORAL_H

#include <iostream>
#include <string>
#include "../../include/listas/Multilista.h"
#include "../../include/listas/Lista.h"
#include "../gestores/GestorArchivos.h"
#include "../modelos/Ciudad.h"
#include "IndiceAVL.h"

using namespace std;

class MultilistaElectoral {
private:
    Multilista<int,int,int> regionesCiudades;   // nivel1=region, nivel2=ciudad, nivel3 no usado
    Multilista<int,int,int> ciudadPartidos;     // nivel1=ciudad, nivel2=partido, nivel3 no usado
    IndiceAVL mapCiudadPartidoCandidato;        // "CP:<ciudad>:<partido>" -> candidatoId

    Lista<int> regionesRegistradas;
    Lista<int> ciudadesRegistradas;             // ciudades con region
    Lista<int> ciudadesConPartidos;             // ciudades con partidos registrados

    static string keyCP(int idCiudad, int idPartido) {
        return string("CP:") + to_string(idCiudad) + ":" + to_string(idPartido);
    }

    static void joinEnterosConComa(const Lista<int>& lst, string& out) {
        out.clear();
        int n = lst.obtenerTamano();
        for (int i = 0; i < n; ++i) {
            int v;
            if (!lst.obtenerElemento(i, v)) continue;
            if (!out.empty()) out += ",";
            out += to_string(v);
        }
    }

    static void splitComasEnEnteros(const string& s, Lista<int>& out) {
        out.limpiar();
        int start = 0;
        for (int i = 0; i <= (int)s.size(); ++i) {
            if (i == (int)s.size() || s[i] == ',') {
                string token = s.substr(start, i - start);
                if (!token.empty()) {
                    try { out.insertarFinal(stoi(token)); } catch (...) {}
                }
                start = i + 1;
            }
        }
    }

    static bool listaContiene(const Lista<int>& lst, int valor) {
        return lst.buscar(valor) != -1;
    }

public:
    MultilistaElectoral() {}
    ~MultilistaElectoral() { limpiar(); }

    // regiones -> ciudades
    void insertarRegion(int idRegion) {
        if (!regionesCiudades.existeNivel1(idRegion)) {
            regionesCiudades.insertarNivel1(idRegion);
        }
        if (!listaContiene(regionesRegistradas, idRegion)) {
            regionesRegistradas.insertarFinal(idRegion);
        }
    }

    bool existeRegion(int idRegion) const {
        return regionesCiudades.existeNivel1(idRegion);
    }

    bool insertarCiudadEnRegion(int idRegion, int idCiudad) {
        insertarRegion(idRegion);
        bool ok = regionesCiudades.insertarNivel2(idRegion, idCiudad);
        if (ok && !listaContiene(ciudadesRegistradas, idCiudad)) {
            ciudadesRegistradas.insertarFinal(idCiudad);
        }
        return ok;
    }

    Lista<int>* obtenerCiudadesDeRegion(int idRegion) {
        return regionesCiudades.obtenerListaNivel2(idRegion);
    }

    int contarCiudades(int idRegion) const {
        return regionesCiudades.contarNivel2(idRegion);
    }

    // construir regiones->ciudades desde lista de ciudades
    void construirRegionesCiudades(const Lista<Ciudad>& ciudades) {
        int n = ciudades.obtenerTamano();
        for (int i = 0; i < n; ++i) {
            Ciudad c;
            if (!ciudades.obtenerElemento(i, c)) continue;
            insertarCiudadEnRegion(c.getIdRegion(), c.getId());
        }
    }

    // ciudad -> partidos
    bool insertarPartidoEnCiudad(int idCiudad, int idPartido) {
        if (!ciudadPartidos.existeNivel1(idCiudad)) {
            ciudadPartidos.insertarNivel1(idCiudad);
        }
        bool ok = ciudadPartidos.insertarNivel2(idCiudad, idPartido);
        if (ok && !listaContiene(ciudadesConPartidos, idCiudad)) {
            ciudadesConPartidos.insertarFinal(idCiudad);
        }
        return ok;
    }

    Lista<int>* obtenerPartidosDeCiudad(int idCiudad) {
        return ciudadPartidos.obtenerListaNivel2(idCiudad);
    }

    bool existePartidoEnCiudad(int idCiudad, int idPartido) const {
        return ciudadPartidos.existeNivel2(idCiudad, idPartido);
    }

    // mapping ciudad+partido -> candidato (unico por ciudad y partido)
    // si ya existe, actualiza el candidato
    bool registrarCandidatoAlcaldia(int idCiudad, int idPartido, int idCandidato) {
        insertarPartidoEnCiudad(idCiudad, idPartido);
        return mapCiudadPartidoCandidato.insertar(keyCP(idCiudad, idPartido), idCandidato);
    }

    bool obtenerCandidatoAlcaldia(int idCiudad, int idPartido, int& idCandidatoOut) const {
        return mapCiudadPartidoCandidato.buscar(keyCP(idCiudad, idPartido), idCandidatoOut);
    }

    // helpers de listado para iterar nivel 1 sin exponer internals
    void listarRegiones(Lista<int>& out) const {
        out.limpiar();
        int n = regionesRegistradas.obtenerTamano();
        for (int i = 0; i < n; ++i) {
            int v;
            if (regionesRegistradas.obtenerElemento(i, v)) out.insertarFinal(v);
        }
    }

    void listarCiudadesConPartidos(Lista<int>& out) const {
        out.limpiar();
        int n = ciudadesConPartidos.obtenerTamano();
        for (int i = 0; i < n; ++i) {
            int v;
            if (ciudadesConPartidos.obtenerElemento(i, v)) out.insertarFinal(v);
        }
    }

    void limpiar() {
        regionesCiudades.limpiar();
        ciudadPartidos.limpiar();
        mapCiudadPartidoCandidato.limpiar();
        regionesRegistradas.limpiar();
        ciudadesRegistradas.limpiar();
        ciudadesConPartidos.limpiar();
    }

    // persistencia simple:
    // - regiones_ciudades.txt: regionId|ciudadId1,ciudadId2,...
    // - ciudad_partidos.txt:   ciudadId|partidoId1,partidoId2,...
    // - ciudad_partido_candidato.txt: ciudadId|partidoId|candidatoId
    bool guardarEnArchivos(const string& rutaRegionesCiudades,
                            const string& rutaCiudadPartidos,
                            const string& rutaCPC) {
        // guardar regiones -> ciudades
        {
            const int CAP = 4096;
            string lineas[CAP];
            int k = 0;

            int nr = regionesRegistradas.obtenerTamano();
            for (int i = 0; i < nr; ++i) {
                int idRegion;
                if (!regionesRegistradas.obtenerElemento(i, idRegion)) continue;

                Lista<int>* listaCiudades = regionesCiudades.obtenerListaNivel2(idRegion);
                if (listaCiudades == NULL) continue;

                string ciudadesCSV;
                joinEnterosConComa(*listaCiudades, ciudadesCSV);
                lineas[k++] = to_string(idRegion) + "|" + ciudadesCSV;
                if (k >= CAP) break;
            }
            if (!GestorArchivos::escribirLineas(rutaRegionesCiudades, lineas, k)) return false;
        }

        // guardar ciudad -> partidos
        {
            const int CAP = 4096;
            string lineas[CAP];
            int k = 0;

            int nc = ciudadesConPartidos.obtenerTamano();
            for (int i = 0; i < nc; ++i) {
                int idCiudad;
                if (!ciudadesConPartidos.obtenerElemento(i, idCiudad)) continue;

                Lista<int>* listaPartidos = ciudadPartidos.obtenerListaNivel2(idCiudad);
                if (listaPartidos == NULL) continue;

                string partidosCSV;
                joinEnterosConComa(*listaPartidos, partidosCSV);
                lineas[k++] = to_string(idCiudad) + "|" + partidosCSV;
                if (k >= CAP) break;
            }
            if (!GestorArchivos::escribirLineas(rutaCiudadPartidos, lineas, k)) return false;
        }

        // guardar ciudad-partido-candidato
        {
            const int CAP = 8192;
            string lineas[CAP];
            int k = 0;

            int nc = ciudadesConPartidos.obtenerTamano();
            for (int i = 0; i < nc; ++i) {
                int idCiudad;
                if (!ciudadesConPartidos.obtenerElemento(i, idCiudad)) continue;

                Lista<int>* listaPartidos = ciudadPartidos.obtenerListaNivel2(idCiudad);
                if (listaPartidos == NULL) continue;

                int np = listaPartidos->obtenerTamano();
                for (int j = 0; j < np; ++j) {
                    int idPartido;
                    if (!listaPartidos->obtenerElemento(j, idPartido)) continue;

                    int idCandidato = -1;
                    if (mapCiudadPartidoCandidato.buscar(keyCP(idCiudad, idPartido), idCandidato)) {
                        lineas[k++] = to_string(idCiudad) + "|" + to_string(idPartido) + "|" + to_string(idCandidato);
                        if (k >= CAP) break;
                    }
                }
                if (k >= CAP) break;
            }
            if (!GestorArchivos::escribirLineas(rutaCPC, lineas, k)) return false;
        }

        return true;
    }

    bool cargarDesdeArchivos(const string& rutaRegionesCiudades,
                            const string& rutaCiudadPartidos,
                            const string& rutaCPC) {
        limpiar();

        // cargar regiones -> ciudades
        {
            const int CAP = 4096;
            string lineas[CAP];
            int n = 0;
            if (!GestorArchivos::leerLineas(rutaRegionesCiudades, lineas, CAP, n)) return false;

            for (int i = 0; i < n; ++i) {
                const string& s = lineas[i];
                int p = (int)s.find('|');
                if (p == -1) continue;
                int idRegion = 0;
                try { idRegion = stoi(s.substr(0, p)); } catch (...) { continue; }
                insertarRegion(idRegion);

                Lista<int> tmpCiudades;
                splitComasEnEnteros(s.substr(p + 1), tmpCiudades);
                int m = tmpCiudades.obtenerTamano();
                for (int j = 0; j < m; ++j) {
                    int idCiudad;
                    if (tmpCiudades.obtenerElemento(j, idCiudad)) {
                        insertarCiudadEnRegion(idRegion, idCiudad);
                    }
                }
            }
        }

        // cargar ciudad -> partidos
        {
            const int CAP = 4096;
            string lineas[CAP];
            int n = 0;
            if (!GestorArchivos::leerLineas(rutaCiudadPartidos, lineas, CAP, n)) return false;

            for (int i = 0; i < n; ++i) {
                const string& s = lineas[i];
                int p = (int)s.find('|');
                if (p == -1) continue;
                int idCiudad = 0;
                try { idCiudad = stoi(s.substr(0, p)); } catch (...) { continue; }

                Lista<int> tmpPartidos;
                splitComasEnEnteros(s.substr(p + 1), tmpPartidos);
                int m = tmpPartidos.obtenerTamano();
                for (int j = 0; j < m; ++j) {
                    int idPartido;
                    if (tmpPartidos.obtenerElemento(j, idPartido)) {
                        insertarPartidoEnCiudad(idCiudad, idPartido);
                    }
                }
            }
        }

        // cargar ciudad-partido-candidato
        {
            const int CAP = 8192;
            string lineas[CAP];
            int n = 0;
            if (!GestorArchivos::leerLineas(rutaCPC, lineas, CAP, n)) return false;

            for (int i = 0; i < n; ++i) {
                const string& s = lineas[i];
                int p1 = (int)s.find('|');
                int p2 = (p1 == -1 ? -1 : (int)s.find('|', p1 + 1));
                if (p1 == -1 || p2 == -1) continue;

                int idCiudad = 0, idPartido = 0, idCandidato = -1;
                try {
                    idCiudad = stoi(s.substr(0, p1));
                    idPartido = stoi(s.substr(p1 + 1, p2 - p1 - 1));
                    idCandidato = stoi(s.substr(p2 + 1));
                } catch (...) { continue; }

                registrarCandidatoAlcaldia(idCiudad, idPartido, idCandidato);
            }
        }

        return true;
    }
};

#endif // MULTILISTAELECTORAL_H