#ifndef MENUESTADISTICAS_H
#define MENUESTADISTICAS_H

#include <iostream>
#include <string>
#include <map>
#include "../sistema/SistemaElectoral.h"
#include "../sistema/Estadisticas.h"
#include "../sistema/Simulador.h"

using namespace std;

class MenuEstadisticas {
private:
    SistemaElectoral& sys;
    
    void pausar() {
        cout << "Presione ENTER para continuar...";
        cin.ignore(10000, '\n');
    }
    
public:
    MenuEstadisticas(SistemaElectoral& s) : sys(s) {}

    void mostrar() {
        bool salir = false;
        while (!salir) {
            cout << "\n-- Menu Estadisticas --\n";
            cout << "1. Generar todas las estadisticas (requiere simulacion)\n";
            cout << "2. Ver censo por region\n";
            cout << "3. Volver\n";
            cout << "Opcion: ";
            int op; 
            if (!(cin >> op)) { cin.clear(); cin.ignore(10000, '\n'); continue; }
            cin.ignore(10000, '\n');
            
            if (op == 1) {
                generarTodasEstadisticas();
            } else if (op == 2) {
                Estadisticas::estadisticaCensoPorRegion(sys, "data/resultados/estadisticas_presidencia_por_region.txt");
                cout << "Generado en data/resultados/estadisticas_presidencia_por_region.txt\n";
                pausar();
            } else if (op == 3) {
                salir = true;
            }
        }
    }
    
    void generarTodasEstadisticas() {
        cout << "\n=== Generando estadisticas completas ===\n";
        
        unsigned int seed; 
        cout << "Seed para simulacion (entero): "; 
        if (!(cin >> seed)) { cin.clear(); cin.ignore(10000, '\n'); seed = 12345; }
        cin.ignore(10000, '\n');
        
        Simulador::seed(seed);
        
        // Collect results from all cities for alcaldía
        Lista<ResultadoCiudad> resultadosAlcaldia;
        Lista<ResultadoCiudad> resultadosPresidencia;
        
        const Lista<Ciudad>& ciudades = sys.getCiudades();
        
        cout << "Simulando alcaldias para todas las ciudades...\n";
        for (int i = 0; i < ciudades.obtenerTamano(); ++i) {
            Ciudad c;
            if (!ciudades.obtenerElemento(i, c)) continue;
            
            ResultadoElectoral resAlc(0);
            string partidoGanador;
            char sexoGanador = 'M';
            map<string, int> votosPorPartido;
            int votosHombres = 0, votosMujeres = 0;
            
            if (Simulador::simularAlcaldiaCiudadExtendido(sys, c.getId(), resAlc,
                                                          partidoGanador, sexoGanador,
                                                          votosPorPartido,
                                                          votosHombres, votosMujeres)) {
                ResultadoCiudad rc;
                rc.idCiudad = c.getId();
                rc.nombreCiudad = c.getNombre();
                rc.idRegion = c.getIdRegion();
                rc.censo = resAlc.getCensoElectoral();
                rc.votosEmitidos = resAlc.getTotalVotosEmitidos();
                rc.votosEnBlanco = resAlc.getVotosEnBlanco();
                rc.votosNulos = resAlc.getVotosNulos();
                rc.abstencion = resAlc.getAbstencion();
                rc.ganador = resAlc.getGanador();
                rc.partidoGanador = partidoGanador;
                rc.sexoGanador = sexoGanador;
                rc.votosPorPartido = votosPorPartido;
                rc.votosHombres = votosHombres;
                rc.votosMujeres = votosMujeres;
                resultadosAlcaldia.insertarFinal(rc);
            }
        }
        
        // Generate alcaldía statistics
        cout << "Generando estadisticas de alcaldias...\n";
        Estadisticas::estadisticaAlcaldiaPorRegion(sys, resultadosAlcaldia,
            "data/resultados/estadisticas_alcaldia_por_region.txt");
        Estadisticas::estadisticaAlcaldiaDetallePorRegion(sys, resultadosAlcaldia,
            "data/resultados/estadisticas_alcaldia_detalle_por_region.txt");
        Estadisticas::estadisticaAlcaldiaTotalesNacionales(sys, resultadosAlcaldia,
            "data/resultados/estadisticas_alcaldia_totales_nacionales.txt");
        
        // Simulate presidential election by city
        cout << "Simulando presidencia por departamento...\n";
        Simulador::seed(seed); // Reset seed for consistency
        for (int i = 0; i < ciudades.obtenerTamano(); ++i) {
            Ciudad c;
            if (!ciudades.obtenerElemento(i, c)) continue;
            
            ResultadoElectoral resPres(0);
            map<string, int> votosPorPartido;
            int votosHombres = 0, votosMujeres = 0;
            
            if (Simulador::simularPresidenciaPorCiudad(sys, c.getId(), resPres,
                                                       votosPorPartido,
                                                       votosHombres, votosMujeres)) {
                ResultadoCiudad rc;
                rc.idCiudad = c.getId();
                rc.nombreCiudad = c.getNombre();
                rc.idRegion = c.getIdRegion();
                rc.censo = resPres.getCensoElectoral();
                rc.votosEmitidos = resPres.getTotalVotosEmitidos();
                rc.votosEnBlanco = resPres.getVotosEnBlanco();
                rc.votosNulos = resPres.getVotosNulos();
                rc.abstencion = resPres.getAbstencion();
                rc.ganador = resPres.getGanador();
                rc.votosPorPartido = votosPorPartido;
                rc.votosHombres = votosHombres;
                rc.votosMujeres = votosMujeres;
                resultadosPresidencia.insertarFinal(rc);
            }
        }
        
        // National presidential simulation
        cout << "Simulando presidencia nacional...\n";
        Simulador::seed(seed);
        ResultadoElectoral resPaisTotal(0);
        Simulador::simularPresidenciaPais(sys, resPaisTotal);
        
        // Generate presidential statistics
        cout << "Generando estadisticas de presidencia...\n";
        Estadisticas::estadisticaPresidenciaPorRegion(sys, resultadosPresidencia,
            "data/resultados/estadisticas_presidencia_por_region.txt");
        Estadisticas::estadisticaPresidenciaPais(sys, resPaisTotal,
            "data/resultados/estadisticas_presidencia_pais.txt");
        Estadisticas::estadisticaPresidenciaDetallePorDepartamento(sys, resultadosPresidencia,
            "data/resultados/estadisticas_presidencia_detalle_por_departamento.txt");
        Estadisticas::estadisticaPresidenciaDetallePais(sys, resPaisTotal, resultadosPresidencia,
            "data/resultados/estadisticas_presidencia_detalle_pais.txt");
        
        cout << "\n=== Estadisticas generadas exitosamente ===\n";
        cout << "Archivos generados en data/resultados/:\n";
        cout << "  - estadisticas_alcaldia_por_region.txt\n";
        cout << "  - estadisticas_alcaldia_detalle_por_region.txt\n";
        cout << "  - estadisticas_alcaldia_totales_nacionales.txt\n";
        cout << "  - estadisticas_presidencia_por_region.txt\n";
        cout << "  - estadisticas_presidencia_pais.txt\n";
        cout << "  - estadisticas_presidencia_detalle_por_departamento.txt\n";
        cout << "  - estadisticas_presidencia_detalle_pais.txt\n";
        
        pausar();
    }
};

#endif // MENUESTADISTICAS_H