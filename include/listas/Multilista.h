/**
 * @file Multilista.h
 * @brief Multilista generica con tres niveles de enlace
 * @author SebassBarreto
 * @date 2025-12-04
 * 
 * Implementacion de una multilista que permite organizar datos en multiples dimensiones. 
 * Cada nodo puede tener enlaces a:
 * - Siguiente en la lista principal
 * - Sublista secundaria
 * - Sublista terciaria
 * 
 * Ejemplo de uso: Candidato -> Ciudades -> Partidos
 * 
 * Complejidad espacial: O(n * m * p) donde n, m, p son elementos en cada nivel
 */

#ifndef MULTILISTA_H
#define MULTILISTA_H

#include <iostream>
#include "../listas/Lista.h"


template <typename T, typename U, typename V>
class NodoMultilista {
private:
    T dato;
    Lista<U>* listaNivel2;      // segunda dimension
    Lista<V>* listaNivel3;      // tercera dimension
    NodoMultilista<T, U, V>* siguiente;

public:
    NodoMultilista();
    NodoMultilista(const T& valor);
    ~NodoMultilista();

    const T& getDato() const;
    T& getDato();
    void setDato(const T& valor);

    Lista<U>* getListaNivel2();
    Lista<V>* getListaNivel3();

    NodoMultilista<T, U, V>* getSiguiente() const;
    void setSiguiente(NodoMultilista<T, U, V>* sig);

    bool tieneSiguiente() const;
    void imprimir() const;
};

//multilista con tres niveles de profundidad
//T Tipo de dato del nivel 1
//U Tipo de dato del nivel 2
//V Tipo de dato del nivel 3
template <typename T, typename U, typename V>
class Multilista {
private:
    NodoMultilista<T, U, V>* cabeza;
    int tamano;

    NodoMultilista<T, U, V>* buscarNodo(const T& dato) const;

public:
    
    Multilista();

    
    ~Multilista();

    //NIVEL 1
    void insertarNivel1(const T& dato);

   
    bool eliminarNivel1(const T& dato);

    
    bool existeNivel1(const T& dato) const;

    //NIVEL2
    bool insertarNivel2(const T& datoNivel1, const U& datoNivel2);

    
    bool eliminarNivel2(const T& datoNivel1, const U& datoNivel2);

    
    bool existeNivel2(const T& datoNivel1, const U& datoNivel2) const;

    
    Lista<U>* obtenerListaNivel2(const T& datoNivel1);

    

    //NIVEL 3
    bool insertarNivel3(const T& datoNivel1, const V& datoNivel3);

   
    bool eliminarNivel3(const T& datoNivel1, const V& datoNivel3);

    
    bool existeNivel3(const T& datoNivel1, const V& datoNivel3) const;

   
    Lista<V>* obtenerListaNivel3(const T& datoNivel1);

    //utilidades

    
    int obtenerTamano() const;

   
    bool estaVacia() const;

    
    void imprimir() const;

    
    void imprimirNivel1() const;

    
    void limpiar();

    
    int contarNivel2(const T& datoNivel1) const;

   
    int contarNivel3(const T& datoNivel1) const;
};

//implementacion nodoMultilista

template <typename T, typename U, typename V>
NodoMultilista<T, U, V>::NodoMultilista() : siguiente(NULL) {
    listaNivel2 = new Lista<U>();
    listaNivel3 = new Lista<V>();
}

template <typename T, typename U, typename V>
NodoMultilista<T, U, V>::NodoMultilista(const T& valor) : dato(valor), siguiente(NULL) {
    listaNivel2 = new Lista<U>();
    listaNivel3 = new Lista<V>();
}

template <typename T, typename U, typename V>
NodoMultilista<T, U, V>::~NodoMultilista() {
    delete listaNivel2;
    delete listaNivel3;
    siguiente = NULL;
}

template <typename T, typename U, typename V>
const T& NodoMultilista<T, U, V>::getDato() const {
    return dato;
}

template <typename T, typename U, typename V>
T& NodoMultilista<T, U, V>::getDato() {
    return dato;
}

template <typename T, typename U, typename V>
void NodoMultilista<T, U, V>::setDato(const T& valor) {
    dato = valor;
}

