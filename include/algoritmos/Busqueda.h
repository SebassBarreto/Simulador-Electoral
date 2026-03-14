#ifndef BUSQUEDA_H
#define BUSQUEDA_H


class Busqueda {
private:
    template <typename T>
    static int busquedaBinariaRecursiva(T arr[], int inicio, int fin, const T& objetivo);

public:
    //BUSQUEDA LINEAL 
    template <typename T>
    static int busquedaLineal(T arr[], int n, const T& objetivo);

    //BUSQUEDA BINARIA
    template <typename T>
    static int busquedaBinaria(T arr[], int n, const T& objetivo);

 
    template <typename T>
    static int busquedaBinariaRec(T arr[], int n, const T& objetivo);

   
    template <typename T>
    static int contar(T arr[], int n, const T& objetivo);

 
    template <typename T>
    static bool encontrarMinimo(T arr[], int n, T& resultado);

  
    template <typename T>
    static bool encontrarMaximo(T arr[], int n, T& resultado);
};

//implementacion

template <typename T>
int Busqueda::busquedaLineal(T arr[], int n, const T& objetivo) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == objetivo) {
            return i;
        }
    }
    return -1;
}

template <typename T>
int Busqueda::busquedaBinaria(T arr[], int n, const T& objetivo) {
    int inicio = 0;
    int fin = n - 1;

    while (inicio <= fin) {
        int medio = inicio + (fin - inicio) / 2;

        if (arr[medio] == objetivo) {
            return medio;
        }

        if (arr[medio] < objetivo) {
            inicio = medio + 1;
        } else {
            fin = medio - 1;
        }
    }

    return -1;
}

template <typename T>
int Busqueda::busquedaBinariaRecursiva(T arr[], int inicio, int fin, const T& objetivo) {
    if (inicio > fin) {
        return -1;
    }

    int medio = inicio + (fin - inicio) / 2;

    if (arr[medio] == objetivo) {
        return medio;
    }

    if (arr[medio] < objetivo) {
        return busquedaBinariaRecursiva(arr, medio + 1, fin, objetivo);
    } else {
        return busquedaBinariaRecursiva(arr, inicio, medio - 1, objetivo);
    }
}

template <typename T>
int Busqueda::busquedaBinariaRec(T arr[], int n, const T& objetivo) {
    return busquedaBinariaRecursiva(arr, 0, n - 1, objetivo);
}

template <typename T>
int Busqueda::contar(T arr[], int n, const T& objetivo) {
    int contador = 0;
    for (int i = 0; i < n; i++) {
        if (arr[i] == objetivo) {
            contador++;
        }
    }
    return contador;
}

template <typename T>
bool Busqueda::encontrarMinimo(T arr[], int n, T& resultado) {
    if (n <= 0) {
        return false;
    }

    resultado = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] < resultado) {
            resultado = arr[i];
        }
    }
    return true;
}

template <typename T>
bool Busqueda::encontrarMaximo(T arr[], int n, T& resultado) {
    if (n <= 0) {
        return false;
    }

    resultado = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > resultado) {
            resultado = arr[i];
        }
    }
    return true;
}

#endif // BUSQUEDA_H
