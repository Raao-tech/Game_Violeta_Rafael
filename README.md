# The Haunted Castle — PPROG · Violeta & Rafael & Salva & Javier

> Juego de aventura **textual en ASCII** desarrollado en C para la asignatura de PPROG (UAM · EPS).
> Versión I3 — Abril 2026

---

## Descripción

El jugador (o jugadores) exploran un castillo compuesto por salas conectadas mediante **enlaces (Links)** bidireccionales que pueden estar abiertos o cerrados. Es posible recoger objetos, inspeccionarlos, hablar con personajes amistosos y combatir contra enemigos en un sistema de combate por turnos con resultado aleatorio.

El motor gráfico renderiza en ASCII la sala actual junto con las cuatro salas adyacentes (N/S/E/W), el inventario del jugador activo, las barras de salud y los mensajes de los personajes.

---

## Características destacadas (I3)

- **Multijugador por turnos** — varios jugadores rotan en orden; cada turno pertenece a un jugador distinto.
- **Links bidireccionales** — cada enlace almacena el estado abierto/cerrado de forma independiente por sentido de travesía (`orig→dest` y `dest→orig`), lo que permite pasajes de un solo sentido.
- **Descubrimiento de salas (F12)** — las salas desconocidas se muestran como cajas vacías hasta que el jugador las visita; una vez descubiertas se revelan su `gdesc`, objetos y personajes.
- **Sistema de combate PvP** — `attack` funciona tanto contra NPCs enemigos como contra otros jugadores en la misma sala; el autoataque queda vetado.
- **Motor gráfico enriquecido** — renderiza las 4 salas vecinas, barras de salud ASCII (`[###--] 3 !`), etiquetas `[ALLY]`/`[ENEMY]` en personajes y borde de color por jugador activo.
- **Log de partida (F14)** — bandera `-l <fichero>` que registra cada comando con su resultado `OK`/`ERROR`.
- **Inventario multi-objeto** — el jugador puede llevar varios objetos simultáneamente; `inspect` comprueba tanto el inventario como la sala actual.
- **Memoria limpia** — 0 leaks/errores Valgrind, 0 advertencias con `-Wall -Wextra -Wpedantic`.

---

## Estructura del proyecto

```
.
├── src/                  # Código fuente (.c)
│   ├── game.c
│   ├── game_actions.c
│   ├── game_loop.c
│   ├── game_reader.c
│   ├── graphic_engine.c
│   ├── command.c
│   ├── space.c
│   ├── player.c
│   ├── object.c
│   ├── character.c
│   ├── links.c
│   ├── inventory.c
│   ├── set.c
│   └── entity.c
├── headers/              # Cabeceras (.h)
│   ├── game.h
│   ├── game_actions.h
│   ├── game_reader.h
│   ├── graphic_engine.h
│   ├── command.h
│   ├── space.h
│   ├── player.h
│   ├── object.h
│   ├── character.h
│   ├── links.h
│   ├── inventory.h
│   ├── set.h
│   ├── entity.h
│   └── types.h
├── obj/                  # Objetos compilados (generado por make)
├── doc/                  # Documentación Doxygen (generada por make doc)
├── tests/                # Tests unitarios por módulo
│   ├── Makefile
│   ├── set_test.c / set_test.h
│   ├── entity_test.c / entity_test.h
│   ├── space_test.c / space_test.h
│   ├── player_test.c / player_test.h
│   ├── character_test.c / character_test.h
│   ├── game_test.c / game_test.h
│   ├── command_test.c / command_test.h
│   ├── links_test.c / links_test.h
│   └── object_test.c / object_test.h
├── otros/                # Logs y memoria de Ingit
│   ├── output.log
│   └── memoria_ingit.txt
├── castle.dat            # Mapa principal (el castillo)
├── anthill.dat           # Mapa alternativo (el hormiguero)
├── *.cmd                 # Ficheros de comandos para pruebas de integración
├── libscreen.a           # Librería de pantalla (proporcionada, ASCII only)
├── Makefile
├── Doxyfile
├── Ingit.sh              # Asistente de Git del equipo
└── README.md
```

---

## Arquitectura del sistema

### Entidades principales

