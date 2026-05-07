/**
 * @brief Graphic engine (Pokemon-style top-down view + sprite player)
 *
 * 
 * 
 * @file graphic_engine.c
 * @author Rafael
 * @version 3
 * @date 02-05-2026
 * @copyright GNU Public License
 */

#include "graphic_engine.h"
#include "command.h"
#include "game.h"
#include "player.h"
#include "space.h"
#include "object.h"
#include "character.h"
#include "numen.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <inventory.h>




/* ====================================================================== */
/*                     DIMENSIONES TOTALES DE LA PANTALLA                 */
/* ====================================================================== */
#define BIG_TEXT_SIZE			 28
#define MEDIUM_TEXT_SIZE		 20
#define SMALL_TEXT_SIZE 		 16
/*------------------------OVERLAY (HUD MINIMO ARRIBA)---------------------*/
#define OVERLAY_H        SCALE*2      /* franja superior con HP, nombre y objeto activo*/
#define OVERLAY_PAD      8
/*------------------------RIGHT SIDE PANEL (HUD DERECHA)-------------------*/
#define RIGHT_SIDE_PANEL_W       100     /* Franja derecha con numens activos y objetos*/
/*------------------------SKILL PANEL (HUD ABAJO)-------------------*/
#define RIGHT_SIDE_PANEL_W       100     /* Franja derecha con numens activos y objetos*/
/*------------------------TOTAL DE LA PANTALLA-------------------*/
#define WIDTH_SCREEN (WIDHT_MAP + RIGHT_SIDE_PANEL_W )
#define HIGHT_SCREEN (HIGHT_MAP + OVERLAY_H)

/* ====================================================================== */
/*                     COLORES (OVERALYS, TEXTO, TITULOS, ETC )           */
/* ====================================================================== */

#define COLOR_OVERLAY    (Color){ 0, 0, 0, 160 }
#define COLOR_HP_OK      GREEN
#define COLOR_HP_LOW     RED
#define COLOR_TITLE      GOLD
#define COLOR_TEXT       RAYWHITE

/* Color de fallback cuando una textura no existe — un verde tipo cesped. */
#define COLOR_FALLBACK_BG     (Color){ 60, 120, 60, 255 }
#define COLOR_FALLBACK_PLAYER (Color){ 220, 200, 50, 255 }

/* ====================================================================== */
/*                          STRUCT INTERNA                                 */
/* ====================================================================== */

/* Cada slot empareja un id (o nombre) con su Texture2D cargada. */
typedef struct
{
	Id        id;          /* NO_ID si slot vacio */
	Texture2D tex;
} TexEntry;

typedef struct
{
	char      name[WORD_SIZE];    /* "" si slot vacio */
	Texture2D tex;
} TexEntryName;

struct _Graphic_engine
{
	/* Cache de texturas — se carga una vez en load_textures. */
	TexEntry      space_textures [MAX_SPACE_TEX];
	TexEntryName  player_textures[MAX_PLAYER_TEX];
	TexEntryName  numen_textures [MAX_NUMEN_TEX];
	TexEntryName  object_textures [MAX_OBJECT_TEX];

	Bool textures_loaded;
};

/* ====================================================================== */
/*                       PRIVATE FUNCTION PROTOTYPES                       */
/* ====================================================================== */

static Texture2D* ge_get_space_texture  (Graphic_engine* ge, Id space_id);
static Texture2D* ge_get_player_texture (Graphic_engine* ge, const char* name);
static Texture2D* ge_get_numen_texture (Graphic_engine* ge, const char* name);
static Texture2D* ge_get_object_texture (Graphic_engine* ge, const char* name);

static Bool ge_texture_is_valid 		(Texture2D* tex);



static void ge_paint_background  		(Graphic_engine* ge, Game* game, Player* player);
static void ge_paint_player      		(Graphic_engine* ge, Player* player);
static void ge_paint_overlay     		(Game* game, Player* player);
static void ge_paint_right_side_panel	(Graphic_engine* ge, Game* game);
static void ge_paint_numen_right_panel	(Graphic_engine* ge, Numen* numen, int n_painted);
static void ge_paint_object_right_panel	(Graphic_engine* ge, Object* object, int n_painted);
static void ge_paint_active_numen		(Graphic_engine* ge, Game* game, Player* player);
static void ge_paint_objects			(Graphic_engine* ge, Game* game, Player* player);
static void ge_paint_space_numens		(Graphic_engine* ge, Game* game, Player* player);




static const char* ge_status_to_str (Status s);
static const char* ge_cmd_to_str    (CommandCode c);

static int		ge_find_idx_in_list (Id active_id, int n, Id (*get_at)(Player*, int), Player* player);
static void		ge_cycle_active_object (Player* player);
static void		ge_cycle_active_numen  (Game* game, Player* player);
static 	int		_ge_max_radio_skill_of_numen (Numen* numen);
Status	_ge_draw_bars (Numen* numen);

/* ====================================================================== */
/*                         PUBLIC: CREATE / DESTROY                        */
/* ====================================================================== */

