/**
 * @brief It implements the command interpreter for visual mode (Raylib)
 *
 * @file command_raylib.c
 * @author Rafael
 * @version 2
 * @date 24-01-2026
 * @copyright GNU Public License
 */

#include "command.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>


/* ----------------------------------------------------------------------
 * command_raylib_get_user_input  (modo visual: traduce teclas a Command)
 *
 * STUB del Bloque 0. Solo capturamos ESC para que el usuario pueda
 * salir limpiamente del modo visual. La traduccion completa de
 * flechas/WASD a WALK + Direction, teclas 1-4 a ATTACK + skill_id,
 * y la deteccion de "tengo input este frame" llegara en el Bloque 1
 * cuando ampliemos Command con direction, skill_id y has_input.
 *
 * Reglas a recordar :
 *   - IsKeyPressed(KEY_X)  -> evento puntual (al pulsar la tecla una vez)
 *   - IsKeyDown(KEY_X)     -> mientras este pulsada (continuo)
 *   - GetKeyPressed()      -> devuelve int (keycode); util para texto,
 *                             NO para reconocer una tecla concreta.
 * ---------------------------------------------------------------------- */
Status command_raylib_get_user_input(Command *command) {
    if (!command) return ERROR;

    if (IsKeyPressed(KEY_ESCAPE)) {
        return command_set_code(command, EXIT);
    }

    /* 
     *   if (IsKeyPressed(KEY_UP)    || IsKeyPressed(KEY_W)) -> WALK + N
     *   if (IsKeyPressed(KEY_DOWN)  || IsKeyPressed(KEY_S)) -> WALK + S
     *   if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) -> WALK + E
     *   if (IsKeyPressed(KEY_LEFT)  || IsKeyPressed(KEY_A)) -> WALK + W
     *   if (IsKeyPressed(KEY_E))                            -> MOVE (interactuar)
     *   if (IsKeyPressed(KEY_ONE..FOUR))                    -> ATTACK + skill_id
     *   En todos los casos: command_set_has_input(command, TRUE).
     */

    return OK;
}