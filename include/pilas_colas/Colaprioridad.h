#ifndef COLAPRIORIDAD_H
#define COLAPRIORIDAD_H

#include <iostream>


template <typename T>
class NodoPrioridad {
private:
    T dato;
    int prioridad;
    NodoPrioridad<T>* siguiente;

public:
    NodoPrioridad();
    NodoPrioridad(const T& valor, int prio);
    ~NodoPrioridad();

    const T& getDato() const;
    T& getDato();
    void setDato(const T& valor);

    int getPrioridad() const;
    void setPrioridad(int prio);

    NodoPrioridad<T>* getSiguiente() const;
    void setSiguiente(NodoPrioridad<T>* sig);

    void imprimir() const;
};


template <typename T>
class ColaPrioridad {
private:
    NodoPrioridad<T>* frente;
    int tamano;

public:

    ColaPrioridad();

  
    ~ColaPrioridad();

   
    void encolar(const T& dato, int prioridad);

   
    bool desencolar(T& resultado);

   
    bool desencolar(T& resultado, int& prioridad);

    
    bool desencolar();

    
    bool frente_elemento(T& resultado) const;

    
    bool frente_elemento(T& resultado, int& prioridad) const;

    //utilidades

    
    int obtenerTamano() const;

    
    bool estaVacia() const;

   
    void limpiar();

   
    void imprimir() const;

   
    bool existe(const T& dato) const;

    
    int contarPorPrioridad(int prioridad) const;

    
    bool cambiarPrioridad(const T& dato, int nuevaPrioridad);
};

//implementacion del nodo de prioridad

template <typename T>
NodoPrioridad<T>::NodoPrioridad() : prioridad(0), siguiente(NULL) {
}

template <typename T>
NodoPrioridad<T>::NodoPrioridad(const T& valor, int prio) 
    : dato(valor), prioridad(prio), siguiente(NULL) {
}

template <typename T>
NodoPrioridad<T>::~NodoPrioridad() {
    siguiente = NULL;
}

template <typename T>
const T& NodoPrioridad<T>::getDato() const {
    return dato;
}

template <typename T>
T& NodoPrioridad<T>::getDato() {
    return dato;
}

template <typename T>
void NodoPrioridad<T>::setDato(const T& valor) {
    dato = valor;
}

template <typename T>
int NodoPrioridad<T>::getPrioridad() const {
    return prioridad;
}

template <typename T>
void NodoPrioridad<T>::setPrioridad(int prio) {
    prioridad = prio;
}

template <typename T>
NodoPrioridad<T>* NodoPrioridad<T>::getSiguiente() const {
    return siguiente;
}

template <typename T>
void NodoPrioridad<T>::setSiguiente(NodoPrioridad<T>* sig) {
    siguiente = sig;
}

template <typename T>
void NodoPrioridad<T>::imprimir() const {
    std::cout << dato;
}

//implementacion cola de prioridad

template <typename T>
ColaPrioridad<T>::ColaPrioridad() : frente(NULL), tamano(0) {
}

template <typename T>
ColaPrioridad<T>::~ColaPrioridad() {
    limpiar();
}

//op basicas

template <typename T>
void ColaPrioridad<T>::encolar(const T& dato, int prioridad) {
    NodoPrioridad<T>* nuevo = new NodoPrioridad<T>(dato, prioridad);
    
    //si la cola esta vacia o el nuevo tiene mayor prioridad que el frente
    if (estaVacia() || prioridad < frente->getPrioridad()) {
        nuevo->setSiguiente(frente);
        frente = nuevo;
    } else {
        //buscar la posicion correcta segun prioridad
        NodoPrioridad<T>* actual = frente;
        while (actual->getSiguiente() != NULL && 
               actual->getSiguiente()->getPrioridad() <= prioridad) {
            actual = actual->getSiguiente();
        }
        nuevo->setSiguiente(actual->getSiguiente());
        actual->setSiguiente(nuevo);
    }
    
    tamano++;
}

