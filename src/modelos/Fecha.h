#ifndef FECHA_H
#define FECHA_H

#include <string>
#include <iostream>
#include <ctime>
#include <sstream>

using namespace std;

class Fecha{
private:
    int dia;
    int mes;
    int anio;
public:
    Fecha(): dia(1), mes(1), anio(2000){} //apartir del 2000

    Fecha(int d, int m, int a): dia(d), mes(m), anio(a){
        if(!esValida()){dia = 1; mes = 1; anio = 2000;}
    }

    explicit Fecha(const string& fechaStr){
        if(!parsear(fechaStr)){dia = 1; mes = 1; anio = 2000; }
    }

    bool parsear(const string& fechaStr) {
        if (fechaStr.size() < 8) return false;

        //buscar separadores
        size_t pos1 = fechaStr.find_first_of("/-");
        if (pos1 == string::npos) return false;
        size_t pos2 = fechaStr.find_first_of("/-", pos1 + 1);
        if (pos2 == string::npos) return false;

        //extraer substrings
        string sDia  = fechaStr.substr(0, pos1);
        string sMes  = fechaStr.substr(pos1 + 1, pos2 - pos1 - 1);
        string sAnio = fechaStr.substr(pos2 + 1);

        //validar que solo contengan dígitos
        auto soloDigitos = [](const string& s) {
            if (s.empty()) return false;
            for (char c : s) if (c < '0' || c > '9') return false;
            return true;
        };
        if (!soloDigitos(sDia) || !soloDigitos(sMes) || !soloDigitos(sAnio)) return false;

        //convertir a enteros
        int d = 0, m = 0, a = 0;
        try {
            d = stoi(sDia);
            m = stoi(sMes);
            a = stoi(sAnio);
        } catch (...) {
            return false;
        }

        // asignar y validar
        dia = d; mes = m; anio = a;
        if (!esValida()) {
            dia = 1; mes = 1; anio = 2000;
            return false;
        }
        return true;
    }

    bool esValida() const{
        if(anio < 1900) return false;
        if(mes < 1 || mes > 12) return false;
        int diasPorMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; //dias por meses en, feb, mar, abr, may, jun...
        if(dia < 1 || dia > diasPorMes[mes - 1]) return false;
        return true;
    }

    int calcularEdad() const{
        time_t t = time(nullptr);
        tm* now = localtime(&t);
        int anioActual = now->tm_year + 1900;
        int mesActual = now->tm_mon + 1;
        int diaActual = now->tm_mday;

        int edad = anioActual - anio;
        if(mesActual < mes || (mesActual == mes && diaActual < dia)) edad--;
        return edad;
    }

    bool esMayorDeEdad() const{
        return calcularEdad() >= 18;
    }

    bool cumpleEdadCandidato() const{
        int e = calcularEdad();
        return (e >= 25 && e <= 75);
    }

    string toString() const {
        ostringstream ss;
        ss << (dia < 10 ? "0" : "") << dia << "/"
            << (mes < 10 ? "0" : "") << mes << "/"
            << anio;
        return ss.str();
    }

    void mostrar() const{
        cout<< "Fecha: "<< toString() << "(Valida: "<<(esValida() ? "Si" : "No") << ")"<<endl;
    }

    //getters
    int getDia() const {
        return dia;
    }
    int getMes() const {
        return mes;
    }
    int getAnio() const {
        return anio;
    }
    //setters
    void setDia(int d){
        dia = d;
    }
    void setMes(int m){
        mes = m;
    }
    void setAnio(int a){
        anio = a;
    }
};

#endif //FECHA_H