Graphic_engine*
graphic_engine_create (void)
{
	Graphic_engine* ge = (Graphic_engine*)calloc (1, sizeof (Graphic_engine));
	int i;

	if (!ge) return NULL;

	/* calloc ya pone todo a 0, pero somos explicitos para que se lea
	 * lo que esta pasando: ningun id, ningun nombre, ninguna textura. */
	for (i = 0; i < MAX_SPACE_TEX;  i++) ge->space_textures[i].id  = NO_ID;
	for (i = 0; i < MAX_PLAYER_TEX; i++) ge->player_textures[i].name[0] = '\0';
	for (i = 0; i < MAX_NUMEN_TEX;  i++) ge->numen_textures[i].name[0]  = '\0';
	for (i = 0; i < MAX_OBJECT_TEX; i++) ge->object_textures[i].name[0] = '\0';

	ge->textures_loaded = FALSE;
	return ge;
}

void
graphic_engine_destroy (Graphic_engine* ge)
{
	int i;
	if (!ge) return;

	/* Liberamos las texturas SOLO si la ventana sigue viva. */
	if (IsWindowReady () && ge->textures_loaded)
	{
		for (i = 0; i < MAX_SPACE_TEX; i++)
			if (ge->space_textures[i].id != NO_ID)
				UnloadTexture (ge->space_textures[i].tex);
		for (i = 0; i < MAX_PLAYER_TEX; i++)
			if (ge->player_textures[i].name[0] != '\0')
				UnloadTexture (ge->player_textures[i].tex);
		for (i = 0; i < MAX_NUMEN_TEX; i++)
			if (ge->numen_textures[i].name[0] != '\0')
				UnloadTexture (ge->numen_textures[i].tex);
		for (i = 0; i < MAX_OBJECT_TEX; i++)
			if (ge->object_textures[i].name[0] != '\0')
				UnloadTexture (ge->object_textures[i].tex);
	}

	/* La ventana se cierra DESPUES de descargar texturas */
	if (IsWindowReady ()) CloseWindow ();

	free (ge);
}

/* ====================================================================== */
/*                       PUBLIC: MENU INIT (sin cambios)                   */
/* ====================================================================== */

MenuResult
graphic_engine_init (Graphic_engine* ge)
{
	MenuResult result;
	Bool exit_on = FALSE;

	result.data_name[0] = '\0';
	result.init_numen   = NO_ID;
	result.menu_out     = OUT_ERR;

	if (!ge) return result;

	InitWindow (WIDHT_MAP+OVERLAY_H, HIGHT_MAP+RIGHT_SIDE_PANEL_W, TITLE);
	SetTargetFPS (FPS);

	while (!WindowShouldClose () && exit_on == FALSE)
	{
		BeginDrawing ();
		ClearBackground (RAYWHITE);

		if (result.menu_out == OUT_ERR)
		{
			DrawText (TITLE, (int)(WIDHT_MAP * (3.0 / 8.0)), 100, 20, DARKGRAY);

			if (GuiButton ((Rectangle){ (int)(WIDHT_MAP * (3.0 / 8.0)), 150, 200, 50 }, "NEW GAME"))
				result.menu_out = NEW_GAME;
			if (GuiButton ((Rectangle){ (int)(WIDHT_MAP * (3.0 / 8.0)), 220, 200, 50 }, "LOAD GAME"))
				result.menu_out = LOAD_GAME;
			if (GuiButton ((Rectangle){ (int)(WIDHT_MAP * (3.0 / 8.0)), 290, 200, 50 }, "Exit"))
			{
				result.menu_out = EXIT_Q;
				exit_on         = TRUE;
			}
		}
		else if (result.menu_out == NEW_GAME)
		{
			DrawText ("Choose your Initial Numen", (int)(WIDHT_MAP * (3.0 / 8.0)), 100, 20, DARKGRAY);

			if (GuiButton ((Rectangle){ (int)(WIDHT_MAP * (6.0 / 8.0)), 10, 100, 30 }, "BACK"))
				result.menu_out = OUT_ERR;
			if (GuiButton ((Rectangle){ (int)(WIDHT_MAP * (2.0 / 8.0)), 150, 200, 50 }, "BROCOCHACHO"))
			{
				strncpy (result.data_name, F_DATA_N, WORD_SIZE);
				result.data_name[WORD_SIZE] = '\0';
				result.init_numen           = First_numen;
				exit_on                     = TRUE;
			}
			if (GuiButton ((Rectangle){ (int)(WIDHT_MAP * (2.0 / 8.0)), 220, 200, 50 }, "SQUIRTLE"))
			{
				strncpy (result.data_name, F_DATA_N, WORD_SIZE);
				result.data_name[WORD_SIZE] = '\0';
				result.init_numen           = Second_numen;
				exit_on                     = TRUE;
			}
			if (GuiButton ((Rectangle){ (int)(WIDHT_MAP * (2.0 / 8.0)), 290, 200, 50 }, "CHARMANDER"))
			{
				strncpy (result.data_name, F_DATA_N, WORD_SIZE);
				result.data_name[WORD_SIZE] = '\0';
				result.init_numen           = third_numen;
				exit_on                     = TRUE;
			}
		}
		else if (result.menu_out == LOAD_GAME)
		{
			DrawText ("WHITCH YOUR GAME?", 300, 100, 20, DARKGRAY);

			if (GuiButton ((Rectangle){ (int)(WIDHT_MAP * (6.0 / 8.0)), 10, 100, 30 }, "BACK"))
				result.menu_out = OUT_ERR;
			if (GuiButton ((Rectangle){ (int)(WIDHT_MAP * (2.0 / 8.0)), 150, 200, 50 }, "LOAD GAME #1"))
			{
				strncpy (result.data_name, F_DATA_S_1, WORD_SIZE);
				result.data_name[WORD_SIZE] = '\0';
				result.init_numen           = NO_ID;
				exit_on                     = TRUE;
			}
			if (GuiButton ((Rectangle){ (int)(WIDHT_MAP * (2.0 / 8.0)), 220, 200, 50 }, "LOAD GAME #2"))
			{
				strncpy (result.data_name, F_DATA_S_2, WORD_SIZE);
				result.data_name[WORD_SIZE] = '\0';
				result.init_numen           = NO_ID;
				exit_on                     = TRUE;
			}
		}

		EndDrawing ();
	}

	if (WindowShouldClose () && result.menu_out == OUT_ERR)
		result.menu_out = EXIT_Q;

	return result;
}

