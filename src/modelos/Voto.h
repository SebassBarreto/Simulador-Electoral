#ifndef VOTO_H
#define VOTO_H

#include <iostream>
#include <string>
#include <sstream>
#include <cstdint>

using namespace std;

/**
 * Tipos de voto segun el enunciado:
 * - Valido: voto para un candidato
 * - Blanco: opcion 0 en tarjeton
 * - Nulo: voto invalido por marcaciones incorrectas
 * - Abstencion: no emitio voto (se contabiliza para simulacion, no es un voto sobre tarjeton)
 */

enum class TipoVoto : uint8_t {
    Valido = 0,
    Blanco = 1,
    Nulo   = 2,
    Abstencion = 3
};

class Voto{
private:
    int idCiudad; // ciudad donde se emite (censo y agregacion por region)
    int idPartido;
    int idCandidato;
    TipoVoto tipo;
    char sexo; //M o F segun solicitado para estadisticas por genero

public: 
    Voto(): idCiudad(-1), idPartido(0), idCandidato(-1), tipo(TipoVoto::Nulo), sexo('M'){}

    //voto valido (candidato elegido)
    Voto(int ciudad, int partido, int candidato, char sexoV): idCiudad(ciudad), idPartido(partido), idCandidato(candidato),
    tipo(TipoVoto::Valido), sexo((sexoV=='M'||sexoV=='F')?sexoV:'M'){} 

    //voto blanco/nulo/abstencion (sin candidato - partido)
    Voto(TipoVoto t, int ciudad, char sexoV): idCiudad(ciudad), idPartido(0), idCandidato(-1), tipo(t), sexo((sexoV=='M'||sexoV=='F')?sexoV:'M') {} 

    //getters
    int getIdCiudad() const{ 
        return idCiudad;
    }

    int getIdPartido() const{
        return idPartido;
    }

    int getIdCandidato() const{
        return idCandidato;
    }

    TipoVoto getTipo() const{
        return tipo;
    }

    char getSexo() const{
        return sexo;
    }

    //setters
    void setIdCiudad(int c){
        idCiudad = c;
    }
    void setIdPartido(int p){
        idPartido = p;
    }
    void setIdCandidato(int c){
        idCandidato = c;
    }
    void setTipo(TipoVoto t){
        tipo = t;
    }
    void setSexo(char s){
        sexo = (s=='M'||s=='F')?s:'M'; 
    }

    //utilidades
    static const char* tipoStr(TipoVoto t){
        switch(t){
            case TipoVoto::Valido:  return "VALIDO";
            case TipoVoto::Blanco:  return "BLANCO";
            case TipoVoto::Nulo:  return "NULO";
            case TipoVoto::Abstencion:  return "ABSTENCION";
        }
        return "DESCONOCIDO";
    }

    static char tipoChar(TipoVoto t) {
        switch (t) {
            case TipoVoto::Valido:     return 'V';
            case TipoVoto::Blanco:     return 'B';
            case TipoVoto::Nulo:       return 'N';
            case TipoVoto::Abstencion: return 'X';
        }
        return '?';
    }

    bool esValido() const{
        return tipo == TipoVoto::Valido;
    }
    bool esBlanco() const{
        return tipo == TipoVoto::Blanco;
    }
    bool esNulo() const{
        return tipo == TipoVoto::Nulo;
    }
    bool esAbstencion() const{
        return tipo == TipoVoto::Abstencion;
    }

    string toString() const{
        stringstream ss;
        ss << "Voto{ ciudad="<<idCiudad
            <<", tipo="<<tipoStr(tipo)
            <<", sexo="<<sexo;
        if(esValido()){
            ss << ", partido="<<idPartido
                <<", candidato="<<idCandidato;
        }

        ss << "}";
        return ss.str();
    }

    //serializacion a archivo plano
    std::string toFileFormat() const {
        std::stringstream ss;
        ss << idCiudad << "|"
            << tipoChar(tipo) << "|"
            << sexo << "|"
            << idPartido << "|"
            << idCandidato;
        return ss.str();
    }

    //parse desde línea de archivo plano: idCiudad|tipo|sexo|idPartido|idCandidato
    //retorna true si el parseo fue exitoso
    bool fromFileFormat(const std::string& linea) {
        //busqueda manual de separadores para compatibilidad con dev c++
        int campos[4] = { -1, -1, -1, -1 }; // posiciones de '|'
        int countSep = 0;
        for (int i = 0; i < (int)linea.size() && countSep < 4; ++i) {
            if (linea[i] == '|') {
                campos[countSep++] = i;
            }
        }
        if (countSep != 4) return false;

        string sCiudad     = linea.substr(0, campos[0]);
        string sTipo       = linea.substr(campos[0] + 1, campos[1] - campos[0] - 1);
        string sSexo       = linea.substr(campos[1] + 1, campos[2] - campos[1] - 1);
        string sPartido    = linea.substr(campos[2] + 1, campos[3] - campos[2] - 1);
        string sCandidato  = linea.substr(campos[3] + 1);

        //conversiones basicas
        int ciudad = -1, partido = 0, candidato = -1;
        try {
            ciudad    = std::stoi(sCiudad);
            partido   = std::stoi(sPartido);
            candidato = std::stoi(sCandidato);
        } catch (...) {
            return false;
        }

        //tipo
        TipoVoto t = TipoVoto::Nulo;
        if (sTipo.size() == 1) {
            char tc = sTipo[0];
            if (tc == 'V') t = TipoVoto::Valido;
            else if (tc == 'B') t = TipoVoto::Blanco;
            else if (tc == 'N') t = TipoVoto::Nulo;
            else if (tc == 'X') t = TipoVoto::Abstencion;
            else return false;
        } else {
            return false;
        }

        //sexo
        char sx = (sSexo.empty() ? 'M' : sSexo[0]);
        if (sx != 'M' && sx != 'F') sx = 'M';

        idCiudad    = ciudad;
        idPartido   = partido;
        idCandidato = candidato;
        tipo        = t;
        sexo        = sx;
        return true;
    }

    //comparadores para estructuras genericas y algoritmos
    bool operator==(const Voto& other) const {
        return idCiudad == other.idCiudad &&
                idPartido == other.idPartido &&
                idCandidato == other.idCandidato &&
                tipo == other.tipo &&
                sexo == other.sexo;
    }

    bool operator<(const Voto& other) const {
        if (idCiudad != other.idCiudad) return idCiudad < other.idCiudad;
        if (tipo != other.tipo)         return (int)tipo < (int)other.tipo;
        if (idPartido != other.idPartido) return idPartido < other.idPartido;
        return idCandidato < other.idCandidato;
    }

    //impresion compatible con nodos genericos
    friend ostream& operator<<(ostream& os, const Voto& v) {
        os << v.toString();
        return os;
    }

};


#endif //VOTO_H