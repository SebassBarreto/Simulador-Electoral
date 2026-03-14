#ifndef LISTADOBLE_H
#define LISTADOBLE_H

#include <iostream>
#include "../nodos/NodoDoble.h"

template <typename T>
class ListaDoble {
private:
    NodoDoble<T>* cabeza;
    NodoDoble<T>* cola;
    int tamano;

    
    NodoDoble<T>* obtenerNodo(int posicion) const;

public:
   
    ListaDoble();

    
    ~ListaDoble();

    
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

   
    int obtenerTamano() const;

    
    bool estaVacia() const;

    
    void imprimir() const;

   
    void imprimirReversa() const;

    
    void invertir();

   
    int contar(const T& dato) const;//nro de veces que sale en la lista
};

//implementacions

template <typename T>
ListaDoble<T>::ListaDoble() : cabeza(NULL), cola(NULL), tamano(0) {
}

template <typename T>
ListaDoble<T>::~ListaDoble() {
    limpiar();
}

//metodo privado ojo

template <typename T>
NodoDoble<T>* ListaDoble<T>::obtenerNodo(int posicion) const {
    if (posicion < 0 || posicion >= tamano) {
        return NULL;
    }
    
    //si la posicion esta en la segunda mitad, recorrer desde el final
    if (posicion < tamano / 2) {
        //recorrer desde el inicio
        NodoDoble<T>* actual = cabeza;
        for (int i = 0; i < posicion; i++) {
            actual = actual->getSiguiente();
        }
        return actual;
    } else {
        //recorrer desde el final
        NodoDoble<T>* actual = cola;
        for (int i = tamano - 1; i > posicion; i--) {
            actual = actual->getAnterior();
        }
        return actual;
    }
}

//insercion
template <typename T>
void ListaDoble<T>::insertarInicio(const T& dato) {
    NodoDoble<T>* nuevo = new NodoDoble<T>(dato);
    
    if (estaVacia()) {
        cabeza = nuevo;
        cola = nuevo;
    } else {
        nuevo->setSiguiente(cabeza);
        cabeza->setAnterior(nuevo);
        cabeza = nuevo;
    }
    
    tamano++;
}

template <typename T>
void ListaDoble<T>::insertarFinal(const T& dato) {
    NodoDoble<T>* nuevo = new NodoDoble<T>(dato);
    
    if (estaVacia()) {
        cabeza = nuevo;
        cola = nuevo;
    } else {
        nuevo->setAnterior(cola);
        cola->setSiguiente(nuevo);
        cola = nuevo;
    }
    
    tamano++;
}

template <typename T>
bool ListaDoble<T>::insertarEnPosicion(const T& dato, int posicion) {
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
    
    NodoDoble<T>* nuevo = new NodoDoble<T>(dato);
    NodoDoble<T>* actual = obtenerNodo(posicion);
    NodoDoble<T>* anterior = actual->getAnterior();
    
    nuevo->setSiguiente(actual);
    nuevo->setAnterior(anterior);
    anterior->setSiguiente(nuevo);
    actual->setAnterior(nuevo);
    
    tamano++;
    return true;
}

//eliminacion

template <typename T>
bool ListaDoble<T>::eliminarInicio() {
    if (estaVacia()) {
        return false;
    }
    
    NodoDoble<T>* temp = cabeza;
    
    if (tamano == 1) {
        cabeza = NULL;
        cola = NULL;
    } else {
        cabeza = cabeza->getSiguiente();
        cabeza->setAnterior(NULL);
    }
    
    delete temp;
    tamano--;
    return true;
}

template <typename T>
bool ListaDoble<T>::eliminarFinal() {
    if (estaVacia()) {
        return false;
    }
    
    NodoDoble<T>* temp = cola;
    
    if (tamano == 1) {
        cabeza = NULL;
        cola = NULL;
    } else {
        cola = cola->getAnterior();
        cola->setSiguiente(NULL);
    }
    
    delete temp;
    tamano--;
    return true;
}

