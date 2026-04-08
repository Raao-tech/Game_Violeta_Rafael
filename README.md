# The Haunted Castle — PPROG · Violeta & Rafael & Salva & Javier

> Juego de aventura **textual en ASCII** desarrollado en C para la asignatura de PPROG (UAM · EPS).  
> Versión 3.1 — Marzo 2026

---

## Descripción

El jugador (o jugadores) exploran un mundo compuesto por espacios conectados entre sí mediante **enlaces (Links)** que pueden estar abiertos o cerrados. Es posible recoger objetos, hablar con personajes amistosos y combatir contra enemigos en un sistema de combate por turnos aleatorio.

El motor gráfico renderiza en ASCII la sala actual, las salas adyacentes, el inventario del jugador activo y los mensajes de los personajes.

---
   Se suponne, que este repo es el idoneo
## Ingit — Asistente para colaboradores

El repositorio incluye `Ingit.sh`, un asistente interactivo en Bash pensado para simplificar el flujo de trabajo con Git, especialmente en los **ordenadores compartidos de la uni** donde las credenciales no persisten entre sesiones.

### Primer uso

```bash
chmod +x Ingit.sh   # solo la primera vez
./Ingit.sh
```

La primera vez que se ejecuta, Ingit pide tu nombre y tu username de GitHub y los guarda en `otros/memoria_ingit.txt`. En sesiones posteriores te saluda por tu nombre directamente y muestra las opciones de administración.

### Opciones disponibles

| Opción | Descripción |
|---|---|
| `1` | **Iniciar el repo desde un ordenador de la uni** — configura credenciales temporales con tu token, vincula el remoto, hace `fetch` y sincroniza con `origin/main` |
| `2` | **Iniciar el repo desde tu ordenador personal** |
| `3` | **Guardar y enviar cambios** — hace `pull --rebase` para detectar conflictos, te avisa si hay que resolverlos manualmente y luego ejecuta `add → commit → push` |
| `4` | Visualizar ramas |
| `5` | Estado del proyecto (`git status`) |
| `6` | **Salir** — limpia la pantalla, borra las credenciales globales de git (`user.name`, `user.email`) y elimina el remoto, dejando el equipo limpio para el siguiente usuario |

### Flujo típico en la uni

```bash
./Ingit.sh        # → opción 1: introduces usuario, email y token
# ... trabajas en el código ...
./Ingit.sh        # → opción 3: introduces el mensaje del commit y Ingit sube los cambios
./Ingit.sh        # → opción 6: limpia credenciales antes de dejar el equipo
```

### Gestión de conflictos (opción 3)

Si alguien ha modificado las mismas líneas que tú, Ingit lo detecta durante el `rebase`, te muestra instrucciones para resolver el conflicto manualmente y espera a que pulses `Enter` antes de continuar con el push. No hace falta saber los comandos exactos.

### Memoria entre sesiones

Ingit guarda el estado en `otros/memoria_ingit.txt` (ignorado por `.gitignore` si así está configurado). Almacena el número de aperturas, tu nombre, tu username y la fecha del último uso.

---

## Arquitectura del sistema

### Entidades principales

| Entidad | Descripción |
|---|---|
| **Space** | Sala del mapa, con hasta 4 conexiones direccionales, objetos y personajes |
| **Player** | Jugador con posición, salud e inventario de múltiples objetos |
| **Object** | Objeto que puede estar en un espacio o en el inventario de un jugador |
| **Character** | Personaje NPC con salud, mensajes y atributo `friendly` |
| **Links** | Conexión direccional entre dos espacios, con estado abierto/cerrado |

### Módulos

