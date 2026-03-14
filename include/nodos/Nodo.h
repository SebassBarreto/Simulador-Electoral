#ifndef NODO_H
#define NODO_H

#include <iostream>


template <typename T>
class Nodo {
private:
    T dato;              
    Nodo<T>* siguiente;  

public:
    //constructor por defecto
    Nodo();

    Nodo(const T& valor);

   
    Nodo(const T& valor, Nodo<T>* sig);

   
    ~Nodo();

    //getters
    const T& getDato() const;


    T& getDato();

    
    Nodo<T>* getSiguiente() const;

    
    void setDato(const T& valor);

   
    void setSiguiente(Nodo<T>* sig);

    //utilidades

   //verifica si tiene siguiente
    bool tieneSiguiente() const;

    //un print sencillo
    void imprimir() const;
};

// ==================== IMPLEMENTACION ====================

template <typename T>
Nodo<T>::Nodo() : siguiente(NULL) {
}

template <typename T>
Nodo<T>::Nodo(const T& valor) : dato(valor), siguiente(NULL) {
}

template <typename T>
Nodo<T>::Nodo(const T& valor, Nodo<T>* sig) : dato(valor), siguiente(sig) {
}

template <typename T>
Nodo<T>::~Nodo() {
    siguiente = NULL;
}

//"implementacion"
//getters
template <typename T>
const T& Nodo<T>::getDato() const {
    return dato;
}

template <typename T>
T& Nodo<T>::getDato() {
    return dato;
}

template <typename T>
Nodo<T>* Nodo<T>::getSiguiente() const {
    return siguiente;
}

//setters

template <typename T>
void Nodo<T>::setDato(const T& valor) {
    dato = valor;
}

template <typename T>
void Nodo<T>::setSiguiente(Nodo<T>* sig) {
    siguiente = sig;
}

//utilidades
template <typename T>
bool Nodo<T>::tieneSiguiente() const {
    return siguiente != NULL;
}

template <typename T>
void Nodo<T>::imprimir() const {
    std::cout << dato;
}

#endif // NODO_H
