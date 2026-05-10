/**
 * @brief It defines the graphic engine interface (Raylib + raygui)
 *
 * @file graphic_engine.h
 * @author Rafael, Salva, Javier and Violeta
 * @version 1
 * @date 02-05-2026
 * @copyright GNU Public License
 */
#ifndef GRAPHIC_ENGINE_H
#define GRAPHIC_ENGINE_H

#include "game.h"
#include "types.h"

/* Limites del cache interno de texturas. Ajusta si superas el tope. */
#define MAX_SPACE_TEX  16   /* 1 textura por space  */
#define MAX_PLAYER_TEX 4    /* 1 textura por player */
#define MAX_NUMEN_TEX  16   /* 1 textura por numen  */

typedef struct _Graphic_engine Graphic_engine;

/* ----------------------------------------------------------------------
 * Out  (codigo de retorno del menu inicial — sin cambios)
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
    Out  menu_out;
    char data_name[WORD_SIZE + 1];
    Id   init_numen;
} MenuResult;

/**
 * @brief It creates new graphic entities
 * @author Rafael, Javier, Salvador and Violeta
 * 
 * @note The graphic entities are date type which contains attributes
 *
 * @return Graphic engine, initialized, or NULL if memory fails
 */
Graphic_engine* graphic_engine_create  (void);

/**
 * @brief It destroy the graphic entities
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param ge a pointer to the graphic engine
 */
void            graphic_engine_destroy (Graphic_engine* ge);

/**
 * @brief It initialices the graphic entities
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param ge a pointer to the the graphic engine
 * @return The menu reuslt
 */
MenuResult      graphic_engine_init    (Graphic_engine* ge);

/**
 * @brief Carga las texturas del juego una vez tras cargar el .dat
 * @author Rafael
 *
 * Recorre los spaces/players/numens del Game y carga sus PNGs desde:
 *   - space:  ./img_src/background/<space_name>.png  (o el path del gdesc del space si tiene uno)
 *   - player: ./img_src/sprites/players/<name>.png
 *   - numen:  ./img_src/sprites/numens/<name>.png
 *
 * Si una imagen no existe, raylib carga su default (1x1) y nuestro
 * paint_game pinta un fallback de color para que el juego siga
 * funcionando sin crashear.
 *
 * Esta funcion debe llamarse DESPUES de que el game este cargado del .dat
 * y ANTES del primer paint_game.
 */
Status graphic_engine_load_textures (Graphic_engine* ge, Game* game);

/**
 * @brief It prints the game
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param ge a pointer to the the graphic engine
 * @param game the game to print
 */
void graphic_engine_paint_game     (Graphic_engine* ge, Game* game);
/**
 * @brief It prints the handle
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param ge a pointer to the the graphic engine
 * @param game the game to print
 */
void graphic_engine_handle_ui_input (Graphic_engine* ge, Game* game);

#endif