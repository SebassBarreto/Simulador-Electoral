#ifndef NODODOBLE_H
#define NODODOBLE_H

#include <iostream>

//nodo generico para enlace doble (anterior y siguiente)
template <typename T>
class NodoDoble {
private:
    T dato;
    NodoDoble<T>* siguiente;
    NodoDoble<T>* anterior;

public:
    //constructor por defecto
    NodoDoble();

    //constructor con parametro
    NodoDoble(const T& valor);

    
    NodoDoble(const T& valor, NodoDoble<T>* sig, NodoDoble<T>* ant);

   
    ~NodoDoble();

    //getters
    const T& getDato() const;

    
    T& getDato();


    NodoDoble<T>* getSiguiente() const;

    
    NodoDoble<T>* getAnterior() const;

    //setters
    void setDato(const T& valor);

   
    void setSiguiente(NodoDoble<T>* sig);

   
    void setAnterior(NodoDoble<T>* ant);

    //utilidades

    
    bool tieneSiguiente() const;

    
    bool tieneAnterior() const;

   
    bool esPrimero() const;

    
    bool esUltimo() const;

   
    void imprimir() const;
};

//implementacion

template <typename T>
NodoDoble<T>::NodoDoble() : siguiente(NULL), anterior(NULL) {
}

template <typename T>
NodoDoble<T>::NodoDoble(const T& valor) : dato(valor), siguiente(NULL), anterior(NULL) {
}

template <typename T>
NodoDoble<T>::NodoDoble(const T& valor, NodoDoble<T>* sig, NodoDoble<T>* ant) 
    : dato(valor), siguiente(sig), anterior(ant) {
}

template <typename T>
NodoDoble<T>::~NodoDoble() {
    siguiente = NULL;
    anterior = NULL;
}

//getters

template <typename T>
const T& NodoDoble<T>::getDato() const {
    return dato;
}

template <typename T>
T& NodoDoble<T>::getDato() {
    return dato;
}

template <typename T>
NodoDoble<T>* NodoDoble<T>::getSiguiente() const {
    return siguiente;
}

template <typename T>
NodoDoble<T>* NodoDoble<T>::getAnterior() const {
    return anterior;
}

//setters

template <typename T>
void NodoDoble<T>::setDato(const T& valor) {
    dato = valor;
}

template <typename T>
void NodoDoble<T>::setSiguiente(NodoDoble<T>* sig) {
    siguiente = sig;
}

template <typename T>
void NodoDoble<T>::setAnterior(NodoDoble<T>* ant) {
    anterior = ant;
}

//utilidades

template <typename T>
bool NodoDoble<T>::tieneSiguiente() const {
    return siguiente != NULL;
}

template <typename T>
bool NodoDoble<T>::tieneAnterior() const {
    return anterior != NULL;
}

template <typename T>
bool NodoDoble<T>::esPrimero() const {
    return anterior == NULL;
}

template <typename T>
bool NodoDoble<T>::esUltimo() const {
    return siguiente == NULL;
}

template <typename T>
void NodoDoble<T>::imprimir() const {
    std::cout << dato;
}

#endif // NODODOBLE_H
