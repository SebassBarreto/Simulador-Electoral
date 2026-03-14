#ifndef VALIDADORES_H
#define VALIDADORES_H

#include <iostream>
#include <string>

using namespace std;

class Validadores {
public:
    static bool esNumero(const string& s) {
        if (s.empty()) return false;
        for (size_t i = 0; i < s.size(); ++i) {
            if (s[i] < '0' || s[i] > '9') return false;
        }
        return true;
    }

    static bool esSexoValido(char c) {
        return c == 'M' || c == 'F';
    }

    static bool noVacio(const string& s) {
        for (size_t i = 0; i < s.size(); ++i) {
            if (s[i] != ' ' && s[i] != '\t' && s[i] != '\r' && s[i] != '\n') return true;
        }
        return false;
    }
};

#endif // VALIDADORES_H