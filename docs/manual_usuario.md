# Manual de Usuario

Este manual guía el uso del Sistema de Simulación Electoral por consola.

## Requisitos
- Compilador Dev-C++ (o compatible C++).
- Archivos planos en `data/maestros/`:
  - `regiones.txt`, `ciudades.txt`, `partidos.txt`,
  - `candidatos_alcaldia.txt`, `candidatos_presidencia.txt`.

## Inicio
1. Compile el proyecto (abra `main.cpp` en Dev-C++ y construya).
2. Ejecute el binario.
3. Si la carga es exitosa, verá el menú principal.

## Menú Principal
- Consultas:
  - Por partido y región: lista de candidatos a alcaldía con nombre, edad, sexo.
  - Por partido: candidatos a alcaldías de capitales.
  - Presidencia: lista de candidatos y sus vicepresidentes con datos personales.
  - Por ciudad: por cada partido, el candidato a la alcaldía.
  - Censo electoral: por ciudad y total nacional.
- Tarjetones:
  - Alcaldía por ciudad: incluye 0. Voto en blanco, 1..n candidatos.
  - Presidencia: incluye voto en blanco, partido y dupla presidente–vicepresidente.
- Simulación:
  - Iniciar simulación de votos (alcaldía y presidencia), con blancos, nulos y abstención.
  - Visualizar resultados:
    - Por ciudad: votos y porcentajes, ganador.
    - Nacional presidencia: votos y porcentajes, ganador.
  - Estadísticas:
    - Alcaldías: general y detallado por región, totales nacionales por partido y sexo.
    - Presidencia: general por región y país, detallado por departamento y total país, verificación de segunda vuelta.
- Guardado:
  - Guardar cambios para futuras ejecuciones.

## Reglas durante simulación
- No se pueden agregar ni eliminar candidatos mientras la simulación está activa.
- La sumatoria de votos no supera el censo electoral de la ciudad (alcaldía) ni el total nacional (presidencia).

## Notas de uso
- La edad de las personas se calcula a partir de su fecha de nacimiento (no se edita manualmente).
- Para candidatos a alcaldía, la ciudad de residencia debe coincidir con la ciudad donde compiten; de lo contrario no serán registrados.
- Si algún archivo falta o tiene formato inválido, el sistema informará error al cargar.

## Formatos de archivo sugeridos
- Campos separados por delimitador (por ejemplo, `|`), una entidad por línea.
- Usar nombres de regiones y ciudades consistentes entre archivos.

## Solución de problemas
- “Error cargando datos iniciales”: verifique rutas y contenido de `data/maestros`.
- Candidatos no aparecen en tarjetón de alcaldía: revise que su ciudad de residencia exista y coincida; que el partido exista.
- Estadísticas vacías: asegúrese de ejecutar la simulación antes de consultar resultados.
