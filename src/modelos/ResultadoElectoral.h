#ifndef RESULTADOELECTORAL_H
#define RESULTADOELECTORAL_H

#include <map>
#include <string>
#include <iomanip>
#include <sstream>
#include <algorithm>

using namespace std;

class ResultadoElectoral {
private:
    map<string, int> votosPorCandidato; // candidato -> votos
    int votosEnBlanco;
    int votosNulos;
    int abstencion;
    int censoElectoral;
    string ganador;
    bool requiereSegundaVuelta;

    // Validar que no haya sobrepaso del censo
    bool validarVoto() const {
        return getTotalVotosValidos() + votosNulos + votosEnBlanco + abstencion <= censoElectoral;
    }

public:
    // Constructor
    ResultadoElectoral(int censo)
        : votosEnBlanco(0), votosNulos(0), abstencion(0),
            censoElectoral(censo), ganador(""), requiereSegundaVuelta(false) {
        if (censo <= 0) {
            censoElectoral = 0;
        }
    }

    // Setters
    bool agregarVoto(const string& candidato) {
        if (!validarVoto()) return false;
        votosPorCandidato[candidato]++;
        return true;
    }

    bool agregarVotoEnBlanco() {
        if (!validarVoto()) return false;
        votosEnBlanco++;
        return true;
    }

    bool agregarVotoNulo() {
        if (!validarVoto()) return false;
        votosNulos++;
        return true;
    }

    bool agregarAbstencion() {
        if (abstencion < censoElectoral) {
            abstencion++;
            return true;
        }
        return false;
    }

    void calcularResultados() {
        if (votosPorCandidato.empty()) {
            ganador = "Sin candidatos";
            requiereSegundaVuelta = false;
            return;
        }

        auto maxCandidato = max_element(
            votosPorCandidato.begin(),
            votosPorCandidato.end(),
            [](const pair<const string, int>& a, const pair<const string, int>& b) {
                return a.second < b.second;
            });

        ganador = maxCandidato->first;
        int votosGanador = maxCandidato->second;
        int totalValidos = getTotalVotosValidos();

        // Calcular si requiere segunda vuelta
        if (totalValidos > 0) {
            double porcentajeGanador = (static_cast<double>(votosGanador) / totalValidos) * 100.0;
            requiereSegundaVuelta = (porcentajeGanador < 50.0);
        } else {
            requiereSegundaVuelta = false;
        }
    }

    void setGanador(const string& g) {
        ganador = g;
    }

    void setRequiereSegundaVuelta(bool requiere) {
        requiereSegundaVuelta = requiere;
    }

    // Getters
    int getVotos(const string& candidato) const {
        auto it = votosPorCandidato.find(candidato);
        if (it != votosPorCandidato.end()) {
            return it->second;
        }
        return 0;
    }

    int getVotosEnBlanco() const { return votosEnBlanco; }
    int getVotosNulos() const { return votosNulos; }
    int getAbstencion() const { return abstencion; }
    string getGanador() const { return ganador; }
    bool getRequiereSegundaVuelta() const { return requiereSegundaVuelta; }
    int getCensoElectoral() const { return censoElectoral; }

    // Obtener copia del mapa
    const map<string, int>& getVotosPorCandidato() const {
        return votosPorCandidato;
    }

    // Calculos CORREGIDOS
    int getTotalVotosValidos() const {
        int total = votosEnBlanco; // Votos en blanco son validos
        for (const auto& par : votosPorCandidato) {
            total += par.second;
        }
        return total;
    }

    int getTotalVotosEmitidos() const {
        return getTotalVotosValidos() + votosNulos;
    }

    int getParticipacion() const {
        return getTotalVotosEmitidos() + abstencion;
    }

    // Porcentajes sobre censo electoral
    float getPorcentajeSobreCenso(int votos) const {
        if (censoElectoral == 0) return 0.0f;
        return (static_cast<float>(votos) / censoElectoral) * 100.0f;
    }

    // Porcentajes sobre votos validos
    float getPorcentajeSobreValidos(int votos) const {
        int totalValidos = getTotalVotosValidos();
        if (totalValidos == 0) return 0.0f;
        return (static_cast<float>(votos) / totalValidos) * 100.0f;
    }

    // toString mejorado
    string toString() const {
        stringstream ss;
        ss << "RESULTADOS ELECTORALES"<<endl;

        ss << "Censo electoral: " << censoElectoral <<endl;
        ss << "Votos emitidos: " << getTotalVotosEmitidos()
            << " (" << fixed << setprecision(2)
            << getPorcentajeSobreCenso(getTotalVotosEmitidos()) << "%)" << endl;
        ss << "Abstencion: " << abstencion
            << " (" << getPorcentajeSobreCenso(abstencion) << "%)" << endl;

        ss << "VOTOS VaLIDOS: " << endl;
        for (const auto& par : votosPorCandidato) {
            ss << "  " << left << setw(20) << par.first
                << ": " << right << setw(6) << par.second << " votos"
                << " (" << getPorcentajeSobreValidos(par.second) << "% validos)"
                << " (" << getPorcentajeSobreCenso(par.second) << "% censo)" << endl;
        }

        ss << "Votos en blanco: " << votosEnBlanco
            << " (" << getPorcentajeSobreValidos(votosEnBlanco) << "% validos)" << endl;
        ss << "Votos nulos: " << votosNulos << endl;
        ss << "Total validos: " << getTotalVotosValidos() << endl;

        ss << "GANADOR: " << ganador << endl;
        if (requiereSegundaVuelta) {
            ss << "SE REQUIERE SEGUNDA VUELTA" << endl;
        }
        return ss.str();
    }
};

#endif // RESULTADOELECTORAL_H
