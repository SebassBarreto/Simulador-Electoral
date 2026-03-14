#ifndef ORDENAMIENTO_H
#define ORDENAMIENTO_H

#include <iostream>


class Ordenamiento {
private:
    //auxiliares

    template <typename T>
    static void intercambiar(T& a, T& b);

    template <typename T>
    static int particion(T arr[], int inicio, int fin);

    template <typename T>
    static void quickSortRecursivo(T arr[], int inicio, int fin);

    template <typename T>
    static void mezclar(T arr[], int inicio, int medio, int fin);

    template <typename T>
    static void mergeSortRecursivo(T arr[], int inicio, int fin);

public:
    //QUICKSORT
    template <typename T>
    static void quickSort(T arr[], int n);

    //MERGESORT
    template <typename T>
    static void mergeSort(T arr[], int n);

   //SELECTIONSORT
    template <typename T>
    static void insertionSort(T arr[], int n);

   
    template <typename T>
    static bool estaOrdenado(T arr[], int n);

   
    template <typename T>
    static void imprimir(T arr[], int n);

    
    template <typename T>
    static void copiar(T origen[], T destino[], int n);
};

//implementacion

template <typename T>
void Ordenamiento::intercambiar(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

template <typename T>
int Ordenamiento::particion(T arr[], int inicio, int fin) {
    T pivote = arr[fin];
    int i = inicio - 1;

    for (int j = inicio; j < fin; j++) {
        if (arr[j] <= pivote) {
            i++;
            intercambiar(arr[i], arr[j]);
        }
    }
    intercambiar(arr[i + 1], arr[fin]);
    return i + 1;
}

template <typename T>
void Ordenamiento::quickSortRecursivo(T arr[], int inicio, int fin) {
    if (inicio < fin) {
        int pi = particion(arr, inicio, fin);
        quickSortRecursivo(arr, inicio, pi - 1);
        quickSortRecursivo(arr, pi + 1, fin);
    }
}

template <typename T>
void Ordenamiento::quickSort(T arr[], int n) {
    if (n <= 1) return;
    quickSortRecursivo(arr, 0, n - 1);
}

template <typename T>
void Ordenamiento::mezclar(T arr[], int inicio, int medio, int fin) {
    int n1 = medio - inicio + 1;
    int n2 = fin - medio;

    T* izq = new T[n1];
    T* der = new T[n2];

    for (int i = 0; i < n1; i++) {
        izq[i] = arr[inicio + i];
    }
    for (int j = 0; j < n2; j++) {
        der[j] = arr[medio + 1 + j];
    }

    int i = 0, j = 0, k = inicio;

    while (i < n1 && j < n2) {
        if (izq[i] <= der[j]) {
            arr[k] = izq[i];
            i++;
        } else {
            arr[k] = der[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = izq[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = der[j];
        j++;
        k++;
    }

    delete[] izq;
    delete[] der;
}

template <typename T>
void Ordenamiento::mergeSortRecursivo(T arr[], int inicio, int fin) {
    if (inicio < fin) {
        int medio = inicio + (fin - inicio) / 2;
        mergeSortRecursivo(arr, inicio, medio);
        mergeSortRecursivo(arr, medio + 1, fin);
        mezclar(arr, inicio, medio, fin);
    }
}

template <typename T>
void Ordenamiento::mergeSort(T arr[], int n) {
    if (n <= 1) return;
    mergeSortRecursivo(arr, 0, n - 1);
}

template <typename T>
void Ordenamiento::insertionSort(T arr[], int n) {
    for (int i = 1; i < n; i++) {
        T clave = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j] > clave) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = clave;
    }
}

template <typename T>
bool Ordenamiento::estaOrdenado(T arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return false;
        }
    }
    return true;
}

template <typename T>
void Ordenamiento::imprimir(T arr[], int n) {
    std::cout << "[ ";
    for (int i = 0; i < n; i++) {
        std::cout << arr[i];
        if (i < n - 1) {
            std::cout << ", ";
        }
    }
    std::cout << " ]" << std::endl;
}

template <typename T>
void Ordenamiento::copiar(T origen[], T destino[], int n) {
    for (int i = 0; i < n; i++) {
        destino[i] = origen[i];
    }
}

#endif // ORDENAMIENTO_H