/* ====================================================================== */
/*                  PUBLIC: LOAD TEXTURES (una sola vez)                   */
/* ====================================================================== */

Status
graphic_engine_load_textures (Graphic_engine* ge, Game* game)
{
	int   i, n_spaces, slot;
	char  path[256];
	Space* sp;
	Player* pl;
	Numen*  nu;

	if (!ge || !game) return ERROR;
	if (ge->textures_loaded == TRUE) return OK;   /* idempotente */

	/* ---- SPACES ---- */
	n_spaces = game_get_n_spaces (game);
	for (i = 0, slot = 0; i < n_spaces && slot < MAX_SPACE_TEX; i++)
	{
		Id space_id = game_get_space_id_at (game, i);
		sp = game_get_space (game, space_id);
		if (!sp) continue;

		/* El path lo construimos con el name del space, no con el gdesc.
		 * Asi un space "Loby" siempre busca ./img_src/background/Loby.png
		 * sin depender de lo que ponga en el .dat. */
		snprintf (path, sizeof (path), "./img_src/background/%s.png", space_get_name (sp));

		ge->space_textures[slot].id  = space_id;
		ge->space_textures[slot].tex = LoadTexture (path);
		slot++;
	}

	/* ---- PLAYERS ---- */
	{
		int n_players = game_get_n_players (game);
		for (i = 0, slot = 0; i < n_players && slot < MAX_PLAYER_TEX; i++)
		{
			pl = game_get_player_at (game, i);
			if (!pl) continue;

			snprintf (path, sizeof (path), "./img_src/sprites/players/%s.png", player_get_name (pl));

			strncpy (ge->player_textures[slot].name,
					 player_get_name (pl), 63);
			ge->player_textures[slot].name[63] = '\0';
			ge->player_textures[slot].tex = LoadTexture (path);
			slot++;
		}
	}

	/* ---- NUMENS  ---- */
	{
		int n_numens = game_get_n_numens (game);
		for (i = 0, slot = 0; i < n_numens && slot < MAX_NUMEN_TEX; i++)
		{
			nu = game_get_numen_at (game, i);
			if (!nu) continue;
			snprintf (path, sizeof (path),
					  "./img_src/sprites/numens/%s.png", numen_get_name (nu));
			strncpy (ge->numen_textures[slot].name,
					 numen_get_name (nu), 63);
			ge->numen_textures[slot].name[63] = '\0';
			ge->numen_textures[slot].tex = LoadTexture (path);
			slot++;
		}
	}
	/*====== objects =========*/
	{
		int n_objs = game_get_n_objects (game);
		Object* o;
		for (i = 0, slot = 0; i < n_objs && slot < MAX_OBJECT_TEX; i++)
		{
			o = game_get_object_at (game, i);
			if (!o) continue;
		
			snprintf (path, sizeof (path),	"./img_src/sprites/objects/%s.png", obj_get_gdesc (o));
		
			strncpy (ge->object_textures[slot].name, obj_get_name (o), 63);
			ge->object_textures[slot].name[63] = '\0';
			ge->object_textures[slot].tex = LoadTexture (path);
			slot++;
		}
	}

	ge->textures_loaded = TRUE;
	return OK;
}

/* ====================================================================== */
/*                  PUBLIC: PAINT GAME (cada frame)                        */
/* ====================================================================== */

void
graphic_engine_paint_game (Graphic_engine* ge, Game* game)
{
	Player* player;

	if (!ge || !game) return;
	player = game_get_player_by_turn (game);
	if (!player) return;

	ClearBackground (BLACK);

	ge_paint_background  (ge, game, player);
	ge_paint_objects       (ge, game, player);
	ge_paint_space_numens  (ge, game, player);
	ge_paint_active_numen (ge, game, player);
	ge_paint_player      (ge, player);
	ge_paint_overlay     (game, player);
	ge_paint_right_side_panel(ge, game);

}

/* ====================================================================== */
/*                  PUBLIC: NON-COMMAND UI INPUT                           */
/* ====================================================================== */

void
graphic_engine_handle_ui_input (Graphic_engine* ge, Game* game)
{
	Player* player;
	(void)ge;

	if (!game) return;
	player = game_get_player_by_turn (game);
	if (!player) return;

	if (IsKeyPressed (KEY_TAB)) 			ge_cycle_active_object (player);
	if (IsKeyPressed (KEY_LEFT_SHIFT))		ge_cycle_active_numen  (game, player);
}

/* ====================================================================== */
/*                       PRIVATE: BACKGROUND                               */
/* ====================================================================== */

