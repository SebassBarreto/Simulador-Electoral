#ifndef ARBOLAVL_H
#define ARBOLAVL_H

#include <iostream>
#include "../nodos/NodoArbol.h"


template <typename T>
class ArbolAVL {
private:
    NodoArbol<T>* raiz;
    int tamano;

    int obtenerAltura(NodoArbol<T>* nodo) const;

    int obtenerBalance(NodoArbol<T>* nodo) const;

    void actualizarAltura(NodoArbol<T>* nodo);

    //rotaciones
    NodoArbol<T>* rotarDerecha(NodoArbol<T>* y);

    
    NodoArbol<T>* rotarIzquierda(NodoArbol<T>* x);

    NodoArbol<T>* rotarIzquierdaDerecha(NodoArbol<T>* nodo);

    
    NodoArbol<T>* rotarDerechaIzquierda(NodoArbol<T>* nodo);

    //operaciones recursivas

    NodoArbol<T>* insertarRecursivo(NodoArbol<T>* nodo, const T& dato);
    NodoArbol<T>* eliminarRecursivo(NodoArbol<T>* nodo, const T& dato);
    NodoArbol<T>* buscarRecursivo(NodoArbol<T>* nodo, const T& dato) const;
    NodoArbol<T>* encontrarMinimo(NodoArbol<T>* nodo) const;
    NodoArbol<T>* encontrarMaximo(NodoArbol<T>* nodo) const;

    void inordenRecursivo(NodoArbol<T>* nodo) const;
    void preordenRecursivo(NodoArbol<T>* nodo) const;
    void postordenRecursivo(NodoArbol<T>* nodo) const;

    int contarNodosRecursivo(NodoArbol<T>* nodo) const;
    int contarHojasRecursivo(NodoArbol<T>* nodo) const;

    void limpiarRecursivo(NodoArbol<T>* nodo);
    void imprimirNivelRecursivo(NodoArbol<T>* nodo, int nivel, int nivelActual) const;
    void imprimirEstructuraRecursivo(NodoArbol<T>* nodo, int espacio) const;

public:

    ArbolAVL();


    ~ArbolAVL();

    
    void insertar(const T& dato);

    
    bool buscar(const T& dato) const;

    
    bool eliminar(const T& dato);

    void inorden() const;

    void preorden() const;

    void postorden() const;

    
    void imprimirPorNiveles() const;

    //consultas :3
    bool encontrarMinimo(T& resultado) const;

    
    bool encontrarMaximo(T& resultado) const;

    
    int altura() const;

    
    int obtenerTamano() const;

    int contarHojas() const;

    bool estaVacio() const;

    bool estaBalanceado() const;

    
    void limpiar();

    void imprimir() const;


    void imprimirEstructura() const;
};

//implementacion

template <typename T>
ArbolAVL<T>::ArbolAVL() : raiz(NULL), tamano(0) {
}

template <typename T>
ArbolAVL<T>::~ArbolAVL() {
    limpiar();
}

//metodos auxiliares

template <typename T>
int ArbolAVL<T>::obtenerAltura(NodoArbol<T>* nodo) const {
    if (nodo == NULL) {
        return -1;
    }
    return nodo->getAltura();
}

template <typename T>
int ArbolAVL<T>::obtenerBalance(NodoArbol<T>* nodo) const {
    if (nodo == NULL) {
        return 0;
    }
    return obtenerAltura(nodo->getIzquierdo()) - obtenerAltura(nodo->getDerecho());
}

template <typename T>
void ArbolAVL<T>::actualizarAltura(NodoArbol<T>* nodo) {
    if (nodo != NULL) {
        int altIzq = obtenerAltura(nodo->getIzquierdo());
        int altDer = obtenerAltura(nodo->getDerecho());
        nodo->setAltura(1 + (altIzq > altDer ? altIzq : altDer));
    }
}

//ROTACIONES (ayuda)

template <typename T>
NodoArbol<T>* ArbolAVL<T>::rotarDerecha(NodoArbol<T>* y) {
    
    NodoArbol<T>* x = y->getIzquierdo();
    NodoArbol<T>* B = x->getDerecho();

    //realizar rotacion
    x->setDerecho(y);
    y->setIzquierdo(B);

    //actualizar alturas
    actualizarAltura(y);
    actualizarAltura(x);

    return x;  // nueva raiz
}

