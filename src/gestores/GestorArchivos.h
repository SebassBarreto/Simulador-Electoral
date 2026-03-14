#ifndef GESTORARCHIVOS_H
#define GESTORARCHIVOS_H

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class GestorArchivos {
public:
    static bool leerLineas(const string& ruta, string lineas[], int capacidad, int& cantidadLeida) {
        cantidadLeida = 0;
        ifstream in(ruta.c_str());
        if (!in.is_open()) return false;

        string linea;
        while (getline(in, linea)) {
            if (!linea.empty() && linea.back() == '\r') linea.pop_back();
            if (linea.size() == 0) continue;
            if (cantidadLeida >= capacidad) {
                in.close();
                return true;
            }
            lineas[cantidadLeida++] = linea;
        }
        in.close();
        return true;
    }

    static bool escribirLineas(const string& ruta, const string lineas[], int cantidad) {
        ofstream out(ruta.c_str(), ios::out | ios::trunc);
        if (!out.is_open()) return false;
        for (int i = 0; i < cantidad; ++i) {
            out << lineas[i] << "\n";
        }
        out.close();
        return true;
    }

    static bool appendLinea(const string& ruta, const string& linea) {
        ofstream out(ruta.c_str(), ios::out | ios::app);
        if (!out.is_open()) return false;
        out << linea << "\n";
        out.close();
        return true;
    }

    static bool leerArchivoCompleto(const string& ruta, string& contenido) {
        contenido.clear();
        ifstream in(ruta.c_str(), ios::in | ios::binary);
        if (!in.is_open()) return false;
        in.seekg(0, ios::end);
        streampos size = in.tellg();
        in.seekg(0, ios::beg);
        if (size > 0) {
            contenido.resize(static_cast<size_t>(size));
            in.read(&contenido[0], size);
            if (!contenido.empty() && contenido.back() == '\r') contenido.pop_back();
        }
        in.close();
        return true;
    }

    static bool escribirArchivoCompleto(const string& ruta, const string& contenido) {
        ofstream out(ruta.c_str(), ios::out | ios::trunc | ios::binary);
        if (!out.is_open()) return false;
        out.write(contenido.c_str(), static_cast<streamsize>(contenido.size()));
        out.close();
        return true;
    }
};

#endif // GESTORARCHIVOS_H