static void
ge_paint_background (Graphic_engine* ge, Game* game, Player* player)
{
	Id         space_id;
	Texture2D* tex;
	Rectangle  src, dst;

	space_id = player_get_zone (player);
	tex      = ge_get_space_texture (ge, space_id);

	if (ge_texture_is_valid (tex))
	{
		/* Escalamos la textura a la pantalla entera.
		 * src abarca toda la imagen original, dst es toda la pantalla. */
		src = (Rectangle){ 0, 0, (float)tex->width, (float)tex->height };
		dst = (Rectangle){ 0, 0, (float)WIDHT_MAP, (float)HIGHT_MAP };
		DrawTexturePro (*tex, src, dst,
						(Vector2){ 0, 0 }, 0.0f, WHITE);
	}
	else
	{
		/* Sin imagen: rectangulo verde plano para no romper la lectura. */
		DrawRectangle (0, 0, WIDHT_MAP, HIGHT_MAP, COLOR_FALLBACK_BG);
	}


	{
		int i;
		for (i = 1; i < WIDHT;  i++)
			DrawLine (i * SCALE, 0, i * SCALE, HIGHT * SCALE,
					  (Color){ 255, 255, 255, 30 });
		for (i = 1; i < HIGHT;  i++)
			DrawLine (0, i * SCALE, WIDHT * SCALE, i * SCALE,
					  (Color){ 255, 255, 255, 30 });
	}
}

/* ====================================================================== */
/*                       PRIVATE: PLAYER                                    */
/* ====================================================================== */

static void
ge_paint_player (Graphic_engine* ge, Player* player)
{
	int        px, py;
	Texture2D* tex;

	px  = player_get_pos_x (player);
	py  = player_get_pos_y (player);
	tex = ge_get_player_texture (ge, player_get_name (player));

	if (ge_texture_is_valid (tex))
	{
		Rectangle src = { 0, 0, (float)tex->width, (float)tex->height };
		Rectangle dst = { (float)px, (float)py, (float)SCALE, (float)SCALE };
		DrawTexturePro (*tex, src, dst,(Vector2){ 0, 0 }, 0.0f, WHITE);
	}
	else
	{
		/* Cuadrado amarillo de fallback */
		DrawRectangle (px, py, SCALE, SCALE, COLOR_FALLBACK_PLAYER);
		DrawRectangleLines (px, py, SCALE, SCALE, BLACK);
	}
}

/* ====================================================================== */
/*                       PRIVATE: OVERLAY (HUD ARRIBA)                     */
/* ====================================================================== */

static void
ge_paint_overlay (Game* game, Player* player)
{
	Space*       sp;
	Numen*       numen=NULL;
	Object*      object=NULL;
	Id           Id_act_num=NO_ID, Id_act_obj=NO_ID;
	Command*     last_cmd;
	int          hp_numen;
	Color        hp_color;
	const char*  space_name;
	const char*  cmd_label;
	const char*  status_label;
	const char*  numen_name;
	const char*  object_name;

	Id_act_num=player_get_active_numen(player);
	numen=game_get_numen_by_id(game, Id_act_num);

	Id_act_obj=player_get_active_object(player);
	object=game_get_object_by_id(game, Id_act_obj);

	/* Franja semi-transparente arriba */
	DrawRectangle (0, 0, WIDHT_MAP + RIGHT_SIDE_PANEL_W, OVERLAY_H, COLOR_OVERLAY);

	/*Nombre del numen a la izquierda*/
	numen_name=numen_get_name(numen);
	DrawText(numen_name, OVERLAY_PAD ,7, MEDIUM_TEXT_SIZE, COLOR_TEXT);
	/* HP a la izquierda */
	hp_numen = numen_get_health (numen);
	hp_color  = (hp_numen > 3) ? COLOR_HP_OK : COLOR_HP_LOW;
	DrawText (TextFormat ("HP %d", hp_numen),
			  OVERLAY_PAD+20, 36, SMALL_TEXT_SIZE, hp_color);

	/* Nombre del space en el centro */
	sp         = game_get_space (game, player_get_zone (player));
	space_name = sp ? space_get_name (sp) : "?";
	DrawText (space_name,
			  WIDHT_MAP / 2 - MeasureText (space_name, BIG_TEXT_SIZE) / 2,
			  27, BIG_TEXT_SIZE, COLOR_TITLE);
	
	/* Resultado del ultimo comando a la derecha */
	last_cmd = game_get_last_command (game);
	if (last_cmd)
	{
		cmd_label    = ge_cmd_to_str (command_get_code (last_cmd));
		status_label = ge_status_to_str (game_get_last_cmd_status (game));
		DrawText (TextFormat ("%s:%s", cmd_label, status_label),
				  WIDHT_MAP - 60, 40, SMALL_TEXT_SIZE, COLOR_TEXT);
	}

	/*Nombre del object a la izquierda*/
	object_name=obj_get_name(object);
	DrawText(object_name, WIDHT_MAP + RIGHT_SIDE_PANEL_W - MeasureText (space_name, MEDIUM_TEXT_SIZE) / 2, 21, MEDIUM_TEXT_SIZE, COLOR_TEXT); 
}
 
