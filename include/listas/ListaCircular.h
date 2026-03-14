#ifndef LISTACIRCULAR_H
#define LISTACIRCULAR_H

#include <iostream>
#include "../nodos/Nodo.h"

template <typename T>
class ListaCircular {
private:
    Nodo<T>* ultimo;  //apunta al ultimo nodo (el ultimo apunta al primero)
    int tamano;

    
    Nodo<T>* obtenerNodo(int posicion) const;

public:
    
    ListaCircular();

    
    ~ListaCircular();

    
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

    //imprime elementos en lista
    void imprimir() const;

    //mmprime N vueltas completas a la lista circular
    //vueltas Numero de vueltas a dar
    // O(n * vueltas)*/
    void imprimirVueltas(int vueltas) const;

	//rota la lista N posiciones hacia la derecha
    //posiciones Numero de posiciones a rotar
    void rotar(int posiciones);

    //cuenta cuantas veces esta un elemento en la lista
    int contar(const T& dato) const;
};

//implementacion

template <typename T>
ListaCircular<T>::ListaCircular() : ultimo(NULL), tamano(0) {
}

template <typename T>
ListaCircular<T>::~ListaCircular() {
    limpiar();
}

//metodo privado

template <typename T>
Nodo<T>* ListaCircular<T>::obtenerNodo(int posicion) const {
    if (posicion < 0 || posicion >= tamano || estaVacia()) {
        return NULL;
    }
    
    Nodo<T>* actual = ultimo->getSiguiente();  //primero
    for (int i = 0; i < posicion; i++) {
        actual = actual->getSiguiente();
    }
    return actual;
}

//insercion

template <typename T>
void ListaCircular<T>::insertarInicio(const T& dato) {
    Nodo<T>* nuevo = new Nodo<T>(dato);
    
    if (estaVacia()) {
        ultimo = nuevo;
        ultimo->setSiguiente(ultimo);  //apunta a si mismo
    } else {
        nuevo->setSiguiente(ultimo->getSiguiente());
        ultimo->setSiguiente(nuevo);
    }
    
    tamano++;
}

template <typename T>
void ListaCircular<T>::insertarFinal(const T& dato) {
    insertarInicio(dato);
    ultimo = ultimo->getSiguiente();  //el nuevo primero se convierte en ultimo
}

template <typename T>
bool ListaCircular<T>::insertarEnPosicion(const T& dato, int posicion) {
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
bool ListaCircular<T>::eliminarInicio() {
    if (estaVacia()) {
        return false;
    }
    
    Nodo<T>* primero = ultimo->getSiguiente();
    
    if (tamano == 1) {
        delete ultimo;
        ultimo = NULL;
    } else {
        ultimo->setSiguiente(primero->getSiguiente());
        delete primero;
    }
    
    tamano--;
    return true;
}

template <typename T>
bool ListaCircular<T>::eliminarFinal() {
    if (estaVacia()) {
        return false;
    }
    
    if (tamano == 1) {
        delete ultimo;
        ultimo = NULL;
        tamano = 0;
        return true;
    }
    
    Nodo<T>* penultimo = obtenerNodo(tamano - 2);
    delete ultimo;
    ultimo = penultimo;
    ultimo->setSiguiente(penultimo->getSiguiente()->getSiguiente());
    
    tamano--;
    return true;
}

template <typename T>
bool ListaCircular<T>::eliminarEnPosicion(int posicion) {
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
bool ListaCircular<T>::eliminarPorDato(const T& dato) {
    int posicion = buscar(dato);
    if (posicion == -1) {
        return false;
    }
    return eliminarEnPosicion(posicion);
}

template <typename T>
void ListaCircular<T>::limpiar() {
    while (! estaVacia()) {
        eliminarInicio();
    }
}

//busqueda

template <typename T>
int ListaCircular<T>::buscar(const T& dato) const {
    if (estaVacia()) {
        return -1;
    }
    
    Nodo<T>* actual = ultimo->getSiguiente();
    int posicion = 0;
    
    do {
        if (actual->getDato() == dato) {
            return posicion;
        }
        actual = actual->getSiguiente();
        posicion++;
    } while (actual != ultimo->getSiguiente() && posicion < tamano);
    
    return -1;
}

template <typename T>
bool ListaCircular<T>::existe(const T& dato) const {
    return buscar(dato) != -1;
}

template <typename T>
bool ListaCircular<T>::obtenerElemento(int posicion, T& resultado) const {
    Nodo<T>* nodo = obtenerNodo(posicion);
    if (nodo == NULL) {
        return false;
    }
    resultado = nodo->getDato();
    return true;
}

//accesos

template <typename T>
bool ListaCircular<T>::obtenerPrimero(T& resultado) const {
    if (estaVacia()) {
        return false;
    }
    resultado = ultimo->getSiguiente()->getDato();
    return true;
}

template <typename T>
bool ListaCircular<T>::obtenerUltimo(T& resultado) const {
    if (estaVacia()) {
        return false;
    }
    resultado = ultimo->getDato();
    return true;
}

//utilidades

template <typename T>
int ListaCircular<T>::obtenerTamano() const {
    return tamano;
}

template <typename T>
bool ListaCircular<T>::estaVacia() const {
    return ultimo == NULL;
}

template <typename T>
void ListaCircular<T>::imprimir() const {
    if (estaVacia()) {
        std::cout << "Lista vacia" << std::endl;
        return;
    }
    
    Nodo<T>* actual = ultimo->getSiguiente();
    std::cout << "[ ";
    
    for (int i = 0; i < tamano; i++) {
        actual->imprimir();
        if (i < tamano - 1) {
            std::cout << " -> ";
        }
        actual = actual->getSiguiente();
    }
    
    std::cout << " ] -> (vuelve al inicio)" << std::endl;
}

template <typename T>
void ListaCircular<T>::imprimirVueltas(int vueltas) const {
    if (estaVacia()) {
        std::cout << "Lista vacia" << std::endl;
        return;
    }
    
    Nodo<T>* actual = ultimo->getSiguiente();
    int totalElementos = tamano * vueltas;
    
    std::cout << "[ ";
    for (int i = 0; i < totalElementos; i++) {
        actual->imprimir();
        if (i < totalElementos - 1) {
            std::cout << " -> ";
        }
        actual = actual->getSiguiente();
    }
    std::cout << " ]" << std::endl;
}

template <typename T>
void ListaCircular<T>::rotar(int posiciones) {
    if (estaVacia() || tamano == 1) {
        return;
    }
    
    //normalizar posiciones
    posiciones = posiciones % tamano;
    if (posiciones < 0) {
        posiciones = tamano + posiciones;
    }
    
    //rotar moviendo el puntero ultimo
    for (int i = 0; i < posiciones; i++) {
        ultimo = ultimo->getSiguiente();
    }
}

template <typename T>
int ListaCircular<T>::contar(const T& dato) const {
    if (estaVacia()) {
        return 0;
    }
    
    int contador = 0;
    Nodo<T>* actual = ultimo->getSiguiente();
    
    for (int i = 0; i < tamano; i++) {
        if (actual->getDato() == dato) {
            contador++;
        }
        actual = actual->getSiguiente();
    }
    
    return contador;
}


#endif //LISTACIRCULAR_H