template <typename T, typename U, typename V>
Lista<U>* NodoMultilista<T, U, V>::getListaNivel2() {
    return listaNivel2;
}

template <typename T, typename U, typename V>
Lista<V>* NodoMultilista<T, U, V>::getListaNivel3() {
    return listaNivel3;
}

template <typename T, typename U, typename V>
NodoMultilista<T, U, V>* NodoMultilista<T, U, V>::getSiguiente() const {
    return siguiente;
}

template <typename T, typename U, typename V>
void NodoMultilista<T, U, V>::setSiguiente(NodoMultilista<T, U, V>* sig) {
    siguiente = sig;
}

template <typename T, typename U, typename V>
bool NodoMultilista<T, U, V>::tieneSiguiente() const {
    return siguiente != NULL;
}

template <typename T, typename U, typename V>
void NodoMultilista<T, U, V>::imprimir() const {
    std::cout << dato;
}

//implementacion FUNDAMENTAL de multilista ya como tal xd

template <typename T, typename U, typename V>
Multilista<T, U, V>::Multilista() : cabeza(NULL), tamano(0) {
}

template <typename T, typename U, typename V>
Multilista<T, U, V>::~Multilista() {
    limpiar();
}

template <typename T, typename U, typename V>
NodoMultilista<T, U, V>* Multilista<T, U, V>::buscarNodo(const T& dato) const {
    NodoMultilista<T, U, V>* actual = cabeza;
    while (actual != NULL) {
        if (actual->getDato() == dato) {
            return actual;
        }
        actual = actual->getSiguiente();
    }
    return NULL;
}

//nivel1 

template <typename T, typename U, typename V>
void Multilista<T, U, V>::insertarNivel1(const T& dato) {
    if (existeNivel1(dato)) {
        return;  //no insertar duplicados
    }

    NodoMultilista<T, U, V>* nuevo = new NodoMultilista<T, U, V>(dato);

    if (cabeza == NULL) {
        cabeza = nuevo;
    } else {
        NodoMultilista<T, U, V>* actual = cabeza;
        while (actual->getSiguiente() != NULL) {
            actual = actual->getSiguiente();
        }
        actual->setSiguiente(nuevo);
    }

    tamano++;
}

template <typename T, typename U, typename V>
bool Multilista<T, U, V>::eliminarNivel1(const T& dato) {
    if (cabeza == NULL) {
        return false;
    }

    if (cabeza->getDato() == dato) {
        NodoMultilista<T, U, V>* temp = cabeza;
        cabeza = cabeza->getSiguiente();
        delete temp;
        tamano--;
        return true;
    }

    NodoMultilista<T, U, V>* actual = cabeza;
    while (actual->getSiguiente() != NULL) {
        if (actual->getSiguiente()->getDato() == dato) {
            NodoMultilista<T, U, V>* temp = actual->getSiguiente();
            actual->setSiguiente(temp->getSiguiente());
            delete temp;
            tamano--;
            return true;
        }
        actual = actual->getSiguiente();
    }

    return false;
}

template <typename T, typename U, typename V>
bool Multilista<T, U, V>::existeNivel1(const T& dato) const {
    return buscarNodo(dato) != NULL;
}

//nivel 2

template <typename T, typename U, typename V>
bool Multilista<T, U, V>::insertarNivel2(const T& datoNivel1, const U& datoNivel2) {
    NodoMultilista<T, U, V>* nodo = buscarNodo(datoNivel1);
    if (nodo == NULL) {
        return false;
    }

    nodo->getListaNivel2()->insertarFinal(datoNivel2);
    return true;
}

template <typename T, typename U, typename V>
bool Multilista<T, U, V>::eliminarNivel2(const T& datoNivel1, const U& datoNivel2) {
    NodoMultilista<T, U, V>* nodo = buscarNodo(datoNivel1);
    if (nodo == NULL) {
        return false;
    }

    return nodo->getListaNivel2()->eliminarPorDato(datoNivel2);
}

template <typename T, typename U, typename V>
bool Multilista<T, U, V>::existeNivel2(const T& datoNivel1, const U& datoNivel2) const {
    NodoMultilista<T, U, V>* nodo = buscarNodo(datoNivel1);
    if (nodo == NULL) {
        return false;
    }

    return nodo->getListaNivel2()->existe(datoNivel2);
}