/* ====================================================================== */
/*                 PRIVATE: RIHT SIDE PANEL (HUD DERECHA)                 */
/* ====================================================================== */
static void ge_paint_right_side_panel (Graphic_engine* ge, Game* game)
{
	Player*     	player		= 	NULL;
	Numen*       	numen		= 	NULL;
	Object*      	object		= 	NULL;
	const char*  	space_name	=   NULL;
	char*  	gdesc      	=   NULL;
	Id           	Id_act_num = NO_ID;
	Id				Id_act_obj = NO_ID;
	int          	n_nums, n_objs, n, o, i;

	/*Obtenemos al player*/
	player=game_get_player_at(game, PLAYER);

	/*Obtenemos el id del numen_active y luego obtenemos un puntero a dicho numen*/
	Id_act_num=player_get_active_numen(player);
	numen=game_get_numen_by_id(game, Id_act_num);

	/*Obtenemos el id del obj_activo y el puntero a dicho object*/
	Id_act_obj=player_get_active_object(player);
	object=game_get_object_by_id(game, Id_act_obj);
	
	/*Revisamos cuantos numens y objects hay*/
	n_nums=player_get_n_numens(player);
	n_objs=player_get_n_objects(player);


	/* Franja semi-transparente arriba */
	DrawRectangle (WIDHT_MAP, 0, WIDTH_SCREEN, HIGHT_SCREEN, COLOR_OVERLAY);

	 for(n=0; n<n_nums; n++)
		{
			if(gdesc) free(gdesc);
			numen= game_get_numen_by_id (game, player_get_numen_at_inventory (player, n));
			gdesc=numen_get_gdesc(numen);
			ge_paint_numen_right_panel(ge, numen, n);
			if(n==3)	break;
		}
	for(o=0; o<n_objs; o++)
		{
			if(gdesc) free(gdesc);
			object= game_get_object_by_id (game, player_get_object_at_inventory (player, o));
			gdesc=obj_get_gdesc(object);
			ge_paint_object_right_panel(ge, object, n+o+1);
			if(o==3)	break;
		}
	if(gdesc) free(gdesc);
}

/* ====================================================================== */
/*                       PRIVATE: NUMEN (RIGHT PANEL)                     */
/* ====================================================================== */
static void
ge_paint_numen_right_panel (Graphic_engine* ge, Numen* numen, int n_painted)
{
	int        px, py;
	Texture2D* tex;
	
	tex = ge_get_numen_texture (ge, numen_get_name (numen));
	
	px  = WIDHT_MAP;

	py  =(int)(tex->height+SCALE)*n_painted;
   

	if (ge_texture_is_valid (tex))
	{
		Rectangle src = { 0, 0, (float)tex->width, (float)tex->height };
		Rectangle dst = { (float)px, (float)py, (float)SCALE*2, (float)SCALE*2 };
		DrawTexturePro (*tex, src, dst,	(Vector2){ 0, 0 }, 0.0f, WHITE);
	}
	else
	{
		/* Cuadrado amarillo de fallback */
		DrawRectangle (px, py, SCALE, SCALE, COLOR_FALLBACK_PLAYER);
		DrawRectangleLines (px, py, SCALE, SCALE, BLACK);
	}
}

/* ====================================================================== */
/*                       PRIVATE: OBJECT (RIGHT PANEL)                     */
/* ====================================================================== */
static void
ge_paint_object_right_panel(Graphic_engine*ge, Object* object, int n_painted)
{
		int        px, py;
	Texture2D* tex;
	
	tex = ge_get_object_texture (ge, obj_get_name (object));
	
	px  = WIDHT_MAP;

	py  =(SCALE)*n_painted;
   

	if (ge_texture_is_valid (tex))
	{
		Rectangle src = { 0, 0, (float)tex->width, (float)tex->height };
		Rectangle dst = { (float)px, (float)py, (float)SCALE*2, (float)SCALE*2 };
		DrawTexturePro (*tex, src, dst,	(Vector2){ 0, 0 }, 0.0f, WHITE);
	}
	else
	{
		/* Cuadrado amarillo de fallback */
		DrawRectangle (px, py, SCALE*2, SCALE*2, COLOR_FALLBACK_PLAYER);
		DrawRectangleLines (px, py, SCALE*2, SCALE*2, BLACK);
	}
}
/* ====================================================================== */
/*                       PRIVATE: NUMEN (RIGHT PANEL)                     */
/* ====================================================================== */
static void	ge_paint_skill_panel     	(Game* game);

/* ====================================================================== */
/*                       PRIVATE: TEXTURE LOOKUP                           */
/* ====================================================================== */

static Texture2D*
ge_get_space_texture (Graphic_engine* ge, Id space_id)
{
	int i;
	if (!ge || space_id == NO_ID) return NULL;
	for (i = 0; i < MAX_SPACE_TEX; i++)
	{
		if (ge->space_textures[i].id == space_id)	return (&ge->space_textures[i].tex);
	}
	return NULL;
}

static Texture2D*
ge_get_player_texture (Graphic_engine* ge, const char* name)
{
	int i;
	if (!ge || !name) return NULL;
	for (i = 0; i < MAX_PLAYER_TEX; i++)
	{
		if (ge->player_textures[i].name[0] != '\0' &&
			strcmp (ge->player_textures[i].name, name) == 0)
			return &ge->player_textures[i].tex;
	}
	return NULL;
}

/* Una textura "default" de raylib (cuando no se ha cargado fichero)
 * tiene id 0 y dimensiones 0x0 o 1x1. La rechazamos para usar fallback. */
static Bool
ge_texture_is_valid (Texture2D* tex)
{
	if (!tex) return FALSE;
	if (tex->id == 0)  return FALSE;
	if (tex->width <= 1 && tex->height <= 1) return FALSE;
	return TRUE;
}

