#ifndef PARTIDO_H
#define PARTIDO_H

#include <string>
#include <sstream>
using namespace std;

class Partido {
private:
    string nombre;
    string representanteLegal;
    int idPartido;

public:
    // Constructor
    Partido():
    nombre(""), representanteLegal(""), idPartido(0) {}
    
    Partido(string n, string rl, int id):
    nombre(n), representanteLegal(rl), idPartido(id) {}

    // Getters
    string getNombre() const { return nombre; }
    string getRepresentanteLegal() const { return representanteLegal; }
    int getId() const { return idPartido; }

    // Setters
    void setNombre(string n) { nombre = n; }
    void setRepresentanteLegal(string rl) { representanteLegal = rl; }

    // toString
    string toString() const {
        stringstream ss;
        ss << nombre << " Representante: " << representanteLegal;
        return ss.str();
    }

    // toFileFormat
    string toFileFormat() const {
        stringstream ss;
        ss << nombre << "|" << representanteLegal << "|" << idPartido;
        return ss.str();
    }
};

#endif //PARTIDO_H