template <typename T, typename U, typename V>
Lista<U>* Multilista<T, U, V>::obtenerListaNivel2(const T& datoNivel1) {
    NodoMultilista<T, U, V>* nodo = buscarNodo(datoNivel1);
    if (nodo == NULL) {
        return NULL;
    }
    return nodo->getListaNivel2();
}

//nivel 3

template <typename T, typename U, typename V>
bool Multilista<T, U, V>::insertarNivel3(const T& datoNivel1, const V& datoNivel3) {
    NodoMultilista<T, U, V>* nodo = buscarNodo(datoNivel1);
    if (nodo == NULL) {
        return false;
    }

    nodo->getListaNivel3()->insertarFinal(datoNivel3);
    return true;
}

template <typename T, typename U, typename V>
bool Multilista<T, U, V>::eliminarNivel3(const T& datoNivel1, const V& datoNivel3) {
    NodoMultilista<T, U, V>* nodo = buscarNodo(datoNivel1);
    if (nodo == NULL) {
        return false;
    }

    return nodo->getListaNivel3()->eliminarPorDato(datoNivel3);
}

template <typename T, typename U, typename V>
bool Multilista<T, U, V>::existeNivel3(const T& datoNivel1, const V& datoNivel3) const {
    NodoMultilista<T, U, V>* nodo = buscarNodo(datoNivel1);
    if (nodo == NULL) {
        return false;
    }

    return nodo->getListaNivel3()->existe(datoNivel3);
}

template <typename T, typename U, typename V>
Lista<V>* Multilista<T, U, V>::obtenerListaNivel3(const T& datoNivel1) {
    NodoMultilista<T, U, V>* nodo = buscarNodo(datoNivel1);
    if (nodo == NULL) {
        return NULL;
    }
    return nodo->getListaNivel3();
}

//utilidades

template <typename T, typename U, typename V>
int Multilista<T, U, V>::obtenerTamano() const {
    return tamano;
}

template <typename T, typename U, typename V>
bool Multilista<T, U, V>::estaVacia() const {
    return cabeza == NULL;
}

template <typename T, typename U, typename V>
void Multilista<T, U, V>::imprimir() const {
    if (estaVacia()) {
        std::cout << "Multilista vacia" << std::endl;
        return;
    }

    NodoMultilista<T, U, V>* actual = cabeza;
    int contador = 1;

    while (actual != NULL) {
        std::cout << contador << ". ";
        actual->imprimir();
        std::cout << std::endl;

        //imprimir nivel 2
        Lista<U>* lista2 = actual->getListaNivel2();
        if (!lista2->estaVacia()) {
            std::cout << "   Nivel 2: ";
            lista2->imprimir();
        }

        //imprimir nivel 3
        Lista<V>* lista3 = actual->getListaNivel3();
        if (!lista3->estaVacia()) {
            std::cout << "   Nivel 3: ";
            lista3->imprimir();
        }

        std::cout << std::endl;
        actual = actual->getSiguiente();
        contador++;
    }
}

template <typename T, typename U, typename V>
void Multilista<T, U, V>::imprimirNivel1() const {
    if (estaVacia()) {
        std::cout << "Multilista vacia" << std::endl;
        return;
    }

    NodoMultilista<T, U, V>* actual = cabeza;
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

template <typename T, typename U, typename V>
void Multilista<T, U, V>::limpiar() {
    while (cabeza != NULL) {
        NodoMultilista<T, U, V>* temp = cabeza;
        cabeza = cabeza->getSiguiente();
        delete temp;
    }
    tamano = 0;
}

template <typename T, typename U, typename V>
int Multilista<T, U, V>::contarNivel2(const T& datoNivel1) const {
    NodoMultilista<T, U, V>* nodo = buscarNodo(datoNivel1);
    if (nodo == NULL) {
        return -1;
    }
    return nodo->getListaNivel2()->obtenerTamano();
}

template <typename T, typename U, typename V>
int Multilista<T, U, V>::contarNivel3(const T& datoNivel1) const {
    NodoMultilista<T, U, V>* nodo = buscarNodo(datoNivel1);
    if (nodo == NULL) {
        return -1;
    }
    return nodo->getListaNivel3()->obtenerTamano();
}

#endif // MULTILISTA_H