template <typename T>
bool ColaPrioridad<T>::desencolar(T& resultado) {
    if (estaVacia()) {
        return false;
    }
    
    resultado = frente->getDato();
    NodoPrioridad<T>* temp = frente;
    frente = frente->getSiguiente();
    delete temp;
    tamano--;
    
    return true;
}

template <typename T>
bool ColaPrioridad<T>::desencolar(T& resultado, int& prioridad) {
    if (estaVacia()) {
        return false;
    }
    
    resultado = frente->getDato();
    prioridad = frente->getPrioridad();
    NodoPrioridad<T>* temp = frente;
    frente = frente->getSiguiente();
    delete temp;
    tamano--;
    
    return true;
}

template <typename T>
bool ColaPrioridad<T>::desencolar() {
    if (estaVacia()) {
        return false;
    }
    
    NodoPrioridad<T>* temp = frente;
    frente = frente->getSiguiente();
    delete temp;
    tamano--;
    
    return true;
}

template <typename T>
bool ColaPrioridad<T>::frente_elemento(T& resultado) const {
    if (estaVacia()) {
        return false;
    }
    
    resultado = frente->getDato();
    return true;
}

template <typename T>
bool ColaPrioridad<T>::frente_elemento(T& resultado, int& prioridad) const {
    if (estaVacia()) {
        return false;
    }
    
    resultado = frente->getDato();
    prioridad = frente->getPrioridad();
    return true;
}

//utilidades
template <typename T>
int ColaPrioridad<T>::obtenerTamano() const {
    return tamano;
}

template <typename T>
bool ColaPrioridad<T>::estaVacia() const {
    return frente == NULL;
}

template <typename T>
void ColaPrioridad<T>::limpiar() {
    while (! estaVacia()) {
        desencolar();
    }
}

template <typename T>
void ColaPrioridad<T>::imprimir() const {
    if (estaVacia()) {
        std::cout << "Cola de prioridad vacia" << std::endl;
        return;
    }
    
    std::cout << "FRENTE (Mayor prioridad) -> ";
    NodoPrioridad<T>* actual = frente;
    while (actual != NULL) {
        std::cout << "[";
        actual->imprimir();
        std::cout << " | P:" << actual->getPrioridad() << "]";
        if (actual->getSiguiente() != NULL) {
            std::cout << " -> ";
        }
        actual = actual->getSiguiente();
    }
    std::cout << " <- FINAL (Menor prioridad)" << std::endl;
}

template <typename T>
bool ColaPrioridad<T>::existe(const T& dato) const {
    NodoPrioridad<T>* actual = frente;
    while (actual != NULL) {
        if (actual->getDato() == dato) {
            return true;
        }
        actual = actual->getSiguiente();
    }
    return false;
}

template <typename T>
int ColaPrioridad<T>::contarPorPrioridad(int prioridad) const {
    int contador = 0;
    NodoPrioridad<T>* actual = frente;
    
    while (actual != NULL) {
        if (actual->getPrioridad() == prioridad) {
            contador++;
        }
        actual = actual->getSiguiente();
    }
    
    return contador;
}

template <typename T>
bool ColaPrioridad<T>::cambiarPrioridad(const T& dato, int nuevaPrioridad) {
    //buscar y eliminar el elemento
    if (!existe(dato)) {
        return false;
    }
    
    //encontrar el elemento
    T elemento;
    NodoPrioridad<T>* actual = frente;
    NodoPrioridad<T>* anterior = NULL;
    
    while (actual != NULL) {
        if (actual->getDato() == dato) {
            elemento = actual->getDato();
            
            //eliminar el nodo actual
            if (anterior == NULL) {
                frente = actual->getSiguiente();
            } else {
                anterior->setSiguiente(actual->getSiguiente());
            }
            delete actual;
            tamano--;
            
            //reinsertar con nueva prioridad
            encolar(elemento, nuevaPrioridad);
            return true;
        }
        anterior = actual;
        actual = actual->getSiguiente();
    }
    
    return false;
}

#endif // COLAPRIORIDAD_H
