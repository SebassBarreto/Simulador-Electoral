#ifndef COLA_H
#define COLA_H

#include <iostream>
#include "../nodos/Nodo.h"


template <typename T>
class Cola {
private:
    Nodo<T>* frente;
    Nodo<T>* final;
    int tamano;

public:

    Cola();

    
    ~Cola();

   
    void encolar(const T& dato);

    
    bool desencolar(T& resultado);

    //descola sin retornar elemento
    bool desencolar();
	
	
	//obtiene el elemento del frente sin desencolar
    //resultado Variable donde se almacenara el elemento
    //true si se obtuvo correctamente, false si la cola esta vacia
    bool frente_elemento(T& resultado) const;

   
    //obtiene el elemento del frente sin desencolar (solo lectura)
    //retorna referencia constante al elemento del frente
    //true si se obtuvo correctamente, false si la cola esta vacia
    const T& frente_elemento() const;

    //obtiene el elemento del frente sin desencolar
    //resultado Variable donde se almacenara el elemento
    //true si se obtuvo correctamente, false si la cola esta vacia
    bool final_elemento(T& resultado) const;

    //utilidadess
    int obtenerTamano() const;

   
    bool estaVacia() const;

   
    void limpiar();

    
    void imprimir() const;

   
    bool existe(const T& dato) const;

   
    int contar(const T& dato) const;

   
    int posicion(const T& dato) const;
};

//implementacion

template <typename T>
Cola<T>::Cola() : frente(NULL), final(NULL), tamano(0) {
}

template <typename T>
Cola<T>::~Cola() {
    limpiar();
}


//op basicas
template <typename T>
void Cola<T>::encolar(const T& dato) {
    Nodo<T>* nuevo = new Nodo<T>(dato);
    
    if (estaVacia()) {
        frente = nuevo;
        final = nuevo;
    } else {
        final->setSiguiente(nuevo);
        final = nuevo;
    }
    
    tamano++;
}

template <typename T>
bool Cola<T>::desencolar(T& resultado) {
    if (estaVacia()) {
        return false;
    }
    
    resultado = frente->getDato();
    Nodo<T>* temp = frente;
    frente = frente->getSiguiente();
    delete temp;
    tamano--;
    
    if (estaVacia()) {
        final = NULL;
    }
    
    return true;
}

template <typename T>
bool Cola<T>::desencolar() {
    if (estaVacia()) {
        return false;
    }
    
    Nodo<T>* temp = frente;
    frente = frente->getSiguiente();
    delete temp;
    tamano--;
    
    if (estaVacia()) {
        final = NULL;
    }
    
    return true;
}

template <typename T>
bool Cola<T>::frente_elemento(T& resultado) const {
    if (estaVacia()) {
        return false;
    }
    
    resultado = frente->getDato();
    return true;
}

template <typename T>
const T& Cola<T>::frente_elemento() const {
    return frente->getDato();
}

template <typename T>
bool Cola<T>::final_elemento(T& resultado) const {
    if (estaVacia()) {
        return false;
    }
    
    resultado = final->getDato();
    return true;
}

//utilidades

template <typename T>
int Cola<T>::obtenerTamano() const {
    return tamano;
}

template <typename T>
bool Cola<T>::estaVacia() const {
    return frente == NULL;
}

template <typename T>
void Cola<T>::limpiar() {
    while (!estaVacia()) {
        desencolar();
    }
}

template <typename T>
void Cola<T>::imprimir() const {
    if (estaVacia()) {
        std::cout << "Cola vacia" << std::endl;
        return;
    }
    
    std::cout << "FRENTE -> ";
    Nodo<T>* actual = frente;
    while (actual != NULL) {
        std::cout << "[ ";
        actual->imprimir();
        std::cout << " ]";
        if (actual->getSiguiente() != NULL) {
            std::cout << " -> ";
        }
        actual = actual->getSiguiente();
    }
    std::cout << " <- FINAL" << std::endl;
}

template <typename T>
bool Cola<T>::existe(const T& dato) const {
    Nodo<T>* actual = frente;
    while (actual != NULL) {
        if (actual->getDato() == dato) {
            return true;
        }
        actual = actual->getSiguiente();
    }
    return false;
}

template <typename T>
int Cola<T>::contar(const T& dato) const {
    int contador = 0;
    Nodo<T>* actual = frente;
    
    while (actual != NULL) {
        if (actual->getDato() == dato) {
            contador++;
        }
        actual = actual->getSiguiente();
    }
    
    return contador;
}

template <typename T>
int Cola<T>::posicion(const T& dato) const {
    Nodo<T>* actual = frente;
    int pos = 0;
    
    while (actual != NULL) {
        if (actual->getDato() == dato) {
            return pos;
        }
        actual = actual->getSiguiente();
        pos++;
    }
    
    return -1;
}

#endif // COLA_H
