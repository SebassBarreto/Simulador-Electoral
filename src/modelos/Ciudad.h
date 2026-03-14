#ifndef CIUDAD_H
#define CIUDAD_H

#include <iostream>
#include <string>

using namespace std;

class Ciudad{
private:
    int id;
    string nombre;
    int idRegion;
    int poblacion;
    bool esCapital;
public:
    Ciudad(): id(0), nombre(""), idRegion(0), poblacion(0), esCapital(false){}
    Ciudad(int i, const string& n, int r, int p, bool cap = false): id(i), nombre(n), idRegion(r), poblacion(p), esCapital(cap){}

    //getters
    int getId() const{
        return id;
    }
    
    const string getNombre() const{
        return nombre;
    }

    int getIdRegion() const{
        return idRegion;
    }

    int getPoblacion() const{
        return poblacion;
    }

    bool getEsCapital() const{
        return esCapital;
    }

    void setId(int i){
        id = i;
    }

    void setNombre(const string& n){
        nombre = n;
    }

    void setIdRegion(int r){
        idRegion = r;
    }

    void setPoblacion(int p){
        poblacion = p;
    }

    void setEsCapital(bool cap){
        esCapital = cap;
    }

    string toString() const{
        return to_string(id) + "|"+nombre+"|"+to_string(idRegion)+"|"+to_string(poblacion);
    }

    void mostrar() const {
        cout << "Ciudad { id=" << id
            << ", nombre=\"" << nombre
            << "\", idRegion=" << idRegion
            << ", poblacion=" << poblacion
            << ", esCapital=" << (esCapital ? "Si" : "No")
            << " }" << endl;
    }
};
#endif //CIUDAD_H