template <typename T>
bool ListaDoble<T>::eliminarEnPosicion(int posicion) {
    if (posicion < 0 || posicion >= tamano) {
        return false;
    }
    
    if (posicion == 0) {
        return eliminarInicio();
    }
    
    if (posicion == tamano - 1) {
        return eliminarFinal();
    }
    
    NodoDoble<T>* eliminar = obtenerNodo(posicion);
    NodoDoble<T>* anterior = eliminar->getAnterior();
    NodoDoble<T>* siguiente = eliminar->getSiguiente();
    
    anterior->setSiguiente(siguiente);
    siguiente->setAnterior(anterior);
    
    delete eliminar;
    tamano--;
    return true;
}

template <typename T>
bool ListaDoble<T>::eliminarPorDato(const T& dato) {
    int posicion = buscar(dato);
    if (posicion == -1) {
        return false;
    }
    return eliminarEnPosicion(posicion);
}

template <typename T>
void ListaDoble<T>::limpiar() {
    while (!estaVacia()) {
        eliminarInicio();
    }
}

//busqueda

template <typename T>
int ListaDoble<T>::buscar(const T& dato) const {
    NodoDoble<T>* actual = cabeza;
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
bool ListaDoble<T>::existe(const T& dato) const {
    return buscar(dato) != -1;
}

template <typename T>
bool ListaDoble<T>::obtenerElemento(int posicion, T& resultado) const {
    NodoDoble<T>* nodo = obtenerNodo(posicion);
    if (nodo == NULL) {
        return false;
    }
    resultado = nodo->getDato();
    return true;
}

// acceso

template <typename T>
bool ListaDoble<T>::obtenerPrimero(T& resultado) const {
    if (estaVacia()) {
        return false;
    }
    resultado = cabeza->getDato();
    return true;
}

template <typename T>
bool ListaDoble<T>::obtenerUltimo(T& resultado) const {
    if (estaVacia()) {
        return false;
    }
    resultado = cola->getDato();
    return true;
}

//utilidades
template <typename T>
int ListaDoble<T>::obtenerTamano() const {
    return tamano;
}

template <typename T>
bool ListaDoble<T>::estaVacia() const {
    return cabeza == NULL;
}

template <typename T>
void ListaDoble<T>::imprimir() const {
    if (estaVacia()) {
        std::cout << "Lista vacia" << std::endl;
        return;
    }
    
    NodoDoble<T>* actual = cabeza;
    std::cout << "[ ";
    while (actual != NULL) {
        actual->imprimir();
        if (actual->getSiguiente() != NULL) {
            std::cout << " <-> ";
        }
        actual = actual->getSiguiente();
    }
    std::cout << " ]" << std::endl;
}

template <typename T>
void ListaDoble<T>::imprimirReversa() const {
    if (estaVacia()) {
        std::cout << "Lista vacia" << std::endl;
        return;
    }
    
    NodoDoble<T>* actual = cola;
    std::cout << "[ ";
    while (actual != NULL) {
        actual->imprimir();
        if (actual->getAnterior() != NULL) {
            std::cout << " <-> ";
        }
        actual = actual->getAnterior();
    }
    std::cout << " ]" << std::endl;
}

template <typename T>
void ListaDoble<T>::invertir() {
    if (tamano <= 1) {
        return;
    }
    
    NodoDoble<T>* actual = cabeza;
    NodoDoble<T>* temp = NULL;
    
    //intercambiar siguiente y anterior de cada nodo
    while (actual != NULL) {
        temp = actual->getAnterior();
        actual->setAnterior(actual->getSiguiente());
        actual->setSiguiente(temp);
        actual = actual->getAnterior();
    }
    
    //intercambio cabeza y cola
    temp = cabeza;
    cabeza = cola;
    cola = temp;
}

template <typename T>
int ListaDoble<T>::contar(const T& dato) const {
    int contador = 0;
    NodoDoble<T>* actual = cabeza;
    
    while (actual != NULL) {
        if (actual->getDato() == dato) {
            contador++;
        }
        actual = actual->getSiguiente();
    }
    
    return contador;
}

#endif //LISTADOBLE_H