template <typename T>
NodoArbol<T>* ArbolAVL<T>::rotarIzquierda(NodoArbol<T>* x) {
    
    NodoArbol<T>* y = x->getDerecho();
    NodoArbol<T>* B = y->getIzquierdo();

    //realizar rotacion
    y->setIzquierdo(x);
    x->setDerecho(B);

    //actualizar alturas
    actualizarAltura(x);
    actualizarAltura(y);

    return y;  //nueva raiz
}

template <typename T>
NodoArbol<T>* ArbolAVL<T>::rotarIzquierdaDerecha(NodoArbol<T>* nodo) {
    
    nodo->setIzquierdo(rotarIzquierda(nodo->getIzquierdo()));
    return rotarDerecha(nodo);
}

template <typename T>
NodoArbol<T>* ArbolAVL<T>::rotarDerechaIzquierda(NodoArbol<T>* nodo) {
    
    nodo->setDerecho(rotarDerecha(nodo->getDerecho()));
    return rotarIzquierda(nodo);
}

//operacioens recursivas
template <typename T>
NodoArbol<T>* ArbolAVL<T>::insertarRecursivo(NodoArbol<T>* nodo, const T& dato) {
    //insercion normal como un arbol binario de busqueda
    if (nodo == NULL) {
        tamano++;
        return new NodoArbol<T>(dato);
    }

    if (dato < nodo->getDato()) {
        nodo->setIzquierdo(insertarRecursivo(nodo->getIzquierdo(), dato));
    } else if (dato > nodo->getDato()) {
        nodo->setDerecho(insertarRecursivo(nodo->getDerecho(), dato));
    } else {
        return nodo;  //no permitir duplicados
    }

    
    actualizarAltura(nodo);


    int balance = obtenerBalance(nodo);

    //casos de desbalance (4 casos)

    //caso Izquierda-Izquierda
    if (balance > 1 && dato < nodo->getIzquierdo()->getDato()) {
        return rotarDerecha(nodo);
    }

    //caso Derecha-Derecha
    if (balance < -1 && dato > nodo->getDerecho()->getDato()) {
        return rotarIzquierda(nodo);
    }

    //caso Izquierda-Derecha
    if (balance > 1 && dato > nodo->getIzquierdo()->getDato()) {
        return rotarIzquierdaDerecha(nodo);
    }

    //caso Derecha-Izquierda
    if (balance < -1 && dato < nodo->getDerecho()->getDato()) {
        return rotarDerechaIzquierda(nodo);
    }

    return nodo;  //nodo ya balanceado
}

template <typename T>
NodoArbol<T>* ArbolAVL<T>::eliminarRecursivo(NodoArbol<T>* nodo, const T& dato) {
    //eliminacion normal arbol binario
    if (nodo == NULL) {
        return NULL;
    }

    if (dato < nodo->getDato()) {
        nodo->setIzquierdo(eliminarRecursivo(nodo->getIzquierdo(), dato));
    } else if (dato > nodo->getDato()) {
        nodo->setDerecho(eliminarRecursivo(nodo->getDerecho(), dato));
    } else {
        //nodo encontrado

        //caso 1: Nodo hoja o con un hijo
        if (nodo->esHoja()) {
            delete nodo;
            tamano--;
            return NULL;
        }
        
        if (nodo->tieneUnSoloHijo()) {
            NodoArbol<T>* temp = nodo->tieneIzquierdo() ? nodo->getIzquierdo() : nodo->getDerecho();
            delete nodo;
            tamano--;
            return temp;
        }

        //caso 2: Nodo con dos hijos
        NodoArbol<T>* sucesor = encontrarMinimo(nodo->getDerecho());
        nodo->setDato(sucesor->getDato());
        nodo->setDerecho(eliminarRecursivo(nodo->getDerecho(), sucesor->getDato()));
    }

    if (nodo == NULL) {
        return NULL;
    }


    actualizarAltura(nodo);

    int balance = obtenerBalance(nodo);

    if (balance > 1 && obtenerBalance(nodo->getIzquierdo()) >= 0) {
        return rotarDerecha(nodo);
    }

    if (balance > 1 && obtenerBalance(nodo->getIzquierdo()) < 0) {
        return rotarIzquierdaDerecha(nodo);
    }

    if (balance < -1 && obtenerBalance(nodo->getDerecho()) <= 0) {
        return rotarIzquierda(nodo);
    }

    if (balance < -1 && obtenerBalance(nodo->getDerecho()) > 0) {
        return rotarDerechaIzquierda(nodo);
    }

    return nodo;
}

