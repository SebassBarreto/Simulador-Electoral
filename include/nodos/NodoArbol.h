#ifndef NODOARBOL_H
#define NODOARBOL_H

#include <iostream>

//nodo generico para arbol binario hijo izq y derecho 
//base para AVL y binario

template<class T>

class NodoArbol{
	private:
		T dato;
		NodoArbol<T>* izquierdo;
		NodoArbol<T>* derecho;
		int altura; //para AVL balanceo
	public:
		//constructor por defecto
		NodoArbol();
		
		NodoArbol(const T& valor);
		
		NodoArbol(const T& valor, NodoArbol<T>* izq, NodoArbol<T>* der);
		
		~NodoArbol();

		//getters
		const T& getDato() const;
	
	    
	    T& getDato();
	
	    
	    NodoArbol<T>* getIzquierdo() const;
	
	   
	    NodoArbol<T>* getDerecho() const;
	
	    
	    int getAltura() const;
	
	   //setters
	   
	    void setDato(const T& valor);
	
	  
	    void setIzquierdo(NodoArbol<T>* izq);
	
	    
	    void setDerecho(NodoArbol<T>* der);
	
	   
	    void setAltura(int alt);
	
	    //utilidades
	
	    
	    bool tieneIzquierdo() const; //hijo izq?
	
	    
	    bool tieneDerecho() const;//hijo der?
	
	    
	    bool esHoja() const;//hoja?
	
	    
	    bool tieneAmbosHijos() const;
	
	   
	    bool tieneUnSoloHijo() const;
	
	    
	    int contarHijos() const; //si tiene 0,1,2
	
	   
	    void calcularAltura(); //calcula y actualiza altura
	
	    
	    int factorBalance() const; //para avl 
	
	   //print
	    void imprimir() const;
	};
	
	//implementacion
	
	template <typename T>
	NodoArbol<T>::NodoArbol() : izquierdo(NULL), derecho(NULL), altura(0) {
	}
	
	template <typename T>
	NodoArbol<T>::NodoArbol(const T& valor) : dato(valor), izquierdo(NULL), derecho(NULL), altura(0) {
	}
	
	template <typename T>
	NodoArbol<T>::NodoArbol(const T& valor, NodoArbol<T>* izq, NodoArbol<T>* der) 
	    : dato(valor), izquierdo(izq), derecho(der), altura(0) {
	    calcularAltura();
	}
	
	template <typename T>
	NodoArbol<T>::~NodoArbol() {
	    izquierdo = NULL;
	    derecho = NULL;
	}
	
	//getters
	
	template <typename T>
	const T& NodoArbol<T>::getDato() const {
	    return dato;
	}
	
	template <typename T>
	T& NodoArbol<T>::getDato() {
	    return dato;
	}
	
	template <typename T>
	NodoArbol<T>* NodoArbol<T>::getIzquierdo() const {
	    return izquierdo;
	}
	
	template <typename T>
	NodoArbol<T>* NodoArbol<T>::getDerecho() const {
	    return derecho;
	}
	
	template <typename T>
	int NodoArbol<T>::getAltura() const {
	    return altura;
	}
	
	//setters
	
	template <typename T>
	void NodoArbol<T>::setDato(const T& valor) {
	    dato = valor;
	}
	
	template <typename T>
	void NodoArbol<T>::setIzquierdo(NodoArbol<T>* izq) {
	    izquierdo = izq;
	    calcularAltura();
	}
	
	template <typename T>
	void NodoArbol<T>::setDerecho(NodoArbol<T>* der) {
	    derecho = der;
	    calcularAltura();
	}
	
	template <typename T>
	void NodoArbol<T>::setAltura(int alt) {
	    altura = alt;
	}
	
	//utilidades
	
	template <typename T>
	bool NodoArbol<T>::tieneIzquierdo() const {
	    return izquierdo != NULL;
	}
	
	template <typename T>
	bool NodoArbol<T>::tieneDerecho() const {
	    return derecho != NULL;
	}
	
	template <typename T>
	bool NodoArbol<T>::esHoja() const {
	    return izquierdo == NULL && derecho == NULL;
	}
	
	template <typename T>
	bool NodoArbol<T>::tieneAmbosHijos() const {
	    return izquierdo != NULL && derecho != NULL;
	}
	
	template <typename T>
	bool NodoArbol<T>::tieneUnSoloHijo() const {
	    return (izquierdo != NULL && derecho == NULL) || (izquierdo == NULL && derecho != NULL);
	}
	
	template <typename T>
	int NodoArbol<T>::contarHijos() const {
	    int contador = 0;
	    if (izquierdo != NULL) contador++;
	    if (derecho != NULL) contador++;
	    return contador;
	}
	
	template <typename T>
	void NodoArbol<T>::calcularAltura() {
	    int altIzq = (izquierdo != NULL) ?  izquierdo->getAltura() : -1;
	    int altDer = (derecho != NULL) ?  derecho->getAltura() : -1;
	    altura = 1 + ((altIzq > altDer) ? altIzq : altDer);
	}
	
	template <typename T>
	int NodoArbol<T>::factorBalance() const {
	    int altIzq = (izquierdo != NULL) ? izquierdo->getAltura() : -1;
	    int altDer = (derecho != NULL) ? derecho->getAltura() : -1;
	    return altIzq - altDer;
	}
	
	template <typename T>
	void NodoArbol<T>::imprimir() const {
	    std::cout << dato;
	}

#endif //NODOARBOL_H
