#ifndef ARBOLBINARIO_H
#define ARBOLBINARIO_H

#include <iostream>
#include "../nodos/NodoArbol.h"


template <typename T>
class ArbolBinario {
private:
    NodoArbol<T>* raiz;
    int tamano;

    //metodos privados recursivos

    NodoArbol<T>* insertarRecursivo(NodoArbol<T>* nodo, const T& dato);
    NodoArbol<T>* buscarRecursivo(NodoArbol<T>* nodo, const T& dato) const;
    NodoArbol<T>* eliminarRecursivo(NodoArbol<T>* nodo, const T& dato);
    NodoArbol<T>* encontrarMinimo(NodoArbol<T>* nodo) const;
    NodoArbol<T>* encontrarMaximo(NodoArbol<T>* nodo) const;
    
    void inordenRecursivo(NodoArbol<T>* nodo) const;
    void preordenRecursivo(NodoArbol<T>* nodo) const;
    void postordenRecursivo(NodoArbol<T>* nodo) const;
    
    int alturaRecursivo(NodoArbol<T>* nodo) const;
    int contarNodosRecursivo(NodoArbol<T>* nodo) const;
    int contarHojasRecursivo(NodoArbol<T>* nodo) const;
    
    void limpiarRecursivo(NodoArbol<T>* nodo);
    void imprimirNivelRecursivo(NodoArbol<T>* nodo, int nivel, int nivelActual) const;

public:
   
    ArbolBinario();

   
    ~ArbolBinario();

    //insercion y busqueda

    void insertar(const T& dato);

   
    bool buscar(const T& dato) const;

    
    bool eliminar(const T& dato);

    //recorridos
    void inorden() const;

    
    void preorden() const;

   
    void postorden() const;

   
    void imprimirPorNiveles() const;

    //consultas
    bool encontrarMinimo(T& resultado) const;

   
    bool encontrarMaximo(T& resultado) const;

   
    int altura() const;

   //nro de nodos
    int obtenerTamano() const;

   	
    int contarHojas() const;

    
    bool estaVacio() const;

   
    bool esCompleto() const;
 
    //utilidades
    void limpiar();

    void imprimir() const;
};

//implementacion

template <typename T>
ArbolBinario<T>::ArbolBinario() : raiz(NULL), tamano(0) {
}

template <typename T>
ArbolBinario<T>::~ArbolBinario() {
    limpiar();
}

//metodos privados recursivos

template <typename T>
NodoArbol<T>* ArbolBinario<T>::insertarRecursivo(NodoArbol<T>* nodo, const T& dato) {
    if (nodo == NULL) {
        tamano++;
        return new NodoArbol<T>(dato);
    }
    
    if (dato < nodo->getDato()) {
        nodo->setIzquierdo(insertarRecursivo(nodo->getIzquierdo(), dato));
    } else if (dato > nodo->getDato()) {
        nodo->setDerecho(insertarRecursivo(nodo->getDerecho(), dato));
    }
    //si dato == nodo->getDato(), no insertamos duplicados
    
    return nodo;
}