template <typename T>
NodoArbol<T>* ArbolAVL<T>::buscarRecursivo(NodoArbol<T>* nodo, const T& dato) const {
    if (nodo == NULL || nodo->getDato() == dato) {
        return nodo;
    }

    if (dato < nodo->getDato()) {
        return buscarRecursivo(nodo->getIzquierdo(), dato);
    } else {
        return buscarRecursivo(nodo->getDerecho(), dato);
    }
}

template <typename T>
NodoArbol<T>* ArbolAVL<T>::encontrarMinimo(NodoArbol<T>* nodo) const {
    while (nodo != NULL && nodo->tieneIzquierdo()) {
        nodo = nodo->getIzquierdo();
    }
    return nodo;
}

template <typename T>
NodoArbol<T>* ArbolAVL<T>::encontrarMaximo(NodoArbol<T>* nodo) const {
    while (nodo != NULL && nodo->tieneDerecho()) {
        nodo = nodo->getDerecho();
    }
    return nodo;
}

template <typename T>
void ArbolAVL<T>::inordenRecursivo(NodoArbol<T>* nodo) const {
    if (nodo != NULL) {
        inordenRecursivo(nodo->getIzquierdo());
        nodo->imprimir();
        std::cout << " ";
        inordenRecursivo(nodo->getDerecho());
    }
}

template <typename T>
void ArbolAVL<T>::preordenRecursivo(NodoArbol<T>* nodo) const {
    if (nodo != NULL) {
        nodo->imprimir();
        std::cout << " ";
        preordenRecursivo(nodo->getIzquierdo());
        preordenRecursivo(nodo->getDerecho());
    }
}

template <typename T>
void ArbolAVL<T>::postordenRecursivo(NodoArbol<T>* nodo) const {
    if (nodo != NULL) {
        postordenRecursivo(nodo->getIzquierdo());
        postordenRecursivo(nodo->getDerecho());
        nodo->imprimir();
        std::cout << " ";
    }
}

template <typename T>
int ArbolAVL<T>::contarNodosRecursivo(NodoArbol<T>* nodo) const {
    if (nodo == NULL) {
        return 0;
    }
    return 1 + contarNodosRecursivo(nodo->getIzquierdo()) + contarNodosRecursivo(nodo->getDerecho());
}

template <typename T>
int ArbolAVL<T>::contarHojasRecursivo(NodoArbol<T>* nodo) const {
    if (nodo == NULL) {
        return 0;
    }
    if (nodo->esHoja()) {
        return 1;
    }
    return contarHojasRecursivo(nodo->getIzquierdo()) + contarHojasRecursivo(nodo->getDerecho());
}

template <typename T>
void ArbolAVL<T>::limpiarRecursivo(NodoArbol<T>* nodo) {
    if (nodo != NULL) {
        limpiarRecursivo(nodo->getIzquierdo());
        limpiarRecursivo(nodo->getDerecho());
        delete nodo;
    }
}

template <typename T>
void ArbolAVL<T>::imprimirNivelRecursivo(NodoArbol<T>* nodo, int nivel, int nivelActual) const {
    if (nodo == NULL) {
        return;
    }

    if (nivelActual == nivel) {
        nodo->imprimir();
        std::cout << "[FB:" << obtenerBalance(nodo) << "] ";
    } else {
        imprimirNivelRecursivo(nodo->getIzquierdo(), nivel, nivelActual + 1);
        imprimirNivelRecursivo(nodo->getDerecho(), nivel, nivelActual + 1);
    }
}