| Entidad | Descripción |
|---|---|
| **Space** | Sala del mapa: hasta 4 conexiones (N/S/E/W vía Links), conjunto de objetos, conjunto de personajes y flag de descubrimiento |
| **Player** | Jugador con posición, salud, inventario de múltiples objetos y límite configurable de capacidad |
| **Object** | Objeto que puede estar en un espacio o en el inventario de un jugador |
| **Character** | Personaje NPC con salud, mensaje y atributo `friendly` (amistoso/enemigo) |
| **Links** | Conexión **bidireccional** entre dos espacios con estado abierto/cerrado independiente por sentido de travesía |

### Módulos (TADs)

| Módulo | Responsabilidad |
|---|---|
| `game` | Estado global: arrays de espacios, objetos, personajes, jugadores y links; turno activo; último comando y su resultado |
| `game_reader` | Cinco loaders (`#s`, `#o`, `#c`, `#p`, `#l`) que pueblan el estado desde ficheros `.dat` pipe-delimitados |
| `game_actions` | Dispatcher: mapea cada `CommandCode` a su handler privado (move, take, drop, attack, chat, inspect, exit) |
| `game_loop` | Ciclo RENDER → INPUT → UPDATE → LOG → NEXT TURN; gestión del fichero de log |
| `graphic_engine` | Renderizado ASCII con `libscreen`: sala actual + 4 vecinas, inventario, barras de salud, etiquetas de personajes, borde de color por jugador |
| `command` | Parser de entrada: primer token → `CommandCode`, segundo token → nombre del objeto/objetivo |
| `space` | TAD sala: `gdesc` (3 líneas × 9 chars), sets de objetos y personajes, 4 IDs de link, flag discovered |
| `player` | TAD jugador: posición, salud, `Inventory*`, nombre, id |
| `object` | TAD objeto: id, nombre |
| `character` | TAD personaje: id, nombre, posición, salud, `friendly`, mensaje |
| `links` | TAD enlace: id, nombre, id_orig, id_dest, dirección, `open_orig_to_dest`, `open_dest_to_orig` |
| `inventory` | Wrapper de `Set` que añade límite máximo de capacidad (`max_objs`) |
| `set` | Colección de IDs únicos (array dinámico); operaciones CRUD sobre `Id` |
| `entity` | Base común (id + nombre) para Player, Object y Character |

---

## Compilación

**Requisitos:** `gcc`, `make`

```bash
make          # compila todo → ejecutable ./castle
make clean    # elimina objetos y ejecutable
make clean_all  # elimina también obj/ y doc/
```

Flags usados: `-g -Wall -Wextra -Wpedantic -O0 -DDEBUG`

---

## Ejecución

### Modos disponibles

| Comando make | Descripción |
|---|---|
| `make run` | Ejecuta `./castle castle.dat` |
| `make runv` | Lo mismo bajo Valgrind completo |
| `make run_log` | Ejecuta con log en `otros/output.log` |
| `make runv_log` | Valgrind + log |
| `make play` | Menú interactivo: lista los `.dat` disponibles, permite elegir mapa y activar log |
| `make playv` | Igual que `play` pero bajo Valgrind |
| `make run_custom BD=anthill.dat` | Mapa personalizado por argumento |
| `make test_cmd CMD=game1.cmd` | Alimenta un `.cmd` como stdin y vuelca el log |
| `make doc` | Genera documentación Doxygen en `doc/` |
| `make mandar` | Empaqueta el proyecto en `Game_mandar_RaVi.zip` para entrega |

### Ejecución directa

```bash
./castle castle.dat              # partida normal
./castle castle.dat -l otros/output.log  # partida con log
./castle anthill.dat             # mapa del hormiguero
```

---

## Comandos disponibles en el juego

| Comando | Abreviatura | Argumento | Descripción |
|---|---|---|---|
| `exit` | `e` | — | Termina la partida |
| `move` | `m` | `north`/`south`/`east`/`west` | Mueve al jugador activo en la dirección indicada si el link está abierto |
| `take` | `t` | `<nombre_objeto>` | Recoge un objeto de la sala y lo añade al inventario |
| `drop` | `d` | `<nombre_objeto>` | Deposita un objeto del inventario en la sala actual |
| `attack` | `a` | `<nombre_personaje_o_jugador>` | Ataca a un personaje enemigo o a otro jugador en la misma sala |
| `chat` | `c` | `<nombre_personaje>` | Habla con un personaje amistoso en la misma sala |
| `inspect` | `i` | `<nombre_objeto>` o `space` | Muestra la descripción de un objeto (inventario o sala) o de la sala actual |

---

## Mecánicas de juego