/* ====================================================================== */
/*                  PRIVATE: CICLAR active_object / active_numen           */
/* ====================================================================== */

static int
ge_find_idx_in_list (Id active_id, int n, Id (*get_at)(Player*, int), Player* player)
{
	int i;
	if (!get_at || !player) return -1;
	for (i = 0; i < n; i++)
	{
		if (get_at (player, i) == active_id) return i;
	}
	return -1;
}

static void
ge_cycle_active_numen (Game* game, Player* player)
{
    Id      active_id, new_active_id;
    Numen*  new_active;
	Numen*  old_active=NULL;
    Position p;
    int     n, current_idx, next_idx;

    if (!game || !player) return;
    n = player_get_n_numens (player);
    if (n <= 1) return;

    active_id   = player_get_active_numen (player);
    current_idx = ge_find_idx_in_list (active_id, n,
                                       player_get_numen_at_inventory, player);
    next_idx    = (current_idx + 1) % n;
    new_active_id = player_get_numen_at_inventory (player, next_idx);

    if (active_id != NO_ID)
    {
      	old_active = game_get_numen_by_id (game, active_id);
        if (old_active)
        {
            numen_set_pos_x (old_active, NO_POS);
            numen_set_pos_y (old_active, NO_POS);
        }
    }

    new_active = game_get_numen_by_id (game, new_active_id);
    if (new_active)
    {
        p = player_get_position (player);
        if (p.pos_x != NO_POS && p.pos_y != NO_POS)
        {
            numen_set_pos_x (new_active, p.pos_x - SCALE);
            numen_set_pos_y (new_active, p.pos_y);
        }
    }

    player_set_active_numen (player, new_active_id);
}

static void
ge_cycle_active_object (Player* player)
{
    Id      active_id	= NO_ID;
	Id		new_active_id = NO_ID;
    Position p;
    int     n, current_idx, next_idx;

    if (!player) return;
    n = player_get_n_numens (player);
    if (n <= 1) return;

    active_id   = player_get_active_object (player);
    current_idx = ge_find_idx_in_list (active_id, n,	player_get_object_at_inventory, player);
    next_idx    = (current_idx + 1) % n; /*Nos movemos al siguiente object en el inventario*/

    player_set_active_object (player, new_active_id);
}

/* ====================================================================== */
/*                      PRIVATE: ENUM → STRING                             */
/* ====================================================================== */

static const char*
ge_status_to_str (Status estado)
{
	switch (estado)
	{
		case OK:                 return "OK";
		case ERROR:              return "ERROR";
		case ERROR_Attack:       return "ATK!";
		case ERROR_walk:         return "WALK!";
		case ERROR_Chat:         return "CHAT!";
		case ERROR_dir:          return "DIR!";
		case ERROR_take:         return "TAKE!";
		case ERROR_drop:         return "DROP!";
		case ERROR_inspect:      return "INSP!";
		case ERROR_use:          return "USE!";
		case ERROR_open:         return "OPEN!";
		case ERROR_save:         return "SAVE!";
		case ERROR_load:         return "LOAD!";
		case ERROR_recruit:      return "REC!";
		case ERROR_kick:         return "KICK!";
		case ERROR_enemy_attack: return "ENEMY";
		case ERROR_enemy_move:   return "EMOV";
		default:                 return "?";
	}
}

static const char*
ge_cmd_to_str (CommandCode code)
{
	switch (code)
	{
		case UNKNOWN: return "??";
		case EXIT:    return "exit";
		case MOVE:    return "move";
		case WALK:    return "walk";
		case TAKE:    return "take";
		case DROP:    return "drop";
		case ATTACK:  return "atk";
		case CHAT:    return "chat";
		case INSPECT: return "insp";
		case USE:     return "use";
		case SAVE:    return "save";
		case LOAD:    return "load";
		case RECRUIT: return "rec";
		case KICK:    return "kick";
		default:      return "";
	}
}
/* ====================================================================== */
/*                  PRIVATE: ACTIVE NUMEN (pintado)                        */
/* ====================================================================== */

static void
ge_paint_active_numen (Graphic_engine* ge, Game* game, Player* player)
{
	Numen*     num;
	Texture2D* tex;
	Id         active_id;
	int        pos_x_numen, pos_y_numen;

	active_id = player_get_active_numen (player);
	if (active_id == NO_ID) return;

	num = game_get_numen_by_id (game, active_id);
	if (!num) return;

	pos_x_numen = numen_get_pos_x (num);
	pos_y_numen = numen_get_pos_y (num);

	/* Si por algun motivo no tiene posicion valida, lo colocamos
	 * a la derecha del player. */
	if (pos_x_numen == NO_POS || pos_y_numen == NO_POS)
	{
		pos_x_numen = player_get_pos_x (player) + SCALE;
		pos_y_numen = player_get_pos_y (player);
	}

	tex = ge_get_numen_texture (ge, numen_get_name (num));

	if (ge_texture_is_valid (tex))
	{
		Rectangle src = { 0, 0, (float)tex->width, (float)tex->height };
		Rectangle dst = { (float)pos_x_numen, (float)pos_y_numen, (float)SCALE, (float)SCALE };
		DrawTexturePro (*tex, src, dst, (Vector2){ 0, 0 }, 0.0f, WHITE);
	}
	else
	{
		/* Fallback: circulo del color de los numens corruptos vs friendly.
		 * El "B" de Brococacho lo pintamos como letra encima del circulo
		 * para que se distinga sin sprite. */
		Color   col;
		char*   gd;

		col = (numen_get_corrupt (num) == TRUE)
			? (Color){ 200, 60, 60, 255 }     /* enemigo: rojo */
			: (Color){ 80, 200, 80, 255 };    /* amigo: verde  */

		DrawCircle (pos_x_numen + SCALE / 2, pos_y_numen + SCALE / 2, SCALE / 2 - 2, col);
		DrawCircleLines (pos_x_numen + SCALE / 2, pos_y_numen + SCALE / 2, SCALE / 2 - 2, BLACK);

		gd = numen_get_gdesc (num);
		if (gd && gd[0])
			DrawText (TextFormat ("%c", gd[0]),
					  pos_x_numen + SCALE / 2 - 4, pos_y_numen + SCALE / 2 - 6, 12, BLACK);
	}
}

