#ifndef GESTORCIUDADES_H
#define GESTORCIUDADES_H

#include <iostream>
#include <string>
#include "../../include/listas/Lista.h"
#include "../modelos/Ciudad.h"
#include "GestorArchivos.h"
#include "../utilidades/Validadores.h"

using namespace std;

class GestorCiudades {
public:
    static bool cargar(const string& ruta, Lista<Ciudad>& lista) {
        lista.limpiar();
        const int CAP = 2048;
        string lineas[CAP];
        int n = 0;
        if (!GestorArchivos::leerLineas(ruta, lineas, CAP, n)) return false;

        int nextId = 1;
        for (int i = 0; i < n; ++i) {
            const string& s = lineas[i];
            int p1 = (int)s.find('|');
            if (p1 == -1) continue;
            int p2 = (int)s.find('|', p1 + 1);
            int p3 = (p2 == -1 ? -1 : (int)s.find('|', p2 + 1));

            if (p1 != -1 && p2 != -1 && p3 != -1 && Validadores::esNumero(s.substr(0, p1))) {
                // Formato: id|nombre|idRegion|poblacion[|esCapital]
                int id = 0, idRegion = 0, poblacion = 0;
                string nombre = s.substr(p1 + 1, p2 - p1 - 1);
                try {
                    id        = stoi(s.substr(0, p1));
                    idRegion  = stoi(s.substr(p2 + 1, p3 - p2 - 1));
                    poblacion = stoi(s.substr(p3 + 1));
                } catch (...) { continue; }
                bool esCapital = true;
                int p4 = (int)s.find('|', p3 + 1);
                if (p4 != -1) {
                    string capStr = s.substr(p4 + 1);
                    esCapital = (capStr == "1" || capStr == "true" || capStr == "S");
                }
                if (id >= nextId) nextId = id + 1;
                lista.insertarFinal(Ciudad(id, nombre, idRegion, poblacion, esCapital));
            } else if (p1 != -1 && p2 != -1) {
                // Formato: nombre|regionNombre|poblacion[|esCapital]
                string nombre = s.substr(0, p1);
                string regionNombre = s.substr(p1 + 1, p2 - p1 - 1);
                int poblacion = 0;
                try { poblacion = stoi(s.substr(p2 + 1, (p3 == -1 ? s.size() - (p2 + 1) : p3 - p2 - 1))); }
                catch (...) { poblacion = 0; }
                bool esCapital = true;
                if (p3 != -1) {
                    string capStr = s.substr(p3 + 1);
                    esCapital = (capStr == "1" || capStr == "true" || capStr == "S");
                }
                // idRegion desconocido aquí; se pondrá 0 y se ajustará cuando se construyan relaciones
                lista.insertarFinal(Ciudad(nextId++, nombre, 0, poblacion, esCapital));
            }
        }
        return true;
    }

    static bool guardar(const string& ruta, const Lista<Ciudad>& lista) {
        int tam = lista.obtenerTamano();
        if (tam <= 0) {
            const string vacio = "";
            return GestorArchivos::escribirArchivoCompleto(ruta, vacio);
        }
        string out[4096];
        int k = 0;
        Ciudad c;
        for (int i = 0; i < tam; ++i) {
            if (!lista.obtenerElemento(i, c)) continue;
            out[k++] = c.toString();
        }
        return GestorArchivos::escribirLineas(ruta, out, k);
    }
};

#endif // GESTORCIUDADES_H