### Movimiento
El movimiento usa `Links`: se busca el link que une la sala actual con la sala destino en la dirección indicada. Si el link existe y `open_orig_to_dest == TRUE`, el movimiento se autoriza.

### Combate (`attack`)
Se lanza un dado 0–9:
- **0–4** → el enemigo gana el intercambio: el jugador activo pierde 1 HP.
- **5–9** → el jugador gana: el objetivo pierde 1 HP.

Si la salud del jugador activo llega a 0, la partida termina. El PvP sigue las mismas reglas; el autoataque retorna `ERROR`.

### Chat
El personaje objetivo debe estar en la misma sala y ser `friendly = TRUE`. El motor gráfico muestra su mensaje en el panel de descripción.

### Descubrimiento (F12)
Las salas se marcan como descubiertas la primera vez que un jugador las visita. El motor gráfico oculta el `gdesc`, objetos y personajes de las salas no descubiertas.

### Sistema multijugador por turnos
`game_turn_update` avanza al siguiente jugador al final de cada acción, rotando en orden cíclico.

---

## Formato de los ficheros `.dat`

```
#s:<id>|<nombre>|<gdesc1>|<gdesc2>|<gdesc3>
#o:<id>|<nombre>|<space_id>
#l:<id>|<nombre>|<origin_id>|<destiny_id>|<direction>|<open>
#c:<id>|<nombre>|<space_id>|<health>|<friendly>|<message>
#p:<id>|<nombre>|<space_id>|<health>
```

- `<direction>`: `N`, `S`, `E`, `W`
- `<open>`: `1` (abierto) o `0` (cerrado)
- Los links de 6 campos (formato unidireccional del enunciado) son compatibles; los de 7 campos añaden `<open_dest_to_orig>` para la extensión bidireccional.

---

## Tests unitarios

El directorio `tests/` contiene pruebas unitarias para todos los módulos.

```bash
cd tests
make            # compila todos los binarios de test
make run_set    # ejecuta set_test
make run_spc    # ejecuta space_test
make run_ply    # ejecuta player_test
# ... etc.

make vset       # set_test bajo Valgrind
make vspc       # space_test bajo Valgrind
# ... etc.
```

---

## Comprobación de memoria

```bash
make runv                        # Valgrind sobre castle.dat
make playv                       # Valgrind con selección interactiva de mapa
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./castle castle.dat
```

Estado actual: **0 leaks, 0 errores** con todas las entidades cargadas desde `castle.dat`.

---

## Ingit — Asistente para colaboradores

El repositorio incluye `Ingit.sh`, un asistente interactivo en Bash pensado para simplificar el flujo de trabajo con Git en los **ordenadores compartidos de la uni**, donde las credenciales no persisten entre sesiones.

### Primer uso

```bash
chmod +x Ingit.sh   # solo la primera vez
./Ingit.sh
```

La primera vez Ingit pide tu nombre y tu username de GitHub y los guarda en `otros/memoria_ingit.txt`.

### Operaciones disponibles

- **Pull** — actualiza la rama actual desde `origin`.
- **Push** — sube los cambios con mensaje de commit personalizado.
- **Status** — muestra el estado del repositorio.
- **Log** — muestra el historial de commits.
- **Checkout** — cambia de rama o crea una nueva.
- **Merge** — fusiona una rama en la actual.
- **Reset** — deshace el último commit (soft o hard).

---

## Notas de diseño

- El módulo `entity` proporciona una base común (id + nombre) para `Player`, `Object` y `Character`, reduciendo la duplicación de código.
- `Set` e `Inventory` desacoplan la gestión de colecciones del resto de módulos; `Inventory` añade la capa de límite de capacidad sobre `Set`.
- Los **Links** separan la topología del mapa de los propios espacios, permitiendo puertas con estado abierto/cerrado **independiente por dirección** (extensión bidireccional consciente sobre el enunciado).
- El motor gráfico (`graphic_engine`) está deliberadamente desacoplado del estado del juego (`game`) para mantener la separación de responsabilidades.
- `calloc` en lugar de `malloc` para arrays de punteros y contadores, garantizando que todos los campos queden a cero desde la creación.
- `libscreen` solo admite caracteres ASCII de un byte: ningún carácter multibyte (Unicode) debe usarse en `gdesc` ni en los buffers de pantalla.

---

**Autores:** Violeta & Rafael & Salva & Javier
**Asignatura:** Programación (PPROG) — Grado en Ingeniería Informática, UAM
**Licencia:** GNU Public License