/* ====================================================================== */
/*                  PRIVATE: NUMEN TEXTURE LOOKUP                          */
/* ====================================================================== */
static Texture2D*
ge_get_numen_texture (Graphic_engine* ge, const char* name)
{
	int i;
	if (!ge || !name) return NULL;
	for (i = 0; i < MAX_NUMEN_TEX; i++)
	{
		if (ge->numen_textures[i].name[0] != '\0' &&
			strcmp (ge->numen_textures[i].name, name) == 0)
			return &ge->numen_textures[i].tex;
	}
	return NULL;
}
/* ====================================================================== */
/*           PRIVATE: OBJECTS DEL SPACE ACTUAL                             */
/* ====================================================================== */
static void
ge_paint_objects (Graphic_engine* ge, Game* game, Player* player)
{
	Space*     sp;
	Object*    obj;
	Texture2D* tex;
	Id         space_id;
	int        i, n_objs, ox, oy;

	space_id = player_get_zone (player);
	sp       = game_get_space (game, space_id);
	if (!sp) return;

	n_objs = game_get_n_objects (game);
	for (i = 0; i < n_objs; i++)
	{
		obj = game_get_object_at (game, i);
		if (!obj) continue;

		/* Solo pintamos los objetos cuyo space coincide con el actual.
		 * Usamos space_contains_object para no asumir nada de la
		 * implementacion interna. */
		if (space_contains_object (sp, obj_get_id (obj)) == FALSE) continue;

		ox  = obj_get_pos_x (obj);
		oy  = obj_get_pos_y (obj);
		if (ox == NO_POS || oy == NO_POS) continue;

		tex = ge_get_object_texture (ge, obj_get_name (obj));

		if (ge_texture_is_valid (tex))
		{
			Rectangle src = { 0, 0, (float)tex->width, (float)tex->height };
			Rectangle dst = { (float)ox, (float)oy, (float)SCALE, (float)SCALE };
			DrawTexturePro (*tex, src, dst, (Vector2){0, 0}, 0.0f, WHITE);
		}
		else
		{
			/* cuadrado gris con la letra del gdesc */
			char* gd = obj_get_gdesc (obj);
			DrawRectangle (ox, oy, SCALE, SCALE, (Color){ 150, 150, 150, 200 });
			DrawRectangleLines (ox, oy, SCALE, SCALE, BLACK);
			if (gd && gd[0])
				DrawText (TextFormat ("%c", gd[0]),
						  ox + SCALE/2 - 4, oy + SCALE/2 - 6, 12, BLACK);
		}
	}
}


static Texture2D*
ge_get_object_texture (Graphic_engine* ge, const char* name)
{
	int i;
	if (!ge || !name) return NULL;
	for (i = 0; i < MAX_OBJECT_TEX; i++)
	{
		if (ge->object_textures[i].name[0] != '\0' &&
			strcmp (ge->object_textures[i].name, name) == 0)
			return &ge->object_textures[i].tex;
	}
	return NULL;
}