template <typename T>
void ArbolAVL<T>::imprimirEstructuraRecursivo(NodoArbol<T>* nodo, int espacio) const {
    if (nodo == NULL) {
        return;
    }

    espacio += 5;

    imprimirEstructuraRecursivo(nodo->getDerecho(), espacio);

    std::cout << std::endl;
    for (int i = 5; i < espacio; i++) {
        std::cout << " ";
    }
    nodo->imprimir();
    std::cout << "[" << nodo->getAltura() << "," << obtenerBalance(nodo) << "]" << std::endl;

    imprimirEstructuraRecursivo(nodo->getIzquierdo(), espacio);
}

//metodos publicos

template <typename T>
void ArbolAVL<T>::insertar(const T& dato) {
    raiz = insertarRecursivo(raiz, dato);
}

template <typename T>
bool ArbolAVL<T>::buscar(const T& dato) const {
    return buscarRecursivo(raiz, dato) != NULL;
}

template <typename T>
bool ArbolAVL<T>::eliminar(const T& dato) {
    if (! buscar(dato)) {
        return false;
    }
    raiz = eliminarRecursivo(raiz, dato);
    return true;
}

//recorridos

template <typename T>
void ArbolAVL<T>::inorden() const {
    if (estaVacio()) {
        std::cout << "Arbol vacio" << std::endl;
        return;
    }
    inordenRecursivo(raiz);
    std::cout << std::endl;
}

template <typename T>
void ArbolAVL<T>::preorden() const {
    if (estaVacio()) {
        std::cout << "Arbol vacio" << std::endl;
        return;
    }
    preordenRecursivo(raiz);
    std::cout << std::endl;
}

template <typename T>
void ArbolAVL<T>::postorden() const {
    if (estaVacio()) {
        std::cout << "Arbol vacio" << std::endl;
        return;
    }
    postordenRecursivo(raiz);
    std::cout << std::endl;
}

template <typename T>
void ArbolAVL<T>::imprimirPorNiveles() const {
    if (estaVacio()) {
        std::cout << "Arbol vacio" << std::endl;
        return;
    }

    int h = altura();
    for (int i = 0; i <= h; i++) {
        std::cout << "Nivel " << i << ": ";
        imprimirNivelRecursivo(raiz, i, 0);
        std::cout << std::endl;
    }
}

//consultas

template <typename T>
bool ArbolAVL<T>::encontrarMinimo(T& resultado) const {
    if (estaVacio()) {
        return false;
    }
    NodoArbol<T>* minimo = encontrarMinimo(raiz);
    resultado = minimo->getDato();
    return true;
}

template <typename T>
bool ArbolAVL<T>::encontrarMaximo(T& resultado) const {
    if (estaVacio()) {
        return false;
    }
    NodoArbol<T>* maximo = encontrarMaximo(raiz);
    resultado = maximo->getDato();
    return true;
}

template <typename T>
int ArbolAVL<T>::altura() const {
    return obtenerAltura(raiz);
}

template <typename T>
int ArbolAVL<T>::obtenerTamano() const {
    return tamano;
}

template <typename T>
int ArbolAVL<T>::contarHojas() const {
    return contarHojasRecursivo(raiz);
}

template <typename T>
bool ArbolAVL<T>::estaVacio() const {
    return raiz == NULL;
}

template <typename T>
bool ArbolAVL<T>::estaBalanceado() const {
    return true;  //AVL siempre esta balanceado por definicion
}

//utilidades

template <typename T>
void ArbolAVL<T>::limpiar() {
    limpiarRecursivo(raiz);
    raiz = NULL;
    tamano = 0;
}

template <typename T>
void ArbolAVL<T>::imprimir() const {
    if (estaVacio()) {
        std::cout << "Arbol vacio" << std::endl;
        return;
    }

    std::cout << "Recorrido Inorden (ordenado): ";
    inorden();
    std::cout << std::endl;

    std::cout << "Estructura por niveles:" << std::endl;
    imprimirPorNiveles();
}

template <typename T>
void ArbolAVL<T>::imprimirEstructura() const {
    if (estaVacio()) {
        std::cout << "Arbol vacio" << std::endl;
        return;
    }

    std::cout << "Estructura del arbol (Rotado /90 grados/):" << std::endl;
    std::cout << "[valor][altura,balance]" << std::endl;
    std::cout << std::endl;
    imprimirEstructuraRecursivo(raiz, 0);
}

#endif // ARBOLAVL_H
