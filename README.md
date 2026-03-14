# Sistema de Simulación Electoral - Proyecto Final 2025-3

## Descripción
Sistema desarrollado en C++ para la **Registraduría Nacional del Estado Civil** que simula elecciones simultáneas de alcaldes locales y presidencia. El sistema genera tarjetones aleatorios, realiza consultas y estadísticas, y mantiene la persistencia de datos en archivos planos.

## Objetivos
- Simular procesos electorales para alcaldías y presidencia
- Generar tarjetones electorales aleatorios incluyendo voto en blanco
- Procesar y analizar resultados electorales
- Proporcionar consultas y estadísticas detalladas
- Mantener persistencia de datos en archivos planos

## Arquitectura
- Modelos (Region, Ciudad, Partido, Candidato, Voto, ResultadoElectoral)
- Gestores de archivos y entidades (carga/guardado)
- Índices AVL para búsquedas eficientes
- Multilistas para relaciones región-ciudad-partido-candidatos
- Menú principal para interacción
- Utilidades para cálculo de edad y aleatoriedad

## Estructura del proyecto
- `src/modelos`: clases de dominio
- `src/gestores`: carga/guardado y lógica auxiliar
- `src/indices`: `IndiceAVL`, `MultilistaElectoral`
- `src/sistema`: `SistemaElectoral` (orquestador)
- `src/menus`: `MenuPrincipal`
- `include/*`: listas genéricas y estructuras
- `data/maestros`: archivos planos maestros

## Ejecución
1. Compilar con Dev-C++ o cualquier compilador C++ estándar.
2. Asegurar que existan los archivos en `data/maestros/`:
   - `regiones.txt`, `ciudades.txt`, `partidos.txt`,
   - `candidatos_alcaldia.txt`, `candidatos_presidencia.txt`
3. Ejecutar el binario; se cargan datos y se muestra el menú principal.

## Funcionalidades clave
- Consultas por partido y región, por ciudad, y listados de presidencia.
- Tarjetones de alcaldía (por ciudad) y de presidencia (con dupla).
- Censo electoral por ciudad y total nacional.
- Simulación de votos con blancos, nulos y abstención; resultados y estadísticas.
- Control de modificaciones durante simulación.

## Notas
- La edad se calcula desde la fecha de nacimiento (no se guarda como atributo).
- Candidatos a alcaldía requieren residir en la ciudad donde compiten.
- Los cambios se guardan al finalizar la ejecución/simulación.
