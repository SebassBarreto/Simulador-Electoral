# Diseño de la solución

Este documento describe la arquitectura, modelos de datos, estructuras en memoria y flujos principales del Sistema de Simulación Electoral.

## Objetivos de diseño
- Alta cohesión y bajo acoplamiento mediante módulos: modelos, gestores, índices, multilistas y menús.
- Persistencia en archivos planos con carga/guardado al inicio/fin.
- Eficiencia en búsquedas mediante índices AVL y multilistas.
- Portabilidad: compila en Dev-C++ sin dependencias especiales.

## Arquitectura por módulos

- src/modelos
  - Region: id, nombre, censo (derivado), lista de ciudades (referencias/ids).
  - Ciudad: id, nombre, idRegión, censo (generado aleatoriamente).
  - Partido: id, nombre, representanteLegal.
  - Candidato: atributos personales, partido, tipo (ALCALDÍA | PRESIDENCIA), pareja vicepresidencial (para presidencia).
  - Voto: tipo (válido, blanco, nulo, abstención), destino (id candidato o categoría).
  - ResultadoElectoral: agregaciones de votos y porcentajes.

- src/gestores
  - GestorArchivos: utilidad base para lectura/escritura.
  - GestorRegiones, GestorCiudades, GestorPartidos, GestorCandidatos: carga y guardado de cada entidad desde/ hacia archivos planos.
  - GestorSimulacion: generación aleatoria de votos, validación censo y sumatoria.

- src/indices
  - IndiceAVL: índice clave→valor para búsquedas O(log n).
    - Ciudad por nombre
    - Partido por nombre
    - Candidato por identificación
    - Presidente por partido
  - MultilistaElectoral: relaciones:
    - regiones ↔ ciudades
    - ciudades ↔ partidos
    - ciudades+partido ↔ candidatos de alcaldía

- src/sistema
  - SistemaElectoral: orquestador. Carga/guardado, construcción de índices, control de simulación, acceso a multilistas, consultas.

- src/menus
  - MenuPrincipal: navegación de consultas, tarjetones, simulación y reportes.

- src/utilidades
  - Fecha y cálculo de edad desde fecha de nacimiento.
  - Aleatoriedad y distribución de votos.

## Estructuras de datos en memoria

- Lista<T> (include/listas/Lista.h): lista genérica para entidades cargadas.
- IndiceAVL: clave string → int (id/posición), soporta insertar, buscar, limpiar.
- MultilistaElectoral:
  - RegionesCiudades: mapa idRegión → lista de idCiudad
  - CiudadPartidos: mapa idCiudad → lista de idPartido
  - CiudadPartidoCandidatosAlcaldía: par (idCiudad, idPartido) → lista de idCandidato

Estas estructuras evitan recorridos completos y habilitan consultas eficientes.

## Persistencia (archivos planos)
- data/maestros/regiones.txt
- data/maestros/ciudades.txt
- data/maestros/partidos.txt
- data/maestros/candidatos_alcaldia.txt
- data/maestros/candidatos_presidencia.txt

Durante inicialización se cargan y se construyen índices. Al finalizar (o tras simulación) se guardan los cambios.

## Flujos principales

1) Inicialización
- Cargar maestros.
- Construir índices:
  - Ciudad: nombre → id
  - Partido: nombre → id
  - Candidato: identificación → posición (offset para presidencia)
  - Presidente por partido: idPartido → idCandidato
- Construir multilistas (regiones↔ciudades; ciudad↔partidos; registrar candidatos de alcaldía por ciudad y partido respetando residencia).

2) Consultas
- Por partido y región: candidatos a alcaldía (nombre, edad, sexo).
- Por partido: candidatos a alcaldías en capitales (ciudad, nombre, sexo, edad).
- Presidencia: lista de candidatos y vicepresidencias (nombre, edad, ciudad de nacimiento, estado civil).
- Por ciudad: por cada partido, candidato a la alcaldía.
- Tarjetones: alcaldía por ciudad; presidencia con partido y dupla.
- Censo electoral: por ciudad y total nacional.
- Edad: calculada a partir de la fecha de nacimiento en tiempo de ejecución.

3) Simulación
- Bloquear altas/bajas de candidatos cuando simulando = true.
- Generar votos aleatorios:
  - Distribuir en candidatos, voto en blanco, nulos y abstención.
  - No superar censo por ciudad; presidencia suma por país.
- Resultados:
  - Por ciudad: alcaldía y presidencia, con porcentajes y ganador.
  - Nacional presidencia: totales y ganador.
- Estadísticas:
  - Alcaldías:
    - General por región: votos totales, blancos, nulos, abstención (cantidad/porcentaje).
    - Detallado por región: alcaldes por partido, totales por sexo (cantidad/porcentaje).
    - Totales nacional: por partido y por sexo.
  - Presidencia:
    - General por región y total país: votos, blancos, nulos, abstención (cantidad/porcentaje).
    - Detallado por departamento: votos por partido y por sexo.
    - Detallado nacional: votos por partido y por sexo.
    - Segunda vuelta: determinar si hay >50%; si no, top 2.

4) Guardado final
- Persistir cambios y relaciones construidas en archivos planos (incluyendo estructuras multilista cuando aplique).

## Decisiones y validaciones clave
- Registro de candidatos a alcaldía solo si su ciudad de residencia existe y coincide con la ciudad donde compiten.
- Índices para acceso rápido por nombre e identificación.
- Edad no almacenada: se calcula desde fecha de nacimiento.
- Simulación respeta censo electoral (no excede).
- Separación clara entre datos maestros y resultados simulados.

## Posibles extensiones
- Validación de integridad de datos (duplicados, ids).
- Exportación de reportes en CSV.
- Tests unitarios básicos para índices y gestores.