template <typename T>
NodoArbol<T>* ArbolBinario<T>::buscarRecursivo(NodoArbol<T>* nodo, const T& dato) const {
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
NodoArbol<T>* ArbolBinario<T>::eliminarRecursivo(NodoArbol<T>* nodo, const T& dato) {
    if (nodo == NULL) {
        return NULL;
    }
    
    if (dato < nodo->getDato()) {
        nodo->setIzquierdo(eliminarRecursivo(nodo->getIzquierdo(), dato));
    } else if (dato > nodo->getDato()) {
        nodo->setDerecho(eliminarRecursivo(nodo->getDerecho(), dato));
    } else {
        //nodo encontrado
        
        //caso 1: Nodo hoja (sin hijos)
        if (nodo->esHoja()) {
            delete nodo;
            tamano--;
            return NULL;
        }
        
        //caso 2: Nodo con un solo hijo
        if (nodo->tieneUnSoloHijo()) {
            NodoArbol<T>* temp = nodo->tieneIzquierdo() ? nodo->getIzquierdo() : nodo->getDerecho();
            delete nodo;
            tamano--;
            return temp;
        }
        
        //caso 3: Nodo con dos hijos
        //encontrar el sucesor inorden (minimo del subarbol derecho)
        NodoArbol<T>* sucesor = encontrarMinimo(nodo->getDerecho());
        nodo->setDato(sucesor->getDato());
        nodo->setDerecho(eliminarRecursivo(nodo->getDerecho(), sucesor->getDato()));
    }
    
    return nodo;
}

template <typename T>
NodoArbol<T>* ArbolBinario<T>::encontrarMinimo(NodoArbol<T>* nodo) const {
    while (nodo != NULL && nodo->tieneIzquierdo()) {
        nodo = nodo->getIzquierdo();
    }
    return nodo;
}

template <typename T>
NodoArbol<T>* ArbolBinario<T>::encontrarMaximo(NodoArbol<T>* nodo) const {
    while (nodo != NULL && nodo->tieneDerecho()) {
        nodo = nodo->getDerecho();
    }
    return nodo;
}

template <typename T>
void ArbolBinario<T>::inordenRecursivo(NodoArbol<T>* nodo) const {
    if (nodo != NULL) {
        inordenRecursivo(nodo->getIzquierdo());
        nodo->imprimir();
        std::cout << " ";
        inordenRecursivo(nodo->getDerecho());
    }
}

template <typename T>
void ArbolBinario<T>::preordenRecursivo(NodoArbol<T>* nodo) const {
    if (nodo != NULL) {
        nodo->imprimir();
        std::cout << " ";
        preordenRecursivo(nodo->getIzquierdo());
        preordenRecursivo(nodo->getDerecho());
    }
}

template <typename T>
void ArbolBinario<T>::postordenRecursivo(NodoArbol<T>* nodo) const {
    if (nodo != NULL) {
        postordenRecursivo(nodo->getIzquierdo());
        postordenRecursivo(nodo->getDerecho());
        nodo->imprimir();
        std::cout << " ";
    }
}

template <typename T>
int ArbolBinario<T>::alturaRecursivo(NodoArbol<T>* nodo) const {
    if (nodo == NULL) {
        return -1;
    }
    
    int altIzq = alturaRecursivo(nodo->getIzquierdo());
    int altDer = alturaRecursivo(nodo->getDerecho());
    
    return 1 + (altIzq > altDer ? altIzq : altDer);
}

template <typename T>
int ArbolBinario<T>::contarNodosRecursivo(NodoArbol<T>* nodo) const {
    if (nodo == NULL) {
        return 0;
    }
    return 1 + contarNodosRecursivo(nodo->getIzquierdo()) + contarNodosRecursivo(nodo->getDerecho());
}

template <typename T>
int ArbolBinario<T>::contarHojasRecursivo(NodoArbol<T>* nodo) const {
    if (nodo == NULL) {
        return 0;
    }
    if (nodo->esHoja()) {
        return 1;
    }
    return contarHojasRecursivo(nodo->getIzquierdo()) + contarHojasRecursivo(nodo->getDerecho());
}

template <typename T>
void ArbolBinario<T>::limpiarRecursivo(NodoArbol<T>* nodo) {
    if (nodo != NULL) {
        limpiarRecursivo(nodo->getIzquierdo());
        limpiarRecursivo(nodo->getDerecho());
        delete nodo;
    }
}

template <typename T>
void ArbolBinario<T>::imprimirNivelRecursivo(NodoArbol<T>* nodo, int nivel, int nivelActual) const {
    if (nodo == NULL) {
        return;
    }
    
    if (nivelActual == nivel) {
        nodo->imprimir();
        std::cout << " ";
    } else {
        imprimirNivelRecursivo(nodo->getIzquierdo(), nivel, nivelActual + 1);
        imprimirNivelRecursivo(nodo->getDerecho(), nivel, nivelActual + 1);
    }
}

//metodos PUBLICOSS

template <typename T>
void ArbolBinario<T>::insertar(const T& dato) {
    raiz = insertarRecursivo(raiz, dato);
}

template <typename T>
bool ArbolBinario<T>::buscar(const T& dato) const {
    return buscarRecursivo(raiz, dato) != NULL;
}

template <typename T>
bool ArbolBinario<T>::eliminar(const T& dato) {
    if (! buscar(dato)) {
        return false;
    }
    raiz = eliminarRecursivo(raiz, dato);
    return true;
}

//recorridos

template <typename T>
void ArbolBinario<T>::inorden() const {
    if (estaVacio()) {
        std::cout << "Arbol vacio" << std::endl;
        return;
    }
    inordenRecursivo(raiz);
    std::cout << std::endl;
}

template <typename T>
void ArbolBinario<T>::preorden() const {
    if (estaVacio()) {
        std::cout << "Arbol vacio" << std::endl;
        return;
    }
    preordenRecursivo(raiz);
    std::cout << std::endl;
}

template <typename T>
void ArbolBinario<T>::postorden() const {
    if (estaVacio()) {
        std::cout << "Arbol vacio" << std::endl;
        return;
    }
    postordenRecursivo(raiz);
    std::cout << std::endl;
}

template <typename T>
void ArbolBinario<T>::imprimirPorNiveles() const {
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
bool ArbolBinario<T>::encontrarMinimo(T& resultado) const {
    if (estaVacio()) {
        return false;
    }
    NodoArbol<T>* minimo = encontrarMinimo(raiz);
    resultado = minimo->getDato();
    return true;
}

template <typename T>
bool ArbolBinario<T>::encontrarMaximo(T& resultado) const {
    if (estaVacio()) {
        return false;
    }
    NodoArbol<T>* maximo = encontrarMaximo(raiz);
    resultado = maximo->getDato();
    return true;
}

template <typename T>
int ArbolBinario<T>::altura() const {
    return alturaRecursivo(raiz);
}

template <typename T>
int ArbolBinario<T>::obtenerTamano() const {
    return tamano;
}

template <typename T>
int ArbolBinario<T>::contarHojas() const {
    return contarHojasRecursivo(raiz);
}

template <typename T>
bool ArbolBinario<T>::estaVacio() const {
    return raiz == NULL;
}

template <typename T>
bool ArbolBinario<T>::esCompleto() const {
    if (estaVacio()) {
        return true;
    }
    
    int nodosEsperados = 1;
    int h = altura();
    for (int i = 0; i < h; i++) {
        nodosEsperados *= 2;
    }
    nodosEsperados -= 1;
    
    return tamano == nodosEsperados;
}

//utilidades
template <typename T>
void ArbolBinario<T>::limpiar() {
    limpiarRecursivo(raiz);
    raiz = NULL;
    tamano = 0;
}

template <typename T>
void ArbolBinario<T>::imprimir() const {
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

#endif // ARBOLBINARIO_H
