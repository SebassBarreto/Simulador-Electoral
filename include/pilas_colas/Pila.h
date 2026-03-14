#ifndef PILA_H
#define PILA_H

#include <iostream>
#include "../nodos/Nodo.h"


template <typename T>
class Pila {
private:
    Nodo<T>* tope;
    int tamano;

public:
    
    Pila();

    
    ~Pila();

    
    void push(const T& dato);

    
    bool pop(T& resultado);

    //elimina sin retornar el elemento
    bool pop();

  
    //obtiene el elemento del tope sin desapilar
    //param resultado Variable donde se almacenara el elemento
    //return true si se obtuvo correctamente, false si la pila esta vacia 
    bool peek(T& resultado) const;

   
    //obtiene el elemento del tope sin desapilar (solo lectura)
    //resultado referencia constante al elemento del tope
    const T& peek() const;

    //utilidades
    int obtenerTamano() const;

   
    bool estaVacia() const;

   
    void limpiar();

    
    void imprimir() const;

   
    bool existe(const T& dato) const;

    
    int contar(const T& dato) const;
};

//implementacion

template <typename T>
Pila<T>::Pila() : tope(NULL), tamano(0) {
}

template <typename T>
Pila<T>::~Pila() {
    limpiar();
}

//op basicas

template <typename T>
void Pila<T>::push(const T& dato) {
    Nodo<T>* nuevo = new Nodo<T>(dato);
    nuevo->setSiguiente(tope);
    tope = nuevo;
    tamano++;
}

template <typename T>
bool Pila<T>::pop(T& resultado) {
    if (estaVacia()) {
        return false;
    }
    
    resultado = tope->getDato();
    Nodo<T>* temp = tope;
    tope = tope->getSiguiente();
    delete temp;
    tamano--;
    
    return true;
}

template <typename T>
bool Pila<T>::pop() {
    if (estaVacia()) {
        return false;
    }
    
    Nodo<T>* temp = tope;
    tope = tope->getSiguiente();
    delete temp;
    tamano--;
    
    return true;
}

template <typename T>
bool Pila<T>::peek(T& resultado) const {
    if (estaVacia()) {
        return false;
    }
    
    resultado = tope->getDato();
    return true;
}

template <typename T>
const T& Pila<T>::peek() const {
    return tope->getDato();
}

//utilidades

template <typename T>
int Pila<T>::obtenerTamano() const {
    return tamano;
}

template <typename T>
bool Pila<T>::estaVacia() const {
    return tope == NULL;
}

template <typename T>
void Pila<T>::limpiar() {
    while (!estaVacia()) {
        pop();
    }
}

template <typename T>
void Pila<T>::imprimir() const {
    if (estaVacia()) {
        std::cout << "Pila vacia" << std::endl;
        return;
    }
    
    std::cout << "TOPE -> ";
    Nodo<T>* actual = tope;
    while (actual != NULL) {
        std::cout << "[ ";
        actual->imprimir();
        std::cout << " ]";
        if (actual->getSiguiente() != NULL) {
            std::cout << " -> ";
        }
        actual = actual->getSiguiente();
    }
    std::cout << " <- BASE" << std::endl;
}

template <typename T>
bool Pila<T>::existe(const T& dato) const {
    Nodo<T>* actual = tope;
    while (actual != NULL) {
        if (actual->getDato() == dato) {
            return true;
        }
        actual = actual->getSiguiente();
    }
    return false;
}

template <typename T>
int Pila<T>::contar(const T& dato) const {
    int contador = 0;
    Nodo<T>* actual = tope;
    
    while (actual != NULL) {
        if (actual->getDato() == dato) {
            contador++;
        }
        actual = actual->getSiguiente();
    }
    
    return contador;
}

#endif // PILA_H
