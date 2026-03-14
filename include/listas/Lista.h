#ifndef LISTA_H
#define LISTA_H

#include <iostream>
#include "../nodos/Nodo.h" //no olvidar que ".." hace referencia al directorio actual xd

template <typename T>
class Lista {
private:
    Nodo<T>* cabeza;
    Nodo<T>* cola;
    int tamano;

    //obtiene nodo en una posicion especificada retorna el puntero a este
    Nodo<T>* obtenerNodo(int posicion) const;

public:
    
    Lista();

    
    ~Lista();

    
    void insertarInicio(const T& dato);

    
    void insertarFinal(const T& dato);

   
    bool insertarEnPosicion(const T& dato, int posicion);

    
    bool eliminarInicio();

   
    bool eliminarFinal();

   
    bool eliminarEnPosicion(int posicion);

    
    bool eliminarPorDato(const T& dato);


    void limpiar();

    
    int buscar(const T& dato) const;

   
    bool existe(const T& dato) const;

   
    bool obtenerElemento(int posicion, T& resultado) const;

    
    bool obtenerPrimero(T& resultado) const;

    
    bool obtenerUltimo(T& resultado) const;

    //utilidades
    int obtenerTamano() const;

    
    bool estaVacia() const;

    
    void imprimir() const;

   
    void invertir();

   
    int contar(const T& dato) const;
};

//implementacion

template <typename T>
Lista<T>::Lista() : cabeza(NULL), cola(NULL), tamano(0) {
}

template <typename T>
Lista<T>::~Lista() {
    limpiar();
}

//metodo privado
template <typename T>
Nodo<T>* Lista<T>::obtenerNodo(int posicion) const {
    if (posicion < 0 || posicion >= tamano) {
        return NULL;
    }
    
    Nodo<T>* actual = cabeza;
    for (int i = 0; i < posicion; i++) {
        actual = actual->getSiguiente();
    }
    return actual;
}

//insercion

template <typename T>
void Lista<T>::insertarInicio(const T& dato) {
    Nodo<T>* nuevo = new Nodo<T>(dato);
    
    if (estaVacia()) {
        cabeza = nuevo;
        cola = nuevo;
    } else {
        nuevo->setSiguiente(cabeza);
        cabeza = nuevo;
    }
    
    tamano++;
}

template <typename T>
void Lista<T>::insertarFinal(const T& dato) {
    Nodo<T>* nuevo = new Nodo<T>(dato);
    
    if (estaVacia()) {
        cabeza = nuevo;
        cola = nuevo;
    } else {
        cola->setSiguiente(nuevo);
        cola = nuevo;
    }
    
    tamano++;
}

template <typename T>
bool Lista<T>::insertarEnPosicion(const T& dato, int posicion) {
    if (posicion < 0 || posicion > tamano) {
        return false;
    }
    
    if (posicion == 0) {
        insertarInicio(dato);
        return true;
    }
    
    if (posicion == tamano) {
        insertarFinal(dato);
        return true;
    }
    
    Nodo<T>* nuevo = new Nodo<T>(dato);
    Nodo<T>* anterior = obtenerNodo(posicion - 1);
    
    nuevo->setSiguiente(anterior->getSiguiente());
    anterior->setSiguiente(nuevo);
    
    tamano++;
    return true;
}

//eliminacion

template <typename T>
bool Lista<T>::eliminarInicio() {
    if (estaVacia()) {
        return false;
    }
    
    Nodo<T>* temp = cabeza;
    cabeza = cabeza->getSiguiente();
    delete temp;
    
    tamano--;
    
    if (estaVacia()) {
        cola = NULL;
    }
    
    return true;
}

template <typename T>
bool Lista<T>::eliminarFinal() {
    if (estaVacia()) {
        return false;
    }
    
    if (tamano == 1) {
        delete cabeza;
        cabeza = NULL;
        cola = NULL;
        tamano = 0;
        return true;
    }
    
    Nodo<T>* anterior = obtenerNodo(tamano - 2);
    delete cola;
    cola = anterior;
    cola->setSiguiente(NULL);
    
    tamano--;
    return true;
}

template <typename T>
bool Lista<T>::eliminarEnPosicion(int posicion) {
    if (posicion < 0 || posicion >= tamano) {
        return false;
    }
    
    if (posicion == 0) {
        return eliminarInicio();
    }
    
    if (posicion == tamano - 1) {
        return eliminarFinal();
    }
    
    Nodo<T>* anterior = obtenerNodo(posicion - 1);
    Nodo<T>* eliminar = anterior->getSiguiente();
    anterior->setSiguiente(eliminar->getSiguiente());
    delete eliminar;
    
    tamano--;
    return true;
}

template <typename T>
bool Lista<T>::eliminarPorDato(const T& dato) {
    int posicion = buscar(dato);
    if (posicion == -1) {
        return false;
    }
    return eliminarEnPosicion(posicion);
}

template <typename T>
void Lista<T>::limpiar() {
    while (!estaVacia()) {
        eliminarInicio();
    }
}

//busqueda

template <typename T>
int Lista<T>::buscar(const T& dato) const {
    Nodo<T>* actual = cabeza;
    int posicion = 0;
    
    while (actual != NULL) {
        if (actual->getDato() == dato) {
            return posicion;
        }
        actual = actual->getSiguiente();
        posicion++;
    }
    
    return -1;
}

template <typename T>
bool Lista<T>::existe(const T& dato) const {
    return buscar(dato) != -1;
}

template <typename T>
bool Lista<T>::obtenerElemento(int posicion, T& resultado) const {
    Nodo<T>* nodo = obtenerNodo(posicion);
    if (nodo == NULL) {
        return false;
    }
    resultado = nodo->getDato();
    return true;
}

//acceso

template <typename T>
bool Lista<T>::obtenerPrimero(T& resultado) const {
    if (estaVacia()) {
        return false;
    }
    resultado = cabeza->getDato();
    return true;
}

template <typename T>
bool Lista<T>::obtenerUltimo(T& resultado) const {
    if (estaVacia()) {
        return false;
    }
    resultado = cola->getDato();
    return true;
}

//utilidades
template <typename T>
int Lista<T>::obtenerTamano() const {
    return tamano;
}

template <typename T>
bool Lista<T>::estaVacia() const {
    return cabeza == NULL;
}

template <typename T>
void Lista<T>::imprimir() const {
    if (estaVacia()) {
        std::cout << "Lista vacia" << std::endl;
        return;
    }
    
    Nodo<T>* actual = cabeza;
    std::cout << "[ ";
    while (actual != NULL) {
        actual->imprimir();
        if (actual->getSiguiente() != NULL) {
            std::cout << " -> ";
        }
        actual = actual->getSiguiente();
    }
    std::cout << " ]" << std::endl;
}

template <typename T>
void Lista<T>::invertir() {
    if (tamano <= 1) {
        return;
    }
    
    Nodo<T>* anterior = NULL;
    Nodo<T>* actual = cabeza;
    Nodo<T>* siguiente = NULL;
    
    cola = cabeza;
    
    while (actual != NULL) {
        siguiente = actual->getSiguiente();
        actual->setSiguiente(anterior);
        anterior = actual;
        actual = siguiente;
    }
    
    cabeza = anterior;
}

template <typename T>
int Lista<T>::contar(const T& dato) const {
    int contador = 0;
    Nodo<T>* actual = cabeza;
    
    while (actual != NULL) {
        if (actual->getDato() == dato) {
            contador++;
        }
        actual = actual->getSiguiente();
    }
    
    return contador;
}

#endif //LISTA_H
