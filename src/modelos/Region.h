#ifndef REGION_H
#define REGION_H

#include <iostream>
#include <string>

using namespace std;

class Region{
private:
    int id;
    string nombre;
    string descripcion;
public:
    Region(): id(0), nombre(""), descripcion(""){}
    Region(int i, const string& n, const string& d): id(i), nombre(n), descripcion(d){}
    
    //getters
    int getId() const{
        return id;
    }
    const string& getNombre() const{
        return nombre;
    }

    const string& getDescripcion() const{
        return descripcion;
    }

    //setters
    void setId(int i){
        id = i;
    }

    void setNombre(const string& n){
        nombre = n;
    }

    void setDescripcion(const string& d){
        descripcion = d;
    }

    string toString() const{
        return to_string(id) +"|"+nombre+"|"+descripcion;
    }

    void mostrar() const{
        cout << "Region { id=" << id
        << ", nombre=\"" << nombre
        << "\", descripcion=\"" << descripcion
        << "\" }" << endl;
    }
};
#endif //REGION_H