/**
 * @brief It defines the textual graphic engine interface
 *
 * @file graphic_engine.h
 * @author Rafael
 * @version 0
 * @date 21-04-2026
 * @copyright GNU Public License
 */

#ifndef GRAPHIC_ENGINE_H
#define GRAPHIC_ENGINE_H

#include "game.h"  /* Game (para graphic_engine_paint_game) */
#include "types.h" /* Id, WORD_SIZE, Bool, Status */

/* --- Margenes del area de juego --- */
#define POS_X_INIT 0
#define POS_Y_INIT 0
#define MARGIN_WIDHT 125
#define MARGIN_HIGHT 100

typedef struct _Graphic_engine Graphic_engine;

/* ----------------------------------------------------------------------
 * Out
 *
 * Codigo de retorno del menu inicial.
 *  - OUT_ERR : el usuario cerro la ventana sin elegir partida
 *              (X, ESC o cancelacion). El game_loop debe terminar.
 *  - NEW_GAME : el usuario eligio empezar una partida nueva. data_name
 *              contiene el .dat por defecto e init_numen el numen elegido.
 *  - LOAD_GAME: el usuario eligio cargar una partida guardada. data_name
 *              contiene el path del save file.
 *  - EXIT_Q  : el usuario pulso explicitamente "Exit". Funcionalmente
 *              equivalente a OUT_ERR, pero con intencion explicita.
 *
 * Nota: EXIT_Q se llama asi para no chocar con CommandCode::EXIT.
 * ---------------------------------------------------------------------- */
typedef enum
{
    OUT_ERR = -1,
    NEW_GAME,
    LOAD_GAME,
    EXIT_Q
} Out;

typedef struct _MenuResult
{
    Out menu_out;
    char data_name[WORD_SIZE + 1];
    Id init_numen;
} MenuResult;

Graphic_engine* graphic_engine_create (void);
void graphic_engine_destroy (Graphic_engine* ge);
MenuResult graphic_engine_init (Graphic_engine* ge);
void graphic_engine_paint_game (Graphic_engine* ge, Game* game);

#endif