/* ====================================================================== */
/*           PRIVATE: NUMENS DEL SPACE ACTUAL                              */
/*                                                                         */
/* Recorre los numens del juego y pinta los que viven en el space actual   */
/* del player y NO son el active_numen (que ya lo pinta paint_active_numen)*/
/*                                                                         */
/*   - Pinta una barra de HP y de energia encima de los numens              */
/*   - Pinta una barra de HP energia encima del enemigo.                    */
/*   - Si el numen activo del player esta dentro del radio de combate,     */
/*     resalta el enemigo con marco rojo brillante y une con linea al      */
/*     numen activo (feedback visual de "amenaza/objetivo").               */
/* ====================================================================== */
static void
ge_paint_space_numens (Graphic_engine* ge, Game* game, Player* player)
{
	Numen*     num;
	Numen*     active_num;
	Texture2D* tex;
	Id         active_id, num_id, space_id;
	Position	pos_active, pos_numen;
	int        n_numens, i;
	int        dx, dy, dist_sq, range_sq;
	Bool       is_corrupt, is_errant, in_range;
	Color      col;
	char*      gd;
	int        max_health, max_radio;
	float      hp_ratio;

	if (!ge || !game || !player) return;
	pos_active.pos_x = pos_active.pos_x = NO_POS;

	active_id = player_get_active_numen (player); /*Puede ser NO_ID si se  mueren todos, se cerraría el jeugo*/
	space_id  = player_get_zone (player);

	if (space_id == NO_ID) return;


	active_num = (active_id != NO_ID)
	             ? game_get_numen_by_id (game, active_id)
	             : NULL;
	if (!active_num) return;



	pos_active = numen_get_position (active_num);
	max_radio = _ge_max_radio_skill_of_numen (active_num);
	range_sq = max_radio * max_radio;

	n_numens = game_get_n_numens (game);

	for (i = 0; i < n_numens; i++)
	{
		num = game_get_numen_at (game, i);
		if (!num) continue;

		num_id = numen_get_id (num);
		if (num_id == active_id) continue;

		if (game_get_numen_location (game, num_id) != space_id) continue;

		pos_numen = numen_get_position (num);
		if (pos_numen.pos_x == NO_POS || pos_numen.pos_y == NO_POS) continue;

		is_corrupt = numen_get_corrupt (num);
		is_errant = numen_get_following (num);





		/* === SPRITE / FALLBACK === */
		tex = ge_get_numen_texture (ge, numen_get_name (num));

		if (ge_texture_is_valid (tex) == TRUE)
		{
			Rectangle src = { 0, 0, (float)tex->width, (float)tex->height };
			Rectangle dst = { (float)pos_numen.pos_x, (float)pos_numen.pos_y, (float)(SCALE), (float)(SCALE) };
			DrawTexturePro (*tex, src, dst, (Vector2){ 0, 0 }, 0.0f, WHITE);

		}
		else
		{
			col = (is_corrupt == TRUE)
			    ? (Color){ 200, 60, 60, 255 }
			    : (Color){ 80, 200, 80, 255 };

			DrawCircle (pos_numen.pos_x + SCALE, pos_numen.pos_y + SCALE, SCALE - 2, col);
			DrawCircleLines (pos_numen.pos_x + SCALE, pos_numen.pos_y + SCALE, SCALE - 2, BLACK);

			gd = numen_get_gdesc (num);
			if (gd && gd[0])	DrawText (TextFormat ("%c", gd[0]),
				          					pos_numen.pos_x + SCALE - 4, 
											pos_numen.pos_y + SCALE - 6, 
											14, BLACK);

		}

		/*	=====BARRAS DE VIDA Y ENERGIA ===== */
			_ge_draw_bars (num); 
		/*	=================================== */




	}
		/*	=====BARRAS DE VIDA Y ENERGIA ===== */
		_ge_draw_bars (active_num);
		/*	=================================== */

}


Status
_ge_draw_bars (Numen* numen)
{
    int       hp, energy;
    int       max_life, max_energy;
    float     hp_ratio, eng_ratio;
    int       bar_w = SCALE * 2;
    int       bar_h = 4;
    Color     color_bar_life   = GREEN;
    Color     color_bar_energy = (Color){ 230, 200, 40, 255 };
    Position  pos;

    if (!numen) return ERROR;

	/*Si está en mochila no continuar, o si no estaá non hace falta pitnar ninguna barra*/
    pos = numen_get_position (numen);
    if (pos.pos_x == NO_POS || pos.pos_y == NO_POS) return OK;

    if (numen_get_corrupt (numen) == TRUE)
    {
        max_life       = MAX_LIFE_CORRUPT;
        max_energy     = MAX_ENGY_CORRUPT;
        color_bar_life = RED;
    }
    else
    {
        max_life   = MAX_LIFE;
        max_energy = MAX_ENGY;
    }


    hp = numen_get_health (numen);
    hp_ratio = (max_life > 0) ? (float)hp / (float)max_life : 0.0f;
    if (hp_ratio < 0.0f) hp_ratio = 0.0f;
    if (hp_ratio > 1.0f) hp_ratio = 1.0f;

    energy = numen_get_energy (numen);
    eng_ratio = (max_energy > 0) ? (float)energy / (float)max_energy : 0.0f;
    if (eng_ratio < 0.0f) eng_ratio = 0.0f;
    if (eng_ratio > 1.0f) eng_ratio = 1.0f;

    /* === Barra de VIDA  === */
    DrawRectangle (pos.pos_x, pos.pos_y - 8, bar_w, bar_h,
                   (Color){ 60, 60, 60, 200 });
    DrawRectangle (pos.pos_x, pos.pos_y - 8,
                   (int)(bar_w * hp_ratio), bar_h,
                   color_bar_life);
    DrawRectangleLines (pos.pos_x, pos.pos_y - 8, bar_w, bar_h, BLACK);

    /* === Barra de ENERGIA  === */
    DrawRectangle (pos.pos_x, pos.pos_y - 3, bar_w, bar_h,
                   (Color){ 60, 60, 60, 200 });
    DrawRectangle (pos.pos_x, pos.pos_y - 3,
                   (int)(bar_w * eng_ratio), bar_h,
                   color_bar_energy);
    DrawRectangleLines (pos.pos_x, pos.pos_y - 3, bar_w, bar_h, BLACK);

    return OK;
}

static int	_ge_max_radio_skill_of_numen (Numen* numen)
{
	Skills_id	skills[NUM_SKILLS];
	int	i, max_radio;

	if (!numen) return -1;

	/*Inicializamos todos los posibles skills para visualizar cada uno*/
	for (i = 0; i < NUM_SKILLS; i++)	{skills[i] = numen_get_skill_by_index(numen, i);}

	/*Buscamos el skill con el radio más grande*/
	for (i = 0, max_radio = skill_get_radio (skills[i]) ; i < NUM_SKILLS; i++)
	{
		if (max_radio <  skill_get_radio (skills[i])) max_radio =  skill_get_radio (skills[i]);
	}

	return max_radio;
}