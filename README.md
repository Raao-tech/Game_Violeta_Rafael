# The Haunted Castle (PPROG) — Violeta & Rafael

Juego de aventura **textual** (ASCII) desarrollado en C para la práctica I1 de PPROG (UAM). El motor pinta un “mapa” con la sala actual y sus salas adyacentes, permite moverse por el castillo y **recoger/soltar** un objeto.

## Qué hay en el proyecto

### Idea general
- El mundo está compuesto por **espacios (Space)** conectados entre sí (N/S/E/W).
- El jugador (**Player**) está siempre en exactamente un espacio.
- Existe un **objeto (Object)** que puede estar:
  - En un espacio, o
  - En el inventario del jugador (tomado).
- El bucle de juego lee un comando, actualiza el estado y vuelve a pintar la pantalla.

### Consistencia del modelo (reglas que se respetan)
- **Un objeto no puede estar en dos sitios a la vez**: al moverlo, primero se elimina de cualquier espacio que lo contenga y luego se coloca en el destino.
- **Take (t)** solo funciona si:
  - el jugador **no** lleva ya un objeto, y
  - el objeto está en la **misma sala** que el jugador.
- **Drop (d)** solo funciona si el jugador está llevando un objeto.

## Estructura por módulos

- `game.c/.h`: estado global del juego (espacios, jugador, objeto, último comando, etc.).
- `space.c/.h`: definición de espacio y conexiones N/S/E/W + objeto en sala.
- `player.c/.h`: jugador, posición y objeto en inventario.
- `object.c/.h`: objeto (id y nombre).
- `command.c/.h`: lectura y traducción de comandos del usuario.
- `game_actions.c/.h`: aplica una acción al juego según el comando.
- `game_reader.c/.h`: carga el mundo desde fichero `.dat`.
- `graphic_engine.c/.h`: renderizado ASCII con `libscreen`.
- `libscreen.a` / `headers/libscreen.h`: librería de pintado (proporcionada).

## Compilación

Requisitos: `gcc`, `make`.

```bash
make
```

Para limpiar:
```bash
make clean
```

## Ejecución

El programa necesita un fichero de datos (por defecto se usa `castle.dat`).

```bash
./castle castle.dat
```

Controles disponibles:
- `n` / `next`: mover al **sur**
- `b` / `back`: mover al **norte**
- `l` / `left`: mover al **este**
- `r` / `right`: mover al **oeste**
- `t` / `take`: **coger** objeto
- `d` / `drop`: **soltar** objeto
- `e` / `exit`: salir

## Formato del fichero `castle.dat`

Cada espacio se define en una línea con el formato:

```
#s:<id>|<nombre>|<north>|<east>|<south>|<west>
```

- `<north/east/south/west>` es el id del espacio vecino.
- `-1` significa “no hay conexión”.

Ejemplo:
```
#s:12|Main Hall|11|-1|13|121
```

## Comprobación de memoria (Valgrind)

Si quieres verificar fugas de memoria:

```bash
valgrind --leak-check=full --show-leak-kinds=all ./castle castle.dat
```

## Notas de diseño

- El motor gráfico está deliberadamente separado del estado (`game`) para mantener el código modular.
- La localización del objeto se obtiene recorriendo los espacios y localizando en cuál está su `id`.
- Al tomar el objeto, la sala queda sin objeto (`NO_ID`) y el inventario del jugador pasa a contener ese id.

---

**Autores:** Violeta & Rafael
