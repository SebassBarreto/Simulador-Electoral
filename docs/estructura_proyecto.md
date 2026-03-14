# Estructura del proyecto

```
ProyectoElectoral/
├─ .gitignore
├─ README.md
├─ main.cpp
├─ data/
│  └─ maestros/
│     ├─ regiones.txt
│     ├─ ciudades.txt
│     ├─ partidos.txt
│     ├─ candidatos_alcaldia.txt
│     └─ candidatos_presidencia.txt
├─ include/
│  ├─ listas/
│  ├─ arboles/
│  ├─ nodos/
│  ├─ pilas_colas/
│  └─ algoritmos/
├─ src/
│  ├─ modelos/
│  │  ├─ Region.h / .cpp
│  │  ├─ Ciudad.h / .cpp
│  │  ├─ Partido.h / .cpp
│  │  ├─ Candidato.h / .cpp
│  │  ├─ Voto.h / .cpp
│  │  └─ ResultadoElectoral.h / .cpp
│  ├─ gestores/
│  │  ├─ GestorArchivos.h / .cpp
│  │  ├─ GestorRegiones.h / .cpp
│  │  ├─ GestorCiudades.h / .cpp
│  │  ├─ GestorPartidos.h / .cpp
│  │  ├─ GestorCandidatos.h / .cpp
│  │  └─ GestorSimulacion.h / .cpp
│  ├─ indices/
│  │  ├─ IndiceAVL.h / .cpp
│  │  └─ MultilistaElectoral.h / .cpp
│  ├─ sistema/
│  │  └─ SistemaElectoral.h / .cpp
│  ├─ menus/
│  │  └─ MenuPrincipal.h / .cpp
│  └─ utilidades/
│     ├─ Fecha.h / .cpp
│     └─ Aleatorio.h / .cpp
└─ docs/
   ├─ diseño_soluicion.md
   ├─ estructura_proyecto.md
   └─ manual_usuario.md
```

## Detalle por carpetas

- data/maestros: Archivos planos maestros para cargar entidades.
- include/*: Estructuras de datos genéricas (listas, árboles, nodos, pilas/colas) y algoritmos de soporte.
- src/modelos: Clases del dominio electoral.
- src/gestores: Manejo de persistencia y procesos auxiliares (simulación).
- src/indices: Índices y multilistas para eficientar consultas.
- src/sistema: Clase orquestadora del sistema.
- src/menus: Componentes de interacción por consola.
- src/utilidades: Funciones generales (edad, aleatoriedad).
- docs: Documentación del proyecto.

## Archivos de datos esperados
- regiones.txt: nombre región | ciudades asociadas (o ids) | censo agregado
- ciudades.txt: nombre ciudad | región | censo (aleatorio generado)
- partidos.txt: nombre partido | representante legal
- candidatos_alcaldia.txt: datos personales | ciudad residencia | partido
- candidatos_presidencia.txt: datos personales | partido | dupla vicepresidencial
