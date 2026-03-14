#ifndef INDICEAVL_H
#define INDICEAVL_H

#include <iostream>
#include <string>
#include <unordered_map>
#include "../../include/arboles/ArbolAVL.h"
#include "../../include/listas/Lista.h"
#include "../gestores/GestorArchivos.h"

using namespace std;

struct IndiceEntry {
    string clave; // ej: "Candidato:100101" o "Ciudad:3"
    int referencia;    // por ejemplo posicion en Lista<>, o id numerico
    IndiceEntry() : clave(""), referencia(-1) {}
    IndiceEntry(const string& k, int r) : clave(k), referencia(r) {}
    bool operator<(const IndiceEntry& o) const { return clave < o.clave; }
    bool operator>(const IndiceEntry& o) const { return clave > o.clave; }
    bool operator==(const IndiceEntry& o) const { return clave == o.clave; }
    string toFileLine() const { return clave + "|" + to_string(referencia); }
    static bool fromFileLine(const string& line, IndiceEntry& out) {
        size_t p = line.find('|');
        if (p == string::npos) return false;
        out.clave = line.substr(0, p);
        try { out.referencia = stoi(line.substr(p + 1)); }
        catch (...) { return false; }
        return true;
    }
};

class IndiceAVL {
private:
    ArbolAVL<IndiceEntry> arbol;
    Lista<IndiceEntry> registros; // auxiliar para recuperar referencia y persistir
    unordered_map<string, int> cacheRapido; // O(1) lookup optimization
public:
    IndiceAVL() {}
    ~IndiceAVL() { limpiar(); }

    bool insertar(const string& clave, int referencia) {
        IndiceEntry e(clave, referencia);
        
        // Update cache
        cacheRapido[clave] = referencia;
        
        if (registros.existe(e)) {
            // actualizar referencia en la lista (opcional)
            // buscar y actualizar
            int pos = registros.buscar(e);
            if (pos != -1) {
                IndiceEntry tmp;
                registros.obtenerElemento(pos, tmp);
                tmp.referencia = referencia;
                registros.eliminarEnPosicion(pos);
                registros.insertarEnPosicion(tmp, pos);
            }
            arbol.insertar(e); //asegurar sincronia del arbol tambien en actualizacion
            return true;
        }
        arbol.insertar(e);
        registros.insertarFinal(e);
        return true;
    }

    bool buscar(const string& clave, int& referenciaOut) const {
        // O(1) lookup using unordered_map cache (always in sync with AVL)
        auto it = cacheRapido.find(clave);
        if (it != cacheRapido.end()) {
            referenciaOut = it->second;
            return true;
        }
        return false; // Not found in cache means not in index
    }

    bool eliminar(const string& clave) {
        IndiceEntry key(clave, 0);
        bool ok = arbol.eliminar(key); //capturar estado de eliminacion en arbol
        
        // Remove from cache
        cacheRapido.erase(clave);
        
        // eliminar de registros
        bool foundList = false;
        IndiceEntry tmp;
        int tam = registros.obtenerTamano();
        for (int i = 0; i < tam; ++i) {
            if (!registros.obtenerElemento(i, tmp)) continue;
            if (tmp.clave == clave) {
                registros.eliminarEnPosicion(i);
                foundList = true;
                break;
            }
        }
        return ok && foundList;
    }

    bool guardarEnArchivo(const string& ruta) {
        int tam = registros.obtenerTamano();
        if (tam == 0) {
            return GestorArchivos::escribirArchivoCompleto(ruta, string(""));
        }
        string out[4096];
        int k = 0;
        IndiceEntry tmp;
        for (int i = 0; i < tam; ++i) {
            registros.obtenerElemento(i, tmp);
            out[k++] = tmp.toFileLine();
        }
        return GestorArchivos::escribirLineas(ruta, out, k);
    }

    bool cargarDesdeArchivo(const string& ruta) {
        registros.limpiar();
        arbol.limpiar();
        cacheRapido.clear();
        const int CAP = 8192;
        string lines[CAP];
        int n = 0;
        if (!GestorArchivos::leerLineas(ruta, lines, CAP, n)) return false;
        for (int i = 0; i < n; ++i) {
            IndiceEntry e;
            if (!IndiceEntry::fromFileLine(lines[i], e)) continue;
            arbol.insertar(e);
            registros.insertarFinal(e);
            cacheRapido[e.clave] = e.referencia; // Populate cache
        }
        return true;
    }

    void limpiar() {
        registros.limpiar();
        arbol.limpiar();
        cacheRapido.clear();
    }
};


#endif // INDICEAVL_H