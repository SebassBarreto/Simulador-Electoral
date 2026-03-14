#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <iostream>
#include <string>
#include "../../include/listas/Lista.h"

using namespace std;

class Utilidades {
public:
    static void split(const string& s, char sep, Lista<string>& out) {
        out.limpiar();
        int start = 0;
        for (int i = 0; i <= (int)s.size(); ++i) {
            if (i == (int)s.size() || s[i] == sep) {
                string token = s.substr(start, i - start);
                out.insertarFinal(token);
                start = i + 1;
            }
        }
    }

    static string trim(const string& s) {
        int i = 0, j = (int)s.size() - 1;
        while (i <= j && (s[i] == ' ' || s[i] == '\t' || s[i] == '\r' || s[i] == '\n')) i++;
        while (j >= i && (s[j] == ' ' || s[j] == '\t' || s[j] == '\r' || s[j] == '\n')) j--;
        if (j < i) return string("");
        return s.substr(i, j - i + 1);
    }

    static bool toInt(const string& s, int& out) {
        try {
            out = stoi(s);
            return true;
        } catch (...) {
            return false;
        }
    }
};

#endif // UTILIDADES_H