| Fichero | Responsabilidad |
|---|---|
| `game.c/.h` | Estado global: espacios, jugadores, objetos, personajes, links, turno |
| `space.c/.h` | Sala y sus conexiones, objetos y personajes que contiene |
| `player.c/.h` | Jugador: posición, salud e inventario |
| `object.c/.h` | Objeto (id y nombre) |
| `character.c/.h` | Personaje NPC: salud, mensaje, atributo friendly |
| `inventory.c/.h` | Inventario basado en `Set`, permite múltiples objetos |
| `set.c/.h` | Conjunto de IDs sin duplicados |
| `entity.c/.h` | Entidad base con id y nombre (reutilizada por Player/Object/Character) |
| `links.c/.h` | Enlace entre dos espacios con dirección y estado abierto/cerrado |
| `command.c/.h` | Lectura y traducción de comandos del usuario |
| `game_actions.c/.h` | Aplica una acción al estado del juego según el comando recibido |
| `game_reader.c/.h` | Carga el mundo desde fichero `.dat` |
| `game_loop.c` | Bucle principal: leer → actualizar → renderizar |
| `graphic_engine.c/.h` | Renderizado ASCII con `libscreen` |
| `libscreen.a` | Librería de pintado de pantalla (proporcionada) |

---

## Compilación

**Requisitos:** `gcc`, `make`

```bash
make
```

Limpiar objetos y ejecutable:

```bash
make clean
```

---

## Ejecución

El juego requiere un fichero de datos como argumento:

```bash
./castle castle.dat
```

También está disponible el mapa del hormiguero:

```bash
./castle anthill.dat
```

---

## Comandos disponibles

| Comando | Abreviatura | Descripción |
|---|---|---|
| `next` | `n` | Mover al **sur** |
| `back` | `b` | Mover al **norte** |
| `left` | `l` | Mover al **este** |
| `right` | `r` | Mover al **oeste** |
| `take <objeto>` | `t <objeto>` | Recoger un objeto de la sala actual |
| `drop <objeto>` | `d <objeto>` | Soltar un objeto del inventario |
| `attack <personaje>` | `a <personaje>` | Atacar a un personaje enemigo en la sala |
| `chat <personaje>` | `c <personaje>` | Hablar con un personaje amistoso |
| `exit` | `e` | Salir del juego |

### Reglas de las acciones

- **take**: el objeto debe estar en la sala actual. El inventario admite múltiples objetos.
- **drop**: el jugador debe tener el objeto. El objeto se coloca en la sala actual.
- **attack**: el personaje debe estar en la misma sala, ser **enemigo** (`friendly = FALSE`) y estar vivo. Se lanza un dado 0–9: valores 0–4 el enemigo gana (el jugador pierde 1 HP), valores 5–9 el jugador gana (el enemigo pierde 1 HP). Si la salud del jugador llega a 0, la partida termina.
- **chat**: el personaje debe estar en la misma sala y ser **amistoso** (`friendly = TRUE`). El motor gráfico muestra su mensaje.
- Los **links cerrados** bloquean el movimiento en esa dirección.

---

## Sistema multijugador por turnos

El juego soporta varios jugadores. Cada turno pertenece a un jugador distinto, rotando en orden. La función `game_turn_update` avanza al siguiente jugador tras cada acción.

---

## Formato de los ficheros `.dat`

```
#s:<id>|<nombre>|<gdesc1>|<gdesc2>|<gdesc3>
#o:<id>|<nombre>|<space_id>
#l:<id>|<nombre>|<origin_id>|<destiny_id>|<direction>|<open>
#c:<id>|<nombre>|<space_id>|<health>|<friendly>|<message>
#p:<id>|<nombre>|<space_id>|<health>
```

Donde `<direction>` puede ser `N`, `S`, `E`, `W` y `<open>` es `1` (abierto) o `0` (cerrado).

---

## Comprobación de memoria

```bash
valgrind --leak-check=full --show-leak-kinds=all ./castle castle.dat
```

---

## Notas de diseño

- El módulo `entity` proporciona una base común (id + nombre) para `Player`, `Object` y `Character`, reduciendo duplicación de código.
- `Set` e `Inventory` desacoplan la gestión de colecciones del resto de módulos.
- Los **Links** separan la topología del mapa de los propios espacios, permitiendo puertas con estado abierto/cerrado independientemente de la dirección de traversal.
- El motor gráfico (`graphic_engine`) está deliberadamente desacoplado del estado del juego (`game`) para mantener la separación de responsabilidades.

---

## Tests

El directorio `tests/` contiene pruebas unitarias para los distintos módulos.

```bash
make tests   # si está definido en el Makefile
```

---

**Autores:** Violeta & Rafael & Salva & Javier 
**Asignatura:** Programación (PPROG) — Grado en Ingeniería Informática, UAM  
**Licencia:** GNU Public License